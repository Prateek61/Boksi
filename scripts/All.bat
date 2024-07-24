@REM Setup project, Compile and Run

@echo off

@REM Check if current directory is scripts
IF EXIST .\scripts\ (
    @REM Change directory to scripts
    cd scripts
)

@REM Setup project
call Setup.bat SkipPause

@REM Compile project
call Compile.bat SkipPause

@REM Run the compiled project
call Run.bat SkipPause

@REM Pause the script
IF [%1] == [] GOTO PauseLabel
GOTO SkipPause
:PauseLabel
pause
:SkipPause
