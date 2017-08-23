
::删除目录
rd /s /q version_lua
rd /s /q version

::拷贝项目res目录资源到当前version目录下，去除*.tmp *.zip *.rar *.svn *.bak
robocopy %~dp0\..\..\BaLe\res version\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak
robocopy %~dp0\..\..\BaLe\src version\src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak




