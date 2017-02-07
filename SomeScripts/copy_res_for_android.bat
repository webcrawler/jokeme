::该目录下从Resources拷贝资源到My_Android_Resources下，踢出*.tmp *.zip *.svn *.bak这些文件 
robocopy Resources My_Android_Resources /e /xf /mir /xa:h *.tmp *.zip *.svn *.bak

::set path=E:\MyWork\FengHuangNew\My_Android_Resources\res\Game\
::%~dp0  当前目录
set path=My_Android_Resources\res\Game\

@echo 删除Game资源......
rmdir /s /q %path%xxx1
rmdir /s /q %path%xxx2

@echo ------------------------------------------- Android Res 拷贝完成！-------------------------------------------

::os.system('E:\MyWork\copy_res_for_android.bat')