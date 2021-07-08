# distlib
 Distance related functions implemented as SQLite run-time loadable extension.
 Any UTF-8 strings are supported.

| SQLite function | Description |  
| --- | --- |
| `ldist(x,y)` | Levenshtein distance as integer >= 0 | 
| `lsim(x,y)` | Levenshtein similarity as double between 0.0 and 1.0  |
| `dldist(x,y)` | Damerau–Levenshtein distance as integer >= 0 |
| `dlsim(x,y)` | Damerau–Levenshtein similarity as double between 0.0 and 1.0 |
| `jsim(x,y)` | Jaro similarity as double between 0.0 and 1.0 | 
| `jwsim(x,y)` | Jaro-Winkler similarity as double between 0.0 and 1.0 | 
| `lcstr(x,y)` | Longest common substring (to be re-implemented) |
| `lcstrl(x,y)` | Longest common substring length |
| `perm(x)` | Permutation table-valued function for strings up to len(x)>=10 | 
| `subseq(x)` | Subsequences table-valued function for strings up to len(x)>=16 | 
<br>
Examples:

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
| `select lcstr('carpenter', 'sharpener');` | 5 |
| `select * from perm('ABCDEFGH');` | ABCDEFGH,ABCDEFHG, ... n! rows |
| `select * from perm ('Smørrebrød');` | Smørrebrød,Smørrebødr, ... n! rows |
| `select * from subseq('ABCDEFGH');` | A,AB,ABC, ... max. 2^n-1 rows |
<br>

