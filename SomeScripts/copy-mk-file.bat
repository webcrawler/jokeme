
set path=E:\xxx\cocos2d\external
set file=prebuilt\android\Android.mk
set copyPath=prebuilt\android\armeabi

for /d %%i in (*) do (
if not "%%i"=="cocos2d" (
copy /y %~dp0%%i\%file% %path%\%%i\%file%
copy /y %~dp0%%i\%copyPath% %path%\%%i\%copyPath%
)
)
:: %~dp0 ->µ±Ç°Ä¿Â¼
copy /y %~dp0joke.mk %path%

pause