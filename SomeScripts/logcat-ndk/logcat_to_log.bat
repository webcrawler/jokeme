
REM 模拟器

set workdir=%~dp0

cd D:\Program Files\Microvirt\MEmu
D:
adb start-server
cd E:\work\android\adt-bundle-windows-x86_64-20140702\sdk\platform-tools
E:
adb connect 127.0.0.1:21503

adb logcat > %workdir%log.txt