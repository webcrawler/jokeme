
::ɾ��Ŀ¼
rd /s /q all_res
rd /s /q img_res
rd /s /q img_res_encrypt

set par=%1

set copyDir_1=%~dp0\..\..\BaLe\res
set copyDir_2=%~dp0\..\..\..\%agent_client%\BaLe\res

::������ĿresĿ¼��Դ����ǰall_resĿ¼�£�ȥ��*.tmp *.zip *.rar *.svn *.bak
if "%par%"=="456" (robocopy %copyDir_2% all_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak) else (robocopy %copyDir_1% all_res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak)


::������ǰall_resĿ¼�µ�png��jpg�ļ���Ŀ¼img_res��
robocopy all_res img_res /e /xa:h *.jpg *.png /s

::xxtea����Ŀ¼img_res�µ�ͼƬ��Դ��Ŀ¼img_res_encrypt��
call %~dp0\..\quick_encrypt_bin\pack_files.bat -i img_res -o img_res_encrypt -ek ekudjrjf -es r4gdegf


::ɾ��Ŀ¼ (��Ŀ¼����Դ������) ���������ò����ܵ���ԴĿ¼
rd /s /q img_res_encrypt\hall\share



::�ϲ��Ѿ����ܹ���ͼƬ��Դimg_res_encrypt��all_resĿ¼
xcopy img_res_encrypt all_res /s /e /y

::ɾ��Ŀ¼
rd /s /q img_res
rd /s /q img_res_encrypt



