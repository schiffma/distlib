:: Extension path - specify where this extension will be built.
SET EXT_PATH="C:\Users\schiffma\Google Drive\GitHub\distlib"

:: Set path variables
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

CD %EXT_PATH%

:: Build extensions

cl /EHsc /FojaroWinkler.obj /c jaroWinkler.cpp 
cl /EHsc /Fopylcs.obj /c pylcs.cpp 
cl /EHsc /Fdldist.obj /c dldist.cpp 
cl /EHsc /Flcsubstr.obj /c lcsubstr.cpp 
cl /EHsc /Foperm.obj /c perm.cpp 
cl /EHsc /Fosubseq.obj /c subseq.cpp 
cl /EHsc /FoRegistExt.obj /c RegistExt.cpp 
cl /EHsc /Foutf8_unicode.obj /c utf8_unicode.cpp
link /DLL /OUT:distlib_64.dll utf8_unicode.obj RegistExt.obj perm.obj subseq.obj jaroWinkler.obj pylcs.obj dldist.obj lcsubstr.obj

del *.obj
del *.exp
del *.lib

pause
