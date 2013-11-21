PinTemplate
===========

A template of pintool project.

Building pintool is very complicated, so I made this template that would helps you create and build pintool more easier.

##Usage

###Create project

1. Install [CMake](http://www.cmake.org) 2.6 or above.

2. Download [pin tool sdk](http://software.intel.com/en-us/articles/pintool) corresponding to your complier.

3. Fork this repo and clone to your machine.

4. Config the pin sdk path and output binary name in 'pincfg.txt'.

5. Change the default project name in 'CMakeLists.txt' to your project name
```
    PROJECT(PINTOOL)
```
6. Copy source files to the main folder.

7. Done!

##Complie 

Assuming that compiling with Windows SDK 7.1.

###For 32bit target

1. Run SetEnv.cmd with arguments `'/x86 /xp'`.

2. Change directory to bld32.

3. Run ..\mk.bat to generate NMake Makefile.

4. Use 'NMake' to build the target.

###For 64bit target

1. Run SetEnv.cmd with arguments `'/x64 /xp'`.

2. Change directory to bld64.

3. Run ..\mk.bat to generate NMake Makefile.

4. Use 'NMake' to build the target.

## Changelog

2013.11.20

- First public verison.
