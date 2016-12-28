 
 1. mac 自定义类绑定：
 创建把MyClass.h .cpp加入clsaaes工程目录
 进入game/MyGame/frameworks/cocos2d-x/tools/tolua拷贝一个ini文件
 修改ini文件
 	[MyClass] （改）
	prefix           = MyClass
	target_namespace = MyNameSpace
	headers          = %(cocosdir)s/../runtime-src/Classes/MyClass.h
	classes          = MyClass
 修改genbindings.py
  cmd_args 加 'MyClass.ini' : ('MyClass', 'lua_MyClass_auto'), \ 把其他注释了
  进入game/MyGame/frameworks/cocos2d-x/tools/tolua，执行python genbingings.py
  在frameworks/cocos2d-x/cocos/scripting/lua-bindings/auto/下找到生产的hpp和cpp文件，把他们加进cocos2d_lua_bindings项目。
  由于lua_MyClass_auto.cpp文件要引用MyClass.h文件，而这俩文件分属于不同的子项目，互相不认识头文件的搜寻路径，因此需要手工修改一下
  cocos2d_lua_bindings.xcodeproj子项目的User Header Search Paths配置。加 $(SRCROOT)/../../../../../runtime-src/Classes

  后记补充：如果C++类定义了namespace，则需要修改frameworks/cocos2d-x/tools/bindings-generator/targets/lua/conversions.yaml文件，
  定义namespace与Lua之间的映射关系，否则会报conversion wasn't set错误
  "MyNameSpace::": "MyNameSpace."
  
  
 error: ImportError: No module named yaml
  解决：readme文件下有：输入下面3条命令 下载安装
  * Install python dependices by pip.
    sudo easy_install pip
    sudo pip install PyYAML
	sudo pip install Cheetah
http://segmentfault.com/a/1190000000631630
http://www.cocoachina.com/bbs/read.php?tid-196416-page-1.html

lua调用C++:
上面的MyClass绑定完后，按照Lua_web_socket.h的写法注册消息lua_MyClass_auto.cpp：
static int tolua_MyClass_registerCallbackHandler(lua_State* tolua_S)
{
#ifndef TOLUA_REELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"MyClass",0,&tolua_err))
        goto tolua_lerror;
    else
#endif
    {
        MyClass* self    = (MyClass*)  tolua_tousertype(tolua_S,1,0);
        if (NULL != self ) {
            int handler = (  toluafix_ref_function(tolua_S,2,0));
            ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::EVENT_CUSTOM_BEGAN);
        }
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static void extendMyClassLua(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"MyClass");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"registerCallbackHandler");
        lua_pushcfunction(tolua_S,tolua_MyClass_registerCallbackHandler);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

TOLUA_API int register_all_MyClass(lua_State* tolua_S)
{
    tolua_open(tolua_S); 
    
    tolua_module(tolua_S,nullptr,0);
    tolua_beginmodule(tolua_S,nullptr);
    
    lua_register_MyClass_MyClass(tolua_S);
    // 添加Q!!!!!!!
    extendMyClassLua(tolua_S);
    
    tolua_endmodule(tolua_S);
    return 1;
}
lua 调用注册函数：
function onSystemCallback(nType, sMsg)
    cclog("消息IIIIII。。。。"..nType)
end
local myc = MyClass:instance()
myc:registerCallbackHandler(onSystemCallback)

c++ 调用执行上面注册的handler
void MyClass::scriptTest()
{
    int nHandler = cocos2d::ScriptHandlerMgr::getInstance()->getObjectHandler(this, cocos2d::ScriptHandlerMgr::HandlerType::EVENT_CUSTOM_BEGAN);
    if(nHandler == 0) {
        return;
    }
    auto _stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
    _stack->pushInt(123);
    _stack->executeFunctionByHandler(nHandler, 1);
//    _stack->pushInt((int)SDK_MSG_APPSTORY_BUY_SUCCESS);
//    _stack->pushString(productIdentifier.c_str());
//    _stack->pushString(transactionIdentifier.c_str());
//    _stack->pushString(transactionReceipt.c_str());
//    _stack->executeFunctionByHandler(nHandler, 4); // 参数个数
    _stack->clean();
}



2. adb logcat > /Users/admin/Documents/log17.txt   // 输出log到外部文件
ndk-stack -sym /Users/admin/Documents/git/BatteryClient/Maze2.0.0/proj.android/obj/local/armeabi  -dump /Users/admin/Documents/log17.txt

