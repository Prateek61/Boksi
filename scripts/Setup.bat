@echo off

cd ..
premake5 gmake2

cd scripts

@REM Pause the script
echo Setup complete!

IF [%1] == [] GOTO PauseLabel
GOTO SkipPause
:PauseLabel
pause
:SkipPause