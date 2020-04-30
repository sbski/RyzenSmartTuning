REM chcp 65001
@echo OFF

:start
if /I "%~1" EQU "-i" goto :install
if /I "%~1" EQU "-u" goto :uninstall
if /I NOT "%~1" EQU "-u" (
IF /I NOT "%~1" EQU "-i" ( 
echo Enter Parameter "-i" to install the driver "-u" to uninstall the driver 
goto :end)
)

:uninstall
goto stop_services

:stop_services
	echo.
	echo.
	echo Stopping AMDProfilerLoadService service...

	sc stop AMDProfilerLoadService >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDProfilerLoadService service stoppped.
	) else 	(
		echo AMDProfilerLoadService has not been started.
	)

	echo Deleting AMDProfilerLoadService service...
	sc delete AMDProfilerLoadService >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDProfilerLoadService service deleted.
	) else 	(
		echo AMDProfilerLoadService does not exists as an installed service
	)

	echo Stopping AMDCpuProfiler service...
	sc stop AMDCpuProfiler >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDCpuProfiler service stoppped.
	) else 	(
		echo AMDCpuProfiler services has not been started.
	)

	echo Stopping AMDPowerProfiler service...
	sc stop AMDPowerProfiler >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDPowerProfiler service stoppped.
	) else 	(
		echo AMDPowerProfiler services has not been started.
	)

	echo Stopping AMDPcore service...
	sc stop AMDPcore >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDPcore service stoppped.
	) else 	(
		echo AMDPcore services has not been started.
	)

	goto delete_services
   	
:delete_services
	echo.
	echo.
	echo Deleting AMDCpuProfiler service...
	sc delete AMDCpuProfiler >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDCpuProfiler service deleted.
	) else 	(
		echo AMDCpuProfiler does not exists as an installed service.
	)

	echo Deleting AMDPowerProfiler service...
	sc delete AMDPowerProfiler >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDPowerProfiler service deleted.
	) else 	(
		echo AMDPowerProfiler does not exists as an installed service
	)

	echo Deleting AMDPcore service...
	sc delete AMDPcore >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDPcore service deleted.
	) else 	(
		echo AMDPcore does not exists as an installed service
	)

	goto delete_files

     	
: delete_files 
   	echo.
   	echo.
   	echo Deleting installed service files from C:\Windows\System32\drivers directory
	set folder=""%Systemroot%\System32\drivers"
	pushd %folder% 
	del /s AMDCpuProfiler.* >nul 2>&1
	del /s AMDPowerProfiler.* >nul 2>&1
	del /s AMDPcore.* >nul 2>&1
	popd
	goto verify_service

:install
goto create_servcice

:create_servcice
	set system32=""%Systemroot%\System32"
	echo Creating and starting AMDProfilerLoadService which will creates and start AMDPcore, AMDPowerProfiler and AMDCpuProfiler services.
	pushd %system32%
	set bin_path="%~dp0AMDProfilerLoadService.exe" 

	sc.exe create AMDProfilerLoadService type= own start= auto binPath= %bin_path%   >nul 2>&1
	sc description AMDProfilerLoadService "AMD uProf Driver Load Service"
	sc.exe start AMDProfilerLoadService >nul 2>&1
	popd
	goto verify_service
		
:verify_service
	echo.
	echo.
	echo Verifing AMDProfilerLoadService, AMDPcore, AMDPowerProfiler and AMDCpuProfiler services....
	set ret=0
	pushd %system32%
	timeout /t 5 > nul
	sc.exe query AMDProfilerLoadService | findstr "STATE" | findstr "RUNNING"  >nul 2>&1

	if %errorLevel% == 0 (
		echo AMDProfilerLoadService is running.
	) else 	(
		call :service_failed AMDProfilerLoadService
	)  

	sc.exe query AMDPcore | findstr "STATE" | findstr "RUNNING" >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDPcore is running.
	) else 	(
		call :service_failed AMDPcore
	)  

	sc.exe query AMDPowerProfiler | findstr "STATE" | findstr "RUNNING" >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDPowerProfiler is running.
	) else 	(
		call :service_failed AMDPowerProfiler 
	)  

	sc.exe query AMDCpuProfiler | findstr "STATE" | findstr "RUNNING" >nul 2>&1
	if %errorLevel% == 0 (
		echo AMDCpuProfiler is running.
	) else 	(
		call :service_failed AMDCpuProfiler
	)  
     
	goto :EOF	

:service_failed
	echo %* is not running. Please reboot your machine.
	popd
	exit /B 0
