
::删除目录
rd /s /q all_res
rd /s /q img_res
rd /s /q img_res_encrypt

set par=%1

set copyDir_1=%~dp0\..\..\BaLe\res
set copyDir_2=%~dp0\..\..\..\%agent_client%\BaLe\res

::拷贝项目res目录资源到当前all_res目录下，去除*.tmp *.zip *.rar *.svn *.bak
if "%par%"=="456" (robocopy %copyDir_2% all_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak) else (robocopy %copyDir_1% all_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak)


::拷贝当前all_res目录下的png和jpg文件到目录img_res下
robocopy all_res img_res /e /xa:h *.jpg *.png /s

::xxtea加密目录img_res下的图片资源到目录img_res_encrypt下
call %~dp0\..\quick_encrypt_bin\pack_files.bat -i img_res -o img_res_encrypt -ek ekudjrjf -es r4gdegf


::删除目录 (该目录下资源不加密) 在这里配置不加密的资源目录
rd /s /q img_res_encrypt\hall\share



::合并已经加密过的图片资源img_res_encrypt到all_res目录
xcopy img_res_encrypt all_res /s /e /y

::删除目录
rd /s /q img_res
rd /s /q img_res_encrypt



