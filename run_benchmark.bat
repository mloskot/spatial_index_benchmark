@echo off
IF NOT EXIST "%1" GOTO :NOBUILDDIR

for /r %%f in (%1\*.exe) do %%f
GOTO :EOF

:NOBUILDDIR
@echo Cannot find '%1' build directory