:: Extension path - specify where this extension will be built.
SET EXT_PATH="C:\Users\schiffma\Google Drive\GitHub\distlib"

:: Set path variables
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

CD %EXT_PATH%

:: Build extensions

rem cl /EHsc /FoSQLiteExt.obj /c SQLiteExt.cpp 
cl /EHsc /FojaroWinkler.obj /c jaroWinkler.cpp 
cl /EHsc /Fopylcs.obj /c pylcs.cpp 
cl /EHsc /Fdldist.obj /c dldist.cpp 
cl /EHsc /Foperm.obj /c perm.cpp 
rem cl /EHsc /Fosubseq.obj /c subseq.cpp
cl /EHsc /Fosubseq.obj /c subseq.cpp 
cl /EHsc /FoRegistExt.obj /c RegistExt.cpp 
link /DLL /OUT:distlib_64.dll RegistExt.obj perm.obj subseq.obj jaroWinkler.obj pylcs.obj dldist.obj

del *.obj
del *.exp
del *.lib

pause
