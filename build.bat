@echo off

@REM Check for debug warnings and errors
call $compile-debug.bat    Run.c             Run.exe             Shell32.lib User32.lib
call $compile-debug.bat    Elevate.c         Elevate.exe         Shell32.lib User32.lib
call $compile-debug.bat    Install.c         Install.exe         Advapi32.lib User32.lib
call $compile-debug.bat    Uninstall.c       Uninstall.exe       Advapi32.lib User32.lib
call $compile-debug.bat    Install.user.c    Install.user.exe    Advapi32.lib User32.lib
call $compile-debug.bat    Uninstall.user.c  Uninstall.user.exe  Advapi32.lib User32.lib

@REM Check for release warnings and errors
call $compile-release.bat  Run.c             Run.exe             Shell32.lib User32.lib
call $compile-release.bat  Elevate.c         Elevate.exe         Shell32.lib User32.lib
call $compile-release.bat  Install.c         Install.exe         Advapi32.lib User32.lib
call $compile-release.bat  Uninstall.c       Uninstall.exe       Advapi32.lib User32.lib
call $compile-release.bat  Install.user.c    Install.user.exe    Advapi32.lib User32.lib
call $compile-release.bat  Uninstall.user.c  Uninstall.user.exe  Advapi32.lib User32.lib
