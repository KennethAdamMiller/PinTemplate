@echo off

rd /s /q CMakeFiles
rd /s /q src
del cmake_install.cmake
del CMakeCache.txt
del Makefile

cmake -G "NMake Makefiles" ..
