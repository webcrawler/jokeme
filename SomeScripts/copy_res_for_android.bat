::��Ŀ¼�´�Resources������Դ��My_Android_Resources�£��߳�*.tmp *.zip *.svn *.bak��Щ�ļ�
robocopy Resources My_Android_Resources /e /xf /mir /xa:h *.tmp *.zip *.svn *.bak

::ɾ��xx1,xx2Ŀ¼����Դ
set path=E:\xxx\My_Android_Resources\res\Game\
@echo ɾ��xx1,xx2Ŀ¼����Դ......
rd /s /q %path%xx1
rd /s /q %path%xx2
@echo ------ Android Res ������ɣ�------

::python ���ø�bat�ļ�
::os.system('E:\xxx\copy_res_for_android.bat')