addr2line命令获取代码行数：
ndk-stack是通过addr2line来获取代码位置的, 下面使用addr2line查找代码位置:
bash_profile加 export PATH=$PATH:/Users/admin/Documents/game/android-ndk-r9d/toolchains/arm-linux-androideabi-4.6/prebuilt/darwin-x86_64/bin
命令：arm-linux-androideabi-addr2line -e /Users/admin/Documents/git/BatteryClient/Maze2.0.0/proj.android/obj/local/armeabi/libcocos2dcpp.so 00c0489b
(00c0489b 在log backtrace: #xxx pc xxxxxx )
( backtrace: #00  pc 00c0489b  /data/app-lib/com.feiyu.BatteryRun-1/libcocos2dcpp.so )

objdump获取函数信息：
导出函数表:
arm-linux-androideabi-addr2line -f -e /Users/admin/Documents/git/BatteryClient/Maze2.0.0/proj.android/obj/local/armeabi/libcocos2dcpp.so 00c0489b

3.现在的SourceTree版本解决这个真的是太方便了
请看到右边 External Diff 模块 最下面有四个按钮
Stage Hunk 和 Discard Hunk

服务器直接覆盖掉本地冲突整个文件
直接单击按钮:Discard Hunk
本地直接覆盖掉服务器冲突文件
直接单击按钮:Stage Hunk

服务器直接覆盖掉本地冲突文件中某几行
选中文件中冲突的那几行
直接单击按钮:Discard Selected Lines
本地直接覆盖掉服务器冲突某几行
选中文件中冲突的那几行
直接单击按钮:Stage Selected Lines

最后，也可以点击External Diff 按钮，进行编辑。

3. android list targets （获得Android可用的虚拟设备）
   adb devices   (列出连接的设备)

4.
typedef pair <const int, string> value_type;
const std::map<int, string>::value_type init_value[] =
{
    std::map<int, string>::value_type( 0, "123"),
    std::map<int, string>::value_type( 1, "235"),
    std::map<int, string>::value_type( 2, "569"),
};
const std::map<int, string> m_mapPn(init_value, init_value+3);

5. 删除某目录下所有.svn文件 sudo find /Users/admin/Documents/git/BatteryClient/Maze2.0.0/proj.android/ -name ".svn" -exec rm -r {} \;

6. git 导出2个版本之间的差异文件 git diff 205c28c 0ab6198  --name-only >> diff.txt

7. py 学习

chmod +x test.py     # 脚本文件添加可执行权限

8. SourceTree mac下使用BeyondCompare出来冲突文件 http://adad184.com/2015/07/28/use-git-with-sourcetree-and-beyondcompare/
 显示 git配置 git config --list

1. 先安装好Beyond Compare (拷贝Beyond Compare.app到Applications目录)
2. 打开SourceTree的Preferences 选择Diff 在下面的External Diff/Merge中做如下设置
Visual Diff Tool: Other
Diff Command: /usr/local/bin/bcomp
Arguments: \$LOCAL \$REMOTE

Merge Tool: Other
Merge Command: /usr/local/bin/bcomp
Arguments: \$LOCAL \$REMOTE \$BASE \$MERGED

3. 打开终端 输入命令 (链接链接)
ln -s /Applications/Beyond Compare\ Compare.app/Contents/MacOS/bcomp /usr/local/bin/

9. open ~/.bash_profile

10. mac解压bin文件:
    1.cd 进入bin文件目录
    2.获取文件权限 chmod a+x android-ndk-r10c-darwin-x86_64.bin
    3.解压出文件在当前目录下 ./android-ndk-r10c-darwin-x86_64.bin

11. cocos2dx3.2 ndkr9d，在android5.0下 curl无效
描述
cocos2dx3.2 ndkr9d，在android5.0下 curl无效
原因
curl引起
解决方法
更新cocos官方最新curl, https://github.com/cocos2d/cocos2d-x-3rd-party-libs-bin
使用ndkr10c  (maybe ndkr10e)
编译提示 BundleReader::tell() 函数定义和声明的类型不一致，修改声明 long int tell() 为 ssize_t tell();  （ndk bug）
参考
https://github.com/cocos2d/cocos2d-x/issues/9138
http://cocos2d-x.org/news/307
http://discuss.cocos2d-x.org/t/httpclient-does-not-work-correctly-with-android-5-0/18190

环境变量设置：

CLASSPATH  .;%JAVA_HOME%\lib;%JAVA_HOME%\lib\tools.jar
JAVA_HOME  D:\Program Files\Java\jdk1.7.0_51
path  %JAVA_HOME%\bin;C:\Program Files\Java\jre7\bin;C:\Python27;


COCOS_CONSOLE_ROOT  D:\cocos2d-x-3.2\tools\cocos2d-console\bin
ANDROID_SDK_ROOT  D:\android\adt-bundle-windows-x86-20140702\adt-bundle-windows-x86-20140702\sdk
ANT_ROOT  D:\android\apache-ant-1.9.3-bin\apache-ant-1.9.3\bin
NDK_ROOT  D:\android\android-ndk-r9-windows-x86\android-ndk-r9
Path   %NDK_ROOT%;%ANT_ROOT%;%ANDROID_SDK_ROOT%;%ANDROID_SDK_ROOT%\tools;%ANDROID_SDK_ROOT%\platform-tools;%COCOS_CONSOLE_ROOT%;
编译打包(1):
1. 按住shift键+鼠标右键 选择在此处(F:\cocos2d-x-3.2alpha0\cocos2d-x-3.2alpha0\tests\cpp-empty-test)打开命令窗口
2. cocos compile -p android –-ap 20
3. 生成包 F:\cocos2d-x-3.2alpha0\cocos2d-x-3.2alpha0\tests\cpp-empty-test\publish\android\CppEmptyTest-debug.apk
4. cd到apk目录 adb install CppEmptyTest-debug.apk(安装到内置存储), adb install -s CppEmptyTest-debug.apk(安装到sdcard)
编译打包(2)(旧方法):
1. 进入 F:\cocos2d-x-3.2alpha0\cocos2d-x-3.2alpha0\build
2. 查看当前sdk中所包含的target以及相应id: android list targets
3. python android-build.py -p 19 cpp-empty-test     (19为target)
4. 生成包 F:\cocos2d-x-3.2alpha0\cocos2d-x-3.2alpha0\tests\cpp-empty-test\proj.android\bin\CppEmptyTest-debug.apk


使用Cocos2d-Console这个工具来创建和构建工程，这个工具提供了下面几个功能：
new 创建一个新的工程
compile 编译当前工程，生成二进制文件
deploy 发布程序到一个平台
run 编译和发布，和运行程序

cocos new Hello -p com.fjut.org -l cpp -d E:\

/////////////////// 
http://[jenkins-server]/[command] 
在这里[command]可以是：exit 退出，restart 重启， reload 重载。

1、BUILD FAILED: Android Unable to resolve target 'android-8'
在打包.apk的时候报下错误：
BUILD FAILED: Android Unable to resolve target 'android-8' .
这是因为原有project.properties 的 Project target 的版本号与你当前的android环境所支持的AVD版本号不一致，
解决： 打开项目文件project.properties ,修改其 target值，如 target=android-10//(对应的版本) 如果没有project.properties, 自己建立一个。

1、Android NDK: WARNING: APP_PLATFORM android-9 is larger than android:minSdkVersion 8 in ./AndroidManifest.xml
解决： NDK指定的版本与minSdkVersion不一致,找到proj.android目录下的AndroidManifest.xml，修改其minSdkVersion值  如<uses-sdk android:minSdkVersion="9"/>

3. Installation error: INSTALL_FAILED_INSUFFICIENT_STORAGE
解决方法1：直接在手机上面卸载该APK
解决方法2：试试修改一下manifest文件 :添加一句: 
android:installLocation="preferExternal" 

4.
android mac上环境变量配置 open ~/.bash_profile

export NDK_ROOT=/Users/admin/Documents/game/android-ndk-r9d
export PATH=$NDK_ROOT:$PATH

export COCOS_CONSOLE_ROOT=/Users/admin/Documents/game/cocos2d-x-3.2/tools/cocos2d-console/bin
export PATH=$COCOS_CONSOLE_ROOT:$PATH

export ANDROID_SDK_ROOT=/Users/admin/Documents/game/adt-bundle-mac-x86_64-20140624/sdk
export PATH=$ANDROID_SDK_ROOT:$PATH
export PATH=$ANDROID_SDK_ROOT/tools:$ANDROID_SDK_ROOT/platform-tools:$PATH

export ANT_ROOT=/Users/admin/Documents/game/apache-ant-1.9.5/bin
export PATH=$ANT_ROOT:$PATH

5. iOS开发-Xcode Debug、Release、Archive、Profile、Analyze概念解释
   http://blog.csdn.net/mad1989/article/details/40658033

6. 报错: "relocation overflow in R_ARM_THM_CALL"  解决: 删除proj.android下obj文件夹，重新build

12. Gitlab配置ssh连接:
1. ssh-keygen -t rsa -C "xx@feiyu.com" -C 选项后是备注，可随意。
命令执行后会要求输入key存储的文件名和passphrase：
输入一个特有的文件名，否则使用默认的 id_rsa。
passphrase。不输入也可以。输入之后，提交的时候要输入这个passphrase
完成后在 ~/.ssh/ 会生成2个文件。id_rsa 和 id_rsa.pub。前者是私钥，注意保管，后者是公钥。
目录：/Users/admin/.ssh/id_rsa.pub，打开拷贝里面公钥，登录gitlab: Profile Settings => SSH-/Keys => Add SSH key。
测试：$ ssh -T git@github.com  如果出现错误提示：Permission denied (publickey) 因为新生成的key不能加入ssh就会导致连接不上github，输入以下命令：
先输入$ ssh-agent，再输入$ ssh-add ~/.ssh/id_rsa，这样就可以了
2. git clone git@192.168.1.22:xx/xx.git
3. SourceTree 添加文件夹

7. I faced a problem during "weibo sdk"(like facebook sdk) integration for android, cocos2dx3.2.
when the sdk is login success it will trigger my app from jni, that my app will show a UI.
the issue is that some CCLabels is replaced with black (ios will be fine, the issue is just on android)。
so i use "runAction" to let the UI show at next frame, then it will be ok.
the app enter foreground will trigger cocos2d::VolatileTextureMgr::reloadAllTextures() to reload texture.
so we should handle UI at next frame(maybe need more time).
the following code:
auto act = cocos2d::CallFunc::create([=](){
    showUI();
});
Scene* pScene = Director::getInstance()->getRunningScene();
pScene->runAction(act);

8. android.mk 包含文件： （ http://blog.ready4go.com/blog/2013/10/12/update-android-dot-mk-with-local-src-files-and-local-c-includes/ ）
http://blog.csdn.net/ruglcc/article/details/7814546/
# 配置自己的源文件目录
MY_FILES_PATH  :=  $(LOCAL_PATH) \ #（为了寻找 jni/hellocpp/main.cpp）
$(LOCAL_PATH)/../../Classes
# 指定查找源文件后缀名
MY_FILES_SUFFIX := %.cpp %.c
# 递归遍历目录下的所有的文件
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
# 获取相应的源文件
MY_ALL_FILES := $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*.*) )
MY_ALL_FILES := $(MY_ALL_FILES:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(MY_ALL_FILES))
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)
# 去除字串的重复单词
define uniq =
$(eval seen :=)
$(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
${seen}
endef
# 递归遍历获取所有目录
MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))
# 赋值给NDK编译系统
LOCAL_SRC_FILES  := $(MY_SRC_LIST)
LOCAL_C_INCLUDES := $(MY_ALL_DIRS)

