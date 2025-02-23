@echo off

set "ifile=.\src\%1"
set "ofile=.\release\%2"

echo RELEASE %ifile%
mkdir .\release
mkdir .\obj\release

if not exist ".\release\vcruntime140.dll" (
  copy ".\vcruntime140.dll" ".\release\vcruntime140.dll"
)

shift
shift

set "lnker=/link /OPT:REF /OPT:ICF"
:loop
if not "%1"=="" (
  set "lnker=%lnker% %1"
  shift
  goto loop
)

@REM clean up
del %ofile%.obj
del %ofile%

@REM cl                               - The compiler program
@REM /utf-8                           - Treat source files as UTF-8 encoded
@REM /TC                              - Sets language to C
@REM /O2 /GL /Gy /DNDEBUG /EHsc /MD   - Optimization flags
@REM /Wall                            - Output all warnings
@REM /link                            - Link external libraries
@REM /Fe                              - Output executable path
@REM /Fo                              - Output object path
cl /utf-8 /TC /O2 /GL /Gy /DNDEBUG /EHsc /MD /Wall %ifile% /Fe:%ofile% /Fo:.\obj\%ofile%.obj %lnker%

@REM Check if cl command failed
IF %ERRORLEVEL% NEQ 0 (
  echo(
  echo Compile failed. Pausing for review.
  pause
)
echo(
