@echo off
C:\msys64\ucrt64\bin\g++.exe -std=c++17 -Wall -Wextra -Iinclude src\main.cpp src\menu\*.cpp src\patient\*.cpp src\staff\*.cpp src\storage\*.cpp src\triage\*.cpp -o TriageSystem.exe
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)
echo Build succeeded: TriageSystem.exe
