@echo off

if not defined VC_ENV_SET (
	call "G:\Visual Studio 2022\VC\Auxiliary\Build\vcvars64.bat"
	set VC_ENV_SET=1
)

setlocal

cl src\*.c /O2 /MT /I"include" /Fo.\release\obj\ /Ferelease\JMF_Engine2D.exe /link /NODEFAULTLIB:LIBCMT /LIBPATH:"lib" user32.lib opengl32.lib gdi32.lib freetype.lib

endlocal