9. ios 判断网络状态
使用apple教程Reachability.h, Reachability.m
int getNetworkStatus(std::string remoteHostName)
{
    NSString* nsRemoteHostName = [NSString stringWithUTF8String:remoteHostName.c_str()];
    Reachability* pReachability = nil;
    
    if (remoteHostName.length() > 0)
    {
        // a Reachability object for a remote host
        pReachability = [Reachability reachabilityWithHostName:nsRemoteHostName];
    }
    else
    {
        // a Reachability object for the internet
        pReachability = [Reachability reachabilityForInternetConnection];
    }
    
    NetworkStatus netStatus = [pReachability currentReachabilityStatus];
    switch (netStatus)
    {
            // NOT NET
        case NotReachable:
        {
            return 0;
            break;
        }
            // 3G/GPRS
        case ReachableViaWWAN:
        {
            return 1;
            break;
        }
            // WIFI
        case ReachableViaWiFi:
        {
            return 2;
            break;
        }
    }
    // unknow
    return 3;
}
int status = getNetworkStatus();
NSLog(@"网络状态: %d", status);

android 判断网络状态:
    需要访问网络，首先需要添加权限
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
public int getNetworkStatus(Context context)
{
    ConnectivityManager connActivityManager = (ConnectivityManager) context.getSystemService(context.CONNECTIVITY_SERVICE);
    NetworkInfo mNetworkInfo = connActivityManager.getActiveNetworkInfo();
    if (mNetworkInfo != null && mNetworkInfo.isAvailable())
    {
        // wifi
        if (mNetworkInfo.getType()==ConnectivityManager.TYPE_WIFI)
        {
            return 2;
        }
        // gprs
        else if (mNetworkInfo.getType()==ConnectivityManager.TYPE_MOBILE)
        {
            return 3;
        }
        // unknow
        else
        {
            return 0;
        }
    }
    // not available
    return 1;
}

10. xcodebuild, xcrun 命令行编译打包ipa
xcodebuild -scheme 'maze2.0.0 iOS' -destination 'platform=iphonesimulator,name=iPhone 6 Plus' -derivedDataPath build

11. 十分钟搞清字符集和字符编码:
小伙伴QQ上传过来的文件，在我们本地打开又乱码了。于是为了实现转换标准，各种字符集标准就出现了。简单的说字符集就规定了某个文字对应的二进制数字存放方式（编码）和某串二进制数值代表了哪个文字（解码）的转换关系。
简单的说乱码的出现是因为：编码和解码时用了不同或者不兼容的字符集。
http://cenalulu.github.io/linux/character-encoding/

12. .dSYM(debugging SYMbols)又称为调试符号表，是苹果为了方便调试和定位问题而使用的一种调试方案，本质上使用的是起源于贝尔实验室的DWARF（Debugging With Attributed Record Formats），其在.xcarchive目录中的层次结构为：
.xcarchive
--dSYMs
|--Your.app.dSYM
|--Contents
|--Resources
|--DWARF

