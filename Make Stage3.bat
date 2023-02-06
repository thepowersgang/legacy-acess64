@echo off
cd stage3
call "Make All.bat"
cd ..
copy A64St3.bin b: