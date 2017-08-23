1. small_version.ini配置游戏小版本号
2. file_format_check.exe检查游戏工程res和src目录下文件是否有空格
3. genarate_package.bat 加密图片，lua. 拷贝加密后的资源到项目encrypt_res目录下。和拷贝到更新资源目录
4. genarate_apk.bat打包apk

5. android 环境变量配置：
CLASSPATH  .;%JAVA_HOME%\lib;%JAVA_HOME%\lib\tools.jar
JAVA_HOME  D:\Program Files\Java\jdk1.7.0_51
path  %JAVA_HOME%\bin;C:\Program Files\Java\jre7\bin;C:\Python27;

COCOS_CONSOLE_ROOT  D:\cocos2d-x-3.2\tools\cocos2d-console\bin
ANDROID_SDK_ROOT  D:\android\adt-bundle-windows-x86-20140702\adt-bundle-windows-x86-20140702\sdk
ANT_ROOT  D:\android\apache-ant-1.9.3-bin\apache-ant-1.9.3\bin
NDK_ROOT  D:\android\android-ndk-r9-windows-x86\android-ndk-r9
Path   %NDK_ROOT%;%ANT_ROOT%;%ANDROID_SDK_ROOT%;%ANDROID_SDK_ROOT%\tools;%ANDROID_SDK_ROOT%\platform-tools;%COCOS_CONSOLE_ROOT%;

6. --ap android-15 :
generate_not_encrypt_apk.bat
genarate_apk/genarate_apk_debug.bat
genarate_apk_release.bat