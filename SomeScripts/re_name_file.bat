
echo 批量重命名文件夹下所有png jpg文件
@echo off
set /p var=请输入命名前缀:
setlocal enabledelayedexpansion
set n=1
for /f %%i in ('dir /b *.png') do (
ren "%%i" %var%_!n!.png
set /a n+=1)

setlocal enabledelayedexpansion
for /f %%i in ('dir /b *.jpg') do (
ren "%%i" %var%_!n!.jpg
set /a n+=1)

echo 批量重命名完成！ 
pause