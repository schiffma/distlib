# distlib

![Alt text](levenshtein.png?raw=true "Example")

 Distance related functions implemented as SQLite run-time loadable extension.
 Any UTF-8 strings are supported.</br>

**Functions provided:**

| SQLite function | Description |  
| --- | --- |
| `ldist(x,y)` | Levenshtein distance as integer >= 0 | 
| `lsim(x,y)` | Levenshtein similarity as double between 0.0 and 1.0  |
| `dldist(x,y)` | Damerau–Levenshtein distance as integer >= 0 |
| `dlsim(x,y)` | Damerau–Levenshtein similarity as double between 0.0 and 1.0 |
| `jsim(x,y)` | Jaro similarity as double between 0.0 and 1.0 | 
| `jwsim(x,y)` | Jaro-Winkler similarity as double between 0.0 and 1.0 | 
| `lcstr(x,y)` | Longest common substring |
| `lcstrl(x,y)` | Longest common substring length |
| `lcseq(x,y)` | Longest common subsequence |
| `lcseql(x,y)` | Longest common subsequence length |
| `perm(x)` | Permutation table-valued function for strings up to len(x)>=10 | 
| `subseq(x)` | Subsequences table-valued function for strings up to len(x)>=16 | 
<br>

**Examples:**

| Query example | Result example |
|  --- | --- | 
| `select ldist('kitten','sitting');` | 3 | 
| `select lsim('kitten','sitting');` | 0.571428571428571 |
| `select ldist('a cat','an act');` | 3 | 
| `select dldist('a cat','an act');` | 2 | 
| `select lsim('a cat','an act');` | 0.5 |
| `select lsim('a cät','an äct');` | 0.5 |
| `select dlsim('a cat','an act');` | 0.666666666666667 |
| `select ldist('fish', 'ifsh');` | 2 |
| `select dldist('fish', 'ifsh');` | 1 |   
| `select ldist('kitten','sitting');` | 3 | 
| `select jsim('Clark Kent','Claire Kent');` | 0.869023569023569 |
| `select jwsim('Peter Parker','Pete Parker');` | 0.928787878787879 |
| `select jwsim('Péter Pärker','Péte Pärker');` | 0.928787878787879 |
| `select lcstr('carpenter', 'sharpener');` | arpen |
| `select lcstrl('carpenter', 'sharpener');` | 5 |
| `select lcseq('cärpenter', 'shärpener');` | ärpener |
| `select lcseql('cärpenter', 'shärpener');` | 7 |
| `select * from perm('ABCDEFGH');` | ABCDEFGH,ABCDEFHG, ... n! rows |
| `select * from perm ('Smørrebrød');` | Smørrebrød,Smørrebødr, ... n! rows |
| `select * from subseq('ABCDEFGH');` | A,AB,ABC, ... max. 2^n-1 rows |
<br>

**Bulk examples:**

`WITH orig_ AS (SELECT 'Geräte' AS orig),`</br> 
`perm_ AS (SELECT orig, permut FROM perm(orig) JOIN orig_ ON 1=1)`</br>
`SELECT * FROM (`</br>
`SELECT orig, permut, lcstr(orig, permut) lcstr, lcstrl(orig, permut) lcstrl,`</br> 
`lcseq(orig, permut) lcseq, lcseql(orig, permut) lcseql FROM perm_`</br>
`) ORDER BY lcstrl desc, lcseql desc;`

`DROP TABLE IF EXISTS perm_gen;`</br>
`CREATE TABLE IF NOT EXISTS perm_gen AS`</br>
`WITH orig_ AS (SELECT 'Z''Hansrüed' AS orig),`</br> 
`perm_ AS (SELECT orig, permut FROM perm(orig) JOIN orig_ ON 1=1)`</br>
`SELECT * FROM perm_;`

`DROP TABLE IF EXISTS perm_test;`</br>
`CREATE TABLE IF NOT EXISTS perm_test AS`</br>
`SELECT orig, permut, lcstr(orig, permut) lcstr, lcstrl(orig, permut) lcstrl, lsim(orig, permut) lsim,`</br> 
`dlsim(orig, permut) dlsim, jsim(orig, permut) jsim, jwsim(orig, permut) jwsim FROM perm_gen;`</br>

`DROP TABLE IF EXISTS subseq_gen;`</br>
`CREATE TABLE IF NOT EXISTS subseq_gen AS`</br>
`WITH orig_ AS (SELECT 'Lückenbüßer' AS orig),`</br> 
`subseq_ AS (SELECT orig, subseq FROM subseq(orig) JOIN orig_ ON 1=1)`</br>
`SELECT * FROM subseq_;`</br>

`DROP TABLE IF EXISTS subseq_test;`</br>
`CREATE TABLE IF NOT EXISTS subseq_test AS`</br>
`SELECT orig, subseq, lcstr(orig, subseq) lcstr, lcstrl(orig, subseq) lcstrl, lsim(orig, subseq) lsim,`</br> 
`dlsim(orig, subseq) dlsim, jsim(orig, subseq) jsim, jwsim(orig, subseq) jwsim from subseq_gen;`</br>

**How to build the .dll/.so library?**

The library was primary built on Windows 10 64-bit with Visual Studio Community 2019 and tested with DB Browser for SQLite.</br>
Smoke testing was also done on Linux Mint 19.3 "Tricia" and the latest Raspberry Pi OS on a Raspberry Pi 400. 

Compile on Windows by reviewing & calling win64build_extensions_generic.bat :

`cl /EHsc /FojaroWinkler.obj /c jaroWinkler.cpp`</br> 
`cl /EHsc /Fopylcs.obj /c pylcs.cpp`</br> 
`cl /EHsc /Fdldist.obj /c dldist.cpp`</br> 
`cl /EHsc /Flcsubstr.obj /c lcsubstr.cpp`</br> 
`cl /EHsc /Foperm.obj /c perm.cpp`</br> 
`cl /EHsc /Fosubseq.obj /c subseq.cpp`</br> 
`cl /EHsc /FoRegistExt.obj /c RegistExt.cpp`</br> 
`cl /EHsc /Foutf8_unicode.obj /c utf8_unicode.cpp`</br>
`link /DLL /OUT:distlib_64.dll utf8_unicode.obj RegistExt.obj perm.obj subseq.obj jaroWinkler.obj pylcs.obj dldist.obj lcsubstr.obj`
 
Compile on Linux/Raspberry Pi OS in your default shell :

`chmod 755 linux_build_extensions.sh`</br>
`./linux_build_extensions.sh`



