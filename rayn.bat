@echo off

if %# == 0 ( echo usage: %0 scene & quit )

set scene=%@path[%1]%@name[%1]

.\rayn.exe %scene.txt -o %scene.tga %2$

if errorlevel == 0 start %scene.tga

unset scene
