
::从本地项目拷贝资源到当前目录的version目录
cd generate_debug_file_md5
call copy_res.bat

::编译lua为lua字节码
::call luacompile.bat

::拷贝res目录
::robocopy version_lua\res version\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::生成文件md5
python generate_debug.py

::远程更新服务器地址(目前在内网)
set update_dir=\\DESKTOP-36PF8JG\update_resource\

::拷贝资源到远程地址
robocopy version\src %update_dir%src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak
robocopy version\res %update_dir%res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::拷贝project_dev.manifest, version_dev.manifest到远程地址 
copy /y manifest\project_dev.manifest %update_dir%project_dev.manifest
copy /y manifest\version_dev.manifest %update_dir%version_dev.manifest

::是否打apk包

set chooseResult=y

if %chooseResult%==y (
::删除项目的encrypt_res目录目录
rd /s /q %~dp0..\BaLe\encrypt_res

::拷贝version目录资源到项目的encrypt_res目录下，去除*.tmp *.zip *.rar *.svn *.bak
robocopy version %~dp0..\BaLe\encrypt_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::拷贝project_dev.manifest到工程目录version下并重命名为project.manifest
copy /y manifest\project_dev.manifest %~dp0..\BaLe\encrypt_res\res\version\project.manifest

cd ../genarate_apk
call genarate_apk_debug.bat
) else (
	echo ok
)