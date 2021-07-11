/*

RegistExt.h, Lorenz Schiffmann 2021
RegistExt.h, Robert Oeffner 2018

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

*/

#include "sqlite3ext.h"
#include <string>
#include "pylcs.hpp"

// C++ functions must be declared before extern "C" !!!

double jaroDistance(const std::string& a, const std::string& b);
double jaroWinklerDistance(const std::string& a, const std::string& b);

int levenshtein_dist(const std::string &word1, const std::string &word2);
double levenshtein_distp(const std::string &word1, const std::string &word2);
int dl_dist(const std::string &word1, const std::string &word2);
double dl_distp(const std::string &str1, const std::string &str2);

int lcs_length_(const std::string &str1, const std::string &str2);
structRet lcs2_length_(const std::string &str1, const std::string &str2);

std::string lcseq(const std::string &X, const std::string &Y);
std::string lcstr(const std::string &X, const std::string &Y);

#ifdef __cplusplus
extern "C" {
#endif


extern const sqlite3_api_routines *sqlite3_api;
extern sqlite3 *thisdb;


extern sqlite3_module permModule;

int permConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
  );
int permDisconnect(sqlite3_vtab *pVtab);
int permOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor);
int permClose(sqlite3_vtab_cursor *cur);
int permNext(sqlite3_vtab_cursor *cur);
int permColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i);
int permRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid);
int permEof(sqlite3_vtab_cursor *cur);
int permFilter(
  sqlite3_vtab_cursor *pVtabCursor,
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
  );
int permBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo);

extern sqlite3_module subseqModule;

int subseqConnect(
  sqlite3 *db,
  void *pAux,
  int argc, const char *const*argv,
  sqlite3_vtab **ppVtab,
  char **pzErr
  );
int subseqDisconnect(sqlite3_vtab *pVtab);
int subseqOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor);
int subseqClose(sqlite3_vtab_cursor *cur);
int subseqNext(sqlite3_vtab_cursor *cur);
int subseqColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i);
int subseqRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid);
int subseqEof(sqlite3_vtab_cursor *cur);
int subseqFilter(
  sqlite3_vtab_cursor *pVtabCursor,
  int idxNum, const char *idxStr,
  int argc, sqlite3_value **argv
  );
int subseqBestIndex(sqlite3_vtab *tab, sqlite3_index_info *pIdxInfo);

#ifdef __cplusplus
}
#endif


#pragma once

