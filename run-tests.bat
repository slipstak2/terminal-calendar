@echo off

CALL :RunTests .\x64\Debug\common-tests.exe
CALL :RunTests .\x64\Release\common-tests.exe

CALL :RunTests .\x64\Debug\terminal-controls-tests.exe
CALL :RunTests .\x64\Release\terminal-controls-tests.exe

pause

:RunTests
%~1
if %ERRORLEVEL% NEQ 0 (
	exit -1
)