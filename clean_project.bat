@echo off
rd /S /Q .vs
rd /S /Q bin
del RDRMenu2.sln
del src\*.vcxproj*
del *.ipch
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)
