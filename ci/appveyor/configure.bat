@echo off

set CMAKE_GENERATOR=Ninja

if /i "%1" == "x86" goto :x86
if /i "%1" == "i386" goto :x86
if /i "%1" == "amd64" goto :amd64
if /i "%1" == "x86_64" goto :amd64
if /i "%1" == "x64" goto :amd64

echo Invalid argument: '%1'
exit -1

:x86
set TARGET_CPU=x86
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"

:amd64
set TARGET_CPU=amd64
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

:finalize
if "%TARGET_CPU%" == "" goto :amd64
if "%CONFIGURATION%" == "" (set CONFIGURATION=Release)

set CMAKE_CONFIGURE_FLAGS=-G "%CMAKE_GENERATOR%" -DCMAKE_BUILD_TYPE="%CONFIGURATION%" -DTARGET_CPU="%TARGET_CPU%"

cmake -S . -B ./build %CMAKE_CONFIGURE_FLAGS%
