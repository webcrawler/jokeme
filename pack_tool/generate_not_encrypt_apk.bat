
cd %~dp0..\BaLe

::É¾³ýÄ¿Â¼
rd /s /q encrypt_res
robocopy res encrypt_res\res /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak
robocopy src encrypt_res\src /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

cocos run -p android --ap android-15 --ndk-toolchain arm-linux-androideabi-4.8 -m release