13. 使Xcode调试控制台色彩更丰富 : https://github.com/robbiehanson/XcodeColors
下载编译XcodeColors target 插件自动拷贝到"~/Library/Application Support/Developer/Shared/Xcode/Plug-ins/XcodeColors.xcplugin"，退出xcode，重启。
使用：
#define XCODE_COLORS_ESCAPE @"\033["
#define XCODE_COLORS_RESET_FG  XCODE_COLORS_ESCAPE @"fg;" // Clear any foreground color
#define XCODE_COLORS_RESET_BG  XCODE_COLORS_ESCAPE @"bg;" // Clear any background color
#define XCODE_COLORS_RESET     XCODE_COLORS_ESCAPE @";"   // Clear any foreground or background color
#define LogBlue(frmt, ...) NSLog((XCODE_COLORS_ESCAPE @"fg0,0,255;" frmt XCODE_COLORS_RESET), ##__VA_ARGS__)
#define LogRed(frmt, ...) NSLog((XCODE_COLORS_ESCAPE @"fg255,0,0;" frmt XCODE_COLORS_RESET), ##__VA_ARGS__)

调用:
//setenv("XcodeColors", "YES", 0);
//char *xcode_colors = getenv("XcodeColors");
//if (xcode_colors && (strcmp(xcode_colors, "YES") == 0))
//{
//    NSLog(@"// XcodeColors is installed and enabled!");
//}

NSLog(@"After building the XcodeColors plugin for the first time, you MUST RESTART XCODE.");
NSLog(@"If you still don't see colors below, please consult the README.");
NSLog(XCODE_COLORS_ESCAPE @"fg0,0,255;" @"Blue text" XCODE_COLORS_RESET);
NSLog(XCODE_COLORS_ESCAPE @"bg220,0,0;" @"Red background" XCODE_COLORS_RESET);
NSLog(XCODE_COLORS_ESCAPE @"fg0,0,255;"
      XCODE_COLORS_ESCAPE @"bg220,0,0;"
      @"Blue text on red background"
      XCODE_COLORS_RESET);

NSLog(XCODE_COLORS_ESCAPE @"fg209,57,168;" @"You can supply your own RGB values!" XCODE_COLORS_RESET);
LogBlue(@"Blue text via macro");

14 xcode 命令行打包ipa:
target里的building setting -> code signing Resources rules path 加 $(SDKROOT)/ResourceRules.plist

#!/bin/bash
security unlock-keychain -p " " ~/Library/Keychains/login.keychain
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
rm -rf "$DIR/build"
rm -rf "$DIR/sshTest.ipa"
cd $DIR
xcodebuild -target sshTest clean
xcodebuild -target sshTest CODE_SIGN_IDENTITY="iPhone Distribution: xxx"
xcrun -sdk iphoneos packageapplication -v /Users/admin/Documents/sshTest/build/Release-iphoneos/sshTest.app \
-o /Users/admin/Documents/sshTest/sshTest.ipa --sign "iPhone Distribution: xxx" \
--embed "/Users/admin/Documents/xxx.mobileprovision"

(越狱设备上安装就不需要sign embed喽)
问题：ResourceRules.plist: cannot read resources
解决：code signing -> code signing resource rules path 加 $(SDKROOT)/ResourceRules.plist

windows上通过ssh操作mac命令：
问题： "User interaction is not allowed"
解决:
shell加 security unlock-keychain -p " " ~/Library/Keychains/login.keychain
// 查看目录
security list-keychains
// 解锁 密码~
security unlock ~/Library/Keychains/login.keychain
// 设置有效期1小时
security set-keychain-settings -t 3600 -l ~/Library/Keychains/login.keychain
// 查看
security show-keychain-info ~/Library/Keychains/login.keychain

15. xcode 代码git管理 ：
source control - > xxx_starter->configure xxx_starter->remote
安装最新jdk http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html
设置环境变量：open  /Library/Java/JavaVirtualMachines/  看到安装的jdk ：/Library/Java/JavaVirtualMachines/jdk1.8.0_65.jdk/Contents/Home
open ~/.bash_profile  -> export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_65.jdk/Contents/Home

Jenkins安装：
从http://mirrors.jenkins-ci.org/war/latest/jenkins.war下载Jenkins War文件。打开一个终端窗口，运行以下命令：
nohup java -jar ~/Downloads/jenkins.war --httpPort=8081 --ajp13Port=8010 >/tmp/jenkins.log 2>&1 &
为启动Jenkins创建一个别名： open ~/.bash_profile  -> 加入: alias jenkins="nohup java -jar ~/jenkins.war --httpPort=8081 --ajp13Port=8010 > /tmp/jenkins.log 2>&1 &"
终端执行 jenkins 启动
访问: http://localhost:8081/
系统管理-> 插件 -> 搜索"GIT plugin"安装
创建项目-> Item名称 ->选择构建一个自由风格的软件项目 ->项目名称->源码管理 选择git ->Repository URL -> git@github.com:webcrawler/test.git-> 构建->execute shell填写：
if [ -d "${WORKSPACE}/builds" ]; then rm -rf ${WORKSPACE}/builds; fi;
mkdir ${WORKSPACE}/builds
xcodebuild -project ${WORKSPACE}/${JOB_NAME}/proj.ios_mac/${JOB_NAME}.xcodeproj -scheme "${JOB_NAME} iOS" archive -archivePath ${WORKSPACE}/builds/archive DEBUG_INFORMATION_FORMAT="dwarf"
xcodebuild -exportArchive -exportFormat IPA -archivePath ${WORKSPACE}/builds/archive.xcarchive -exportPath ${WORKSPACE}/builds/${JOB_NAME}.ipa -exportProvisioningProfile 'XC Ad Hoc: com.737.batteryrun2.cn'
增加构建后操作步骤-> archive the artifacts -> 输入: "builds/*.*" (去掉引号)

16. Travis CI (该服务可以为你的GitHub项目提供持续集成的支持) https://travis-ci.org/
GitHub 项目与 Travis 链接上
在项目根目录创建一个名叫 .travis.yml文件
假如项目为ios工程 写入内容:language: objective-c
http://objccn.io/issue-6-5/, http://docs.travis-ci.com/user/getting-started/

