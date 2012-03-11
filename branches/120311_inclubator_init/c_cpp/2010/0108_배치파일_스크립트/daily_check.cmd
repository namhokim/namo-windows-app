@echo off
echo [서비스]
sc query uPortSvc | find "SERVICE_NAME"
sc query uPortSvc | find "상태"
sc query uPortSvcAgent.exe | find "SERVICE_NAME"
sc query uPortSvcAgent.exe | find "상태"
sc query Tomcat6 | find "SERVICE_NAME"
sc query Tomcat6 | find "상태"
sc query CUBRIDService | find "SERVICE_NAME"
sc query CUBRIDService | find "상태"
sc query LPDSVC | find "SERVICE_NAME"
sc query LPDSVC | find "상태"
sc query SKeeper | find "SERVICE_NAME"
sc query SKeeper | find "상태"
echo.
echo [Network]
ipconfig.exe
echo.
echo [DB Relay Station]
tasklist | find "DB relay station.exe"
notepad.exe output.txt
taskmgr.exe
"C:\Program Files\DoubleTake\DTMgmtCon.exe"
"C:\Program Files\DoubleTake\fcc.exe"
REM 오늘 날짜의 로그
notepad "D:\SINDOH\uPortAgent\AgentLog\uPortCheck-%date%.txt"
REM 어제 날짜의 로그
for /f "tokens=1-3 delims=- " %%A in ('echo %DATE%') do (set YY=%%A&SET MM=%%B&SET DD=%%C)

set /a DD=1%DD%-101
set /a MM=1%MM%-100

if  %DD% GTR 0 goto end
set /a MM=%MM%-1

if %MM% GTR 0 goto leap

set /a DD=31
set /a MM=12
set /a YY=%YY%-1
REM echo %YY%, %MM%, %DD% -> 2005, 12, 31
goto end

:leap
set /a TT=%YY%/4
set /a TT=%TT%*4
if not %YY% equ %TT% goto mon%MM%
set /a DD=29
goto end

:mon1
:mon3
:mon5
:mon7
:mon8
:mon10
set /a DD=31
goto end

:mon2
set /a DD=28
goto end

:mon4
:mon6
:mon9
:mon11
set /a DD=30
goto end

:end
set /a DD=%DD% + 100
set DD=%DD:~1,2%
set /a MM=%MM% + 100
set MM=%MM:~1,2%

notepad "D:\SINDOH\uPortAgent\AgentLog\uPortCheck-%YY%-%MM%-%DD%.txt"
"C:\CUBRID\cubridmanager\cubridmanager.exe"