@echo off
setlocal EnableDelayedExpansion
set CurrentPath=%~dp0
set "ProjectPath=%CurrentPath:~0,-1%"
for %%I in ("%ProjectPath%") do set "ProjectPath=%%~dpI"

set PathFile=%ProjectPath%\src\common\path.h

if not exist "%PathFile%" (
 echo %PathFile% not exist
 exit /b 1
)

(
 echo #ifndef Path_h
 echo #define Path_h
 echo #include ^<string^>
 echo #if defined(_WIN64^) ^|^| defined(_WIN32^)
 echo const std::string ProjectPath = R"(%ProjectPath%)";
 echo #endif
 echo #endif
) > %PathFile%
