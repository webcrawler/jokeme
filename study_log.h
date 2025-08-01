 
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
  
  2017.03.15补充：
  绑定error：
  path: path: E:\Android\android-ndk-r10e\toolchains\llvm-3.3\prebuilt\windows-x86_64 are not valid!。
  toolchain目录下找不到llvm-3.3的版本，但是存在3.5版本，则修改genbindings.py 把 3.3改成3.5 。
  
  绑定成功后把MyClass.h .cpp 放在cocos2d-x\external\xxx 目录下。添加进工程。hpp和cpp 也添加进工程。
  注册到lua： 打开cocos2d-x\cocos\scripting\lua-bindings\manual\lua_module_register.cpp 添加头文件：#include "scripting/lua-bindings/auto/MyClass.h".
  函数int lua_module_register(lua_State* L) 里面添加 register_all_MyClass(L);  ok
  
 error: ImportError: No module named yaml
  解决：readme文件下有：输入下面3条命令 下载安装
  * Install python dependices by pip.
    sudo easy_install pip
    sudo pip install PyYAML
	sudo pip install Cheetah
http://segmentfault.com/a/1190000000631630
http://www.cocoachina.com/bbs/read.php?tid-196416-page-1.html

绑定出错：raise NoOptionError(option, section) NoOptionError: No option 'cxxgeneratordir' in section: 'DEFAULT'
解决: 报错地方tools/bindings-generator/generator.py
userconfig.read('userconf.ini')改成userconfig.read('E:/works/cocos2dx-spine-upgrade/frameworks/cocos2d-x/tools/tolua/userconf.ini')

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
（安装内置还是sdcard和手机设置有关系，install -s 会上传apk到手机sdcard，而install则是上传到手机内置存储位置）
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
解决： NDK指定的android sdk版本与minSdkVersion不一致,找到proj.android目录下的AndroidManifest.xml，修改其minSdkVersion值  如<uses-sdk android:minSdkVersion="9"/>

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
PATH -> E:\fjut\cocos2d-x-3.13.1\cocos2d-x-3.13.1\tools\cocos2d-console\bin;D:\Python27
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

55. My Google AppID:  
webcrawler1988 myappid-1180  myappid2-1180  myappid3-1181  myappid4-1181  myappid5-1183  
myappid6-1183 myappid7-1183  myappid8-1185  myappid9-1185  myappid10-1185  myappid11-1185
		     --------------------------------
webcrawler1988|myappid-1180|myappid2-1180|myappid3-1181|myappid4-1181|myappid5-1183|myappid6-1183|myappid7-1183|myappid8-1185|myappid9-1185|myappid10-1185|myappid11-1185		     

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
 打开cocos2d-x-3.13.1\tools\cocos2d-console\plugins\plugin_deploy.py 找到 apk_path = compile_dep.apk_path
 在其后面添加：
