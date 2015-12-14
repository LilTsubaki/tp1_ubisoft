@ECHO OFF

CLS

CD /D "%~dp0"

SET ZZIP=C:\Program Files\7-Zip\7z.exe

rem --------------------------------------------------------------------------------------------------------------------
SET ZIP_FOLDER=..\..\SAVE

FOR %%P IN (11 12 14) DO CALL :LABEL_BUILD_PLATFORM %%P
GOTO :EOF

rem --------------------------------------------------------------------------------------------------------------------
:: loop subroutine
:LABEL_BUILD_PLATFORM

SET BUILD_PLATFORM=%1

FOR %%C IN (32 64) DO CALL :LABEL_BUILD_CONFIGURATION %%C

GOTO :EOF

rem --------------------------------------------------------------------------------------------------------------------
:: loop subroutine
:LABEL_BUILD_CONFIGURATION

SET BUILD_CONFIGURATION=%1

IF "%BUILD_CONFIGURATION%"=="32"	SET BUILD_CFG=x86
IF "%BUILD_CONFIGURATION%"=="64"	SET BUILD_CFG=x64
	
SET ZIP=SFML-2.3.2-windows-vc%BUILD_PLATFORM%-%BUILD_CONFIGURATION%-bit.zip
SET FOLDER_LIB_OUT=lib_v%BUILD_PLATFORM%0_%BUILD_CFG%
SET FOLDER_BIN_OUT=bin_v%BUILD_PLATFORM%0_%BUILD_CFG%
	
SET TITLE=Platform=%BUILD_PLATFORM% Configuration=%BUILD_CONFIGURATION% Zip=%ZIP% Lib=%FOLDER_LIB_OUT% Bin=%FOLDER_BIN_OUT%

ECHO %TITLE%

"%ZZIP%" e %ZIP_FOLDER%\%ZIP% -y -o%FOLDER_LIB_OUT% *.lib -r
"%ZZIP%" e %ZIP_FOLDER%\%ZIP% -y -o%FOLDER_BIN_OUT% *.dll -r

GOTO :EOF

rem --------------------------------------------------------------------------------------------------------------------
::end of file
:EOF
