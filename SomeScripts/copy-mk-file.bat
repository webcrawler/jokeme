
set path=E:\MyWork\ScmjGameNew\Clicent\GameSC\cocos2d\external
set file=prebuilt\android\Android.mk

for /d %%i in (*) do (
copy /y %~dp0%%i\%file% %path%\%%i\%file%
)
:: %~dp0 ->��ǰĿ¼
copy /y %~dp0joke.mk %path%

pause