
::ɾ��Ŀ¼
rd /s /q version_lua
rd /s /q version

::������ĿresĿ¼��Դ����ǰversionĿ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak
robocopy %~dp0\..\..\BaLe\res version\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak
robocopy %~dp0\..\..\BaLe\src version\src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak




