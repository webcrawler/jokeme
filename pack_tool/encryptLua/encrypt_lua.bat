
::ɾ��Ŀ¼
rd /s /q all_src
rd /s /q encrypt_lua

set par=%1

set copyDir_1=%~dp0\..\..\BaLe\src
set copyDir_2=%~dp0\..\..\..\%agent_client%\BaLe\src

::������ĿsrcĿ¼��Դ����ǰall_srcĿ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak
if "%par%"=="456" (robocopy %copyDir_2% all_src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak) else (robocopy %copyDir_1% all_src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak)

::����lua
call compile_lua.bat

::ɾ��all_src�µ�����lua�ļ�
for /f "delims=" %%i in ('dir /b/a-d/s %~dp0all_src\*.lua') do del %%i /q /f

::��encrypt_luaĿ¼�¼��ܹ���luac�ļ��ϲ���all_srcĿ¼
xcopy encrypt_lua all_src /s /e /y

::ɾ��Ŀ¼
rd /s /q encrypt_lua



