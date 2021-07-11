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

cl /EHsc /FojaroWinkler.obj /c jaroWinkler.cpp 
cl /EHsc /Fopylcs.obj /c pylcs.cpp 
cl /EHsc /Fdldist.obj /c dldist.cpp 
cl /EHsc /Flcsubstr.obj /c lcsubstr.cpp 
cl /EHsc /Foperm.obj /c perm.cpp 
cl /EHsc /Fosubseq.obj /c subseq.cpp 
cl /EHsc /FoRegistExt.obj /c RegistExt.cpp 
cl /EHsc /Foutf8_unicode.obj /c utf8_unicode.cpp
link /DLL /OUT:distlib_64.dll utf8_unicode.obj RegistExt.obj perm.obj subseq.obj jaroWinkler.obj pylcs.obj dldist.obj lcsubstr.obj
 
Compile on Linux by linux64_build_extensions.sh :  
 g++ -fPIC -lm -shared jaroWinkler.cpp  pylcs.cpp dldist.cpp lcsubstr.cpp perm.cpp subseq.cpp RegistExt.cpp utf8_unicode.cpp -o distlib_64.so 
 
Load extension on Windows:
 sqlite> .load distlib.dll
 sqlite>

Load extension on Linux:
 sqlite> .load ./histograms.so
 sqlite>
 
Load extension in DB Browser for SQLite (Windows / Linux)
  DB Browser for SQLite -> "Load Extension..."

*/

#include <vector>
#include <string>

#include "RegistExt.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

using std::string;

#ifdef __cplusplus
extern "C" {
#endif


SQLITE_EXTENSION_INIT1

void jaroDistanceFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return; 
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  l = jaroDistance(zIn1, zIn2);
  sqlite3_result_double(context, l);  
}


void jaroWinklerDistanceFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  l = jaroWinklerDistance(zIn1, zIn2);
  sqlite3_result_double(context, l);  
}

void levenshteinDistanceFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  sqlite3_int64 l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  // l = levenshtein_distance(zIn1, zIn2);
  l = levenshtein_dist(zIn1, zIn2);
  sqlite3_result_int64(context, l);  
}

void levenshteinDistanceFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  //l = levenshtein_distancep(zIn1, zIn2);
  l = levenshtein_distp(zIn1, zIn2);
  sqlite3_result_double(context, l);  
}

void dl_distFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  sqlite3_int64 l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  l = dl_dist(zIn1, zIn2);
  sqlite3_result_int64(context, l);  
}

void dl_distFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  double l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  l = dl_distp(zIn1, zIn2);
  sqlite3_result_double(context, l);  
}

void lcstrFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  string l = "";
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  l = lcstr(zIn1, zIn2);
  sqlite3_result_text(context, l.c_str(),l.size(), SQLITE_TRANSIENT);  
}

void lcstrlFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  sqlite3_int64 l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  structRet ret = lcs2_length_(zIn1, zIn2);
  sqlite3_result_int64(context, ret.max);  
}

void lcseqFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  string l = "";
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  l = lcseq(zIn1, zIn2);
  sqlite3_result_text(context, l.c_str(),l.size(), SQLITE_TRANSIENT);  
}

void lcseqlFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  sqlite3_int64 l = 0;
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  l = lcs_length_(zIn1, zIn2);
  sqlite3_result_int64(context, l);  
}

/* void dummyFuncP(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  string l = "";
  assert( argc==2 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  if( sqlite3_value_type(argv[1])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  string zIn2 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[1])); 
  //u32string s = StringToWString(zIn1);
  u32string s =  U"aöü";
  s = s + s[2];
  sqlite3_result_text(context, u32_to_ascii(s).c_str(), u32_to_ascii(s).size(), SQLITE_TRANSIENT);  
} */



/* void dummyFunc1P(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  int l = 0;
  assert( argc==1 );
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  string zIn1 = sqlite3_mprintf("%s",  (const unsigned char*)sqlite3_value_text(argv[0]));
  sqlite3_result_int64(context, StringToWString(zIn1).size());  
} */

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

  rc = sqlite3_create_module(db, "perm",  &permModule, 0);
  rc = sqlite3_create_module(db, "subseq",  &subseqModule, 0);
  
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

  rc = sqlite3_create_function(db, "lcstr", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, lcstrFuncP, 0, 0); 

  rc = sqlite3_create_function(db, "lcstrl", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, lcstrlFuncP, 0, 0); 

  rc = sqlite3_create_function(db, "lcseq", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, lcseqFuncP, 0, 0); 

  rc = sqlite3_create_function(db, "lcseql", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, lcseqlFuncP, 0, 0); 				   

/*   rc = sqlite3_create_function(db, "dummy", 2,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, dummyFuncP, 0, 0); 
				   
  rc = sqlite3_create_function(db, "dummy", 1,
                   SQLITE_UTF8|SQLITE_DETERMINISTIC,
                   0, dummyFunc1P, 0, 0); 	 */			   

#endif
  return rc;
}



#ifdef __cplusplus
}
#endif
