
::删除项目的encrypt_res目录目录
rd /s /q %~dp0..\..\BaLe\encrypt_res

::拷贝version目录资源到项目的encrypt_res目录下，去除*.tmp *.zip *.rar *.svn *.bak
robocopy version %~dp0..\..\BaLe\encrypt_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::拷贝project_dev.manifest到工程目录version下并重命名为project.manifest
copy /y manifest\project_dev.manifest %~dp0..\..\BaLe\encrypt_res\res\version\project.manifest


::拷贝更新资源到更新svn目录下
svnpath
set svnPath_debug="E:\update_resource_debug"
set svnPath_release="E:\update_resource"

::for /f "delims== tokens=1*" %%a in ('type host_model ^|findstr /i "model"') do set bRelease=%%b
set /p bRelease=<"host_model"

if "%bRelease%" == "false" ( 
set svnpath=%svnPath_debug%
) else (
set svnpath=%svnPath_release%
)

::删除目录
::rd /s /q %svnpath%\res
::rd /s /q %svnpath%\src

::拷贝version目录资源到更新服目录下，去除*.tmp *.zip *.rar *.svn *.bak （本地使用）
robocopy version\res %svnpath%\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak
robocopy version\src %svnpath%\src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::拷贝manifest目录资源到更新服目录下，去除*.tmp *.zip *.rar *.svn *.bak （本地使用）
copy manifest %svnpath%

