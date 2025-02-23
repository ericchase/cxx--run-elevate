@echo off

call $compile-debug.bat  test\Test.c  Test.exe  Shell32.lib User32.lib
debug\Test.exe
