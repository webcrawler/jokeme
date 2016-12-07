/**
*  @brief: 文件读写
*  @Created by fjut on 16-11-28
*/

#ifndef __H_FILE_MANAGER_H__
#define __H_FILE_MANAGER_H__

#if 0

#include "cocos2d.h"

using namespace cocos2d;

// 用户信息
typedef struct _AccountInf
{
	std::string name;
	std::string pwd;
	// todo add
}AccountInf;
typedef std::map<std::string, AccountInf> AccountArr;

class FileManager
{
public:
	FileManager();
	static FileManager& getInstance();
	static void destroyInstance();

private:
	void writeDataToFile(const Data& data, const std::string& path);
	Data readDataFromFile(const std::string& path);

public:
	void writeUserData(AccountArr& accArr);
	AccountArr readUserData();
	void deleteUserByName(const std::string& name);
	void deleteAllUsers();

	void deleteFile();

private:
	std::string writablePath;
};

#define g_file FileManager::getInstance()

#ednif

#endif 
