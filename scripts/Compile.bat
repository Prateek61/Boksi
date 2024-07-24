@REM Compile project with make

@echo off

cd ..
make config=release CC=gcc

cd scripts
@REM Pause the script

IF [%1] == [] GOTO PauseLabel
GOTO SkipPause

:PauseLabel
pause

:SkipPause