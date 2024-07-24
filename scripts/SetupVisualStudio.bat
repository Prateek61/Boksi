@REM Project Setup for Visual Studio

@echo off

cd ..
premake5 vs2022

cd scripts

@REM Pause the script
echo Setup complete!

IF [%1] == [] GOTO PauseLabel
GOTO SkipPause
:PauseLabel
pause
:SkipPause