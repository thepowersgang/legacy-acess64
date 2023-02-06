@echo off
PATH \tools\elf_cross\bin;\Mingw\bin;\projects\KernelBuildTools
cd src
\Mingw\bin\mingw32-make.exe all
cd ..
copy Acess64.bin b:
pause