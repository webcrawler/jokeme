%��Ŀ¼�´�Resources������Դ��My_Android_Resources�£��߳�*.tmp *.zip *.svn *.bak��Щ�ļ�%
robocopy Resources My_Android_Resources /e /xf /mir /xa:h *.tmp *.zip *.svn *.bak

set path=E:\MyWork\FengHuangNew\My_Android_Resources\res\Game\
@ECHO ɾ��Game��Դ......
rd /s /q %path%AllHappy
rd /s /q %path%BMW
rd /s /q %path%Four
rd /s /q %path%LYLand
rd /s /q %path%MooCow
rd /s /q %path%Zhajinhua
rd /s /q %path%TexasPoker
@ECHO ------------------------------------------- Android Res ������ɣ�-------------------------------------------

::os.system('E:\MyWork\FengHuangNew\copy_res_for_android.bat')