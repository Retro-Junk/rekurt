@echo off
set CDIR=..\tc201
set CINC=%CDIR%\include
set CLIB=%CDIR%\lib
set path=%CDIR%\bin

%CDIR%\bgi\bgiobj.exe %CDIR%\bgi\egavga.bgi egavga.obj _EGAVGA_driver
tcc -eKURTAN.EXE EGAVGA.OBJ KURT0.C GPRINTF.C KURTAN.C GRAPHICS.LIB CL.LIB
