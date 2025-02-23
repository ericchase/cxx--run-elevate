@echo off

call $compile-release.bat  Echo.c       Echo.exe       Shell32.lib User32.lib

@REM Check for debug warnings and errors
call $compile-debug.bat    Run.c        Run.exe        Shell32.lib User32.lib
@REM call $compile-debug.bat    Elevate.c    Elevate.exe    Shell32.lib User32.lib
@REM call $compile-debug.bat    Install.c    Install.exe    Advapi32.lib User32.lib
@REM call $compile-debug.bat    Uninstall.c  Uninstall.exe  Advapi32.lib User32.lib

@REM Check for release warnings and errors
call $compile-release.bat  Run.c        Run.exe        Shell32.lib User32.lib
@REM call $compile-release.bat  Elevate.c    Elevate.exe    Shell32.lib User32.lib
@REM call $compile-release.bat  Install.c    Install.exe    Advapi32.lib User32.lib
@REM call $compile-release.bat  Uninstall.c  Uninstall.exe  Advapi32.lib User32.lib
