/*
subseq.cpp, Lorenz Schiffmann 2021

SQLite extension for calculating interpolated curve of one dimensional scatterplot 
of column values as well as corresponding standard deviations.

The MIT License (MIT)

Copyright (c) 2021 Lorenz Schiffmann

subseqission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to subseqit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this subseqission notice shall be included in all
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
#include <set>
#include <vector>
#include <cstdlib>

#include "RegistExt.h"
// #include "helpers.h"
#include <assert.h>
#include <memory.h>


#ifndef SQLITE_OMIT_VIRTUALTABLE




#ifdef __cplusplus
extern "C" {
#endif


// Finds and stores result in st for a given
// string s.
void generate(std::set<std::string>& st, std::string s)
{
    if (s.size() == 0)
        return;
 
    // If current string is not already present.
    if (st.find(s) == st.end()) {
        st.insert(s);
 
        // Traverse current string, one by one
        // remove every character and recur.
        for (int i = 0; i < s.size(); i++) {
            std::string t = s;
            t.erase(i, 1);
            generate(st, t);
        }
    }
    return;
}


/* subseq_cursor is a subclass of sqlite3_vtab_cursor which will
** serve as the underlying representation of a cursor that scans
** over rows of the result
*/
typedef struct subseq_cursor subseq_cursor;
struct subseq_cursor {
  sqlite3_vtab_cursor base;  /* Base class - must be first */
  int isDesc;                /* True to count down rather than up */
  sqlite3_int64 iRowid;      /* The rowid */
  std::string    subseq;
  sqlite3_int64  nrows;
  std::set<std::string> st;
  std::set<std::string>::iterator it; 
};



int subseqConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
)
{
  sqlite3_vtab *pNew;
  int rc;
/* The hidden columns serves as arguments to the subseq function as in:
SELECT * FROM subseq('tblname', 'xcolid', 'ycolid', nbins, minbin, maxbin);
They won't show up in the SQL tables.
*/
  rc = sqlite3_declare_vtab(db,
// Order of columns MUST match the order of the above enum ColNum
  "CREATE TABLE subseq(subseq TEXT, nrows INTEGER HIDDEN)");
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
** This method is the destructor for subseq_cursor objects.
*/
int subseqDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

/*
** Constructor for a new subseq_cursor object.
*/
int subseqOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  subseq_cursor *pCur;
  // allocate c++ object with new rather than sqlite3_malloc which doesn't call constructors
  pCur = new subseq_cursor;
  if (pCur == NULL) return SQLITE_NOMEM;
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

/*
** Destructor for a subseq_cursor.
*/
int subseqClose(sqlite3_vtab_cursor *cur){
  delete cur;
  return SQLITE_OK;
}


/*
** Advance a subseq_cursor to its next row of output.
*/
int subseqNext(sqlite3_vtab_cursor *cur){
  subseq_cursor *pCur = (subseq_cursor*)cur;
  pCur->iRowid++;
  pCur->it++;
  return SQLITE_OK;
}

/*
** Return values of columns for the row at which the subseq_cursor
** is currently pointing.
*/
int subseqColumn(
  sqlite3_vtab_cursor *cur,   /* The cursor */
  sqlite3_context *ctx,       /* First argument to sqlite3_result_...() */
  int i                       /* Which column to return */
){
  subseq_cursor *pCur = (subseq_cursor*)cur;

  if (i==0) {   // only 1 column to return
    pCur->subseq = *pCur->it;
    sqlite3_result_text(ctx, pCur->subseq.c_str(),pCur->subseq.size(),0);
  }
  //pCur->it++;
  return SQLITE_OK;
}

/*
** Return the rowid for the current row.  In this implementation, the
** rowid is the same as the output value.
*/
int subseqRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
  subseq_cursor *pCur = (subseq_cursor*)cur;
  *pRowid = pCur->iRowid;
  return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/

int subseqEof(sqlite3_vtab_cursor *cur) {
  subseq_cursor *pCur = (subseq_cursor*)cur;
  //return pCur->iRowid >= pCur->nrows;
  //return (pCur->it == pCur->st.end());
  return pCur->iRowid >= pCur->nrows;
}



int subseqFilter(
  sqlite3_vtab_cursor *pVtabCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  subseq_cursor *pCur = (subseq_cursor *)pVtabCursor;
  const int max_len = 16;
  int l = 0; 
  
  if (argc > 0) {
    l = strlen((const char*)sqlite3_value_text(argv[0]));
  }
  else {
    const char *zText = "Function subseq(INPUT1) requires exactly one non-empty argument.\n";
    pCur->base.pVtab->zErrMsg = sqlite3_mprintf(zText);
    return SQLITE_ERROR;	  
  }

  if(argc > 0 && l>0 && l <= max_len)
  {
	//pCur->subseq = sqlite3_mprintf("%s",  (const char*)sqlite3_value_text(argv[0]));  
	std::string s = sqlite3_mprintf("%s",  (const char*)sqlite3_value_text(argv[0]));
	generate(pCur->st, s);
	pCur->it = pCur->st.begin();	  
	pCur->subseq = *pCur->it;
	pCur->nrows = pCur->st.size();
    pCur->iRowid = 0;	
	
  } else
  {	 
    const std::string errmsg = "Length of INPUT1 argument for function subseq(INPUT1) must be between 1 and " \
	                   + std::to_string(max_len) + "\n";
    pCur->base.pVtab->zErrMsg = sqlite3_mprintf(errmsg.c_str());
    return SQLITE_ERROR;	  
  }
  

  return SQLITE_OK;
}


// critical, do not modify unless you are familiar in detail !!!!
int subseqBestIndex(
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
** generate_subseq virtual table.
*/
sqlite3_module subseqModule = {
  0,                         /* iVersion */
  0,                         /* xCreate */
  subseqConnect,             /* xConnect */
  subseqBestIndex,           /* xBestIndex */
  subseqDisconnect,          /* xDisconnect */
  0,                         /* xDestroy */
  subseqOpen,                /* xOpen - open a cursor */
  subseqClose,               /* xClose - close a cursor */
  subseqFilter,              /* xFilter - configure scan constraints */
  subseqNext,                /* xNext - advance a cursor */
  subseqEof,                 /* xEof - check for end of scan */
  subseqColumn,              /* xColumn - read data */
  subseqRowid,               /* xRowid - read data */
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
