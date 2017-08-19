md publish
md publish\include
md publish\lib
md publish\lib\Debug
md publish\lib\Release
xcopy .\UVSocket\*.h .\publish\include /Y
xcopy .\Debug\UVSocket.lib .\publish\lib\Debug /Y
xcopy .\Release\UVSocket.lib .\publish\lib\Release /Y
xcopy .\libuv\*.* .\publish\*.* /Y/e