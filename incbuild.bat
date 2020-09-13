@echo off
rem =====================================================
rem =====   simple project for build sm solution   ======
rem =====================================================
rem Jason 2010-12-15
@echo simple call incredibuild to build project.

set BUILD_PARAM1=/build
set BUILD_CMD=buildconsole
set BUILD_PARAM2=
set BUILD_PARAM3=/ShowTime /OpenMonitor
set BUILD_PARAM4=
set BUILD_PARAM5=/log=.\log\build.log
set TITLE=/Title="@Jason's mini build tool.@"
set CMD_STR=

if not exist .\log\ md log
cd log
del /q *.*
cd ..

if {%1} == {} goto error_param_null
set BUILD_PRJ="%1"

if "%2"=="clean" set BUILD_PARAM1=/clean
if "%2"=="unit" set BUILD_PARAM1=/compile=
if "%2"=="r" set BUILD_PARAM1=/rebuild
if "%2"=="c" set BUILD_PARAM1=/build
if "%2"=="b" set BUILD_PARAM1=/preset=
set BUILD_PARAM2=%3
if {%3}=={} set BUILD_PARAM2=/cfg="Release|Win32"
goto build

:build
@echo building %BUILD_PRJ% now.
if "%2"=="clean" goto cleanup
if "%2"=="unit" goto unit
if "%2"=="b" set CMD_STR=%BUILD_CMD% %BUILD_PRJ% %BUILD_PARAM1%%BUILD_PARAM2% %BUILD_PARAM3% %BUILD_PARAM5% %TITLE%
if "%2"=="b" goto cmd
set CMD_STR=%BUILD_CMD% %BUILD_PRJ% %BUILD_PARAM1% %BUILD_PARAM2% %BUILD_PARAM3% %BUILD_PARAM5% %TITLE%
goto cmd

:cmd
%CMD_STR%
goto end

:error_param_null
@echo input error! param must not be empty!
@echo usage :
@echo incbuild [proj] [r/c/b/clean/unit] [Padding]
@echo 	r , rebuild
@echo 	c , build
@echo 	b , batch build
@echo 	clean , cleanup
@echo 	unit , compile units
goto end

:cleanup
%BUILD_CMD% %BUILD_PRJ% %BUILD_PARAM1% %BUILD_PARAM2% /ShowTime %BUILD_PARAM5% %TITLE%
goto end

:unit
set BUILD_PARAM4=%4
if {%4}=={} set BUILD_PARAM4=/cfg="Release|Win32"
%BUILD_CMD% %BUILD_PRJ% %BUILD_PARAM1%%BUILD_PARAM2% %BUILD_PARAM4% /ShowTime %BUILD_PARAM5% %TITLE%
goto end

:end
@echo terminate.
@echo press any key to exit.
@pause
