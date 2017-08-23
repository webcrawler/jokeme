
::删除目录
rd /s /q version
rd /s /q manifest

set param=%1

::拷贝..\encryptImage\all_res目录资源到当前res目录下，去除*.tmp *.zip *.rar *.svn *.bak
robocopy %~dp0..\encryptImage\all_res version\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak


::拷贝..\encryptLua\all_src目录资源到当前src目录下，去除*.tmp *.zip *.rar *.svn *.bak
robocopy %~dp0..\encryptLua\all_src version\src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::不更新project.manifest
::删除res\version\project.manifest
del version\res\version\project.manifest

::生成文件md5
if "%param%"=="456" (python generate_for_win.py) else (python generate.py)
