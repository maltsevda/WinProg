@echo off

if exist "%PROGRAMFILES(X86)%" (
	call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
) else (
	call "C:\Program Files\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
)

cd /D "%1"

nmake /NOLOGO || goto end

rem for %%i in (*.exe) do start "" /b "%%i"

:end
rem Nothing here
