@echo off
setlocal ENABLEDELAYEDEXPANSION

set bindir=bin\
set srcdir=..\src\

if not exist %bindir% mkdir %bindir%
pushd %bindir%


set cFlags=/Z7 /GF /FC /Oi /EHac /fp:fast /std:c++latest /MT /D"BUILD_DEBUG=1" /wd4464 /wd4514 /wd4505 /wd4710 /wd5045 /wd4711 /nologo /link /incremental:no
set cFlags=/Od %cFlags%
::set cFlags=/Wall %cFlags%
::set cFlags=/O2 /Ob3 /GL %cFlags% /LTCG
if "%1" == "dll" (
    echo definitions...
    set cFlags=/D"TL_API_DLL" %cFlags%
    cl %srcdir%definitions.cpp /LD !cFlags! /out:tl.dll /IMPLIB:"tl.lib"
    if %errorlevel% neq 0 goto fail
    set cFlags=!cFlags! tl.lib
) else (
    set cFlags=%srcdir%definitions.cpp %cFlags%
)

echo main_sse...
cl %srcdir%main.cpp %cFlags% /out:main_sse.exe
if %errorlevel% neq 0 goto fail
::echo main_avx...
::cl %srcdir%main.cpp /arch:AVX %cFlags% /out:main_avx.exe
::if %errorlevel% neq 0 goto fail
::echo main_avx2...
::cl %srcdir%main.cpp /arch:AVX2 %cFlags% /out:main_avx2.exe
::if %errorlevel% neq 0 goto fail

goto success

:fail
echo [31mCompilation failed[0m
goto end
:success
echo [32mCompilation succeeded[0m
:end
popd