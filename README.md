# distlib
 Distance related functions implemented as SQLite run-time loadable extension.

| SQlite function | Description |  Query example | Result example
| --- | --- | --- | --- | 
| `ldist(x,y)` | Levenshtein distance as whole number  | select ldist('kitten','sitting'); | 3 |
| | | select ldist('a cat','an act'); | 3 |
| | | select ldist('fish', 'ifsh'); | 2 |
| `lsim(x,y)` | Levenshtein similarity as double between 0.0 and 1.0  | select lsim('kitten','sitting'); | 0.571428571428571 |
| `dldist(x,y)` | Damerau–Levenshtein distance as whole number | select ldist('kitten','sitting'); | 3 |
| | | select  dldist('a cat','an act'); | 2 |
| | | select dldist('fish', 'ifsh'); | 1 |
| `dlsim(x,y)` | Damerau–Levenshtein similarity as double between 0.0 and 1.0 | select dlsim('a cat','an act'); | 0.666666666666667 |
| `jsim(x,y)` | Jaro similarity as double between 0.0 and 1.0 | select jsim('Clark Kent','Claire Kent'); | 0.869023569023569 |
| `jwsim(x,y)` | Jaro-Winkler similarity as double between 0.0 and 1.0 | select jwsim('Peter Parker','Pete Parker'); | 0.928787878787879 |
| `lcstr(x,y)` | Longest common substring | select lcstr('carpenter', 'sharpener'); | arpen |



WITH orig_ AS (SELECT 'ABCDEFGHIJK' AS orig),
subseq_ AS (SELECT orig, subseq FROM subseq(orig) JOIN orig_ ON 1=1)
SELECT orig, subseq, lsim(orig, subseq) lsim, dlsim(orig, subseq) dlsim, jsim(orig, subseq) jsim, jwsim(orig, subseq) jwsim from subseq_;

CREATE TABLE SUBSEQ10_DIST_C as
WITH orig_ AS (SELECT 'Teststring' AS orig),
perm_ AS (SELECT orig, permut FROM perm(orig) JOIN orig_ ON 1=1)
SELECT orig, permut, lsim(orig, permut) lsim, dlsim(orig, permut) dlsim, jsim(orig, permut) jsim, jwsim(orig, permut) jwsim from perm_;