17.
dynamic_cast: 通常在基类和派生类之间转换时使用,run-time cast
const_cast: 主要针对const和volatile的转换.
static_cast: 一般的转换，no run-time check.通常，如果你不知道该用哪个，就用这个。
reinterpret_cast: 用于进行没有任何关联之间的转换，比如一个字符指针转换为一个整形数。

18. 在linux下执行sh文件时提示下面信息：
-bash: ./xx.sh: Permission denied
解决：进入改目录，然后
chmod 777 xx.sh

19. 编译平台和运行平台
Deployment Target
指的是你的APP能支持的最低系统版本，如要支持iOS6以上，就设置成iOS6即可。
Base SDK
指的是用来编译APP的SDK（Software Development Kit）的版本，一般保持当前XCode支持的最新的就好，如iOS8.4。SDK其实就是包含了所有的你要用到的头文件、链接库的集合，
你的APP里面用的各种类、函数，能编译、链接成最后的安装包，就要靠它，苹果每次升级系统，新推出的各种API，也是在SDK里面。所以一般Base SDK肯定是大于等于Deployment Target的版本。
区分
既然Base SDK的版本大于等于Deployment Target的版本，那么就要小心了，因为“只要用到的类、方法，在当前的Base SDK版本里面存在，就可以编译通过！但是一旦运行APP的手机的系统版本
低于这些类、方法的最低版本要求，APP就会Crash！”
所以并不是说，能编译通过的，就一定能运行成功！还要在运行时检查！

宏只在编译时生效！
宏定义只是纯粹的文本替换，只在编译时起作用。如下代码：
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= 70000
NSLog(@"Tutuge");
#endif
被宏定义包起来的代码是否会执行，在编译时就决定好了，无论你是用什么系统运行，宏定义再也没有什么用

编译时检查SDK版本，运行时检查系统版本
这个是最基本的适配手段。
// 编译时判断：检查SDK版本
__IPHONE_OS_VERSION_MAX_ALLOWED: 值等于Base SDK，即用于检查SDK版本的。
__IPHONE_OS_VERSION_MIN_REQUIRED: 值等于Deployment Target，检查支持的最小系统版本。

// 编译时判断：检查SDK版本
#if __IPHONE_OS_VERSION_MAX_ALLOWED > 50000
// 运行时判断：检查当前系统版本
if ([UIDevice currentDevice].systemVersion.floatValue > 5.0f)
{
    //
}
#endif

20. Errors running builder 'Scanner Configuration Builder' on project 'xx proj'. java.lang.NullPointerException
proj.android下面的 ".cproject"文件
fix: 更新sdk
                                               
21. eclipse 卡在
解决：关闭eclipse, 进入 /Users/admin/.android 删除cache目录和ddms.cfg文件 打开eclipse

22. cocos2d-x 项目 eclipse打开报错：An error has occurred. See error log for more details. java.lang.NullPointerException
fix: 打开proj.android下面的 ".cproject"文件 删除最后一个cconfiguration


23. test:
1.查看xcode的版本号和build版本
$ xcodebuild -version

2.显示当前系统的sdk、及其版本
$ xcodebuild -showsdks

3.显示工程项目信息
先cd到工程目录下（有＊.xcodeproj的目录，比如MakeFileTest.xcodeproj），然后输入命令
$ xcodebuild -list

查看帮助：$ xcodebuild -help，或者随便输入一个不存在的选项：$ xcodebuild -fuck
查看用法：$ xcodebuild -usage
查看已安装 sdk 列表：$ xcodebuild -showsdks
查看当前 Xcode 版本：$ xcodebuild -version
查看 xcodebuild 目录：$ xcode-select -print-path
查看项目配置(先cd到项目目录)：$ xcodebuild -showBuildSettings
编译项目：$ xcodebuild

24. 接入umeng分享出现问题：Unable to execute dex: Multiple dex files define Lcom/sina/sso/RemoteSSO
解决：umeng自带的sina weibo 和之前项目接入的sina weibo jar冲突了，把新引入的 com.sina.sso 下的RemoteSSO.aidi删除即可

25. 使用umeng自带的cocos2dx demo接入 一直没有图片分享 只有文字
解决：demo里面的文件CCUMSocialController.java下 方法getFileName 直接eturn fullname.split("/")[1]; 数组第2个了 导致返回的图片路径错了
应该要去掉 "res/" 或者 "assert/" 后返回字符串, demo 只考虑 assert /res 直接目录下的图片资源了 没考虑里面还有可能是文件夹呢.

26. 制作带签名的调试证书：
1. 首先当然是先复制一份正式证书出来作为要修改为的临时调试证书。
2. 修改keystore密码的命令(keytool为JDK带的命令行工具)：
keytool -storepasswd -keystore my.keystore
其中，my.keystore是复制出来的证书文件，执行后会提示输入证书的当前密码，和新密码以及重复新密码确认。这一步需要将密码改为android。
3. 修改keystore的alias：
keytool -changealias -keystore my.keystore -alias my_name -destalias androiddebugkey
这一步中，my_name是证书中当前的alias，-destalias指定的是要修改为的alias，这里按规矩来，改为androiddebugkey！这个命令会先后提示输入keystore的密码和当前alias的密码。
4. 修改alias的密码：
keytool -keypasswd -keystore my.keystore -alias androiddebugkey
这一步执行后会提示输入keystore密码，alias密码，然后提示输入新的alias密码，同样，按规矩来，改为android！
以上几个操作执行后，my.keystore就是符合规矩的debug keystore了，接着在Eclipse的ADT设置中选中这个custom debug keystore即可。

27. cocos2dx android 动态加载多个第三方so库：
1. proj.android/jni 下创建prebuilt文件夹，把so库放到该文件夹下。
2. 打开Android.mk文件，添加：

LOCAL_PATH := $(call my-dir)

// add so ！！！
include $(CLEAR_VARS)
# libegamepay_dr2.so  $(TARGET_ARCH_ABI)/
LOCAL_MODULE := egamepay_dr2
LOCAL_SRC_FILES :=  prebuilt/libegamepay_dr2.so
include $(PREBUILT_SHARED_LIBRARY)
LOCAL_SHARED_LIBRARIES := $(LOCAL_PATH)/prebuilt/egamepay_dr2

include $(CLEAR_VARS)
# libentryex.so    $(TARGET_ARCH_ABI)/
LOCAL_MODULE := entryex
LOCAL_SRC_FILES :=  prebuilt/libentryex.so
include $(PREBUILT_SHARED_LIBRARY)
LOCAL_SHARED_LIBRARIES := $(LOCAL_PATH)/prebuilt/entryex

