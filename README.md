# distlib
 Distance related functions implemented as SQLite run-time loadable extension.

| Function | Description |
| --- | --- |
| `jsim(x,y)` | Jaro similarity normalized to 0.0 and 1.0 |
| `jwsim(x,y)` | Jaro-Winkler similarity normalized to 0.0 and 1.0 |

WITH orig_ AS (SELECT 'ABCDEFGHIJK' AS orig),
subseq_ AS (SELECT orig, subseq FROM subseq(orig) JOIN orig_ ON 1=1)
SELECT orig, subseq, lsim(orig, subseq) lsim, dlsim(orig, subseq) dlsim, jsim(orig, subseq) jsim, jwsim(orig, subseq) jwsim from subseq_;

CREATE TABLE SUBSEQ10_DIST_C as
WITH orig_ AS (SELECT 'Teststring' AS orig),
perm_ AS (SELECT orig, permut FROM perm(orig) JOIN orig_ ON 1=1)
SELECT orig, permut, lsim(orig, permut) lsim, dlsim(orig, permut) dlsim, jsim(orig, permut) jsim, jwsim(orig, permut) jwsim from perm_;
