
set h=%time:~0,2%

set h=%h: =0%

set dt=%date:~0,4%%date:~5,2%%date:~8,2%%h%%time:~3,2%%time:~6,2%

copy /y %~dp0..\BaLe\publish\android\BaLe-release-signed.apk apks\BaLe-release-signed_%dt%.apk

@echo apks\BaLe-release-signed_%dt%.apk
