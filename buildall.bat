
REM please adjust the path to your Visual Studio installation

call "d:\programme\Microsoft Visual Studio 12.0\Common7\Tools\VsDevCmd.bat"
msbuild /t:build /p:Configuration=debug;Platform=Win32
msbuild /t:build /p:Configuration=release;Platform=Win32
msbuild /t:build /p:Configuration=debug;Platform=x64
msbuild /t:build /p:Configuration=release;Platform=x64
pause