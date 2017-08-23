
::删除目录
rd /s /q all_src
rd /s /q encrypt_lua

set par=%1

set copyDir_1=%~dp0\..\..\BaLe\src
set copyDir_2=%~dp0\..\..\..\%agent_client%\BaLe\src

::拷贝项目src目录资源到当前all_src目录下，去除*.tmp *.zip *.rar *.svn *.bak
if "%par%"=="456" (robocopy %copyDir_2% all_src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak) else (robocopy %copyDir_1% all_src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak)

::加密lua
call compile_lua.bat

::删除all_src下的所有lua文件
for /f "delims=" %%i in ('dir /b/a-d/s %~dp0all_src\*.lua') do del %%i /q /f

::对encrypt_lua目录下加密过的luac文件合并到all_src目录
xcopy encrypt_lua all_src /s /e /y

::删除目录
rd /s /q encrypt_lua



