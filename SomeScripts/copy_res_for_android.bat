::��Ŀ¼�´�Resources������Դ��My_Android_Resources�£��߳�*.tmp *.zip *.svn *.bak��Щ�ļ� 
robocopy Resources My_Android_Resources /e /xf /mir /xa:h *.tmp *.zip *.svn *.bak

::set path=E:\MyWork\FengHuangNew\My_Android_Resources\res\Game\
::%~dp0  ��ǰĿ¼
set path=My_Android_Resources\res\Game\

@echo ɾ��Game��Դ......
rmdir /s /q %path%xxx1
rmdir /s /q %path%xxx2

@echo ------------------------------------------- Android Res ������ɣ�-------------------------------------------

::os.system('E:\MyWork\copy_res_for_android.bat')