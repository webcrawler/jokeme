/**
*  @brief: ÎÄ¼þ¶ÁÐ´
*  @Created by fjut on 16-11-28
*/

#include "FileManager.h"

static const char* filePath = "FILEMANAGER";
static const char* AccFile = "ACCOUNT";

FileManager::FileManager()
{
	writablePath = FileUtils::getInstance()->getWritablePath() + filePath + "/";
	if (!FileUtils::getInstance()->isDirectoryExist(writablePath))
	{
		FileUtils::getInstance()->createDirectory(writablePath);
	}
}

FileManager& FileManager::getInstance()
{
	static FileManager _instance;
	return _instance;
}

void FileManager::destroyInstance()
{

}

void FileManager::writeDataToFile(const Data& data, const std::string& path)
{
	std::string fullPath = writablePath + path;
	FileUtils::getInstance()->writeDataToFile(data, fullPath.c_str());
}

Data FileManager::readDataFromFile(const std::string& path)
{
	std::string fullPath = writablePath + path;
	Data data = FileUtils::getInstance()->getDataFromFile(fullPath);
	return data;
}

void FileManager::writeUserData(AccountArr& accArr)
{
	Data writeData;
	writeData.copy((unsigned char *)&accArr, sizeof(AccountArr));
	writeDataToFile(writeData, AccFile);
}

AccountArr FileManager::readUserData()
{
	Data readData = readDataFromFile(AccFile);
	if (readData.isNull())
	{
		AccountArr arr;
		return arr;
	}
	AccountArr* buffer = (AccountArr*)malloc(sizeof(AccountArr)*(readData.getSize() + 1));
	memcpy(buffer, readData.getBytes(), readData.getSize());
	AccountArr arr = *buffer;
	free(buffer);
	return arr;

	//Data readData = FileUtils::getInstance()->getDataFromFile(fullPath);
	//buffer = (unsigned char*)malloc(sizeof(unsigned char) * (readData.getSize() + 1));
	//memcpy(buffer, readData.getBytes(), readData.getSize());
	//buffer[readData.getSize()] = '\0';
	//std::string readDataStr((const char*)buffer);
	//free(buffer);
}

void FileManager::deleteUserByName(const std::string& name)
{
	AccountArr arr = readUserData();
	auto it = arr.find(name);
	if (it == arr.end())
	{
		CCLOG("deleteUserByName fail, can't find the user by the name %s", name.c_str());
		return;
	}
	arr.erase(it);
	writeUserData(arr);
}

void FileManager::deleteAllUsers()
{

}
