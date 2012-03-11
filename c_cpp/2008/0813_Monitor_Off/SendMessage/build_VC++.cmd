@echo off
@SET VSINSTALLDIR=d:\Program Files\Microsoft Visual Studio 8
@SET VCINSTALLDIR=d:\Program Files\Microsoft Visual Studio 8\VC
@SET FrameworkDir=C:\WINDOWS\Microsoft.NET\Framework
@SET FrameworkVersion=v2.0.50727
@SET FrameworkSDKDir=d:\Program Files\Microsoft Visual Studio 8\SDK\v2.0
@if "%VSINSTALLDIR%"=="" goto error_no_VSINSTALLDIR
@if "%VCINSTALLDIR%"=="" goto error_no_VCINSTALLDIR

@echo Setting environment for using Microsoft Visual Studio 2005 x86 tools.

@rem
@rem Root of Visual Studio IDE installed files.
@rem
@set DevEnvDir=d:\Program Files\Microsoft Visual Studio 8\Common7\IDE

@set PATH=d:\Program Files\Microsoft Visual Studio 8\Common7\IDE;d:\Program Files\Microsoft Visual Studio 8\VC\BIN;d:\Program Files\Microsoft Visual Studio 8\Common7\Tools;d:\Program Files\Microsoft Visual Studio 8\Common7\Tools\bin;d:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;d:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\bin;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;d:\Program Files\Microsoft Visual Studio 8\VC\VCPackages;%PATH%
@set INCLUDE=d:\Program Files\Microsoft Visual Studio 8\VC\ATLMFC\INCLUDE;d:\Program Files\Microsoft Visual Studio 8\VC\INCLUDE;d:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\include;%INCLUDE%
@set LIB=d:\Program Files\Microsoft Visual Studio 8\VC\ATLMFC\LIB;d:\Program Files\Microsoft Visual Studio 8\VC\LIB;d:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\lib;d:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\lib;%LIB%
@set LIBPATH=C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;d:\Program Files\Microsoft Visual Studio 8\VC\ATLMFC\LIB

@goto end

:error_no_VSINSTALLDIR
@echo ERROR: VSINSTALLDIR variable is not set. 
@goto end

:error_no_VCINSTALLDIR
@echo ERROR: VCINSTALLDIR variable is not set. 
@goto end

:end

cl.exe -O1 monitoroff.c User32.Lib
rc.exe monitoroff.rc
if exist monitoroff.exe del monitoroff.exe
link /out:monitoroff.exe /subsystem:windows monitoroff.obj monitoroff.res User32.lib
if exist monitoroff.obj del monitoroff.obj
if exist monitoroff.res del monitoroff.res
pause