@echo off
cd src
PATH \tools\elf_cross\bin;\Mingw\bin;\projects\KernelBuildTools
\Mingw\bin\mingw32-make.exe clean
pause