28. android 下ant.properties文件签名证书配置：
key.alias.password=123456
key.store.password=123456
key.store=/Users/admin/Documents/shellIpaTest/proj.android/mykey.keystore
key.alias=fjut

29. mac 打开android sdk mananger: 终端进入/Users/admin/Documents/game/adt-bundle-mac-x86_64-20140624/sdk/tools
    执行./android sdk

30. mac解压:
ndk$ chmod a+x android-ndk-r10c-darwin-x86_64.bin
ndk$ ./android-ndk-r10c-darwin-x86_64.bin

31. android 修改项目名称: /Users/admin/Documents/git/BatteryClient/Maze2.0.0/proj.android_feiyu/.project
<projectDescription>
<name>改名</name>

32. android.mk 包含源代码文件:

# 配置自己的源文件目录和源文件后缀名
MY_FILES_PATH  :=   $(LOCAL_PATH) \
                    $(LOCAL_PATH)/../../Classes

MY_FILES_SUFFIX := %.cpp %.c

# 递归遍历目录下的所有的文件
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# 获取相应的源文件
MY_ALL_FILES := $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*.*) )
MY_ALL_FILES := $(MY_ALL_FILES:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(MY_ALL_FILES))
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)

# 去除字串的重复单词
define uniq =
$(eval seen :=)
$(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
${seen}
endef

# 递归遍历获取所有目录
MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))

# 赋值给NDK编译系统
LOCAL_SRC_FILES  := $(MY_SRC_LIST)
LOCAL_C_INCLUDES := $(MY_ALL_DIRS)

33. "your hardware does not support this application " 
unity android studio
https://segmentfault.com/a/1190000005646078
http://blog.csdn.net/justfwd/article/details/49308199
https://github.com/googlesamples/android-ndk/blob/master/hello-jni/app/build.gradle
指定支持架构。不指定则默认支持所有架构（可以打出apk包解压查看lib目录），假如当前项目没有对应架构的so库，则打进apk的/lib/<abi>/lib<name>.so只有目录没有对应的so库。导致安装的时候没有copy对应的so, 运行出错
productFlavors {  
    fjut {  
        ndk {  
            abiFilters "armeabi-v7a"  
            abiFilters "x86"
        }  
    }  
}  
当一个应用安装在设备上，只有该设备支持的CPU架构对应的.so文件会被安装。在x86设备上，libs/x86目录中如果存在.so文件的话，会被安装，如果不存在，
则会选择armeabi-v7a中的.so文件，如果也不存在，则选择armeabi目录中的.so文件（因为x86设备也支持armeabi-v7a和armeabi）。

http://unity3d.com/cn/unity/system-requirements
Unity5 does not support armeabi, just armeabi-v7。---- Android：OS 2.3.1 或更新版本；含有 NEON 支持的 ARMv7 (Cortex) CPU，或 Atom CPU；OpenGL ES2.0或更高版本。

https://developer.android.com/ndk/guides/abis.html#am
By default, the NDK generates machine code for the armeabi ABI. You can generate ARMv7-a-compatible machine code, instead, 
by adding the following line to your Application.mk file.
APP_ABI := armeabi-v7a
To build machine code for two or more distinct ABIs, using spaces as delimiters. For example:
APP_ABI := armeabi armeabi-v7a
This setting tells the NDK to build two versions of your machine code: one for each ABI listed on this line. For more 
information on the values you can specify for the APP_ABI variable, see Android.mk.
When you build multiple machine-code versions, the build system copies the libraries to your application project path, 
and ultimately packages them into your APK, so creating a fat binary. A fat binary is larger than one containing only 
the machine code for a single system; the tradeoff is gaining wider compatibility, but at the expense of a larger APK.
At installation time, the package manager unpacks only the most appropriate machine code for the target device. For details,
 see Automatic extraction of native code at install time.

Note: ARMv7-based Android devices running 4.0.3 or earlier install native libraries from the armeabi directory instead of the armeabi-v7a directory 
if both directories exist. This is because /lib/armeabi/ comes after /lib/armeabi-v7a/ in the APK. This issue is fixed from 4.0.4.

34. mac下 打开git忽略的全局文件列表 open ~/.gitignore_global (目录git工具界面修改) , 针对项目忽略的文件列表在项目目录下的 .gitignore sourcetree 可以在设置->高级... 打开编辑。

35. unity 设置屏幕在Landscape自动旋转，导出的ios工程默认是home键在右边UIInterfaceOrientationLandscapeRight(符合操作习惯)。在info.plist下保证UIInterfaceOrientationLandscapeRight在前面。：
    <key>UISupportedInterfaceOrientations</key>
    <array>
        <string>UIInterfaceOrientationLandscapeRight</string>  // The device is in landscape mode, with the device held upright and the home button on the right side.
        <string>UIInterfaceOrientationLandscapeLeft</string>  // The device is in landscape mode, with the device held upright and the home button on the left side.
    </array>
    Note: 和unity提供的 LandscapeLeft LandscapeRight  定义方向相反。

36. keytool -list -v -keystore keystore.jks
    jarsigner -verify -verbose -certs my_application.apk

37. adb push /Users/admin/Documents/123.zip sdcard/  (Copies a specified file from your development computer to an emulator/device instance)
    adb pull sdcard/123.zip /Users/admin/Documents/  (Copies a specified file from an emulator/device instance to your development computer)
    可以是目录。

38. xcodebuild test -destination "platform=iOS,id=400d20d00baf8d4997b47be0416cf5c44dd2d3bc" -scheme Unity-iPhone  (id 是设备id)

