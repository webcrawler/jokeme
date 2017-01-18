::该目录下从Resources拷贝资源到My_Android_Resources下，踢出*.tmp *.zip *.svn *.bak这些文件
robocopy Resources My_Android_Resources /e /xf /mir /xa:h *.tmp *.zip *.svn *.bak

::删除xx1,xx2目录下资源
set path=E:\xxx\My_Android_Resources\res\Game\
@echo 删除xx1,xx2目录下资源......
rd /s /q %path%xx1
rd /s /q %path%xx2
@echo ------ Android Res 拷贝完成！------

::python 调用该bat文件
::os.system('E:\xxx\copy_res_for_android.bat')