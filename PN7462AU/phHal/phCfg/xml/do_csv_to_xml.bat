@echo off

cd /d %~dp0

IF NOT EXIST _Initiator.csv  GOTO :NOTHING_TO_DO_WITHOUT_INITIATOR_CSV
IF NOT EXIST _Target.csv     GOTO :NOTHING_TO_DO_WITHOUT_TARGET_CSV
IF EXIST _Sheet3.csv DEL _Sheet3.csv

del _ClifEE.xml
del _ClifEE.c.txt

IF DEFINED PYTHON_EXE GOTO :GOT_PY_EXE
IF EXIST C:\Python27\python.exe (
    SET PYTHON_EXE=C:\Python27\python.exe
    GOTO :GOT_PY_EXE
)

IF EXIST C:\Python27.32\python.exe (
    SET PYTHON_EXE=C:\Python27.32\python.exe
    GOTO :GOT_PY_EXE
)

IF EXIST C:\Python26\python.exe (
    SET PYTHON_EXE=C:\Python26\python.exe
    GOTO :GOT_PY_EXE
)

set PYTHON_EXE=python.exe
:GOT_PY_EXE

IF EXIST _ClifEE.xml del _ClifEE.xml
IF EXIST _ClifEE.c.txt del _ClifEE.c.txt

%PYTHON_EXE% clifcsv_to_xml.py _Initiator.csv  _Target.csv _Version.csv _ClifEE

IF "%ERRORLEVEL%" == "0" GOTO :CLIFCSV_TO_XML_DONE
    GOTO :ERROR_WHILE_CONVERTING_CLIF_CSV_TO_XML

:CLIFCSV_TO_XML_DONE

IF NOT EXIST _ClifEE.xml   GOTO :ERROR_NO_ClifEE_XML
IF NOT EXIST _ClifEE.c.txt GOTO :ERROR_NO_ClifEE_TXT
copy /y _ClifEE.c.txt ..\..\..\phCommon\inc\phhalRf_LP_Int.h
copy /y _ClifEE.v.txt    ..\..\phhalRf\src\phhalRf_LP_V.h

IF EXIST user_ee.bat call user_ee.bat


@REM
@REM pause here if you want to see the output of user_ee.bat
@REM

IF "%ERRORLEVEL%" == "0" GOTO :EOF
    echo "Some error occured while running user_ee.bat"
    pause
GOTO :EOF

:ERROR_NO_ClifEE_TXT
    ECHO ERROR. Could not create CLIF EE .c.txt
    SET ERRORLEVEL=2
    pause
    GOTO :EOF

:ERROR_NO_ClifEE_XML
    ECHO ERROR. Could not create CLIF EE XML
    SET ERRORLEVEL=2
    pause
    GOTO :EOF

:NOTHING_TO_DO_WITHOUT_TARGET_CSV
    ECHO Nothing to do without _Target.csv
    pause
    GOTO :EOF

:ERROR_WHILE_CONVERTING_CLIF_CSV_TO_XML
    ECHO Error occured while converting CLIF CSV TO XML
    pause
    GOTO :EOF


:NOTHING_TO_DO_WITHOUT_INITIATOR_CSV
    ECHO Nothing to do without _Initiator.csv
    pause
    GOTO :EOF
