%该目录下从Resources拷贝资源到My_Android_Resources下，踢出*.tmp *.zip *.svn *.bak这些文件%
robocopy Resources My_Android_Resources /e /xf /mir /xa:h *.tmp *.zip *.svn *.bak

set path=E:\MyWork\FengHuangNew\My_Android_Resources\res\Game\
@ECHO 删除Game资源......
rd /s /q %path%AllHappy
rd /s /q %path%BMW
rd /s /q %path%Four
rd /s /q %path%LYLand
rd /s /q %path%MooCow
rd /s /q %path%Zhajinhua
rd /s /q %path%TexasPoker
@ECHO ------------------------------------------- Android Res 拷贝完成！-------------------------------------------

::os.system('E:\MyWork\FengHuangNew\copy_res_for_android.bat')