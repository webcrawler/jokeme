
::E:\MyWork\trunk\client\BaLe

set workdir=%~dp0

del %workdir%log.txt

adb logcat > %workdir%log.txt