#os.path.getsize
apk_size = os.path.getsize(apk_path)
apk_size_ = (apk_size//(1000*1000))
print("<<<<APK size>>>>: %d MB\n" % apk_size_)

63. 创建目录，穿入的path不能带目录结构，只能是单层目录，eg："res"可以。 "res/game"不行
要创建多层目录可以一层一层创建。
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif
static std::string createDownloadedDir(const std::string& path)
{
	std::string pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
	pathToSave += path;
	// Create the folder if it doesn't exist
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR* pDir = NULL;
	pDir = opendir(pathToSave.c_str());
	if (!pDir)
	{
		mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(pathToSave.c_str(), 0);
	}
#endif
	return pathToSave;
}

64. 下载apk安装issues:
apk下载到sdcard,安装正常。
apk下载到getWritablePath(), 调用包安装前需要取得该目录权限：
String[] cmd = {"chmod", "777", storePath};  
ProcessBuilder builder = new ProcessBuilder(cmd);  
try {  
	builder.start();  
} catch (IOException e) {  
	// TODO Auto-generated catch block  
	e.printStackTrace(); 
}  

65. python pause
# fjut add
print("%s\n" % "install apk? enter any key then continue")
os.system("pause")

66. win使用curl:
#include <curl/curl.h>
#include <curl/easy.h>
// 属性->c/c++->常规->附加包含目录 添加: $(EngineRoot)external\curl\include\win32
// 属性->链接器->常规->附加依赖项 添加: libcurl_imp.lib

67. Android Studio 
/proj.android-studio/app/build.gradle 下指定 ABI
默认情况下，Gradle会针对NDK支持的ABI将您的原生库构建到单独的.so文件中，并将其全部封装到您的APK中。
如果您希望Gradle仅构建和封装原生库的特定ABI配置，您可以在模块级build.gradle文件中使用ndk.abiFilters标志指定这些配置，如下所示：
android {
  ...
  defaultConfig {
    ...
    externalNativeBuild { 
      cmake {...}
      // or ndkBuild {...}
    }

    ndk {
      // Specifies the ABI configurations of your native
      // libraries Gradle should build and package with your APK.
      abiFilters 'x86', 'x86_64', 'armeabi', 'armeabi-v7a',
                   'arm64-v8a'
    }
  }
  buildTypes {...}
  externalNativeBuild {...}
}

cocos compile -p android --ap android-15 --android-studio --ndk-toolchain arm-linux-androideabi-4.8

68. Configure multiple APKs for ABIs :
splits {
	// Configures multiple APKs based on ABI.
	abi {
		// Enables building multiple APKs per ABI.
		enable true
		// By default all ABIs are included, so use reset() and include to specify that we only
		// want APKs for x86, armeabi-v7a, and mips.
		// Resets the list of ABIs that Gradle should create APKs for to none.
		reset()
		// Specifies a list of ABIs that Gradle should create APKs for.
	    // (生成2个包含对应abi的apk)
		include "x86", "armeabi-v7a"    
		// Specifies that we do not want to also generate a universal APK that includes all ABIs.
		// 为true则同时也生成一个包含全部可用的（app/libs/工程找得到的abi目录，确不为空目录）abi的apk
		universalApk false
	}
}

69. cocos2dx 使用Android Studio，先生成so，再生成apk
1. cocos compile -p android --ap android-15 --android-studio --ndk-toolchain arm-linux-androideabi-4.8
2. Android Studio 打开proj.android-studio, 直接run.

70. Android Studio 导入package，alt+enter

71. android 渠道号配置：
AndroidManifest.xml配置渠道号：
<application xxxxx>
	<meta-data android:name="CHANNEL" android:value="fjut" />
</application>
// 获取渠道号
public class AppActivity extends Cocos2dxActivity {
    private static Activity activity = null;
    @Override
    protected void onCreate(final Bundle icicle) {
        super.onCreate(icicle);
        try {
            ApplicationInfo appInfo = this.getPackageManager().getApplicationInfo(this.getPackageName(), PackageManager.GET_META_DATA);
            String msg = appInfo.metaData.getString("CHANNEL");
            System.out.println("my_channel:" + msg);
        }
        catch (PackageManager.NameNotFoundException e) {
        }
    }
}

另外一种方法: 如果在META-INF目录内添加空文件，可以不用重新签名应用。因此，通过为不同渠道的应用添加不同的空文件，可以唯一标识一个渠道。
采用这种方式，每打一个渠道包只需复制一个apk，在META-INF中添加一个使用渠道号命名的空文件即可。
执行python脚本：（1.txt空文件）
import zipfile
zipped = zipfile.ZipFile("E:/MyGithub/jokeme/SomeScripts/123.apk", 'a', zipfile.ZIP_DEFLATED) # sys.argv[1]
empty_channel_file = "META-INF/channel_123"
zipped.write("E:/MyGithub/jokeme/SomeScripts/1.txt", empty_channel_file)
zipped.close()

java 代码获取渠道123  "META-INF/channel_123" ：
public static String getChannel(Context context) {
	ApplicationInfo appinfo = context.getApplicationInfo();
	String sourceDir = appinfo.sourceDir;
	String ret = "";
	ZipFile zipfile = null;
	try {
		zipfile = new ZipFile(sourceDir);
		Enumeration<?> entries = zipfile.entries();
		while (entries.hasMoreElements()) {
			ZipEntry entry = ((ZipEntry) entries.nextElement());
			String entryName = entry.getName();
			if (entryName.startsWith("META-INF/channel")) {
				ret = entryName;
				break;
			}
		}
	} catch (IOException e) {
		e.printStackTrace();
	} finally {
		if (zipfile != null) {
			try {
				zipfile.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	String[] split = ret.split("_");
	if (split != null && split.length >= 2) {
		return ret.substring(split[0].length() + 1);

	} else {
		return "";
	}
}
测试：String channel = getChannel(this);
      showDialog("channel", channel);

批量配置渠道号：http://tech.meituan.com/mt-apk-packaging.html
http://www.cnblogs.com/ct2011/p/4152323.html

72. python使用import xxtea.
先安装pip(python3.4自带pip可省略下载安装pip): https://bootstrap.pypa.io/get-pip.py 
python2.7使用: https://bootstrap.pypa.io/pip/2.7/get-pip.py  下载完get-pip.py 执行python "{path}\get-pip.py" (执行需要挂代理)
安装在C:\Python27\Scripts，如果执行pip不存在，则cd进入此目录再执行

然后安装xxtea：pip install xxtea -U
假如提示：“command 'cl.exe' failed: No such file or directory”  见 
http://stackoverflow.com/questions/38059732/error-command-cl-exe-failed-no-such-file-or-directory  
http://stackoverflow.com/questions/29909330/microsoft-visual-c-compiler-for-python-3-4
下载安装针对python27: https://www.microsoft.com/en-gb/download/confirmation.aspx?id=44266
提示： “fatal error C1083: Cannot open include file: 'stdint.h': No such file or directory”
下载stdint.h文件 放到C:\Users\xxx\AppData\Local\Programs\Common\Microsoft\Visual C++ for Python\9.0\VC\include\stdint.h

73. Cocos2dxActivity -> onLoadNativeLibraries 加载libcocos2dcpp.so 
E:\MyGithub\jokeme\MyFramework\proj.android\build.xml 下修改生成的apk名称。
E:\MyGithub\jokeme\MyFramework\proj.android\res\values\strings.xml 下修改安装后桌面上显示的名称
E:\MyGithub\jokeme\MyFramework\proj.android\ant.properties 添加keystore配置:
key.alias.password=joke
key.store.password=joke
key.store=../keystore/joke.keystore
key.alias=android

74. std::map find查找：
auto it = m_downloaderMap.find(srcUrl);
if (it == m_downloaderMap.end())
{
	return;
}
delete m_downloaderMap[srcUrl];
m_downloaderMap.erase(it);

std::vector find查找：
std::vector<int> vec;
vec.push_back(0);
vec.push_back(1);
vec.push_back(2);
auto it = std::find(vec.begin(), vec.end(), 1);
if (it != vec.end())
{
	int d = *it;
}

75. python搭建http服务器：
python -m SimpleHTTPServer 8001
http://localhost:8001/1.txt

76. android 编译找不到libluajit.a 等一些 .a文件。是因为项目从svn checkout下来的，提交的项目默认不提交.a文件。
从其他项目拷贝external覆盖

77. ios error: The resource could not be loaded because the App Transport Security policy requires the use of a secure connection.
fix:在Info.plist中添加NSAppTransportSecurity类型Dictionary。
	在NSAppTransportSecurity下添加NSAllowsArbitraryLoads类型Boolean,值设为YES
	
78. mac 自带svn 使用：
终端 open ~/.subversion/ 打开config 找到store-passwords = yes  store-auth-creds = yes 设置为yes保存。下次使用不用输密码
sudo svn up
svn ci -m "test commit lua" src/123.lua

79. Skipped 'ApiController.php' -- Node remains in conflict At revision 271. Summary of conflicts: Skipped paths: 1
解决方法：
svn remove --force ApiController.php svn resolve --accept=working ApiController.php
svn update

80. 更新android sdk. 国内镜像：mirrors.neusoft.edu.cn 端口: 80. 选中Force https://...sources to be.  packages->Reload，如果能获取新版本，表明可以正常访问镜像
81. 查看apk信息：
	首先找到aapt工具，在Android SDK文件夹下的build-tools包里，如下：
	cd D:/Android/SDK/build-tools/23.0.0_rc3
	然后使用aapt dump bading XXX.apk就能看到VersionCode等信息
	aapt dump badging C:/Users/xx/Desktop/app-release_1.0.9.apk
	如果提示 "ERROR: dump failed because no AndroidManifest.xml found" 可能是apk路径有中文

82. 节点曲线运动，计算节点转动的角度(和x轴夹角)
    local bezier1 = {
        cc.p(100, display.height / 2),
        cc.p(100, display.height / 2),
        cc.p(1000, 0)
    }
    local bezierTo1 = cc.BezierTo:create(20, bezier1)
    local fish1 = ccui.ImageView:create("imgs/by_follow_2.png")
    fish1:runAction(bezierTo1)
    self:addChild(fish1, GameMainOrder.followFishAni)
    self.prePos = cc.p(fish1:getPosition())

    local function tick(dt)
        local curPos = cc.p(fish1:getPosition())
        local subPos  = cc.pSub(curPos, self.prePos)
        local tanf = math.atan2(subPos.x, subPos.y)
        local angle = tanf*180/math.pi
        cclog("angle:"..angle)
        self.prePos = curPos
    end
    cc.Director:getInstance():getScheduler():scheduleScriptFunc(tick, 0, false)
	
83. IDA下载 http://www.52pojie.cn/thread-319763-1-1.html

84. 获取系统版本：adb shell getprop ro.build.version.release
    获取系统api版本：adb shell getprop ro.build.version.sdk

85. 打开SDK Manager，列表尾部找到Extras 更新Android Support Repository。更希完后
打开目录 adt-bundle-windows-x86_64/sdk/extras/android/support/v4 拷贝android-support-v4.jar到项目libs目录
打开eclipse界面 选中android-support-v4.jar右键Build Path > Add to Build Path。

86. 大小端模式: https://www.bysocket.com/?p=615  
http://www.ruanyifeng.com/blog/2016/11/byte-order.html 
https://blog.csdn.net/twlkyao/article/details/11580033

在计算机系统中，我们是以字节为单位的，每个地址单元都对应着一个字节，一个字节为8bit。
但是在C语言中除了8bit的char之外，还有16bit的short型，32bit的long型（要看具体的编译器）。
另外，对于位数大于8位的处理器，例如16位或者32位的处理器，由于寄存器宽度大于一个字节，
那么必然存在着一个如果将多个字节安排的问题。因此就导致了大端存储模式和小端存储模式。

在C/C++程序的编写中，当多个基本数据类型或复合数据结构要占用同一片内存时，我们要使用联合体；
当多种类型，多个对象，多个事物只取其一时（我们姑且通俗地称其为“n 选1”），
我们也可以使用联合体来发挥其长处。首先看一段代码：
union myun   
{  
 　　struct { int x; int y; int z; }u;   
　　 int k;   
}a;   
int main()   
{   
　　 a.u.x =4;  
 　　a.u.y =5;   
　　 a.u.z =6;   
　　 a.k = 0;   
　　 printf("%d %d %d\n",a.u.x,a.u.y,a.u.z);  
　　 return 0;  
}  
union类型是共享内存的，以size最大的结构作为自己的大小，这样的话，
myun这个结构就包含u这个结构体，而大小也等于u这个结构体的大小，
在内存中的排列为声明的顺序x,y,z从低到高，然后赋值的时候，在内存中，
就是x的位置放置4，y的位置放置5，z的位置放置6，现在对k赋值，对k的赋值因为是union，要共享内存，
所以从union的首地址开始放置，首地址开始的位置其实是x的位置，这样原来内存中x的位置就被k所赋的值代替了，
就变为0了，这个时候要进行打印，就直接看内存里就行了，x的位置也就是k的位置是0，而y，z的位置的值没有改变，所以应该是0,5,6。

// 若处理器是Big_endian的，则返回0；若是Little_endian的，则返回1
// 联合体union的存放顺序是所有成员都从低地址开始存放
static int checkCPU()
{
	{
		union w
		{
			int  a;
			char b;
		} c;
		c.a = 1;
		return(c.b == 1);
	}
}

实现同样的功能，我们来看看Linux 操作系统中相关的源代码是怎么做的：
static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };  
#define ENDIANNESS ((char)endian_test.mylong)  

Linux 的内核作者们仅仅用一个union 变量和一个简单的宏定义就实现了一大段代码同样的功能！
由以上一段代码我们可以深刻领会到Linux 源代码的精妙之处！(如果ENDIANNESS=’l’表示系统为little endian,为’b’表示big endian )

87. lua 打包和解包用到的格式串  https://blog.csdn.net/ljxfblog/article/details/44339705 
 https://blog.csdn.net/lovehappy108/article/details/52070737 
 http://cloudwu.github.io/lua53doc/manual.html#6.4.2
 https://my.oschina.net/goal/blog/195749
用于 string.pack， string.packsize， string.unpack 的第一个参数。 它是一个描述了需要创建或读取的结构之布局。
格式串是由转换选项构成的序列。 这些转换选项列在后面：
<: 设为小端编码
>: 设为大端编码
=: 大小端遵循本地设置

http://www.luteus.biz/Download/LoriotPro_Doc/LUA/LUA_For_Windows/lpack/   
https://www.jianshu.com/p/16eec13d3a86

Introduction
This is a simple Lua library for packing and unpacking binary data.
The library adds two functions to the string library: pack and unpack.

string.pack
ret = pack( F, x1, x2, ... )
Description: Packs binary data into a string.
Parameters:
F
A string describing how the values x1, x2, ... are to be interpreted and formatted. 
Each letter in the format string F consumes one of the given values. 
The letter codes understood by pack are listed below (they are inspired by Perl's codes but are not the same). 
Numbers following letter codes in F indicate repetitions.

x1, x2, ...
Values to pack into binary. Only values of type number or string are accepted.

Returns: ret
A (binary) string containing the values packed as described in F.

string.unpack
val, next = unpack( s, F [,init] )

Description:
Packs binary data into a string.

Parameters: s
A (binary) string containing data packed as if by pack.

F
A format string describing what is to be read from s

init
An optional init marks where in s to begin reading the values.

Returns: next
The first value returned by unpack is the next unread position in s, which can be used as the init position 
in a subsequent call to unpack. This allows you to unpack values in a loop or in several steps. 
If the position returned by unpack is beyond the end of s, then s has been exhausted;
 any calls to unpack starting beyond the end of s will always return nil values.

val
One value per letter in F until F or s is exhausted (the letters codes are the same as for pack,
 except that numbers following A are interpreted as the number of characters to read into the string, not as repetitions).

Letter Codes
z : zero-terminated string 
p : string preceded by length byte 
P : string preceded by length word 
a : string preceded by length size_t 
A : string 
f : float 
d : double 
n : Lua number 
c : char 
b : byte (unsigned char) 
h : short 
H : unsigned short 
i : int 
I : unsigned int 
l : long 
L : unsigned long

< : little endian 
> : big endian 
= : native endian

88. "A" 按照ASCII编码是65, 存储在计算机是二进制 01000001

89. luasocket  \frameworks\cocos2d-x\external\lua\luasocket\luasocket_scripts.c 

90. vs工程配置custom build step里面的commond line 命令xcopy很多文件夹，文件很耗时间

91. babelua 调试lua断点无效。解决：require路径有问题

92. sublime 配置cocos2dx项目：
tools -> build system -> new build system -> 输入：
{
	"working_dir": "E:/work/Projects/client/client",
	"shell_cmd":   "E:/work/Projects/client/run/Debug.win32/joke.exe -workdir E:/work/Projects/client/client"
}
保存默认目录 \Sublime Text 3\Packages\User
执行F7运行项目

93. babelua插件配置:
BabeLua For 2015 V3.2.2.0.vsix 
补丁: babelua-compat-1.06-VS2015.zip

lua exe path: E:\work\Projects\client\run\Debug.win32\joke.exe
working path: E:\work\Projects\client\client
command line: -workdir "E:\work\Projects\client\client"

94. luajit off:
local arc = jit.arch
print("arc:"..arc)
jit.off()

95. 逍遥模拟器看不到data目录：
打开自带的文件管理器CyanogenMod File Manager. 点击左上角->设置->常规设置->访问模式->选择超级用户访问模式。
data目录在：根目录/data/data/
查看模拟器和pc共享目录的时候 访问模式改成 安全模式。路径：/storage/emulated/0/Pictures。 改完还是找不到，则重启模拟器

96. 监测android app http请求：httpcanary.apk  https://github.com/MegatronKing/HttpCanary

97. Gradle build 报错：Received status code 400 from server: Bad Request
在gradle.properites 里面把代理去掉.如:
systemProp.https.proxyPort=80
systemProp.http.proxyHost=mirrors.neusoft.edu.cn
systemProp.https.proxyHost=mirrors.neusoft.edu.cn
systemProp.http.proxyPort=80

98. android sdkversion解释：

/**
* compileSdkVersion specifies the Android API level Gradle should use to
* compile your app. This means your app can use the API features included in
* this API level and lower.
*/
compileSdkVersion 28

// Defines the minimum API level required to run the app.
minSdkVersion 15

// Specifies the API level used to test the app.
targetSdkVersion 28

99. ndkr16 废弃armeabi
Support for ARM5 (armeabi) is deprecated. It will no longer build by default with ndk-build, but is still buildable if it is explicitly named, and will be included by "all" and "all32".
Both CMake and ndk-build will issue a warning if you target this ABI.
https://github.com/android-ndk/ndk/wiki/Changelog-r16-beta1

100. Android plugin for Gradle 3.0.0后buildToolsVersion自动指定
If you're using Android plugin for Gradle 3.0.0 or higher, your project automatically uses a default version of the build tools that the plugin specifies. To use a different version of the build tools, specify it using buildToolsVersion in your module's build.gradle, as follows:

101.
    repositories {
        // Gradle 4.1 and higher include support for Google's Maven repo using
        // the google() method. And you need to include this repo to download
        // Android Gradle plugin 3.0.0 or higher.
        google()
        ...
    }

102. android studio运行直接打开逍遥模拟器
1. cmd cd D:\Program Files\Microvirt\MEmu 进入逍遥模拟器安装目录，执行 adb start-server
2. cmd cd E:\work\android\adt-bundle-windows-x86_64-20140702\sdk\platform-tools 进入platform-tools目录，执行 adb connect 127.0.0.1:21503
3. android studio 直接运行项目，选择模拟器设备

103. 批处理 清除编译中间文件
@echo off
echo 清除所有obj pch idb pdb ncb opt plg res sbr ilk suo文件
pause
del /f /s /q .\*.obj
del /f /s /q .\*.pch
del /f /s /q .\*.idb
del /f /s /q .\*.pdb
del /f /s /q .\*.ncb 
del /f /s /q .\*.opt 
del /f /s /q .\*.plg
del /f /s /q .\*.res
del /f /s /q .\*.sbr
del /f /s /q .\*.ilk
del /f /s /q .\*.aps
echo 清除文件完成！
echo. & pause

104. 批处理svn提交
set my_svn_path=E:\work\Projects\PackRes
:: svn commit
::for /f %%i in (%my_svn_path%\filelist.txt) do ( 		
::echo svn file name: %%i
::TortoiseProc.exe /command:commit /path:%my_svn_path%\%%i /closeonend:2
::echo.
::)
TortoiseProc.exe /command:commit /path:%my_svn_path% /closeonend:2

105. iOS openURL调用失败
需要判断下传入的URL连接是否包含http://或https://，如果不包含，则给它添加上即可
/**
 * 把string转换成可以URL跳转的类型（指的是带HTTP或者HTTPS）
 */
- (NSString *)convertToURLFormatWithString:(NSString *)str {
    if ([str containsString:@"http"] || [str containsString:@"https"]) {
        return str;
    }else {
        NSMutableString *strM = [NSMutableString stringWithString:str];
        [strM insertString:@"http://" atIndex:0];
        return strM.copy;
    }
}

106. android targetSdkVersion
compileSdkVersion 和 minSdkVersion 都非常好理解，前者表示编译的 SDK 版本，后者表示应用兼容的最低 SDK 版本。
但是对于 targetSdkVersion 其实很难一句话解析清楚，原文用了“万能”的词 —— interesting 来描述。
以前我也有一些迷糊，看到有些人和我有同样的困惑，本文试图彻底解决这个问题。
原文是这么说的：
targetSdkVersion is the main way Android provides forward compatibility
targetSdkVersion 是 Android 系统提供前向兼容的主要手段。这是什么意思呢？随着 Android 系统的升级，
某个系统的 API 或者模块的行为可能会发生改变，但是为了保证老 APK 的行为还是和以前兼容。
只要 APK 的 targetSdkVersion 不变，即使这个 APK 安装在新 Android 系统上，
其行为还是保持老的系统上的行为，这样就保证了系统对老应用的前向兼容性。
https://www.race604.com/android-targetsdkversion/
https://medium.com/androiddevelopers/picking-your-compilesdkversion-minsdkversion-targetsdkversion-a098a0341ebd#.egywqatjg
https://chinagdg.org/2016/01/picking-your-compilesdkversion-minsdkversion-targetsdkversion/

107. 
-- imagePath：被裁减的图片路径
-- maskPath： 裁剪模板图片路径
-- 返回Node
local function getClippNode(imagePath, maskPath)
	local img = cc.Sprite:create(imagePath)
	local mask = nil
	local frame = cc.SpriteFrameCache:getInstance():getSpriteFrame(maskPath)
	if nil ~= frame then
		mask = cc.Sprite:createWithSpriteFrame(frame)
	else
		mask = cc.Sprite:create(maskPath)
	end
	local maskSize = mask:getContentSize()
	local imgSize = img:getContentSize()

	img:setScaleX(maskSize.width/imgSize.width)
	img:setScaleY(maskSize.height/imgSize.height)

	local clippingNode = cc.ClippingNode:create()
	clippingNode:setContentSize(maskSize)
	clippingNode:setStencil(mask)
	clippingNode:setAlphaThreshold(0.05) 
	clippingNode:addChild(img)
	return clippingNode
end

108. ios app闪退：Message from debugger: Terminated due to signal 13
SIGPIPE	13 表示：Broken pipe: write to pipe with no readers
https://www.lifewire.com/signal-linux-command-4094016
尝试send到一个已关闭的 socket上两次，就会出现此信号，也就是用协议TCP的socket编程，服务器是不能知道客户机什么时候已经关闭了socket，
导致还在向该已关 闭的socket上send，导致SIGPIPE。

109. 批处理

::进入当前目录
cd %~dp0
E:

set my_path=E:\work
::调用bat
call %~dp0my_path\%my_path%-res.bat

::删除目录
rd /s /q %my_pathxxx%\qq

::删除文件
del /s /q %my_pathxxx%\qq.zip

::创建文件件
md %my_path%\joke

::拷贝文件
COPY /y %my_path%\joke.zip %my_path%\..\joke

::拷贝文件夹及下面所有文件, 不拷贝*.tmp *.zip *.rar *.svn *.bak文件
robocopy %my_path%\haha %my_path%\client /e /xf /mir /xa:h *.tmp *.zip *.rar *.svn *.bak

::提交svn
TortoiseProc.exe /command:commit /path:%my_path% /closeonend:2

::打开文件目录
start %~dp0..\jokeme

::用google Chrome打开 网站
"C:\Program Files (x86)\Google\Chrome\Application\chrome.exe" http://baidu.com

::关闭所有文件夹
taskkill /f /IM explorer.exe
start C:\Windows\explorer.exe


::解压文件 解压joke.zip到joke目录下 指定秘密
call WinRAR x -ibck -k -o+ -r -m5 -hp 密码 %my_path%\joke.zip %my_path%\joke

::压缩文件 压缩client目录到client.zip 指定秘密
call WinRAR a -ibck -k -r -m5 -hp 密码 %my_path%\client.zip client

::删除压缩包内内容 删除压缩包内res\logon和src\logon 2个目录
call WinRAR d -ibck -hp 密码 %my_path%\client.zip client\res\logon client\src\logon

::改名 %my_path%\下的client.zip 名字重命名为client.jj
REN %my_path%\client.zip client.jj

110. 开源库集成器Vcpkg  https://blog.csdn.net/cjmqas/article/details/79282847
下载：https://github.com/microsoft/vcpkg
执行 bootstrap-vcpkg.bat 即可编译。编译好以后会在同级目录下生成vcpkg.exe文件

查看Vcpkg支持的开源库列表
执行命令
vcpkg search

安装jsoncpp库 执行命令
vcpkg install jsoncpp

“集成到全局”适用于Visual Studio开发环境和msbuild命令行。执行命令：
vcpkg integrate install

111. Android NDK: WARNING: Deprecated NDK_TOOLCHAIN_VERSION value: 4.9. GCC is no longer supported
NDK版本太高。

112. ip地址字符串转整形
#include <stdint.h>
static int lua_ipStrToInt(lua_State* L)
{
	const char* ipStr = (const char*)luaL_checkstring(L, 1);
	uint32_t addrInt[4] = {0};
	uint32_t intVal = 0;
	sscanf(ipStr, "%d.%d.%d.%d", &addrInt[0], &addrInt[1], &addrInt[2], &addrInt[3]);
	int i = 0;
	for (i = 3; i >= 0; --i)
	{
		intVal = intVal << 8;
		intVal += addrInt[i];
	}
	//printf("ipInt = %d", intVal);
	lua_pushnumber(L, intVal);
	return 1;
}

113. web调试工具 google插件 Restlet Client

114. 限速软件 NetKimiter

115. adb logcat 查看逍遥模拟器log
1. cmd cd D:\Program Files\Microvirt\MEmu 进入逍遥模拟器安装目录，执行 adb start-server
2. cmd cd E:\work\android\adt-bundle-windows-x86_64-20140702\sdk\platform-tools 进入platform-tools目录，执行 adb connect 127.0.0.1:21503
3. cmd adb logcat 或者输出到文本 adb logcat > E:\log.txt

批处理bat:
cd D:\Program Files\Microvirt\MEmu
D:
adb start-server
cd E:\work\android\adt-bundle-windows-x86_64-20140702\sdk\platform-tools
E:
adb connect 127.0.0.1:21503
adb logcat
pause

116. 批处理，输出logcat到文本文件
set workdir=%~dp0
del %workdir%log.txt
adb logcat > %workdir%log.txt

117. cocos2d-x3.16不再支持ADT，Android SDK Tools 25.3.0+去除了ant脚本和android命令 ，使得cocos命令无法打包Android的Eclipse工程（proj.android）。
cocos2d-x3.17 由于 Google 官方自  Android SDK Tools 25.3.0 (就已放弃 ant 支持。移除旧有的 proj.android ant 工程，新的 proj.android 是 Android Studio 工程。
NDK r16 (弃用了 armeabi，引擎调整默认的编译架构从 armeabi 为 armeabi-v7a。 https://www.cocos.com/1462

118. cocos2d-x3.17.2 Gradle插件版本3.1.0 Gradle 版本4.4/4.6 ndk-r16b

119.配置好Gradle后可以使用Gradle工具命令执行打包
cd E:\work\MyNew3172\frameworks\runtime-src\proj.android 
gradlew task-name (task-name在MyNew3172/Tasks/build/下的task)比如 gradlew assemble / gradlew assembleDebug / gradlew assembleRelease
查看项目的所有可用编译任务的列表：gradlew tasks

120. cocos命令打包错误： Invalid or unsupported command "update project -t android-21。
sdk版本太高 重新配置下环境变量

121. java找不到 org.apache.http.util.EncodingUtils
Android6.0 org.apache.http.util.EncodingUtils等相关类被移除。
在gradle作如下配置, 引入 useLibrary 'org.apache.http.legacy' 即可
android {
	...
    useLibrary 'org.apache.http.legacy'
    ...
}

122. android 添加 iconv 支持：(NDK r15移除了android_support的iconv功能android_support库在${NDK}/sources/android/support目录，
相比NDK r14b，NDK r15b删掉了src/musl-locale整个目录。只能自己下载源码编译使用http://ftp.gnu.org/pub/gnu/libiconv/)
LOCAL_WHOLE_STATIC_LIBRARIES += android_support
$(call import-module, android/support)
eg:
LOCAL_STATIC_LIBRARIES := cclua_static
# add iconv support
LOCAL_WHOLE_STATIC_LIBRARIES += android_support
# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END
include $(BUILD_SHARED_LIBRARY)
$(call import-module, cocos/scripting/lua-bindings/proj.android)
#add iconv support
$(call import-module, android/support)

123. eclipse工程 import其他依赖工程后 在工程右键properties->android下添加要依赖的工程。
124. flashxp ftp命令上传资源
cd D:\FlashFXP
D:
flashfxp.exe -upload ftp://账号:密码@10.10.10.0:21 -localpath="E:\work\Projects\joke\"  -remotepath="\"

125. lua string.format 会四舍五入。eg：string.format("%0.2f", 10.236) == 10.24。
不需要四舍五入则可以这样：保留2位小数local myNum = num - num % 0.01。保留3位小数local myNum = num - num % 0.001。

126. 批处理bat 传递接收参数
传递: call copy_res.bat 123654
接收(%0~%9)：echo %1 %2 %3

127. bat处理输出中文乱码
REM 声明采用UTF-8编码
chcp 65001

128. android studio工程下 build.gradle打印 println '你好' 输出在左下角build->toggle view下查看 (run也可以查看)

129. cocos2dx3.17.2 使用ndk-build, android studio版本3.4.1, gradle plugin版本3.1.0, gradle版本4.6 ndk-r16b.
使用cmake编译, 报错：CMake Error: CMake was unable to find a build program corresponding to "Ninja". 
CMAKE_MAKE_PROGRAM is not set. You probably need to select a different build tool 
修改 gradle plugin版本3.2.0 可以编译成功, 但是android assets资源没有拷贝进去。

解决：gradle plugin版本3.1.0 不变。在sdk manager -> sdk tool去掉cmake3.10.2.49884.4 保留cmake3.6.4111459
编译成功, assets资源也成功拷贝进去。

130. android native编译方式为cmake, 添加新.h .cpp, 在工程CMakeLists.txt里 GAME_HEADER和GAME_SOURCE添加新加的.h和.cpp文件。
eg:
if(ANDROID)
	...
	...
	# fjut add
	list(APPEND GAME_HEADER 
		${RUNTIME_SRC_ROOT}/Classes/HelloWorldScene.h
		${RUNTIME_SRC_ROOT}/Classes/joke.h
	)
    list(APPEND GAME_SOURCE 
		${RUNTIME_SRC_ROOT}/Classes/HelloWorldScene.cpp
		${RUNTIME_SRC_ROOT}/Classes/joke.cpp
	)
	
131. android 关闭多点触控。在appActivity.java  onCreate() 方法后面加 this.getGLSurfaceView().setMultipleTouchEnabled(false)

132. git包含子项目 git submodule add   
如果还要初始化、抓取并检出任何嵌套的子模块，请使用简明的 git submodule update --init --recursive
https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%AD%90%E6%A8%A1%E5%9D%97

133. win32 改变控制台输出文字颜色:
static int lua_consoleColor(lua_State* L)
{
	WORD color = (WORD)luaL_checknumber(L, -1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	return 0;
}

static int lua_consoleColorReset(lua_State* L)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return 0;
}

134. lua os.clock() 调用c函数，在不同平台有着不同的定义，有溢出问题，有可能会返回负数。
用luasocket 或者 c函数gettimeofday 替代：
local socket = require("socket")
local start = socket.gettime()

135. 我们都知道TCP是一个基于字节流的网络传输协议，它并不关注数据的机构。
而我们所说的粘包/半包指的是接收的消息结构与期望不符，
既然TCP并不关注消息的结构自然也就不保证消息按结构交付了， 但是TCP会保证数据可靠，顺序的交付。
粘包/半包指的是应用层对接收到的不符合期望的消息结构的一种描述， 而这个问题本身就该应用层来处理。
https://blog.cc1234.cc/articles/file-descriptor/file-descriptor.html

136. int占多少个字节相关
http://c.biancheng.net/ref/limits_h/
机器字长是指计算机进行一次整数运算所能处理的二进制数据的位数。

C语言的一个优良传统是标量类型（整数是典型的标量类型）要以一种对每个计算机体系结构都很自然的方式表示。例如，基本整型 int 的弹性就很大，它希望自己的大小等于一个机器字长：
在 16 位环境中，机器字长为 16 位，int 占用 2 个字节；
在 32 位环境中，机器字长为 32 位，int 占用 4 个字节；
在 64 位环境中，机器字长为 64 位，int 依然占用 4 个字节。
int 占用一个机器字长的内存是C语言的初衷，在 16 位环境和 32 环境中确实如此，但是到了 64 位环境中就不遵循这个规则了，这是为什么呢？

计算机发展到 64 位是后来的事情，已经是比较晚的年代了，C语言已经存在了二三十年了，此时已经有大量基于 32 位环境编写的代码，在这些代码中，程序员认为 int 的长度就是 4，并不一定向后兼容，如果冒然将 int 的长度增加到 8 个字节，就可能会导致原来的代码不能正常运行，这多少有点不能接受。

另外，经过多年的编程实践，程序员也已经习惯 int 的长度为 4 了，强行增加 int 的长度也可能让程序员反感。

总之，由于各种历史原因，64 位环境下的 int 依然占用 4 个字节的内存。
这个优良的传统对程序的效率来说是个不错的选择，但是对可移植性来说，就真的是个麻烦了，于是C语言标准委员会发明了 <limits.h> 头文件来捕捉标量类型在不同计算机体系结构之间的变化。


https://blog.csdn.net/xiaoshengqdlg/article/details/44676387
计算机发明后，为了在计算机中表示字符，人们制定了一种编码，叫ASCII码。ASCII码由一个字节中的7位(bit)表示，范围是0x00 - 0x7F 共128个字符。他们以为这128个数字就足够表示abcd....ABCD....1234 这些字符了。
　　咳......说英语的人就是“笨”！后来他们突然发现，如果需要按照表格方式打印这些字符的时候，缺少了“制表符”。于是又扩展了ASCII的定义，使用一个字节的全部8位(bit)来表示字符了，这就叫扩展ASCII码。范围是0x00 - 0xFF 共256个字符。
　　咳......说中文的人就是聪明！中国人利用连续2个扩展ASCII码的扩展区域（0xA0以后）来表示一个汉字，该方法的标准叫GB-2312。后来，日文、韩文、阿拉伯文、台湾繁体（BIG-5）......都使用类似的方法扩展了本地字符集的定义，现在统一称为 MBCS 字符集（多字节字符集）。这个方法是有缺陷的，因为各个国家地区定义的字符集有交集，因此使用GB-2312的软件，就不能在BIG-5的环境下运行（显示乱码），反之亦然。
　　咳......说英语的人终于变“聪明”一些了。为了把全世界人民所有的所有的文字符号都统一进行编码，于是制定了UNICODE标准字符集。UNICODE 使用2个字节表示一个字符(unsigned shor int、WCHAR、_wchar_t、OLECHAR)。这下终于好啦，全世界任何一个地区的软件，可以不用修改地就能在另一个地区运行了。虽然我用 IE 浏览日本网站，显示出我不认识的日文文字，但至少不会是乱码了。UNICODE 的范围是 0x0000 - 0xFFFF 共6万多个字符，其中光汉字就占用了4万多个。嘿嘿，中国人赚大发了:0)


137. 安装pip:
下载pip压缩包 https://pypi.org/project/pip/#files 
解压cmd进入目录 执行：python setup.py install
环境变量添加：C:\Pythonxx\Scripts
pip install + 要安装的项目
eg: pip install requests

138. 执行python 错误:SyntaxError: Non-ASCII character '\xe4' 
#/usr/bin/python
#encoding=utf-8
写在最开头顶端，前面不能有换行

139. bat: call a.exe 执行调用a.exe后不会自动关闭cmd窗口。start a.exe 执行后会自动关闭cmd窗口。

140. cpp获取当前目录：
#include <direct.h>
char buffer[MAX_PATH];
_getcwd(buffer, MAX_PATH);
std::string workDir = buffer;

141.cocos2dx 给exe指定工作目录，执行bat：start game.exe -workdir %~dp0../../game

142. typeid是C++的关键字之一，等同于sizeof这类的操作符。typeid操作符的返回结果是名为type_info的标准库类型的对象的引用
printf("%s\n", typeid(HelloWorld).name()); // "class HelloWorld"
printf("%s\n", typeid(int).name()); // "int"
printf("%s\n", typeid(cocos2d::Node).name()); // "class cocos2d::Node"

143. cpp绑定到lua 手动绑定(参考引擎做法)：
MyLuaBindingTest.h:
	#ifndef _MY_LUA_BINDING_TEST_H_
	#define _MY_LUA_BINDING_TEST_H_

	#ifdef __cplusplus
	extern "C"
	{
	#endif
	#include "tolua++.h"
	#ifdef __cplusplus
	}
	#endif

	extern int reg_lua_binding_test1(lua_State* L);
	extern int reg_lua_binding_test2(lua_State* L);
	extern int reg_lua_binding_test3(lua_State* L);

	#endif
	
MyLuaBindingTest.cpp:
	#include "MyLuaBindingTest.h"

	#ifdef __cplusplus
	extern "C"
	{
	#endif
	#include "scripting/lua-bindings/manual/tolua_fix.h"
	#ifdef __cplusplus
	}
	#endif

	#include "cocos2d.h"
	#include "extensions/cocos-ext.h"
	#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
	#include "HelloWorldScene.h"

	#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	#include <dirent.h>
	#include <sys/stat.h>
	#endif

	USING_NS_CC;
	USING_NS_CC_EXT;

	static int lua_cocos2dx_test1(lua_State* L)
	{
		if (L == nullptr) return 0;

		int argc = lua_gettop(L);
		if (argc != 0)
		{
			CCLOG("'createDownloadDir' function wrong number of arguments: %d, was expecting %d\n", argc, 0);
			return 0;
		}

		std::string pathToSave = FileUtils::getInstance()->getWritablePath();
		pathToSave = pathToSave + "tmpDir";

		// create dir
	#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
		DIR *pDir = NULL;
		pDir = opendir(pathToSave.c_str());
		if (!pDir)
		{
			mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
		}
	#else
		if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
		{
			CreateDirectoryA(pathToSave.c_str(), 0);
		}
	#endif

		tolua_pushstring(L, pathToSave.c_str());
		return 1;
	}

	static int lua_cocos2dx_test2(lua_State* L)
	{
		unsigned int i = 123654;
		//lua_pushnumber(L, i); 
		tolua_pushnumber(L, i);
		return 1;
	}

	static int lua_cocos2dx_test3(lua_State* L)
	{
		unsigned int i = 123654789;
		//lua_pushnumber(L, i); 
		tolua_pushnumber(L, i);

		// test
		printf("%s\n", typeid(HelloWorld).name()); // "class HelloWorld"
		printf("%s\n", typeid(int).name()); // "int"
		printf("%s\n", typeid(cocos2d::Node).name()); // "class cocos2d::Node"

		return 1;
	}

	// lua调用
	// lua_cocos2dx_test1()
	int reg_lua_binding_test1(lua_State* L)
	{
		tolua_open(L);
		tolua_module(L, NULL, 0);
		tolua_beginmodule(L, NULL);
		tolua_function(L, "lua_cocos2dx_test1", lua_cocos2dx_test1);
		tolua_endmodule(L);
		return 0;
	}

	// lua调用
	// local jokei2 = cc.JokeClass:new()
	// print("lua_cocos2dx_test3 = "..jokei2)
	int reg_lua_binding_test2(lua_State* L)
	{
		lua_register(L, "lua_cocos2dx_test2", lua_cocos2dx_test2);
		return 0;
	}

	// lua调用
	// local jokei2 = cc.JokeClass:new()
	// print("lua_cocos2dx_test3 = "..jokei2)
	int reg_lua_binding_test3(lua_State* L)
	{
		lua_getglobal(L, "_G");
		if (lua_istable(L, -1))//stack:...,_G,
		{
			tolua_open(L);
			tolua_module(L, "cc", 0);
			tolua_beginmodule(L, "cc");

			tolua_usertype(L, "cc.JokeClass");
			tolua_cclass(L, "JokeClass", "cc.JokeClass", "cc.Ref", nullptr);

			tolua_beginmodule(L, "JokeClass");
			tolua_function(L, "new", lua_cocos2dx_test3);
			tolua_endmodule(L);

			std::string typeName = "HelloWorldScene"; // typeid(cocos2d::HelloWorldScene).name();
			g_luaType[typeName] = "cc.JokeClass";
			g_typeCast["JokeClass"] = "cc.JokeClass";

			tolua_endmodule(L);
		}
		lua_pop(L, 1);

		return 0;
	}

144. 限速软件 NetLimiter 注册码:
Registration Name: Vladimir Putin #2
Registration Code: XLEVD-PNASB-6A3BD-Z72GJ-SPAH7

145. 帧同步
https://www.jianshu.com/p/8cca5458c45b
https://blog.csdn.net/jxw167/article/details/71602447
https://blog.csdn.net/weixin_43405546/article/details/92428696
https://blog.csdn.net/FlyToCloud/article/details/104650408
https://gameinstitute.qq.com/community/detail/104156

帧同步模型最大的缺点是对所有玩家的延迟都有要求，一般来说要求在50毫秒以内，如果有一个客户端网络卡住了，所有客户端都要停下来等待。
另外在帧同步模式中，数据同步的频率较高，网络延迟越小越好。由于TCP的滑动窗口机制和重传机制，导致延时机制，导致延时无法控制。
因此帧同步一般采用UDP进行网络传输，但UDP又会衍生出可靠性问题，对于客户端，如果某些UDP包没有收到，就会出现丢帧的情况。

客户端A的操作A1与客户端B的操作B1，共同封装成OperateCmd数据发送给PVP服务器，PVP服务器每66毫秒产生一个逻辑祯，在该桢所在时间段内，
收到A1和B1后，生成一个Frame数据块，在该帧时间结束时，将Frame发送给客户端A和B。Frame数据块内有该帧的帧号，客户端A和B接收到Frame数据后，
便知道该帧内，客户端A和客户端B都做了什么操作。然后根据接收到的消息A1和B1进行游戏表现，最终呈现给玩家A和B的结果是一致性的，从而实现客户端A和B的数据同步。

帧同步既然是在特定时间发送，也就是说每隔一段时间收集用户操作指令，那么要间隔多久内。例如每隔一段时间搜索用户的操作。
此时，如果时间太快则网络速率达不到要求，如果时间太长则用户操作不流程。那多少才比较合适呢？根据统计玩家至少要在50ms—100ms可以完成一次。

146. c++ 位域 
https://www.runoob.com/cprogramming/c-bit-fields.html
http://c.biancheng.net/cpp/html/102.html
https://docs.microsoft.com/zh-cn/cpp/cpp/cpp-bit-fields?view=vs-2019
http://www.yuan-ji.me/C-C-%E4%BD%8D%E5%9F%9F-Bit-fields-%E5%AD%A6%E4%B9%A0%E5%BF%83%E5%BE%97/

struct joke
{
	unsigned int a : 30;
	unsigned int  : 0;	// 跳过int在下个类型排
	unsigned int b : 2; // 第二个int开始
	unsigned int c : 31; // 一个int排不下，差1个bits 继续占下一个int的位置
};
int size = sizeof(joke); // 3个int = 12byte

struct joke
{
	unsigned int ready : 2;
	unsigned int error : 2;
	unsigned int command : 4;
	unsigned int sector_no : 24;  // 总共占1个int 32bits
}
int size = sizeof(joke); // 1个int = 4byte

147. yasio 搭建测试 https://docs.yasio.org/zh_CN/latest/
bulid.bat:

cd %~dp0
E:

git clone https://github.com/yasio/yasio
cd yasio
git submodule update --init --recursive

cd build
cmake ..
cmake --build . --config Debug

148. Sol2实现cpp和lua绑定
https://github.com/ThePhD/sol2
https://hulinhong.com/2018/06/20/lua_cpp_sol2/

149. winsock初始化失败 sock = socket(af, type, protocol);返回INVALID_SOCKET
解决：为了在应用程序当中调用任何一个Winsock API函数，首先第一件事情就是必须通过WSAStartup函数
完成对Winsock服务的初始化，因此需要调用WSAStartup函数。使用Socket的程序在使用Socket之前必须调用WSAStartup函数。
	// 初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	// 再调用::socket()
	sock = socket(af, type, protocol);

150. androidstudio 使用iconv转码找不到iconv 则ndk使用r14b

151. add tmp:
// add by joke 
void yasioClientTest()
{
	//io_service service({ "www.ip138.com", 80 });

	// test.yasio.org  5001
	//  "47.115.77.205"  6801
	//io_service service({"test.yasio.org", 5001});

	//io_service service({"47.115.77.205", 6801});

	io_service service({"10.10.10.3", 8077});

	//service->set_option(YOPT_C_LFBFD_PARAMS,
	//	0,     // channelIndex, 信道索引
	//	65535, // maxFrameLength, 最大包长度
	//	0,     // lenghtFieldOffset, 长度字段偏移，相对于包起始字节
	//	4, // lengthFieldLength, 长度字段大小，支持1字节，2字节，3字节，4字节
	//	0 // lengthAdjustment：如果长度字段字节大小包含包头，则为0， 否则，这里=包头大小
	//);

	service.set_option(YOPT_S_DEFERRED_EVENT, 0, 65535, 0, 1, 0); // 直接在网络线程分派网络事件
	//service.set_option(YOPT_S_DEFERRED_EVENT, 0); // 直接在网络线程分派网络事件

	//resolv_fn_t resolv = [&](std::vector<ip::endpoint>& endpoints, const char* hostname, unsigned short port)
	//{
	//	int par = service.resolve(endpoints, hostname, port);
	//	return par;
	//};
	//service.set_option(YOPT_S_RESOLV_FN, &resolv);

	service.start([&](event_ptr&& ev) {
		switch (ev->kind())
		{
		case YEK_PACKET: {
			// ev->packet() 返回全包数据
			auto packet = std::move(ev->packet());
			auto tmpData = packet.data();

			// add by joke
			auto p0 = *tmpData;
			auto p1 = *(++tmpData);
			auto p2 = *(++tmpData);

			tmpData--;
			tmpData--;
			fwrite(tmpData, packet.size(), 1, stdout);
			fflush(stdout);
			break;
		}
		case YEK_CONNECT_RESPONSE:
			if (ev->status() == 0)
			{
				auto transport = ev->transport();
				if (ev->cindex() == 0)
				{
					obstream obs;

					// add by joke
					obs.write_bytes("123");


					//obs.write_bytes("GET /index.htm HTTP/1.1\r\n");

					//obs.write_bytes("Host: www.ip138.com\r\n");

					//obs.write_bytes("User-Agent: Mozilla/5.0 (Windows NT 10.0; "
					//	"WOW64) AppleWebKit/537.36 (KHTML, like Gecko) "
					//	"Chrome/79.0.3945.117 Safari/537.36\r\n");
					//obs.write_bytes("Accept: */*;q=0.8\r\n");
					//obs.write_bytes("Connection: Close\r\n\r\n");

					service.write(transport, std::move(obs.buffer()));
					int tmp = 0;
				}
			}
			break;
		case YEK_CONNECTION_LOST:
			printf("The connection is lost.\n");
			break;
		}
	});
	// open channel 0 as tcp client
	service.open(0, YCK_TCP_CLIENT);
	getchar();
	
	// joke add
	service.stop();

	getchar();
	getchar();
	getchar();
}

152. creator 编译android工程，下载的gradle插件zip包错误了 导致解压失败：
Exception in thread "main" java.lang.RuntimeException: java.util.zip.ZipException: error in opening zip file
删除gradle zip包,重来， 使用镜像地址重新下载

153. "objs-debug/editor_support_static/__/scripting/js-bindings/auto/jsb_cocos2dx_editor_support_auto.o.d': No such file or directory"
路径太长问题。ndk改用r17 即可。

154. cocos creator2.1.1, gradel4.10.3, gradle插件3.2.0, ndkr17c 打包成功。打包没有res src一般是gradle插件版本升级导致拷贝资源失败。

155. cocos creator调试android, chrome打开连接不上：{IP} 为 Android设备的本地IP，{}也要替换，192.168.0.100:6086 哎没注意折腾好久。

156. babelua调试cocos2dlua项目, 断点部分目录无效.
解决: 修改Cocos2dxLuaLoader.cpp, 把luaL_loadbuffer改luaL_loadfile。
主要代码:
//LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
//stack->luaLoadBuffer(L, reinterpret_cast<const char*>(chunk.getBytes()),
//                     static_cast<int>(chunk.getSize()), chunkName.c_str());

// 改为 ========>>
// add by fjut on 2020.11.19
// 原本代码是直接把lua文件读取出buffer,再调用luaL_loadbuffer,这样没法再文件断点. 
// 改为直接加载文件luaL_loadfile,这样就可以从文件断点.
// xxtea解密过程就不能使用了.
chunkName = utils->fullPathForFilename(chunkName);
luaL_loadfile(L, chunkName.c_str());
//CCLOG("luaL_loadfile: %s\n", chunkName.c_str());

157. vs断点数值型显示十六进制改为显示十进制：在watch或者locals窗口选中一个变量右键选择 "hexadecimal display"(显示16进制)

158. 本地搭建web服。逍遥模拟器内部访问http://10.0.2.2:port/xxx

159. python2.7 安装后C:\Python27\下没有Scripts。直接安装python2.7.14即可解决此问题。
C:\Python27\加入环境变量path。C:\Python27\Scripts也加入,方便使用pip。安装pip: easy_install pip.
使用pip安装包: pip inatall xxx. eg：pip inatall shutil. pip install Pillow

160. Android studio,Gradle打包，将so库放在libs\{abi}\下。eg: proj.android-studio\app\libs\armeabi\joke.so
对应的工程build.gradle里sourceSets.main 指定jniLibs目录为libs。eg:
sourceSets.main {
	java.srcDir "src"
	res.srcDir "res"
	jniLibs.srcDir "libs"
	manifest.srcFile "AndroidManifest.xml"
	assets.srcDir "assets"
}

161. github使用 "personal access token" 登录操作：
在个人信息setting->Developer setting->personal access token->Generate new token->选择失效时间,选中repo。
使用eg: https://"token"@github.com/webcrawler/jokeme.git

162. android studio gradle打包位置：view->tool windows->gradle，在面板选择proj.android-studio->tasks->other->assembleRelease.

163. bat if else：

set param=%1
cd %~dp0

if "%param%" == "works" (set workDirSrc=%~dp0..\works)^
else if "%param%" == "works2" (set workDirSrc=%~dp0..\works1)^
else if "%param%" == "works3" (set workDirSrc=%~dp0..\works2)^
else if "%param%" == "works4" (set workDirSrc=%~dp0..\works3)

start %workDirSrc%
exit

164. pip安装包 使用国内镜像
pip install xxxxx -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com

165. cmd adb logcat 输出中文乱码。加入chcp 65001 即可

166. git设置 autocrlf false  safecrlf true  

167. NetLimiter4 注册名: Vladimir Putin #2  注册码: XLEVD-PNASB-6A3BD-Z72GJ-SPAH7

168. Android Studio gradle build项目时控制台输出中文乱码。解决：找到 studio64.exe.vmoptions 文件,文件末尾加入：-Dfile.encoding=UTF-8

169. Android9.0 报错：java.lang.NoClassDefFoundError: Failed resolution of: Lorg/apache/http/client/HttpRequestRetryHandler;
解决：在AndroidManifest.xml的application 标签中添加: <uses-library android:name="org.apache.http.legacy" android:required="false" />

170. adb连接逍遥模拟器:adb connect 127.0.0.1:21523  端口号在program files\Microvirt\MEmu\MemuHyperv VMs\MEmu_xx\MEmu_xx.memu-prev里：
<Forwarding name="ADB" proto="1" hostip="127.0.0.1" hostport="21523" guestip="10.0.2.15" guestport="5555"/>。hostip和hostport

171. android studio app/build.gradle 修改apk名字：
android {
    ... ...
    defaultConfig{
        ... ...
        manifestPlaceholders = [k_appName : "哈啰"]   // 设置默认的k_appName
		...
}
对应的AndroidManifest.xml需要对android:label修改为：
<application
    android:icon="@mipmap/ic_launcher"
    android:label="${k_appName}" // 这里取k_appName
>

172. git设置
[core]
	autocrlf = true
	safecrlf = false

173. aws创建aws_access_key_id, aws_secret_access_key
# Python 3.7及以后
# 安装：boto3
# pip install boto3 -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com

# 创建aws_access_key_id, aws_secret_access_key :
# 进入aws后台 https://us-east-1.console.aws.amazon.com/iamv2/home?region=us-east-1#/home
# 选择左边菜单栏"用户"=>"安全凭证"=>"创建访问秘钥"=>"命令行界面(CLI)"=>命名并创建成功。查看秘钥。

174. aws 刷新cdn(具体见代码):
https://boto3.amazonaws.com/v1/documentation/api/latest/reference/services/cloudfront/client/create_invalidation.html
distributionId为CloudFront下分配的ID。

import boto3

session = boto3.Session(
	aws_access_key_id = awsAccessKeyId,
	aws_secret_access_key = awsSecretAccessKey,
	region_name = regionName,
) 
# 获取cloudfront
cloudfront = session.client('cloudfront')
# 创建失效
createInvalidation(cloudfront, distributionId, quantity = 1, filesPath = ['/*'])

# 创建失效
def createInvalidation(cloudfront, distributionId, quantity, filesPath):
    unique_tag = time.strftime('%Y%m%d%H%M%S', time.localtime())
    try:
        response = cloudfront.create_invalidation(
            DistributionId = distributionId,
            InvalidationBatch = {
                'Paths': {
                     'Quantity': quantity,
                     'Items': filesPath
                 },
                'CallerReference': unique_tag
            }
        )
        strResponse = str(response)
        print("返回结果: %s %s" %(strResponse, "创建成功 \n"))
    except cloudfront.exceptions.ClientError as e:
        print(e.response)

175. Call to unavailable function 'system': not available on iOS
解决：https://blog.csdn.net/linyang24/article/details/78053626 新引擎有解决

176. Xcode11.0版本编译Argument value 10880 is outside the valid range [0, 255]
解决：https://blog.csdn.net/ZFY11/article/details/101557310 新引擎有解决
#define BT_SHUFFLE(x,y,z,w) ((w)<<6 | (z)<<4 | (y)<<2 | (x))
改为
#define BT_SHUFFLE(x, y, z, w) (((w) << 6 | (z) << 4 | (y) << 2 | (x)) & 0xff)

177. error: Multiple commands produce：
解决：prjo->Target ->Build Phases->Copy Bundle Resources  删除非法的LICENSE和Info.plist配置

178. xcode开发者账号登录：
xcode->preferences->accounts->输入账号密码->manage certificates->没有develop certificates就add一个。添加一次即可。
选中工程target->signing&capabilities->选中automatically manage signing->选择team ->插入真机调试

179. 证书问题 DemoApp is automatically signed, but provisioning profile 2668e28b-cd73-4405-afff-1e5078bd354d been manually specified
解决：xxapp.xcodeproj显示包内容->文本编辑器打开 project.pbxproj 查找2668e28b-cd73-4405-afff-1e5078bd354d 改成空字符串

180. Unable to prepare for xxx development
解决：https://github.com/filsv/iOSDeviceSupport
拷贝对应的手机系统版本到目录：/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/DeviceSupport

181. 打越狱包:
选择target->Any ios Device(arm64,armv7)->菜单栏Product->Analyze->编译成功后在Produces文件夹下生成app。新建文件夹名Payload把生成的app拖入文件夹，压缩并改名xx.ipa即可

182. 设备管理设置中不允许在此台iPhone上:
解决：手机->设置->设备管理->信任描述文件

183. git 命令行操作:
设置账号密码：
$ git config user.name "qqqq"
$ git config user.password "123456"
$ git config user.email "xxx@qq.com"

查看配置：
git config --list

git pull 失败提示：
warning: 不建议在没有为偏离分支指定合并策略时执行pull操作。 
您可以在执行下一次pull操作之前执行下面一条命令来抑制本消息：
git config pull.rebase false  # 合并（缺省策略）
git config pull.rebase true   # 变基
git config pull.ff only       # 仅快进

您可以将 "git config" 替换为 "git config --global" 以便为所有仓库设置
缺省的配置项。您也可以在每次执行 pull 命令时添加 --rebase、--no-rebase，
或者 --ff-only 参数覆盖缺省设置。

首先理解什么是偏离分支：
当本地的分支落后于远程分支时，本地分支又自行修改项目文件生成了新的提交，这时本地分支再执行git pull命令就不能快进合并，并且还容易发生冲突。这时的本地分支便称为偏离分支，因为这时的本地分支的最新提交跟远程分支的最新提交不同，产生了偏离。
接着理解什么是合并策略：
合并策略便是 git merge --ff-only、git merge --no-ff、git merge --rebase这三种常见的合并策略，分别代表着快进合并、非快进普通合并、变基合并。
而我们执行不带任何选项的git pull命令时，Git就不知道我们到底想用哪种合并策略来执行git pull，因此Git会给出上述的警告文案，建议我们通过git config命令应该按照这三种合并策略的哪种来执行。

git文件 $ git add src/1.txt
git添加所有文件 $ git add -A

git提交: 
git commit -m "我的提交" 

git更新:
$ git pull

git提交:
$ git push

git文件状态:
$ git status

git丢去本地修改，回退到上一个版本：
git reset --hard HEAD^

Vim/Vi编辑器中保存文件并退出: 在底部命令行输入冒号(:)，然后输入写入并退出命令wq，然后按下Enter键

184. 添加xx.framework 记得添加对应的路径Search Paths->Framework Search Paths
使用#import <XXFramework/XXHeader.h>

185. 忽略文件夹
**/xcuserdata
忽略文件
.DS_Store
**/.DS_Store
.DS_Store?

186. xcode切换开发者账号导致：The app identifier "com.xx.xxx" cannot be registered to your development team because it is not available. Change your bundle identifier to a unique string to try again.
解决：修改包名，刷新后，再改回原来的包名

187. ios提交appstore问题：App Store Connect Operation Error ERROR ITMS-90474: "Invalid bundle. The “UIInterfaceOrientationPortraitUpsideDown,UIInterfaceOrientationPortrait” orientations were provided for the UISupportedInterfaceOrientations Info.plist key in the com.xx.io bundle, but you need to include all of the “UIInterfaceOrientationPortrait,UIInterfaceOrientationPortraitUpsideDown,UIInterfaceOrientationLandscapeLeft,UIInterfaceOrientationLandscapeRight” orientations to support iPad multitasking. 
解决：target->General勾选 "Requires full screen"

188. xcode升级到14.3工程问题：The armv7 architecture is deprecated. You should update your ARCHS build setting to remove the armv7 architecture.
解决：去掉valid_archs里  armv7, 只保留arm64(只支持iphone5s及以后的设备安装,见下表)
CPU的不同指令集	对应设备：
i386	模拟器32位处理器
x86_64	模拟器64位处理器
armv7	iPhone 3GS，iPhone4，iPhone 4s，iPad，iPad2，iPad3(The New iPad)，iPad mini，iPod Touch 3G，iPod Touch4
armv7s	iPhone5， iPhone5C，iPad4，iPod5
arm64	iPhone5s，iPhone6、7、8，iPhone6、7、8 Plus，iPhone X，iPad Air，iPad mini2(iPad mini with Retina Display)
arm64e	XS/XS Max/XR/ iPhone 11, iPhone 11 pro

189.xcode修改info.plist文件位置：build setting->info.plist file修改路径

190.xcode多个target使用宏控制：build setting-》 preprocessor macros: 添加tagert_xx=1
使用 #if tagert_xx == 1     #endif

191.target ->info添加 App Transport Security->Allow Arbitrary Loads 设置true 兼容http

192. 从字典取整数值，字典key为code的值类型可以为字符串或者数值类型:
int statusCode = -1;
if ([dict objectForKey:@"code"]) {
	statusCode = [[dict valueForKey:@"code"] intValue];
}

193. NSString* nsStr, std::string str互转:
std::string str = std::string([nsStr UTF8String]);
NSString* nsStr = [NSString stringWithUTF8String:str.c_str()];

194. 从字典取值：
std::string str = std::string([[dict objectForKey:@"email"] UTF8String]);
int num = [[dict valueForKey:@"num"] intValue];
double unitPrice = [dict[@"unitPrice"] doubleValue];
bool bol = [[dict objectForKey:@"bShowFloatingBtn"] boolValue];

195. rapidjson创建json字符串：
rapidjson::StringBuffer buf;
rapidjson::Writer<rapidjson::StringBuffer> writer(buf);

writer.StartObject();
writer.Key("opt"); writer.Bool(opt);
writer.Key("token"); writer.String(token.c_str());
writer.Key("statusCode"); writer.Int(statusCode);
writer.EndObject();
std::string jsonStr = buf.GetString();

196. git合并：Please enter a commit message to explain why this merge is necessary
切换到英文输入法, 按下esc, 输入:wq回车即可

197. iPhoneX到iPhone13是刘海屏(iphonex iphone11, 12, 13, 14)。iphone8及之前机型没有刘海屏。没有iphone9,10机型

198. git checkout . #本地所有修改的。没有的提交的，都返回到原来的状态

199. python快速搭建web服务：
REM py2
REM python -m SimpleHTTPServer 8001

REM py3
python -m http.server 8001

200.Android Studio 报错 Invalid Gradle JDK configuration found.
	删除项目中的如下两个文件：
	.idea/gradle.xml
	.idea/workspace.xml
	然后再重新进行编译。

201. gradle命令打包：
cd 到目录proj.android
执行: gradlew assembleDebug 或者 gradlew assembleRelease

202. android studio打包报错：Program type already present: com.huawei.hms.ads.identifier.AdIdVerifyException
解决：依赖冲突, 双击shift, 全局查找冲突的这个AdIdVerifyException, 发现有两个包都引入了这个,删掉一个包,重新构建.

203. Your development team, "XXX", does not support the In-App Purchase capability:
旧的xcode项目报错，cacpability下删除不了 In-App Purchase。 解决: target->Build Phases->Link Binary With Libraries下删除StoreKit.framework。

204. cocos2d-x cjson加入工程:
lua_extensions.c 下static luaL_Reg luax_exts数组加入 {"cjson", luaopen_cjson},
加头文件#include "cjson/lua_cjson.h"
win下添加libluacocos2d项目添加过滤器命名cjson,并添加external/lua/cjson下的文件。
android: 
cocos2d-x\cocos\scripting\lua-bindings\proj.android\android.mk 或者 frameworks\runtime-src\proj.android\app\jni\Android.mk
文件添加：
LOCAL_SRC_FILES加：
../../../../cocos2d-x/external/lua/cjson/fpconv.c \
../../../../cocos2d-x/external/lua/cjson/lua_cjson.c \
../../../../cocos2d-x/external/lua/cjson/strbuf.c

LOCAL_C_INCLUDES加：
			$(LOCAL_PATH)/../../../../cocos2d-x/external \
			$(LOCAL_PATH)/../../../../cocos2d-x/external/lua 
			
ios：cocos2d_lua_bindings项目下external添加cjson
 

205. git pull 问题：You have divergent branches and need to specify how to reconcile them.
执行：git config pull.rebase false (合并)

206. git命令遇到冲突不好出来 可以删除 eg：git rm 1.txt

207. TortoiseGit查看log显示 too many files to display。解决：TortoiseGit->setting->Advanceed->LogTooManyItemsThresholad设置大点 比如5000

208. xx.jar' to match attributes {artifactType=processed-jar} using transform JetifyTransform
 gradle.properties添加：
# 过滤Msc.jar
android.jetifier.blacklist=Msc.jar

209. java.lang.ClassNotFoundException: Didn't find class "android.support.v4.app.FragmentActivity
 gradle.properties添加：
android.enableJetifier=true
android.useAndroidX=true

210. android public boolean onKeyDown(int keyCode, KeyEvent event) 没有被调用。则可以使用：
@Override
public boolean dispatchKeyEvent(KeyEvent event) {
    if (event.getAction() == KeyEvent.ACTION_DOWN) {
        // keydown logic
        return true;
    }
    return false;
}

211.Android打包出错：java.util.concurrent.ExecutionException: com.android.builder.internal.aapt.v2.Aapt2InternalException: AAPT2 aapt2-3.3.1-5013011-windows Daemon #0: Unexpected error during compile 
解决：app下build.gradle的buildTypes内添加：
	aaptOptions.cruncherEnabled = false
	aaptOptions.useNewCruncher = false
	
212. Gradle sync failed: Could not install Gradle distribution from 'https://services.gradle.org/distributions/gradle-6.3-bin.zip'.  
解决：单独下载以上zip。打开项目gradle\wrapper下gradle-wrapper.properties。distributionUrl下载url,改成本地地址eg: distributionUrl=file:///E:/gradle-6.5-bin.zip

213. 
git添加某个文件到暂存区：git add xx.png
git添加(当前目录)所有已经修改或者删除的tracted文件到暂存区：git add -u
git添加(当前目录)所有已经修改或者删除或者没有tracted文件到暂存区：git add -A 

214. cpu指令集在线查询 https://www.cpu7.com/soc/high.html   https://www.mydrivers.com/zhuanti/tianti/01/index.html
adb查看设备cpu指令集:  adb shell getprop ro.product.cpu.abi
android ABI使用armeabi-v7a:arm64-v8a 在一些cpu指令集是armv8会闪退，则只保留armeabi-v7a。详细参考33条。

215. xsolla api接入
https://github.com/webcrawler/jokeme/blob/master/sdk_xsolla_demo.lua

216.查看远程主机端口是否开放:
telnet安装：控制面板-程序和功能-启用或关闭Windows功能-勾选Telnet客户端
telnet 8.135.2.1{空格}8080
或者在线工具：https://tool.chinaz.com/port

217. docker相关：
https://blog.csdn.net/qq_39611230/article/details/108641842

// 下载镜像 ubuntu
docker pull ubuntu
// 加载镜像,镜像名为ubuntu, 创建容器名ubuntu-container, 并启动命令行
docker run -it --name ubuntu-container ubuntu /bin/bash

//在ubuntu上安装编译工具GCC G++。
sudo apt update
sudo apt install build-essential

// 安装vim编辑器
sudo apt-get install vim-gtk

// 全局设置vim下tab键为4个空格(默认是8个)
打开编辑文件 /etc/vim/vimrc 末尾添加set ts=4


// 测试:
VimTest.h
VimTest.cpp
testVim.cpp(调用VimTest打印)

// 生成执行文件
g++ testVim.cpp VimTest.cpp -o testVim
// 运行
./testVim


# 查看当前所有正在运行的容器
docker ps
# 查看当前所有的容器
docker ps -a

// 停止，重启，启动容器，
docker stop 容器名或容器id. eg: docker stop 9e6bef73496d
docker start 容器名或容器id

docker restart 容器名或容器id. eg: ubuntu-container
// 开启容器并启动命令行
docker exec -it ubuntu-container /bin/bash

218. Microsoft Office 2021 ProPlus 专业增强版 简体中文版：
https://officecdn.microsoft.com/pr/492350f6-3a01-4f97-b9c0-c7c6ddf67d60/media/zh-cn/ProPlus2021Retail.img
激活工具：https://github.com/zbezj/HEU_KMS_Activator

219. 查看逍遥模拟器当前运行的app包名：
连接模拟器:  adb connect 127.0.0.1:21503
获取当前运行APP的appPackage，appActivity:  adb shell "dumpsys window | grep mCurrentFocus"

220. python快速搭建ftp服务器
下载包：pip install pyftpdlib -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com

创建py文件：
#encoding=utf-8

# pip install pyftpdlib -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com
from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer
 
# 实例化DummyAuthorizer来创建ftp用户
authorizer = DummyAuthorizer()
# 参数：用户名，密码，目录，权限
# test为当前目录下的目录
authorizer.add_user('joke', '000000', 'test', perm='elradfmwMT')
handler = FTPHandler
handler.authorizer = authorizer
# 参数：IP，端口，handler
server = FTPServer(('127.0.0.1', 211), handler)
server.serve_forever()
访问：ftp://127.0.0.1:211 

221. jenkins配置本地环境变量：
Dashboard->下拉->Manage jenkins->Configure System->Environment variables下配置PATH, 值eg:C:\Python27;C:\Windows\System32;C:\Program Files\Git\cmd

222. git clone 添加本地目录位置：git clone http://baidu.com/xx.git /Users/admin/Documents/works/hyx

223. android studio依赖外部工程: 
工程settings.gradle内添加:
include ':sub_proj'
project(':sub_proj').projectDir = new File(settingsDir, '../../sub_proj/platform/android/java')
项目下build.gradle添加：
dependencies {
    implementation project(':sub_proj')
}

224. xcode引入其他子工程sub_proj add files to... 使用Create folder references. Copy items if needed。
选中项目，选中target 在 Target Dependencies添加子工程sub_porj iOS, 
Link Binary With Libraries添加libsub_proj iOS.a
target下->Complie Souces添加.mm文件(或者在项目下New Group without Folder 然后add Files to 使用Create folder references,会自动添加进Complie Souces)
target下search path 没有找到这项:framework search paths. 选项选择"ALL"即可查看到全部。

225. Lint found fatal errors while assembling a release target. 
 app下 build.gradle添加 
 android {
    lintOptions {
        checkReleaseBuilds false
        // Or, if you prefer, you can continue to check for errors in release builds,
        // but continue the build even when errors are found:
        abortOnError false
    }
}

226.ios 添加app icons: 右键new file ->选择Asset catalog 命名Images。 选择刚创建的Images，右边小窗右键->ios->New Ios App Icons。命名为AppIcon,
选中AppIcon右边属性窗口选择ios选择All Sizes。拖拉对应的尺寸icon.
选择target ->General->App Icons and Launch Screen-> App Icon 填入刚创建的命为AppIcon。

227. More than one file was found with OS independent path  'lib/arm64-v8a/libsqlite.so'
解决：
android {
        packagingOptions {
        pickFirst 'lib/arm64-v8a/libsqlite.so'
    }
}

228. git 撤销文件本地修改：git checkout [filename]

229. frameworks/cocos2d-x/tools/simulator/libsimulator/proj.ios_mac/libsimulator.xcodeproj: warning: The iOS deployment target 'IPHONEOS_DEPLOYMENT_TARGET' is set to 8.1, but the range of supported deployment target versions is 11.0 to 16.4.99. (in target 'libsimulator iOS' from project 'libsimulator')
error: The armv7 architecture is deprecated. You should update your ARCHS build setting to remove the armv7 architecture. (in target 'libsimulator iOS' from project 'libsimulator')
解决：libsimulator工程building setting下 VALID_ARCHS 只保留arm64

230. Armv9.2 架构，仅支持 64 位指令集，不再兼容 32 位应用。eg: 8Gen3, 天玑9300

231. visual studio 使用正则替换文本： https://www.dongchuanmin.com/archives/471.html
eg: ['cancle']='取消'   使用正则匹配出所有的key： (.*)']='   匹配所有val： ']='(.*)
eg: 把文字：{delayTime = 3, content = "未对话的角色暗色"} 替换成 {delayTime = 3, content = FUNC("未对话的角色暗色")}
则：匹配: content = "(.*)" 替换: content = FUNC("($1)")   
$1表示匹配出的第1个值(.*)，$2表示匹配出的第2个值(.*)

232. android studio工程路径太长会导致编译失败

233. cocos2d-lua 使用vscode调试:
	1. vscode插件中心搜索luaide-lite, 安装。(此插件免费开源: https://github.com/wellshsu/luaide-lite)
	2. 把cocos2d-lua工程lua目录src拖入vscode, vscode左侧栏点击"运行调试"按钮, 选择调试Cocos-Launch, 选择边上设置按钮，弹出的launch.json.
	3. 在launch.json配置项 "name": "Cocos-Launch"，配置好exePath路径.
	4. 下载LuaDebug.lua放入src目录, 下载地址: https://github.com/wellshsu/luaide-lite/blob/master/test/cocos/src/LuaDebug.lua
	5. 在main.lua加入:
		if cc.Application:getInstance():getTargetPlatform() == 0 then
			local breakInfoFun, xpCallFun = require("LuaDebug")("localhost", 7003)
			cc.Director:getInstance():getScheduler():scheduleScriptFunc(breakInfoFun, 0.3, false)
		end
	6. 配置完成, 按下F5启动调试, 打断点, F10, F11调试.
	
234. 查看apk包信息:
aapt是安卓 SDK 默认带的，文件在 SDK/build-tools/xx.xx.xx(版本号)/ 中
aapt是Android Asset Packaging Tool 的缩写，用于处理APK文件。可以使用以下命令来查看APK包信息：
aapt dump badging your_app.apk

235. 接ios sdk发现sdk回调回游戏诱概率发生crash。发现是sdk回调是在子线程触发的。这样在子线程上操作游戏UI, 导致游戏crash。
// NSOperationQueue方式任务是在子线程中执行。
NSOperationQueue *queue = [[NSOperationQueue alloc] init];
[NSURLConnection sendAsynchronousRequest:request queue:queue completionHandler:^(NSURLResponse *response, NSData *data, NSError *error) {
	// 
}
		
解决: sdk回调过来，在主线程内通知游戏UI:
// 主线程中调用
dispatch_async(dispatch_get_main_queue(), ^{
	BOOL isMainThreadB = [[NSThread currentThread] isMainThread];
	NSLog(@"++++++++++++++++ isMainThreadB:%@", isMainThreadB ? @"YES" : @"NO");
	// 通知游戏UI
});

android在主线程中调用UI,使用runOnUiThread。 cocos2dx使用：
Cocos2dxHelper.runOnGLThread(new Runnable() {
	@Override
	public void run() {
		//此时已在主线程中，可以更新UI了
	}
});

236. xcode添加.a第三方静态文件 编译报错：
Undefined symbols for architecture arm64:
  "_OBJC_CLASS_$_XXSSSSS", referenced from:
      objc-class-ref in XXXXX.a(XKKKKKK.o)
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
解决：add Files to "proj"需要选择 "Create groups", "Copy items if needed".
如果还未解决，检查文档是否需要添加一些系统.framework,以上.a静态库可能需要用到.framework。 eg:在Link Binary With Libraries下添加StroeKit.framework。

237. 多台android设备连接的情况，选择其中一台adb调试:
查看设备:
C:\Users\Administrator>adb devices
List of devices attached
63e244520906    device
127.0.0.1:21503 device

adb logcat查看127.0.0.1:21503这台设备log:
adb -s 127.0.0.1:21503 logcat 

238. bat查找字符串是否包含的字符
@echo off
set a=55544333
set b=44
echo %a%| findstr %b% >nul && (
	echo %a%包含%b%
 ) || (
	echo %a%不包含%b%
)

239. sdk接入报错：
[XXSDK.aar] C:\Users\Administrator\.gradle\caches\transforms-1\files-1.1\XXSDK.aar\fd9a081bcdbf4c242e0bd1b83b71d7c3\AndroidManifest.xml:15:9-54 Error:
Missing 'package' key attribute on element package at [XXSDK.aar] AndroidManifest.xml:15:9-54
查看AndroidManifest.xml报错位置(15行)：
    <queries>
        <package android:name="com.project.sdk" />
    </queries>
使用queries元素需要Android Gradle 插件版本是 4.1及以上，因为旧版本的插件并不兼容此元素，出现"Element queries is not allowed here" 的警告，也可能会导致manifest合并失败。

软件包可见性:
在 Android 10 及之前的版本中，应用可以通过 queryIntentActivities() 这样的方法获取到设备中所有已安装的应用列表。在大多数情况下，
这种访问权限远超出了应用实际所需要的权限范围。随着我们不断加强对隐私保护的关注，我们将在 Android 11 (目标 API level 为 30)上引入一些新的变化，从而改变应用查询用户已安装应用并与之交互的方式。为了达到这一目的，我们为特定设备上所安装的应用列表带来了更好的访问控制。
为了更好地 "问责" 访问已安装应用的行为，默认情况下，以 Android 11 为目标平台  的应用默认将只能检测到部分过滤后的已安装应用。
如果想获取更多别的已安装应用列表信息，则需要在应用内的 Android manifest 中添加 <queries> 元素，从而拓宽访问范围。


240. Manifest merger failed : Attribute application@label value=(xxsdk) from AndroidManifest.xml:16:9-37
  	is also present at [XXSDK.aar] AndroidManifest.xml:21:9-41 value=(@string/app_name).
  	Suggestion: add 'tools:replace="android:label"' to <application> element at AndroidManifest.xml:14:5-57:19 to override.
第三方sdk中也定义了application@icon, application@label属性，则会与你的项目发生冲突。
那么解决的方法就是在你的Application节点中加入tools:replace来表示替换三方库中的相关属性，eg：
tools:replace="android:icon"
或者2个都替换 tools:replace="android:icon, android:label"
<application
        android:icon="@drawable/box_icon"
        android:label="@string/app_name"
        tools:replace="android:icon, android:label">

使用tools:replace需要在manifest根节点加上相关的引用，
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
          xmlns:tools="http://schemas.android.com/tools"
          package="com.bcoder.app">

241. gradle升级导致: Direct local .aar file dependencies are not supported when building an AAR.
解决: 将library模块中依赖方式 改成compileOnly fileTree
eg: compileOnly fileTree(dir: 'libs', include: '*.aar')
并拷贝一份aar到主工程libs目录，将主工程AAR依赖方式改成implementation fileTree
eg: implementation fileTree(dir: 'libs', include: '*.aar')

或者:  https://stackoverflow.com/questions/60878599/error-building-android-library-direct-local-aar-file-dependencies-are-not-supp
模拟android studio旧版本, 比如 android studio4.0.1 导入aar 生成响应的配置：

创建目录SDKModule里面创建build.gradle 内容格式如下:
configurations.maybeCreate("default")
artifacts.add("default", file('xx1.aar'))
artifacts.add("default", file('xx2.aar'))
把xx1.aar， xx2.aar 放在和build.gradle同一个目录

工程settings.gradle file添加:
include(":SDKModule")
project(':SDKModule').projectDir = new File(settingsDir, '../../xx/platform/android/SDKModule')

在需要的module对应的build.gradle dependencies加: 
implementation project(":SDKModule")


242. 使用androidX后报错：libs\xx.jar' using Jetifier. Reason: InvalidByteCodeException, message: Error processing 'com\xx\thridparty\a.class' bytecode.. (Run with --stacktrace for more details.)
           This is a known exception, and Jetifier won't be able to jetify this library.
解决: gradle.properits文件添加 android.jetifier.blacklist=xx.jar

243. java.lang.ClassNotFoundException: Didn't find class "com.google.gson.GsonBuilder
解决：gradle引用  implementation 'com.google.code.gson:gson:2.8.6'

java.lang.NoClassDefFoundError: Failed resolution of: Lokhttp3/OkHttpClient$Builder
解决：implementation 'com.squareup.okhttp3:okhttp:3.4.1'
	  implementation 'com.squareup.okhttp3:logging-interceptor:3.4.1'
	  
244. 打开app 收到弹窗提示 "此应用专为旧版Android打造，可能无法正常运行，请尝试检查更新或与开发者联系"
解决：提高targetSdkVersion。补充: targetSdkVersion升级时只能增加不能降低。targetSdkVersion高的apk无法被targetSdkVersion低的apk覆盖安装.

245. 接入sdk，在自定义的appliaction初始化sdk。sdk界面没有调用起来。log也没有任何相关的sdk信息。但是安装apk出现2个app图标。
排查原因是aar内AndroidManifest.xml Activity为android.intent.category.LAUNCHER。
解决：去掉项目activity内<intent-filter>标签。使用sdk的启动activity。

255. excel某一列值是另外一列值100倍再加coin，计算公式:
eg: P列值是A列值100倍再加字符coin。在P列第一行输入公式:=IF(A1="","",A1*100&"coin") 然后单元格左下角往下拉计算每一行值。
此处公式是判断A1单元格是否是空，空则返回空字符""。 非空则返回A1*100&"coin"。A1，下拉复制就是A2, A3, A4....

256. unity 使用monodevelop打开c#中文乱码。解决：tools->fonts->Text Editor->选择Monospace(Normal) 即可
257. window下 unity monodevelop 提示"Do you want to convert the line endings?". 解决：Project-Solution Option-Source ->Code-Code Formatting下 C# source code对应的Line endings 改成windows。

258. cocos2dx spine3.4升级到4.1, 参考:https://forum.cocos.org/t/topic/155493
1. 下载spine4.1, https://github.com/EsotericSoftware/spine-runtimes/tags 解压
2. 删除原工程目录cocos2d-x\cocos\editor-support\spine代码文件(保留工程文件)，
找到解压的spine4.1，把其目录spine-cpp下src, include下的.h .cpp代码文件拷贝到原工程spine目录下。
同样找到spine-cocos2dx目录把其src下代码文件拷贝到原工程spine目录下，注意旗下有目录v3，v4，指的是cocos2dx引擎版本。拷贝相应的版本目录下的代码文件。
3. 以上v3，v4内代码文件直接拷到spine下使用，外部引用代码内有些include路径根据编译error，需要修改下。rg:spine-cocos2dx.h内
4. win上libSpine工程Source Files 删除重新加入。
5. 新增 ANIMATION_INTERRUPT, ANIMATION_DISPOSE, EVENT_SPINE_ANIMATION_PREUPDATE, EVENT_SPINE_ANIMATION_POSTUPDATE 事件类型。
	EVENT_SPINE_ANIMATION_START,
	EVENT_SPINE_ANIMATION_INTERRUPT, // added by joke for spine4.1
	EVENT_SPINE_ANIMATION_END,
	EVENT_SPINE_ANIMATION_COMPLETE,
	EVENT_SPINE_ANIMATION_DISPOSE, // added by joke for spine4.1
	EVENT_SPINE_ANIMATION_EVENT,
	EVENT_SPINE_ANIMATION_PREUPDATE, // added by joke for spine4.1
	EVENT_SPINE_ANIMATION_POSTUPDATE, // added by joke for spine4.1
	
6. lua binding后的文件lua_cocos2dx_spine_auto，lua_cocos2dx_spine_manual 见后面下载链接
7. spine事件兼容spine3.4, 在lua_cocos2dx_spine_manual.cp, executeSpineEvent通知lua添加eventData
   在spineEvent.insert(spineEvent.end(), LuaValueDict::value_type("event", LuaValue::dictValue(dict)));后添加:
   spineEvent.insert(spineEvent.end(), LuaValueDict::value_type("eventData", LuaValue::dictValue(dataDict)));
完整文件下载链接: https://github.com/webcrawler/spine4.1-cocos2dx

259. github 提交失败 "Support for password authentication was removed on August 13, 2021. Please use a personal access token instead"
解决：到个人中心 setting->developer setting ->personal access tokens创建token。提交密码输入token即可

260. cocostudio 导出的csb资源转成工程csd: https://github.com/lyzz0612/csb2csd
     fork for me: https://github.com/webcrawler/csb2csd

261. 升级android gradle plugin version工程报错: CustomMessageMissingMethodException: Could not find method compile() for arguments [project ':xxxxx'] on object of type org.gradle.api.internal.artifacts.dsl.dependencies.DefaultDependencyHandler.
解决：compile改成api

262. app\AndroidManifest.xml:21:9-31:20 Error:
	android:exported needs to be explicitly specified for element <activity#com.game.xx.xx>. Apps targeting Android 12 and higher are required to specify an explicit value for `android:exported` when the corresponding component has an intent filter defined. See https://developer.android.com/guide/topics/manifest/activity-element#exported for details.
	解决：activity下添加android:exported属性  <activity  android:exported="true"
	
263. 报错: Program type already present: android.support.v4.app.INotificationSideChannel
双击shift 查找class: INotificationSideChannel, 有多个包引入，删掉一个即可
找到多个路径：
1. .gradle\caches\transforms-1\files-1.1\core-1.9.0.aar\000xx45854\jars\classes.jar!\android\support\v4\app\INotificationSideChannel.class
2. android\java\libs\android-support-v4.jar!\android\support\v4\app\INotificationSideChannel.class
发现是引用的project里面添加了 implementation 'androidx.appcompat:appcompat:1.6.1', 删除, 只保留libs\android-support-v4.jar 即可。
 
 
264. cocos creator使用vscode开发相关:
	visual studio 打开运行的按钮"start"变成"attach". 解决：没有选择启动项目

	cocos creator 使用vscode配置  https://docs.cocos.com/creator/3.2/manual/zh/getting-started/coding-setup.html
	0. 在 Creator 编辑器主菜单里执行 开发者 -> VS Code 工作流 -> 添加编译任务。该操作会在项目的 .vscode 文件夹下添加 tasks.json 任务配置文件。后续再vscode有修改js，直接Ctrl+p, 激活 快速打开 输入框，
	然后输入 task compile, 通知 cocos creator编译。
	Ctrl+Shift+p 打开输入框，输入keybindings.json。可以自定义快捷键。比如编译脚本：
	// 将键绑定放在此文件中以覆盖默认值
	[
		{
			"key": "ctrl+b", // 请配置自己习惯的快捷键
			"command": "workbench.action.tasks.runTask",
			"args": "compile"
		}
	]

	1. 在 Cocos Creator打开项目，主菜单-> 开发者 -> VS Code 工作流 -> 安装 VS Code 扩展插件。
	该操作会将 Cocos Creator API 适配插件安装到 VS Code 全局的插件文件夹中，一般在用户 Home 文件夹中的 .vscode/extensions 目录下。这个操作只需要执行一次，如果 API 适配插件更新了，则需要再次运行来更新插件。
	2. 主菜单->开发者 -> VS Code 工作流 -> 更新 VS Code 智能提示数据。该操作会将根据引擎 API 生成的 creator.d.ts 数据文件复制到项目根目录下（注意是在 assets 目录外面）
	3. vscode 打开项目，主菜单的 File -> Open， 在弹出的对话框中选择您的项目根目录，也就是 assets、project.json 所在的路径。
	4. 设置文件显示和搜索过滤，vscode菜单下，文件->首选项->设置，找到setting.json (user下)。添加如下，将为 VS Code 设置搜索时排除的目录，和在文件列表中隐藏的文件类型。
	新版 VS Code 打开用户配置文件 USER SETTINGS 时可能是处于 UI 界面状态，此时在上方的搜索框中输入 exclude 搜索，然后在 search.exclude 和 files.exclude 模块中点击 Add Pattern 补充缺少的内容即可。
	{
		"search.exclude": {
			"**/node_modules": true,
			"**/bower_components": true,
			"build/": true,
			"temp/": true,
			"library/": true,
			"**/*.anim": true
		},
		"files.exclude": {
			"**/.git": true,
			"**/.DS_Store": true,
			"**/*.meta": true,
			"library/": true,
			"local/": true,
			"temp/": true
		}
	}
	5. Cocos Creator 编辑器的菜单栏中点击 开发者 -> VS Code 工作流 -> 添加 Chrome Debug 配置，这个菜单命令会在你的项目文件夹下添加一个 .vscode/launch.json 文件作为调试器的配置，
	之后你就可以在 VS Code 里点击左侧栏的 调试 按钮打开调试面板，并在最上方的调试配置中选择 Creator Debug: Launch Chrome，然后点击绿色的开始按钮开始调试。（js文件打断点，按F5调试）
	
265. cocos creator使用webstorm开发相关:
	webstorm激活：打开https://3.jetbra.in/ 找到有星号的连接打开，下载jetbra.zip，运行里面scripts\install-current-user.vbs。等待提示"Done"，再回到刚才打开的页面copy code。
	webStorm按F5，"Edit Configuration" -> add->JavaScript Debug-> JavaScript Debug->URL: http://localhost:7456/ (需要安装chrome插件 JetBrains IDE Support Chrome_2_0_7_.crx ？ )

	webStorm 隐藏文件：文件->设置->编辑器->文件类型->隐藏文件和文件夹：*.meta  library local packages build temp settings ...
	运行->调试->编辑配置->新增调试， Url填http://localhost:7456  浏览器：Chrome, 本地文件目录添加 clientxx。
	文件->设置->搜索JSHint 不启用
	 
	 webstorm中js代码中出现红色波浪线。解决：setting->Langusges&Frameworks->JavaScript->Code Quality Tools->JsHint关闭即可。
	 
	 webstorm快捷键：
	 1. 返回上一步 Atl+左箭头。
	 2. view->tool window->terminal打开终端执行./up.bat   快捷键Ctrl+`打开终端 
	
	webstorm修改了js，激活cocos creator编译: http://localhost:7456/update-db 请求执行即可。webStorm内可以写命令调用此请求，实现代码编译。
	创建up.bat内容如下： curl http://localhost:7456/update-db -s   存放在项目目录下，在webstorm终端执行./up.bat即可
	
	webstorm编辑器界面标签页用法设置成和visual studio一样，不滚动。File->setting->editor->general->edtior tabs->一行显示(挤压标签页)。
	webstorm搜索文件快捷键: 按下 Shift 两次。 搜索内容: Ctrl + Shift + F 
	

266. cocos creator相关：
	1. cocos creator 资源加密：https://forum.cocos.org/t/cocos-creator/95492 资源更新：https://github.com/cocos-creator/cocos-tutorial-hot-update/tree/below-2.3.3
	2. chrome打开开发者工具 快捷键 Ctrl + Shift + j 或者 F12
	3. 在build-templates里改下 html, 增加一个根据useragent判断是desktop还是mobile的函数, 然后根据不同的结果用document.write 显示对应的内容即可. 参考:https://forum.cocos.com/t/topic/59014
	4. 从cocostudio导入到cocos creator，有些default image找不到。查看其他creator工程找到internal本地目录。找到default-assets拷贝进工程目录即可。
	5. Widget 组件会自动调整当前节点的坐标和宽高，不过目前调整后的结果要到下一帧才能在脚本里获取到，除非你先手动调用 updateAlignment。eg: node.getComponent(cc.Widget).updateAlignment()
	6. mac下打开cocos creator。报错:Error: EROFS: read-only file system, open '/Volumes/Cocos Creator/CocosCreator.app/Contents/Resources/static/default-assets/image.meta'. 
	   解决:安装后cocoscreator.app拖到application里，再打开。
	7. creator2.0.9 cc.loader.load，原生平台远程加载不支持图片文件以外类型的资源。
	8. 从已发布的文件中查找使用的引擎版本：搜索发布的目录 "engineVersion" 
	9. cocos creator 触摸在google chrome调试没有反应，在手机浏览器上正常。解决：google chrome上关闭手机调试模式调试。
	10. cocos creator在浏览器运行。google chrome切换到sources找到对应的代码可以打断点调试。
	11. scene目录配置为bundle。运行报错;Can not load the xxScene because it was not in the build settings before
	12.莫名奇妙出现：
		Uncaught TypeError: getComponent: Type must be non-nil
		at Node.addComponent (base-node.ts:930:23)
		at Tt.createInspectorCanvas (inject.js:110:276989)
		at Tt.addDraw (inject.js:110:276678)
		at Tt.initDrawNode (inject.js:110:272890)
		at Mt.update (inject.js:110:281077)
		at inject.js:110:302880
		at Director.emit (callbacks-invoker.ts:344:25)
		at Director.mainLoop (director.ts:906:18)
		at _callback (game.ts:695:26)
	查看堆栈是挂在(inject.js:110:276989)：chrome-extension://hejbkamkfnkifppoaljcidepkhgaahcj/inject.js 想起来是装了一个插件 cc-inspector. 删除即可。
	
	13. cocos creator 预览运行在google chrome上，使用快捷键 Ctrl + P 查找js文件并打断点。
	14. cocos creator spine预览插件:https://github.com/ichenpipi/cocos-skeleton-viewer。
	    下载地址:https://gitee.com/ichenpipi/ccc-skeleton-viewer/releases/download/v1.4.0-v3/ccc-skeleton-viewer.zip 下载解压大片项目packages目录下。
	15. cocos creator2.0.9 发布到android studio.编译问题:
		1. "Parameter specified as non-null is null: method com.android.build.gradle.internal.cxx.configure.JsonGenerationVariantConfiguration., parameter ndkVersion"
		    解决: 修改ndk版本为r16. (ndk.dir=E\:\\android\\sdk\\ndk\\android-ndk-r16) 文档:https://docs.cocos.com/creator/2.0/manual/zh/publish/setup-native-development.html
	    2. inspector_socket_server.o': 'No such file or directory'
			解决: 见编号277. 路径太深导致
		3. 
		
	16. 微信小游戏相关:=> 网站入口: https://mp.weixin.qq.com/ 发布流程：https://gamemaker.weixin.qq.com/doc/minigame/release.html
	
	17. 发布字节小游戏整包需要小于20MB。图片压缩，分包使用"合并所有json"
	    js混淆:assets/main/index.js 文件拖到此处混淆 https://obfuscator.io/  混淆后的文件体积会增大。
		
	18. 微信小游戏分包加载包大小的限制: 
			整个小游戏所有主包+分包大小不超过 30M
			主包不超过 4M
			单个普通分包不限制大小
			单个独立分包不超过 4M
			
		抖音小游戏分包加载包大小的限制: 
			•普通小游戏包
			未配置分包的场景下，每个小游戏允许上传的代码包总大小上限为 20MB。
			•分包后小游戏包
			对于配置了分包的小游戏，默认限制为：
			小游戏整体包（小游戏包整个目录）大小不超过 20MB；
			单个主包不超过 4MB；
			单个分包大小不超过 20MB。
			•开放数据域
			开放数据域文件类似子包概念，该文件 / 目录的大小不超过 4MB。
	
	19. 

267. cocos2dx3.13.3 批量创建spine报错。https://github.com/cocos2d/cocos2d-x/issues/16602 升级到cocos2dx3.14.1问题解决。或者单但升级spine到3.8 https://www.cnblogs.com/tomaszheng/p/14807373.html

268. ios project  duplicate 一个target。导致编译出错: "Multiple commands produce 'xxxx/Build/Products/Debug-iphoneos/xx.app/Info.plist" 解决：在此target下 build Phases->Copy Bundle Resources下删除 info.plist

269. ios： Undefined symbol: _OBJC_CLASS_$_SKAdNetwork。 解决: Build Phases下添加StoreKit.framework

270. 查看apk签名信息 keytool -list -printcert -jarfile xx.apk

271. targetSdkVersion为31, 启动的 Activity需要设置属性 android:exported="true" 否则无法安装。

272. adb 连接mumu模拟器：
	1. 进入mumu模拟器
	2、进入设置-关于手机，连续点击版本号进入开发者模式；
	3、进入设置-系统-开发者选项；
	4、启用USB调试，允许使用ADB与模拟器通信；
	5、打开命令行，通过以下adb命令连接模拟器：adb connect 127.0.0.1:7555
	
273. 当apk的targetSdkVersion为30时，v2SigningEnabled false(只使用v1签名)，在android 11的手机上安装时，会出现安装失败的问题。：INSTALL_PARSE_FAILED_NO_CERTIFICATES: Scanning Failed.: No signature found in package of version 2 or newer for package
解决: targetSdkVersion降低, 改成29。获取添加v2Sign。(注：修改v2SigningEnabled工程需要clean再重新打包。)

274.  SSH连接GUI工具：xshell

275. apk修改assets资源无需apktool，简单重新打包：改成.zip双击打开找到assets下要修改的目录。把修改完的文件拖入其中。再重新改成.apk 即可。

276. vs编译报错： Il mismatch between 'P1' version '20150812' and 'P2' version '20130802'。解决 tools->Extends and Updates->Updates->更新Visual Studio 2015 Update3。
	但是还是安装失败，直接下载离线iso文件 mu_visual_studio_2015_update_3_x86_x64_dvd_8923065.iso，解压文件进入\ServicedSetupPayloads\OfflineCache\installers\VSU_14.0.25420\en\0 用管理员权限执行vsupdate_KB3022398.exe
	iso下载链接：http://download.microsoft.com/download/c/2/6/c26892d8-6a5d-4871-9d46-629f4d430146/vs2015.3.vsu.iso
	
277. cocos creator导出的android项目，编译报错：error: unable to open output file 'E:/work/xx/build/jsb-link/frameworks/runtime-src/proj.android-studio/app/build/intermediates/ndkBuild/debug/obj/local/arm64-v8a/objs-debug/cocos2dx_static/scripting/js-bindings/jswrapper/v8/debugger/inspector_socket_server.o': 'No such file or directory'
解决：路径太长导致, 默认情况下，Windows使用路径长度限制(MAX_PATH)256个字符。修改目录？

278. virtualBox 切换本地电脑或者虚拟机内操作系统，快捷键 Right Ctrl。即键盘右边的Ctrl键
	 virtualBox 安装centos7,使用桥接网络，命令 ip addr查看本地ip。使用xshell登录centos。执行安装宝塔脚本，官网下找到安装脚本： https://www.bt.cn/ 
	 centos需要root超级管理员权限，输入命令su切换到root超级管理员。root没有设置默认密码则，需要sudo passwd root 设定root密码
	 
279. sublime Text4 激活码：
—– BEGIN LICENSE —–

Mifeng User

Single User License

EA7E-1184812

C0DAA9CD 6BE825B5 FF935692 1750523A

EDF59D3F A3BD6C96 F8D33866 3F1CCCEA

1C25BE4D 25B1C4CC 5110C20E 5246CC42

D232C83B C99CCC42 0E32890C B6CBF018

B1D4C178 2F9DDB16 ABAA74E5 95304BEF

9D0CCFA9 8AF8F8E2 1E0A955E 4771A576

50737C65 325B6C32 817DCB83 A7394DFA

27B7E747 736A1198 B3865734 0B434AA5

—— END LICENSE ——

280. IDA调试SO库：https://blog.csdn.net/StepTp/article/details/120983405

cd D:\program files\IDA 7.0\dbgsrv
D:

连接android设备
adb push D:\program files\IDA 7.0\dbgsrv\android_x86_server /data/local/tmp/
adb shell
su
chmod 777 /data/local/tmp/android_x86_server 
# 启动
./data/local/tmp/android_x86_server

adb forward tcp:23946 tcp:23946

adb shell "dumpsys window | grep mCurrentFocus"

adb shell am start -D -n com.x.x.ss/com.game.ss.MainActivity

执行sdk下android monitor: E:\android\sdk\tools\monitor.bat 查看以上app使用端口

jdb -connect com.sun.jdi.SocketAttach:hostname=localhost,port=8638    #  port=8638为以上 com.x.x.ss/com.game.ss.MainActivity使用端口

ida 设置...

281.搭建git服务器。GitBlit是一款开源的、轻量级的Git服务器。http://www.gitblit.com/
下载解压
修改：gitblit-1.9.3\data\defaults.properties：
git.repositoriesFolder = E:/gitRepository
server.httpPort = 12301 # 使用的端口不要与已有端口冲突
server.httpBindInterface = 192.168.1.8
server.httpsBindInterface = 192.168.1.8
执行目下gitblit.cmd 控制台执行完会显示后台地址。http://192.168.1.8:12301/
默认登录账户密码：admin/admin 可在data文件夹users.conf中修改账号密码。

设置成服务方式（Windows Service）启动Gitblit（开机自动启动）
找到nstallService.cmd 文本打来：
设置: SET ARCH=amd64 //64位系统   /*SET ARCH=X86 //若为32位系统*/
添加: SET CD=D:\gitblit-1.9.3   //这里CD的值是我们GitBlit的文件夹路径
修改: --StartParams="" ^        //参数设置为空
重新执行gitblit.cmd。任务管理器->服务 找到gitblit 右键启动

282. cocos creator2.3 发布小游戏子包:
asstes/gameres 配置为子包。构建发布，远程服务器地址填入http://121.25.146.193/update/
打开构建好后目录找到subpackages，把该文件夹拷贝到远程服务器目录update/下。再删除本地subpackages.

283. 在施行夏令时的国家，一年里面有一天只有23小时（夏令时开始那一天），有一天有25小时（夏令时结束那一天），其他时间每天都是24小时。

284. JavaScript 教程：https://wangdoc.com/javascript/  ES6标准入门教程: https://wangdoc.com/es6/  TS：？？

285. Samsung Galaxy Z Flip6 安装apk失败，提示"應用程式套件無效，因此未能完成安装"。提示信息查询https://www.androidos.net.cn/android/10.0.0_r6/raw/frameworks/base/packages/PackageInstaller/res/values-zh-rTW/strings.xml
错误信息为"install_failed_invalid_apk",

286. 查看apk是否为debug打的包：使用apktool工具，apktool d xx.apk  查看AndroidManifest.xml文件是否存在 android:debuggable="true"

287. 微信小游戏体验版本广告显示正常，线上版本不展示。问题：在小程序后台->广告管理->审核提示 "组件审核:小程序广告组件关闭原因：违反小程序流量主广告组件规范，广告展示永久关闭。"
     详情: "1.流量主通过不正当刷粉方式开通流量主账号。"
	 
288. cocos creator2.0.9 i18n插件在ide不存在，官方不维护了。到https://github.com/cocos-creator-packages/i18n/tree/master/runtime-scripts。下载runtime-scripts放入项目Scripts

289. adb logcat 只显示需要的输出(白名单), 使用 grep
	adb logcat | grep "cocos2d-x debug info"
	
	adb logcat 过滤过滤不需要的输出(黑名单), 加 -v即可
	adb logcat | grep -v "TimeStats: RenderEngineTimes are already at its maximum size"

290. 
Egret相关:
	导出android工程资源：egret publish Demo -target android
	下载工具放 C:\Users\Administrator\AppData\Roaming\EgretLauncher\download
	文档：
	https://www.egret.uk/egretengine2d/
	https://github.com/egret-labs/egret-android-support
	http://www.uustory.com/?p=2273
	引擎文档：https://github.com/egret-labs/egret-docs
	UI Editor文档：https://github.com/egret-labs/egret-ui-editor-opensource
	https://www.kuwwz.com/#/EUI/README
	原生发布文档：https://www.egret.uk/egretnative/
	
	Egret Wing快捷键:
		ctrl+p: 查找文件
		F5：启动或者调试当前项目
		*Ctrl+Shift+O: 能够打开快速大纲面板，显示当前文件中定义的所有类，方法和变量。
		*Shift+F12：查找所有引用
		Alt+Left：切换到上一个编辑位置
		*Alt+Shift+F: 格式化代码
	
	Egret Wing排查某文件夹：文件->首选项->工作区设置：
	// 将设置放入此文件中以覆盖默认值和用户设置。
	{
		...
		// 配置排除文件和文件夹的匹配模式.
		"files.exclude": {
			...
			"**/bin-debug": true
		}
	}
	
	发布android:
	egret publish -target android
	生成的工程在项目同一层。从(已经安装)C:\Users\Administrator\AppData\Roaming\EgretLauncher\download\EgretNativeSupport\1.1.2\android\template
	下拷贝android工程到刚生成的目录。
	(EgretNativeSupport是从从https://ogi.teracloud.jp/sbrowser/#https://ogi.teracloud.jp/ds/dav/1202a78a4d474762/下载supports_download.zip解压放入以上目录)
	
	
		
291. vscode:
要操作光标所在文件中的所有代码块：
折叠所有 Ctrl+K+0
展开所有 Ctrl+K+J

仅仅操作光标所处代码块内的代码：
折叠 Ctrl+Shift+[
展开 Ctrl+Shift+]

或者Ctrl+Shift+P 搜索 fold和unfold

292. cocos creator2.4.5 替换加载页cocos图：
	D:\ProgramData\cocos\editors\Creator\2.4.5\resources\static\img\logo_portrait.png。找个透明图替换
	
293. android studio 报错"Invalid Gradle JDK configuration found". 解决: Project Structure->SDK Location->有两个可供设置的选项, 一个是系统默认的 JDK 路径, 一个是 Android Studio 默认的 JDK 路径, 切换一下即可。
修改后继续报错"No matching variant of com.android.tools.build:gradle:7.4.2 was found". 解决:https://blog.csdn.net/msn465780/article/details/129981203. 更改GradleJDK为Java11

294. android studio 全局搜索快捷键： ctrl + shift + f

295. android studio报错：Could not determine java version from '11.0.8'.
	解决：Grade 和 JDK 的版本不兼容，修改。
	egret5.3.10 android工程：Gradle Plugin: 3.3.1, Gradle Version: 4.10.1, JDK: 1.8
	
296. android studio打包出错: Failed to read key AndroidDebugKey from store debug.keystore。解决修改JDK为11版本

297. 安装apk提示："這個應用程式是專為舊版 Android 打造，因此不含最新的隱私保護服務。 安装這個應用程式可能會讓装置面臨風險。進一步瞭解 Play 安全防護。"
	解决：顯示提示：「這個應用程式是專為舊版 Android 打造，因此不含最新的隱私保護服務」
	通知的原因：只有在應用程式的 targetSdkVersion 比目前的 Android API 級別低 2 個以上版本時，才會顯示這些 Play 安全防護警告。舉例來說，如果使用者使用搭載 Android 13 (目前 API = 33) 的裝置，在安裝任何以 API 級別 31 以下為目標的 APK 時，系統會發出警告。您可以在 API 級別頁面查看 Android 版本和對應的 API 級別。

	如果裝置的 API 級別低於目標 API 級別，就不會收到警告。每個新的 Android 版本都會推出變更，可提升安全性與效能，並改善 Android 使用者體驗。其中某些變更僅適用於透過 targetSdkVersion 資訊清單屬性 (也就是目標 API 級別) 明確宣告支援的應用程式。如果您設定讓應用程式支援最新的 API 級別，就能讓使用者享受到這些提升的效益，同時可繼續讓您的應用程式在較舊版本的 Android 上正常運作。指定使用最新 API 級別也有助於讓應用程式充分發揮平台的最新功能，讓使用者用得更加愉快。

	建議的開發人員行動：

	為確保各 Android 版本的相容性，開發人員應確保任何應用程式的新版本都指定最新的 API 級別。如需變更應用程式目標 API 級別的建議，請參閱遷移指南。
	https://developers.google.com/android/play-protect/warning-dev-guidance?hl=zh-tw
	
298. android studio打包，targetSdkVersion从28升级到33后 打出的apk在模拟器(android 12)安装不了，但是在android 9还能安装。
	解决：Android 12（API 31）起，所有包含 <intent-filter> 的组件（Activity、Service、Receiver）​​必须显式声明 android:exported​​（true 或 false）。
	<activity android:name=".MainActivity" android:exported="true"> <!-- 允许外部启动 -->
		<intent-filter>
			<action android:name="android.intent.action.MAIN" />
		</intent-filter>
	</activity>
	<receiver android:name=".MyReceiver" android:exported="false"/> <!-- 仅内部使用 -->
	
299. 