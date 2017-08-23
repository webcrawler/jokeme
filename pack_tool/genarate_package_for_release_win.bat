
set agent_client=agent_client

::加密lua
cd encryptLua
call encrypt_lua.bat 456

::加密图片
cd ../encryptImage
call encrypt_all_img.bat 456

::生成文件md5列表
cd ../generate_file_md5
call genarate_file_md5.bat 456

::删除项目的encrypt_res目录目录
rd /s /q %~dp0..\..\agent_client\BaLe\encrypt_res

::拷贝version目录资源到项目的encrypt_res目录下，去除*.tmp *.zip *.rar *.svn *.bak
robocopy version %~dp0..\..\%agent_client%\BaLe\encrypt_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::拷贝project_dev.manifest到工程目录version下并重命名为project.manifest
copy /y manifest\project_dev.manifest %~dp0..\..\%agent_client%\BaLe\encrypt_res\res\version\project.manifest

set /p bRelease=<"host_model"
if "%bRelease%" == "false" ( 
@echo "model: debug"
) else (
@echo "model: release"
)

pause
