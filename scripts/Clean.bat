@REM Clean the project

@echo off

cd ..
make config=release clean

cd scripts
echo Clean complete!

IF [%1] == [] GOTO PauseLabel
GOTO SkipPause

:PauseLabel
pause

:SkipPause