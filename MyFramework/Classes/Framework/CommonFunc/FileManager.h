/**
*  @brief: 文件读写
*  @Created by fjut on 16-11-28
*/

#ifndef __H_FILE_MANAGER_H__
#define __H_FILE_MANAGER_H__

#include "cocos2d.h"

using namespace cocos2d;

// 用户信息
typedef struct _UserInf
{
	std::string name;
	std::string pwd;
	// todo add
}UserInf;
typedef std::map<std::string, UserInf> UserMap;

// public config
typedef struct _CommonCfg
{
	bool bMusic;
	bool bEffect;
	// todo
}CommonCfg;

class FileManager
{
public:
	FileManager();
	static FileManager& getInstance();
	static void destroyInstance();

private:
	bool writeDataToFile(const ValueMap& valueMap, const std::string& path);
	ValueMap readDataFromFile(const std::string& path);
	bool deleteFile(const std::string& file);

private:
	bool writeUserData(const UserMap& userMap);
	UserMap readUserData();
	void deleteUserCacheData();

public:
	void deleteUserByName(const std::string& name);
	void deleteAllUsers();
	UserMap getAllLocalUsers();
	UserInf getLocalUserByName(const std::string& name);
	bool addLocalUser(const UserInf& inf);
	bool modifyLocalUser(const UserInf& inf);

public:
	CommonCfg getCommonCfg();
	bool modifyCommonCfg(const CommonCfg& cfg);

private:
	std::string writablePath;
	UserMap m_mapUser;
};

#define g_file FileManager::getInstance()

#endif 
