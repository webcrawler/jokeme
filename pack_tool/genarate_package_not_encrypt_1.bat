
::�ӱ�����Ŀ������Դ����ǰĿ¼��versionĿ¼
cd generate_debug_file_md5
call copy_res.bat

::����luaΪlua�ֽ���
::call luacompile.bat

::����resĿ¼
::robocopy version_lua\res version\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::�����ļ�md5
python generate_debug.py

::Զ�̸��·�������ַ(Ŀǰ������)
set update_dir=\\DESKTOP-36PF8JG\update_resource\

::������Դ��Զ�̵�ַ
robocopy version\src %update_dir%src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak
robocopy version\res %update_dir%res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::����project_dev.manifest, version_dev.manifest��Զ�̵�ַ 
copy /y manifest\project_dev.manifest %update_dir%project_dev.manifest
copy /y manifest\version_dev.manifest %update_dir%version_dev.manifest

::�Ƿ��apk��

set chooseResult=y

if %chooseResult%==y (
::ɾ����Ŀ��encrypt_resĿ¼Ŀ¼
rd /s /q %~dp0..\BaLe\encrypt_res

::����versionĿ¼��Դ����Ŀ��encrypt_resĿ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak
robocopy version %~dp0..\BaLe\encrypt_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::����project_dev.manifest������Ŀ¼version�²�������Ϊproject.manifest
copy /y manifest\project_dev.manifest %~dp0..\BaLe\encrypt_res\res\version\project.manifest

cd ../genarate_apk
call genarate_apk_debug.bat
) else (
	echo ok
)