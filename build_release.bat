@echo off

if not defined VC_ENV_SET (
	call "G:\Visual Studio 2022\VC\Auxiliary\Build\vcvars64.bat"
	set VC_ENV_SET=1
)

setlocal

cl src\*.c /O2 /MT /I"include" /Forelease\ /Ferelease\JMF_Engine.exe user32.lib opengl32.lib gdi32.lib

endlocal
