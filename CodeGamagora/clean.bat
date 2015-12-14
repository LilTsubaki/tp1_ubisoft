rem @ECHO OFF

CLS

CD /D "%~dp0"

DEL /F /S /Q ".\BIN\*.exe"
RD /S /Q ".\Build"

DEL /F /S /Q *.sdf
DEL /F /S /Q /A:H *.suo
