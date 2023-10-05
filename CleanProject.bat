@echo off
rd /S /Q .vs
rd /S /Q bin
rd /S /Q int
del *.sln
del src\*.vcxproj*
del *.ipch
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)