39. 
auto t = std::thread(CC_CALLBACK_2(PlayerIconMgr::download, this);
t.detach();
// Create a file to save jpg
bool PlayerIconMgr::download(std::string id, std::string path)
{
    // android
    // pthread_create创建的新线程默认情况下是不能进行Jni接口调用的，
    // 除非Attach到Vm，获得一个JniEnv对象，并且在线程exit前要Detach Vm。
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JavaVM *vm;
    JNIEnv *env;
    vm = JniHelper::getJavaVM();

    JavaVMAttachArgs thread_args;
    thread_args.name = "download path";
    thread_args.version = JNI_VERSION_1_4;
    thread_args.group = NULL;

    vm->AttachCurrentThread(&env, &thread_args);
    std::string dirPath = FileUtils::getInstance()->getWritablePath() + "icon/" + getSourceId();
    vm->DetachCurrentThread();
#else
    std::string dirPath = FileUtils::getInstance()->getWritablePath() + "icon/" + getSourceId();
#endif
}

40. IOS: 
Armv6、armv7、armv7s、arm64都是arm处理器的指令集，所有指令集原则上都是向下兼容的，如iPhone4S的CPU默认指令集为armv7指令集，
但它同时也兼容armv6指令集，只是使用armv6指令集时无法充分发挥其性能，即无法使用armv7指令集中的新特性，同理，iPhone5的处理器标配armv7s指令集，
同时也支持armv7指令集，只是无法进行相关的性能优化，从而导致程序的执行效率没那么高。
需要注意的是iOS模拟器没有运行arm指令集，编译运行的是x86指令集，所以，只有在iOS设备上，才会执行设备对应的arm指令集。
Architectures：      
指明选定Target要求被编译生成的二进制包所支持的指令集支持指令集是通过编译生成对应的二进制数据包实现的，如果支持的指令集数目有多个，
就会编译出包含多个指令集代码的数据包，从而会造成最终编译生成的包很大。

Valid Architectures：
指明可能支持的指令集并非Architectures列表中指明的指令集都会被支持，Valid Architectures限制可能被支持的指令集的范围，
即Valid Architectures和Architectures列表的交集，才是XCode最终生成二进制包所支持的指令集。
比如，将Architectures支持arm指令集设置为：armv7,armv7s，对应的Valid Architectures的支持的指令集设置为：armv7s,arm64，那么此时，XCode生成二进制包所支持的指令集只有armv7s.

Build Active Architecture Only:
指明是否只编译当前连接设备所支持的指令集
该选项起作用的条件有两个，必须同时满足才会起作用：
1. 其值设置为YES
2. XCode成功连接调试设备
假定我们将Build Active Architecture Only值设置为YES，同时XCode连接上手机iPhone5S（匹配指令集arm64）
http://www.cocoachina.com/ios/20140915/9620.html

41. ios奔溃日志查找:
如果你想自己重新生成崩溃报告，请遵照以下指引：
1.xcode连接上真机，target选择真机run
2.编译运行成功，应用启动到默认页面（应用的全屏图片）时，立即在Xcode上点击停止按钮
3.关闭 Xcode
4.在设备上直接打开应用，测试奔溃位置，使之产生奔溃
5.测试完成后，连接设备到电脑上，通过Xcode获取崩溃日志
6.window -> devices -> view devices logs

42. mac上快速搜索工具 Alfred 2
43. void Scheduler::performFunctionInCocosThread(const std::function<void ()> &function)  
/** calls a function on the cocos2d thread. Useful when you need to call a cocos2d function from another thread.  
This function is thread safe.  
@since v3.0  
*/
 eg:     
auto t = std::thread(&AssetsManager::downloadAndUncompress, this);
t.detach();
void AssetsManager::downloadAndUncompress()
{
        // Uncompress zip file.
        if (! uncompress())
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
            	UserDefault::getInstance()->setStringForKey(this->keyOfDownloadedVersion().c_str(),"");
                UserDefault::getInstance()->flush();
                if (this->_delegate)
                    this->_delegate->onError(ErrorCode::UNCOMPRESS);
            });
            break;
        }
	 // ........
 }

44. 打开eclipse报错：“a java runtime environment or java development kit must be available in order to run eclipse”。
解决：打开eclipse.ini 在-vmargs前面添加：
-vm
E:\android\jdk1.8.0_101\jre\bin

45. android eclipse 配置：
1. 导入android项目，import project: cocos2d\cocos\platform\android\java
2. 右键project->properties->android->library， add libray路径 解决error
3. java build path -> libraries-> add jars
4. builders 勾选 CDT Builders 开启c++编译, C/C++ Build下Build command，默认 python ${ProjDirPath}/build_native.py -b release
5. C/C++ Build->Environment add:
PATH -> E:\fjut\cocos2d-x-3.8.1\cocos2d-x-3.8.1\tools\cocos2d-console\bin;D:\Python27
ANT_ROOT ->  E:\android\apache-ant-1.9.7\bin
ANDROID_SDK_ROOT -> E:\android\adt-bundle-windows-x86_64\sdk
NDK_ROOT ->  E:\android\android-ndk-r10b

ant配置： proj.android\ant.properties 下
key.alias.password=fjut
key.store.password=fjut
key.store=/Users/fjut/123.keystore
key.alias=android

46. 执行 cocos compile -p android --ap android-15  报错：
leading to the following error when I try to compile with APP_STL=gnustl_static and NDK_TOOLCHAIN_VERSION = 4.9 in Application.mk:
Android NDK: ERROR:C:/AndroidNDK/sources/cxx-stl/gnu-libstdc++/Android.mk:gnustl_static: LOCAL_SRC_FILES points to a missing file
Android NDK: Check that C:/AndroidNDK/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi-v7a/thumb/libgnustl_static.a exists 
or that its path is correct
解决：NDK找不到4.9版本的toolchain (在cocos2d-x-3.13.1\tools\cocos2d-console\plugins\plugin_compile\build_android.py下：
def get_toolchain_version(self, ndk_root, compile_obj) 官方指定的toochain为4.9,本地ndk下toolchain目录找不到对应4.9的toolchain), 指定toolchain为4.8版本，或者升级ndk版本。 
cocos compile -p android --ap android-15 --ndk-toolchain arm-linux-androideabi-4.8
// eclipse工程可以在 Android.mk 加入： NDK_TOOLCHAIN_VERSION = 4.8
(arm-linux-androideabi-4.8 在ndk下toolchain目录)
《NDK_TOOLCHAIN_VERSION （编译器类型、版本）默认采用的是GCC编译器，对于GCC版本的选择与 NDK版本有关系，
NDK R12，在64位ABI默认是GCC 4.9，32位ABI默认是GCC4.8 》

47. 修改cocos2dx 安装apk时 上传apk到手机存储位置为sdcard (/sdcard/tmp/), 默认存放位置是在系统空间/data/local/tmp/
 打开cocos2d-x-3.13.1\tools\cocos2d-console\plugins\plugin_deploy.py 找到 adb_install = "%s install \"%s\"" % (adb_path, apk_path) 改为：
 adb_install = "%s install -s \"%s\"" % (adb_path, apk_path)

