
REM 大图拆分成小图

chcp 65001 
echo off
color 0B

REM 选择要处理的渠道资源
echo 请输入要拆分的图片路径(eg: c:\test\img.png 输入c:\test\img)：
set /p imgName=
if "%imgName%"=="" (
	exit
)

plistcut.py %imgName%

pause

