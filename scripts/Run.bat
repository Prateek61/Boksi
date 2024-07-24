@REM Run the Compiled project

@echo off

cd ..
.\bin\Release-windows-x86_64\Sandbox\Sandbox.exe

cd scripts

IF [%1] == [] GOTO PauseLabel
GOTO SkipPause

:PauseLabel
pause

:SkipPause