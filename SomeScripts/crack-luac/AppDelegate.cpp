#include "AppDelegate.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "audio/include/SimpleAudioEngine.h"
#include "cocos2d.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif

}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // set default FPS
    Director::getInstance()->setAnimationInterval(1.0 / 60.0f);

    // register lua module
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);

    register_all_packages();

	 _xxteaSign = "7k7kgame";
	_xxteaKey = "zhaoxin";

	string filePath = "E:/WDJDownload/Apps/222/assets/src";

	string format = ".*";
	vector<string> files;
	GetAllFormatFiles(filePath, files, format);
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		CCLOG(files[i].c_str());
		auto first = files[i].find_first_of(".");
		if (first != string::npos)
		{
			if (files[i].size() == first + 5) // luac
			{
				string sx = files[i].substr(first + 1);
				if (sx == "luac")
				{
					crack(files[i].c_str());
				}
			}
		}
	}

    return true;
}

#include <iostream>  
#include <string>  
#include <io.h>  
using namespace cocos2d;

//获取特定格式的文件名  
void AppDelegate::GetAllFormatFiles(string path, vector<string>& files, string format)
{
	//文件句柄    
	long   hFile = 0;
	//文件信息    
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
					GetAllFormatFiles(p.assign(path).append("/").append(fileinfo.name), files, format);
				}
			}
			else
			{
				
				//files.push_back(p.assign(fileinfo.name));  //将文件路径保存，也可以只保存文件名:    p.assign(path).append("\\").append(fileinfo.name) 
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}


//深度优先递归遍历当前目录下文件夹和文件及子文件夹和文件  
void AppDelegate::DfsFolder(std::string path, int layer)
{
	_finddata_t file_info;
	string current_path = path + "/*.*"; //也可以用/*来匹配所有  
	int handle = _findfirst(current_path.c_str(), &file_info);
	//返回值为-1则查找失败  
	if (-1 == handle)
	{
		CCLOG ( "cannot match the path" );
		return;
	}

	do
	{
		//判断是否子目录  
		if (file_info.attrib == _A_SUBDIR)
		{
			//递归遍历子目录  
			//打印记号反映出深度层次  
			//for (int i = 0;i<layer;i++)
			//	CCLOG ("--");
			//CCLOG (file_info.name);
			int layer_tmp = layer;
			if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0)  //.是当前目录，..是上层目录，必须排除掉这两种情况  
				DfsFolder(path + '/' + file_info.name, layer_tmp + 1); //再windows下可以用\\转义分隔符，不推荐  
		}
		else
		{
			//打印记号反映出深度层次  
			//for (int i = 0;i < layer;i++)
			//	CCLOG("--");
			CCLOG (file_info.name);
			//crack();
		}
	} while (!_findnext(handle, &file_info));  //返回0则遍历完  
											   //关闭文件句柄  
	_findclose(handle);
}

#include "external/xxtea/xxtea.h"

void AppDelegate::crack(const char* filename)
{
	static const std::string BYTECODE_FILE_EXT = ".luac";
	static const std::string NOT_BYTECODE_FILE_EXT = ".lua";

	std::string buf(filename);
	//
	// remove .lua or .luac
	//
	size_t pos = buf.rfind(BYTECODE_FILE_EXT);
	if (pos != std::string::npos)
	{
		buf = buf.substr(0, pos);
	}
	else
	{
		pos = buf.rfind(NOT_BYTECODE_FILE_EXT);
		if (pos == buf.length() - NOT_BYTECODE_FILE_EXT.length())
		{
			buf = buf.substr(0, pos);
		}
	}

	FileUtils *utils = FileUtils::getInstance();
	//
	// 1. check .lua suffix
	// 2. check .luac suffix
	//
	std::string tmpfilename = buf + NOT_BYTECODE_FILE_EXT;
	if (utils->isFileExist(tmpfilename))
	{
		buf = tmpfilename;
	}
	else
	{
		tmpfilename = buf + BYTECODE_FILE_EXT;
		if (utils->isFileExist(tmpfilename))
		{
			buf = tmpfilename;
		}
	}

	std::string fullPath = utils->fullPathForFilename(buf);
	Data data = utils->getDataFromFile(fullPath);
	int rn = 0;
	if (!data.isNull())
	{
		const char *chunk = (const char*)data.getBytes();
		int chunkSize = (int)data.getSize();
		const char *chunkName = fullPath.c_str();

		auto skipBOM = [](const char*& chunk, int& chunkSize) {
			// UTF-8 BOM? skip
			if (static_cast<unsigned char>(chunk[0]) == 0xEF &&
				static_cast<unsigned char>(chunk[1]) == 0xBB &&
				static_cast<unsigned char>(chunk[2]) == 0xBF)
			{
				chunk += 3;
				chunkSize -= 3;
			}
		};

		//"2dxLua", strlen("2dxLua"), "XXTEA", strlen("XXTEA")
		_xxteaSignLen = strlen(_xxteaSign);
		_xxteaKeyLen = strlen(_xxteaKey);

		if ( strncmp(chunk, _xxteaSign, _xxteaSignLen) == 0)
		{
			// decrypt XXTEA
			xxtea_long len = 0;
			unsigned char* result = xxtea_decrypt((unsigned char*)chunk + _xxteaSignLen,
				(xxtea_long)chunkSize - _xxteaSignLen,
				(unsigned char*)_xxteaKey,
				(xxtea_long)_xxteaKeyLen,
				&len);
			skipBOM((const char*&)result, (int&)len);


			string ss = string(filename);
			size_t pos = ss.find_first_of(".");
			string str = ss.substr(0, pos + 1) + "lua";

			// 保存解密后的lua
			Data data;
			data.copy(result, len);
			utils->writeDataToFile(data, str);
			// 删除luac
			FileUtils::getInstance()->removeFile(ss);

			free(result);
		}
		else
		{
			skipBOM(chunk, chunkSize);
		}
	}
}


#include <sys/stat.h> 
void AppDelegate::mkdirs(char *muldir)
{
	//int i, len;
	//char str[512];
	//strncpy(str, muldir, 512);
	//len = strlen(str);
	//for (i = 0; i < len; i++)
	//{
	//	if (str[i] == '/')
	//	{
	//		str[i] = '\0';
	//		if (access(str, 0) != 0)
	//		{
	//			mkdir(str, 0777);
	//		}
	//		str[i] = '/';
	//	}
	//}
	//if (len > 0 && access(str, 0) != 0)
	//{
	//	mkdir(str, 0777);
	//}
	//return;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
