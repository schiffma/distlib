/*
perm.cpp, Lorenz Schiffmann 2021

SQLite extension for calculating interpolated curve of one dimensional scatterplot 
of column values as well as corresponding standard deviations.

The MIT License (MIT)

Copyright (c) 2021 Lorenz Schiffmann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "RegistExt.h"
#include "utf8_unicode.hpp"
#include <assert.h>
#include <memory.h>

using std::string;
using std::vector;
using std::to_string;
using std::sort;


#ifndef SQLITE_OMIT_VIRTUALTABLE


#ifdef __cplusplus
extern "C" {
#endif

int factorial(int n)
{
  int f = 1;
  for (int c = 1; c <= n; c++)
    f = f * c;
  return f;
}


/* perm_cursor is a subclass of sqlite3_vtab_cursor which will
** serve as the underlying representation of a cursor that scans
** over rows of the result
*/
typedef struct perm_cursor perm_cursor;
struct perm_cursor {
  sqlite3_vtab_cursor base;  /* Base class - must be first */
  int isDesc;                /* True to count down rather than up */
  sqlite3_int64 iRowid;      /* The rowid */
  string    perm;
  sqlite3_int64  nrows;
  vector<string>    permv;   
};



int permConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
)
{
  sqlite3_vtab *pNew;
  int rc;
/* The hidden columns serves as arguments to the perm function as in:
SELECT * FROM perm('tblname', 'xcolid', 'ycolid', nbins, minbin, maxbin);
They won't show up in the SQL tables.
*/
  rc = sqlite3_declare_vtab(db,
// Order of columns MUST match the order of the above enum ColNum
  "CREATE TABLE perm(permut TEXT, nrows INTEGER HIDDEN)");
  if( rc==SQLITE_OK )
  {
    pNew = *ppVtab = (sqlite3_vtab *)sqlite3_malloc( sizeof(*pNew) );
    if( pNew==0 ) return SQLITE_NOMEM;
    memset(pNew, 0, sizeof(*pNew));
  }
  thisdb = db;
  return rc;
}

/*
** This method is the destructor for perm_cursor objects.
*/
int permDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

/*
** Constructor for a new perm_cursor object.
*/
int permOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  perm_cursor *pCur;
  // allocate c++ object with new rather than sqlite3_malloc which doesn't call constructors
  pCur = new perm_cursor;
  if (pCur == NULL) return SQLITE_NOMEM;
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

/*
** Destructor for a perm_cursor.
*/
int permClose(sqlite3_vtab_cursor *cur){
  delete cur;
  return SQLITE_OK;
}


/*
** Advance a perm_cursor to its next row of output.
*/
int permNext(sqlite3_vtab_cursor *cur){
  perm_cursor *pCur = (perm_cursor*)cur;
  pCur->iRowid++;
  //pCur->it++;
  next_permutation(pCur->permv.begin(), pCur->permv.end()); 
  return SQLITE_OK;
}

/*
** Return values of columns for the row at which the perm_cursor
** is currently pointing.
*/
int permColumn(
  sqlite3_vtab_cursor *cur,   /* The cursor */
  sqlite3_context *ctx,       /* First argument to sqlite3_result_...() */
  int i                       /* Which column to return */
){
  perm_cursor *pCur = (perm_cursor*)cur;
  if (i==0) { // only one column to return  
	pCur->perm = vect2str(pCur->permv);
    sqlite3_result_text(ctx, pCur->perm.c_str(), pCur->perm.size(),0);
  }
  return SQLITE_OK;
}

/*
** Return the rowid for the current row.  In this implementation, the
** rowid is the same as the output value.
*/
int permRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  perm_cursor *pCur = (perm_cursor*)cur;
  *pRowid = pCur->iRowid;
  return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/

int permEof(sqlite3_vtab_cursor *cur) {
  perm_cursor *pCur = (perm_cursor*)cur;
  return pCur->iRowid >= pCur->nrows;
}



int permFilter(
  sqlite3_vtab_cursor *pVtabCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  perm_cursor *pCur = (perm_cursor *)pVtabCursor;
  const int max_len = 10;
  int l = 0; 
  string s;  
  if (argc > 0) {
    s = sqlite3_mprintf("%s",  (const char*)sqlite3_value_text(argv[0]));
	l  = utf8_split(s).size();	
  }
  else {
    const char *zText = "Function PERM(INPUT1) requires exactly one argument.\n";
    pCur->base.pVtab->zErrMsg = sqlite3_mprintf(zText);
    return SQLITE_ERROR;	  
  }

  if(l>0 && l <= max_len)
  {
	pCur->permv = utf8_split(s);
    sort(pCur->permv.begin(), pCur->permv.end()); // initial sorting is important !!!		  
	pCur->perm  = vect2str(pCur->permv);	
	pCur->nrows = (l  > 0) ? factorial(l) : 0;
    pCur->iRowid = 0;	
  } else
  {	
    const string errmsg = "Length of INPUT1 argument for function perm(INPUT1) must be between 1 and " \
	                   + to_string(max_len) + "\n";
    pCur->base.pVtab->zErrMsg = sqlite3_mprintf(errmsg.c_str());
    return SQLITE_ERROR;	  
  }
  

  return SQLITE_OK;
}


// critical !!!!
int permBestIndex(
  sqlite3_vtab *tab,
  sqlite3_index_info *pIdxInfo
){
  int i;
  sqlite3_index_info::sqlite3_index_constraint *p;
  p = pIdxInfo->aConstraint;

  for(i=0, p=pIdxInfo->aConstraint; i<pIdxInfo->nConstraint; i++, p++){
    if( p->iColumn!=1 ) continue;
    if( p->op!=SQLITE_INDEX_CONSTRAINT_EQ ) continue;
    if( !p->usable ) continue;
    pIdxInfo->aConstraintUsage[i].argvIndex = 1;
    pIdxInfo->aConstraintUsage[i].omit = 1;
    pIdxInfo->estimatedCost = (double)10;
    pIdxInfo->estimatedRows = 10;
    return SQLITE_OK;
  }
  pIdxInfo->estimatedCost = (double)1000000000;
  pIdxInfo->estimatedRows = 1000000000;
  return SQLITE_OK;
}




/*
** This following structure defines all the methods for the
** generate_perm virtual table.
*/
sqlite3_module permModule = {
  0,                         /* iVersion */
  0,                         /* xCreate */
  permConnect,             /* xConnect */
  permBestIndex,           /* xBestIndex */
  permDisconnect,          /* xDisconnect */
  0,                         /* xDestroy */
  permOpen,                /* xOpen - open a cursor */
  permClose,               /* xClose - close a cursor */
  permFilter,              /* xFilter - configure scan constraints */
  permNext,                /* xNext - advance a cursor */
  permEof,                 /* xEof - check for end of scan */
  permColumn,              /* xColumn - read data */
  permRowid,               /* xRowid - read data */
  0,                         /* xUpdate */
  0,                         /* xBegin */
  0,                         /* xSync */
  0,                         /* xCommit */
  0,                         /* xRollback */
  0,                         /* xFindMethod */
  0,                         /* xRename */
};



#endif /* SQLITE_OMIT_VIRTUALTABLE */




#ifdef __cplusplus
}
#endif
