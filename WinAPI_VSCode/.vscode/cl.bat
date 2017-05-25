@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

cd /D "%~dp1"

if exist makefile (
	nmake /NOLOGO || goto end
	goto good_nmake
) else (
	cl /utf-8 /W4 "%~nx1" || goto end
	goto good_cl
)

if ERRORLEVEL 1 goto end

:good_cl
	del "%~n1.obj"
	cls
	call "%~n1.exe"
	goto end

:good_nmake
	for %%i in (*.exe) do start "" /b "%%i"

:end
rem Nothing here
