
set agent_client=agent_client

::����lua
cd encryptLua
call encrypt_lua.bat 456

::����ͼƬ
cd ../encryptImage
call encrypt_all_img.bat 456

::�����ļ�md5�б�
cd ../generate_file_md5
call genarate_file_md5.bat 456

::ɾ����Ŀ��encrypt_resĿ¼Ŀ¼
rd /s /q %~dp0..\..\agent_client\BaLe\encrypt_res

::����versionĿ¼��Դ����Ŀ��encrypt_resĿ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak
robocopy version %~dp0..\..\%agent_client%\BaLe\encrypt_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::����project_dev.manifest������Ŀ¼version�²�������Ϊproject.manifest
copy /y manifest\project_dev.manifest %~dp0..\..\%agent_client%\BaLe\encrypt_res\res\version\project.manifest

set /p bRelease=<"host_model"
if "%bRelease%" == "false" ( 
@echo "model: debug"
) else (
@echo "model: release"
)

pause
