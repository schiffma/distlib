/*
RegistExt.cpp, Lorenz Schiffmann 2021
RegistExt.cpp, Robert Oeffner 2018

The MIT License (MIT)

Copyright (c) 2021 Lorenz Schiffmann
Copyright (c) 2017 Robert Oeffner

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




Compile on Windows by calling win64build_extensions_generic.bat :

cl /EHsc /FoSQLiteExt.obj /c SQLiteExt.cpp 
cl /EHsc /FojaroWinkler.obj /c jaroWinkler.cpp 
cl /EHsc /Fopylcs.obj /c pylcs.cpp 
cl /EHsc /Fdldist.obj /c dldist.cpp 
cl /EHsc /Foperm.obj /c perm.cpp 
cl /EHsc /Fosubseq.obj /c subseq.cpp 
cl /EHsc /FoRegistExt.obj /c RegistExt.cpp 
link /DLL /OUT:distlib.dll SQLiteExt.obj RegistExt.obj perm.obj subseq.obj jaroWinkler.obj pylcs.obj dldist.obj
 
 on Windows
 sqlite> .load distlib.dll
 sqlite>
 

*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

#include "RegistExt.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


SQLITE_EXTENSION_INIT1

void jaroDistanceFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int nIn1;
  int nIn2;
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  nIn1 = sqlite3_value_bytes(argv[0]);
  nIn2 = sqlite3_value_bytes(argv[1]);  
  std::string zIn1( reinterpret_cast<char const*>(sqlite3_value_text(argv[0])), nIn1 ) ;
  std::string zIn2( reinterpret_cast<char const*>(sqlite3_value_text(argv[1])), nIn2 ) ;
  l = jaroDistance(zIn1, zIn2);
  sqlite3_result_double(context, l);  
}


void jaroWinklerDistanceFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int nIn1;
  int nIn2;
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  nIn1 = sqlite3_value_bytes(argv[0]);
  nIn2 = sqlite3_value_bytes(argv[1]);  
  std::string zIn1( reinterpret_cast<char const*>(sqlite3_value_text(argv[0])), nIn1 ) ;
  std::string zIn2( reinterpret_cast<char const*>(sqlite3_value_text(argv[1])), nIn2 ) ;
  l = jaroWinklerDistance(zIn1, zIn2);
  sqlite3_result_double(context, l);  
}

void levenshteinDistanceFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int nIn1;
  int nIn2;
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  nIn1 = sqlite3_value_bytes(argv[0]);
  nIn2 = sqlite3_value_bytes(argv[1]);  
  std::string zIn1( reinterpret_cast<char const*>(sqlite3_value_text(argv[0])), nIn1 ) ;
  std::string zIn2( reinterpret_cast<char const*>(sqlite3_value_text(argv[1])), nIn2 ) ;
  // l = levenshtein_distance(zIn1, zIn2);
  l = levenshtein_dist(zIn1, zIn2);
  sqlite3_result_int64(context, l);  
}

void levenshteinDistanceFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int nIn1;
  int nIn2;
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  nIn1 = sqlite3_value_bytes(argv[0]);
  nIn2 = sqlite3_value_bytes(argv[1]);  
  std::string zIn1( reinterpret_cast<char const*>(sqlite3_value_text(argv[0])), nIn1 ) ;
  std::string zIn2( reinterpret_cast<char const*>(sqlite3_value_text(argv[1])), nIn2 ) ;
  //l = levenshtein_distancep(zIn1, zIn2);
  l = levenshtein_distp(zIn1, zIn2);
  sqlite3_result_double(context, l);  
}

void dl_distFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int nIn1;
  int nIn2;
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  nIn1 = sqlite3_value_bytes(argv[0]);
  nIn2 = sqlite3_value_bytes(argv[1]);  
  std::string zIn1( reinterpret_cast<char const*>(sqlite3_value_text(argv[0])), nIn1 ) ;
  std::string zIn2( reinterpret_cast<char const*>(sqlite3_value_text(argv[1])), nIn2 ) ;
  l = dl_dist(zIn1, zIn2);
  sqlite3_result_int64(context, l);  
}

void dl_distFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int nIn1;
  int nIn2;
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  nIn1 = sqlite3_value_bytes(argv[0]);
  nIn2 = sqlite3_value_bytes(argv[1]);  
  std::string zIn1( reinterpret_cast<char const*>(sqlite3_value_text(argv[0])), nIn1 ) ;
  std::string zIn2( reinterpret_cast<char const*>(sqlite3_value_text(argv[1])), nIn2 ) ;
  l = dl_distp(zIn1, zIn2);
  sqlite3_result_double(context, l);  
}


sqlite3 *thisdb = NULL;

#ifdef _WIN32
__declspec(dllexport)
#endif
/* The built library file name excluding its file extension must be part of the 
 function name below as documented on http://www.sqlite.org/loadext.html
*/
int sqlite3_distlib_init( // always use lower case
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
  )
{
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
#ifndef SQLITE_OMIT_VIRTUALTABLE
  if (sqlite3_libversion_number()<3008012)
  {
    *pzErrMsg = sqlite3_mprintf("distlib extension requires SQLite 3.8.12 or later");
    return SQLITE_ERROR;
  }

  rc = sqlite3_create_module(db, "PERM",  &permModule, 0);
  rc = sqlite3_create_module(db, "SUBSEQ",  &subseqModule, 0);
  
  // distance related functions
  rc = sqlite3_create_function(db, "jsim", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, jaroDistanceFunc, 0, 0);		
  rc = sqlite3_create_function(db, "jwsim", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, jaroWinklerDistanceFunc, 0, 0);  

  rc = sqlite3_create_function(db, "ldist", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, levenshteinDistanceFunc, 0, 0); 				   

  rc = sqlite3_create_function(db, "lsim", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, levenshteinDistanceFuncP, 0, 0); 				   

  rc = sqlite3_create_function(db, "dldist", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, dl_distFunc, 0, 0); 				   

  rc = sqlite3_create_function(db, "dlsim", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, dl_distFuncP, 0, 0); 

#endif
  return rc;
}



#ifdef __cplusplus
}
#endif
