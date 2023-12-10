@echo off

if not defined VC_ENV_SET (
	call "G:\Visual Studio 2022\VC\Auxiliary\Build\vcvars64.bat"
	set VC_ENV_SET=1
)

setlocal

cl src\*.c /Zi /DEBUG /D DEBUG_BUILD /MTd /I"include" /Fo.\debug\obj\ /Fedebug\JMF_Engine2D.exe user32.lib opengl32.lib gdi32.lib

endlocal
