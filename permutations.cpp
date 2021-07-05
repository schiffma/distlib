/*
** 2021-06-18   schiffma
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This file implements a table-valued function:
**
**      select * from permutations('abcd');
**
** The function has a single (non-HIDDEN) column named permut that takes
** on all permutations of the string in its argument, including an empty string
** and the input string itself.  
*/
#include <iostream>
#include <vector>
#include <cstdlib>

#include "RegistExt.h"
// #include "helpers.h"
#include <assert.h>
#include <memory.h>
#include <string.h>

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


/* Modified permutation algorithm from 
https://github.com/Anjan50/https-www.hackerrank.com-challenges-permutations-of-strings-problem/blob/master/Permutations%20of%20Strings.c
*/


void swap2(char *s, int i, int j){
    char tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
}


void reverse2(char *s, int start, int end){
    while(start<end){
        swap2(s,start++,end--);
    }
}

int next_permutation2(int n, char *s)
{
    for(int i=n-2;i>-1;i--){
        if(s[i+1] > s[i]){
            /* get min max */
            for(int j=n-1;j>i;j--){
                if(s[j] > s[i]){
                    /* do swap */
                    swap2(s,i,j);
                    /* do reverse */
                    reverse2(s,i+1,n-1);
                    return 1;
                }
            }
        }
    }
    return 0;
}


/* permutations_cursor is a subclass of sqlite3_vtab_cursor which will
** serve as the underlying representation of a cursor that scans
** over rows of the result
*/
typedef struct perm_cursor permutations_cursor;
struct perm_cursor {
  sqlite3_vtab_cursor base;  /* Base class - must be first */
  sqlite3_int64 iRowid;      /* The rowid */
  //std::string    zStr;
  char *zStr;
  sqlite3_int64 nStr;                  /* Length of the string in bytes */
  //sqlite3_int64  nStr;  
  sqlite3_int64  nRows;
};

/*
** The permConnect() method is invoked to create a new
** perm_vtab that describes the generate_perm virtual table.
** As the permCreate method is set to NULL this virtual table is
** an Eponymous-only virtual table, i.e. useful as a table-valued function.
** The hidden columns are the arguments to the function and won't show up 
** in the SQL tables.
** Think of this routine as the constructor for perm_vtab objects.
**
** All this routine needs to do is:
**
**    (1) Allocate the perm_vtab object and initialize all fields.
**
**    (2) Tell SQLite (via the sqlite3_declare_vtab() interface) what the
**        result set of queries against generate_perm will look like.
*/
int permutationsConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
)
{
  sqlite3_vtab *pNew;
  int rc;
  // The hidden columns serves as arguments to the perm function as in:
  // SELECT * FROM perm('tblname', 'colid', nbins, minbin, maxbin, 'discrcolid', discrval);
  // They won't show up in the SQL tables.
  rc = sqlite3_declare_vtab(db,
    // Order of columns MUST match the order of the above enum ColNum
     "CREATE TABLE permutations(zstr TEXT, nrows HIDDEN)");
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
** This method is the destructor for permutations_vtab objects.
*/
int permutationsDisconnect(sqlite3_vtab *pVtab){
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

/*
** Constructor for a new permutations_cursor object.
*/

int permutationsOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  perm_cursor *pCur;
  // allocate c++ object with new rather than sqlite3_malloc which doesn't call constructors
  pCur = new perm_cursor; 
  if( pCur==NULL ) return SQLITE_NOMEM;
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

/*
** Destructor for a perm_cursor.
*/
int permutationsClose(sqlite3_vtab_cursor *cur){
  delete cur;
  return SQLITE_OK;
}


/*
** Advance a permutations_cursor to its next row of output.
*/
int permutationsNext(sqlite3_vtab_cursor *cur){
  perm_cursor *pCur = (perm_cursor*)cur;
  pCur->iRowid++;
  return SQLITE_OK;
}

/*
** Return values of columns for the row at which the permutations_cursor
** is currently pointing.
*/
int permutationsColumn(
  sqlite3_vtab_cursor *cur,   /* The cursor */
  sqlite3_context *ctx,       /* First argument to sqlite3_result_...() */
  int i                       /* Which column to return */
){
  perm_cursor *pCur = (permutations_cursor*)cur;
  if (pCur->iRowid > 0 ){
	 int r  = next_permutation2((int)pCur->nStr, pCur->zStr); 

  } 
  sqlite3_result_text(ctx, pCur->zStr, pCur->nStr, 0);                      
  return SQLITE_OK;
}

/*
** Return the rowid for the current row.  In this implementation, the
** rowid is the same as the output value.
*/
int permutationsRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid){
   perm_cursor *pCur = (perm_cursor*)cur;
  *pRowid = pCur->iRowid;
  return SQLITE_OK;
}

/*
** Return TRUE if the cursor has been moved off of the last
** row of output.
*/
int permutationsEof(sqlite3_vtab_cursor *cur){
  perm_cursor *pCur = (perm_cursor*)cur;
  return pCur->iRowid >= pCur->nRows;
}

/*
** This method is called to "rewind" the permutations_cursor object back
** to the first row of output.  This method is always called at least
** once prior to any call to permutationsColumn() or permutationsRowid() or 
** permutationsEof().
*/
int permutationsFilter(
  sqlite3_vtab_cursor *pVtabCursor, 
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
){
  perm_cursor *pCur = (perm_cursor *)pVtabCursor;
  sqlite3_free(pCur->zStr);
  if (argc>0 && strlen((const char*)sqlite3_value_text(argv[0])) <= 10) {
    pCur->zStr = sqlite3_mprintf("%s", sqlite3_value_text(argv[0]));    
	pCur->nStr = pCur->zStr ? (int)(strlen(pCur->zStr)) : 0;
	pCur->nRows = pCur->zStr ? factorial((int)strlen(pCur->zStr)) : 0;
  } else {
    pCur->zStr = "";
	pCur->nStr = 0;
	pCur->nRows = 0;
  }
  pCur->iRowid = 0;
  return SQLITE_OK;
}

/*
** SQLite will invoke this method one or more times while planning a query
** that uses the virtual table.  This routine needs to create
** a query plan for each invocation and compute an estimated cost for that
** plan.
*/
static int permutationsBestIndex(
  sqlite3_vtab *tab,
  sqlite3_index_info *pIdxInfo
){
  /* Search for a usable equality constraint against column 1 
  ** (original_string) and use it if at all possible */
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
** virtual table.
*/
sqlite3_module permutationsModule = {
  0,                         /* iVersion */
  0,                         /* xCreate */
  permutationsConnect,             /* xConnect */
  permutationsBestIndex,           /* xBestIndex */
  permutationsDisconnect,          /* xDisconnect */
  0,                         /* xDestroy */
  permutationsOpen,                /* xOpen - open a cursor */
  permutationsClose,               /* xClose - close a cursor */
  permutationsFilter,              /* xFilter - configure scan constraints */
  permutationsNext,                /* xNext - advance a cursor */
  permutationsEof,                 /* xEof - check for end of scan */
  permutationsColumn,              /* xColumn - read data */
  permutationsRowid,               /* xRowid - read data */
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
