@echo off
set ARCH=32

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars%ARCH%.bat"

set output=.\GetData.exe

set Gflags=/Gy 
set Dflags=
set Oflags=/O1 
set Zflags=
set flags =/Fo.\obj\ /MT 

set xfiles = GetData.cpp

cl GetData.cpp /MT /O1 /GR- /Ob1 /Oy /GS- /Oi /GL /Os /GF /Gy- /GA /std:c++20 /nologo ^
     /link /MANIFEST:NO  /entry:mainCRTStartup /out:%output%