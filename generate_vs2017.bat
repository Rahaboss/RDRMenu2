@echo off
premake5 vs2017
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)
