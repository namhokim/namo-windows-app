@echo off

lcc -O monitoroff.c
=> cl.exe -O1 monitoroff.c User32.Lib

lrc monitoroff.rc
=> rc.exe monitoroff.rc
lcclnk -o ..\release\monitoroff.exe -s -subsystem windows monitoroff.obj monitoroff.res
=> link /out:monitoroff.exe /subsystem:windows monitoroff.obj monitoroff.res User32.lib
if exist monitoroff.obj del monitoroff.obj
if exist monitoroff.res del monitoroff.res

pause