48. adb install apk时提示： adb: error: connect failed: more than one device/emulator。 解决： adb kill-server 再来。adb devices

49. 查看.keystore签名信息： keytool -list -v -keystore E:\MyWork\android_keystore\xx.keystore -storepass 123654 (密码)
	查看apk签名信息： 解压apk，找到META-INF\CERT.RSA， keytool -printcert -file E:\MyWork\xxx\bin\release\android\xxx-release-signed\META-INF\CERT.RSA 

50. 修改android打包拷贝资源到asserts的配置文件 proj.android/build-cfg.json,     
"copy_resources": 
[
	{
		"from": "../My_Android_Resources",
		"to": ""
	}
]
(调用在cocos2d-x-3.13.1\tools\cocos2d-console\bin\cocos.py)

51. #include "cocos-ext.h"  vs 项目->属性->c/c++ ->常规->调价附加包含目录：$(EngineRoot)

52. node.js 服务器搭建：
参考：http://www.plhwin.com/2014/05/28/nodejs-socketio/
	  http://js8.in/2011/09/28/%E4%BD%BF%E7%94%A8socket-io%E5%92%8Cnode-js%E6%90%AD%E5%BB%BAwebsocket%E5%BA%94%E7%94%A8/

http://www.nodejs.org/ 下载安装
命令行查看 node -v ，npm -v 版本号。
创建工作目录命令行cd进入，创建package.json:
{
  "name": "realtime-server",
  "version": "0.0.1",
  "description": "my first realtime server",
  "dependencies": {}
}
使用npm命令安装express和socket.io
npm install --save express
npm install --save socket.io
工作目录下创建socket.js :
// 引入需要的模块：http和socket.io
var http = require('http'), io = require('socket.io');
//创建server
var server = http.createServer(function(req, res){
  // Send HTML headers and message
  res.writeHead(200,{ 'Content-Type': 'text/html' });
  res.end('# Hello Socket Lover!');
});
//端口8000
server.listen(8080);
//创建socket
var io = io.listen(server);
//添加连接监听
io.on('connection', function(socket){
	//socket.emit('news', "from server joke me!");
	socket.send('messages', "Hi I had get your req -- from server joke me!");
    //连接成功则执行下面的监听
    socket.on('message',function(event){
        console.log('Received message from client!',event);
    });
    //断开连接callback
    socket.on('disconnect',function(){
        console.log('Server has disconnected');
    });
	
	socket.on('login',function(event){
        console.log('yes you had login success!',event);
    });
	socket.send('login', "yes you had login success!");
});
运行：node socket.js
浏览器打开 http://localhost:8080 显示 # Hello Socket Lover!  成功
客户端发起连接： SocketIO::connect("ws://tools.itharbors.com:4000/testpoint", *this); 见 MyFrameWork

53. 打开android sdk manager界面 cmd 进入xxxx\adt-bundle-windows-x86-20140702\sdk\tools 执行 android sdk 
国内sdk镜像下载：android-mirror.bugly.qq.com 端口8080 ，务必选择: Force https://... sources to be fetched using http://... 
然后 点击菜单Package>Reload，如果能获取新版本，表明可以正常访问镜像 
http://android-mirror.bugly.qq.com:8080/include/usage.html

54. vs build error: C2259 “xx 类”: 不能实例化抽象类。
继承的父类里边有纯虚函数在子类没有实现。public EditBoxDelegate，子类必须实现editBoxReturn。

55. My Google AppID:  myappid7-1183  myappid8-1185  myappid9-1185  myappid10-1185  myappid11-1185

56. 打包apk 指定拷贝res资源目录：
打开cocos2d-x-3.13.1\tools\cocos2d-console\plugins\plugin_compile\build_android.py 在 
# copy resources
for cfg in res_files:
	cocos.copy_files_with_config(cfg, app_android_root, assets_dir) 
前面添加（注意不要用tab键盘，用4个空格代替）：
os.system('E:\xxxx\copy_res_for_android.bat') 
copy_res_for_android.bat 如下：
%该目录下从Resources拷贝资源到My_Android_Resources下，踢出*.tmp *.zip *.svn *.bak这些文件%
robocopy Resources My_Android_Resources /e /xf /mir /xa:h *.tmp *.zip *.svn *.bak
set path=E:\xxxx\My_Android_Resources\res\Game\
%删除abc目录和cdf目录%
rd /s /q %path%abc
rd /s /q %path%cdf

57. windows tortoisegit 记住密码：
打开项目目录下 .git/config 加入
	[credential]   
	 helper = store
保存，输入一次密码后第二次就会记住密码了

58. NDK自带的iconv的>> android-ndk-r9d/sources/android/support/include/iconv.h
Android.mk 添加：
LOCAL_WHOLE_STATIC_LIBRARIES += android_support
$(call import-module,android/support)

59. makefile 包含其他makefile :
joke1.mk 内容:
mymkpath = E:/xx/cocos2d/external
mymkfile = prebuilt/android
// joke2 包含joke1
joke2.mk 内容:
include E:/xxx/joke1.mk
#输出
$(warning  $(mymkpath))
mypaths = $(mymkpath)/$(mymkfile)

#在makefile中打印警告或者错误消息的方法：
#$(warning xxxxx)或者$(error xxxxx) 
#输出变量方式为：$(warning  $(XXX)) 

60. arm-Linux-androideabi-g++.exe: error: CreateProcess: No such file or directory
module源文件太多的原因。解决:
Android.mk $(call import-module,.) 前添加 LOCAL_SHORT_COMMANDS := true 
Application.mk添加 APP_SHORT_COMMANDS := true 

61. error: gnu-libstdc++/4.8/libs/armeabi/libgnustl_static.a(math_stubs_long_double.o): multiple definition of 'frexpl'
这里把 LOCAL_STATIC_LIBRARIES += android_support 改为：LOCAL_WHOLE_STATIC_LIBRARIES += android_support

62. apk打包显示apk大小：
 打开cocos2d-x-3.13.1\tools\cocos2d-console\plugins\plugin_deploy.py 找到 adb_install = "%s install \"%s\"" % (adb_path, apk_path) 
 在前面添加：
#os.path.getsize
apk_size = os.path.getsize(apk_path)
apk_size_ = (apk_size//(1000*1000))
print("<<<<APK size>>>>: %d MB\n" % apk_size_)

63. 








