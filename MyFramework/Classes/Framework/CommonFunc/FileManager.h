
#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include "cocos2d.h"
#include <iostream>
#include <fstream>

using namespace cocos2d;
using namespace std;

// 用户信息
typedef struct _AccountInf
{
    char uid[128];
	char name[128];
}AccountInf;

class CFileManager
{
public:
	static CFileManager* instance(); 
	static void freeInstance();

public:
	CFileManager();
	virtual ~CFileManager();

public:
	bool checkFileIsExit(const std::string& fullPathName);
    
public:
    AccountInf readAccountInf(const char* uid, const char* defaultName = "");
    bool writeAccountInf(const char* uid, const char* name);

public:
	void saveUserInfo(string& name, string& pwd);
	vector<string> getUserInfo();
};

#define g_fileMgr CFileManager::instance()

#endif