
::ɾ��Ŀ¼
rd /s /q version
rd /s /q manifest

set param=%1

::����..\encryptImage\all_resĿ¼��Դ����ǰresĿ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak
robocopy %~dp0..\encryptImage\all_res version\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak


::����..\encryptLua\all_srcĿ¼��Դ����ǰsrcĿ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak
robocopy %~dp0..\encryptLua\all_src version\src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::������project.manifest
::ɾ��res\version\project.manifest
del version\res\version\project.manifest

::�����ļ�md5
if "%param%"=="456" (python generate_for_win.py) else (python generate.py)
