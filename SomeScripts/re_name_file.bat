
echo �����������ļ���������png jpg�ļ�
@echo off
set /p var=����������ǰ׺:
setlocal enabledelayedexpansion
set n=1
for /f %%i in ('dir /b *.png') do (
ren "%%i" %var%_!n!.png
set /a n+=1)

setlocal enabledelayedexpansion
for /f %%i in ('dir /b *.jpg') do (
ren "%%i" %var%_!n!.jpg
set /a n+=1)

echo ������������ɣ� 
pause