
::ɾ����Ŀ��encrypt_resĿ¼Ŀ¼
rd /s /q %~dp0..\..\BaLe\encrypt_res

::����versionĿ¼��Դ����Ŀ��encrypt_resĿ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak
robocopy version %~dp0..\..\BaLe\encrypt_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::����project_dev.manifest������Ŀ¼version�²�������Ϊproject.manifest
copy /y manifest\project_dev.manifest %~dp0..\..\BaLe\encrypt_res\res\version\project.manifest


::����������Դ������svnĿ¼��
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

::ɾ��Ŀ¼
::rd /s /q %svnpath%\res
::rd /s /q %svnpath%\src

::����versionĿ¼��Դ�����·�Ŀ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak ������ʹ�ã�
robocopy version\res %svnpath%\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak
robocopy version\src %svnpath%\src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::����manifestĿ¼��Դ�����·�Ŀ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak ������ʹ�ã�
copy manifest %svnpath%

