/**
*  @brief: 文件读写
*  @Created by fjut on 16-11-28
*/

#include "FileManager.h"

static const char* filePath = "FILEMANAGER";
static const char* accFile = "ACCOUNT";
static const char* commonCfgFile = "COMMONCFG";

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
	getInstance().deleteUserCacheData();
}

void FileManager::deleteUserCacheData()
{
	m_mapUser.clear();
}

bool FileManager::writeDataToFile(const ValueMap& valueMap, const std::string& path)
{
	std::string fullPath = writablePath + path;
	return FileUtils::getInstance()->writeValueMapToFile(valueMap, fullPath.c_str());
}

ValueMap FileManager::readDataFromFile(const std::string& path)
{
	std::string fullPath = writablePath + path;
	ValueMap readValueMap = FileUtils::getInstance()->getValueMapFromFile(fullPath);
	return readValueMap;
}

bool FileManager::deleteFile(const std::string& file)
{
	std::string path = writablePath + file;
	if (FileUtils::getInstance()->isFileExist(path))
	{
		return FileUtils::getInstance()->removeFile(path);
	}
	return false;
}

bool FileManager::writeUserData(const UserMap& userMap)
{
	if (userMap.size() == 0) return false;

	ValueMap valueMap;
	for (auto it = userMap.begin(); it != userMap.end(); it++)
	{
		ValueVector vec;
		vec.push_back(Value(it->second.name));
		vec.push_back(Value(it->second.pwd));
		// add
		valueMap[it->first] = vec;
	}
	return writeDataToFile(valueMap, accFile);
}

UserMap FileManager::readUserData()
{
	ValueMap valueMap = readDataFromFile(accFile);
	UserMap userMap;
	for (auto it = valueMap.begin(); it != valueMap.end(); it++)
	{
		UserInf inf;
		ValueVector& valueVector = it->second.asValueVector();
		inf.name = valueVector.at(0).asString();
		inf.pwd = valueVector.at(1).asString();
		// add
		userMap[it->first] = inf;
	}
	return userMap;
}

UserMap FileManager::getAllLocalUsers()
{
	if (m_mapUser.size() == 0)
	{
		m_mapUser = readUserData();
	}
	return m_mapUser;
}

UserInf FileManager::getLocalUserByName(const std::string& name)
{
	if (m_mapUser.size() == 0)
	{
		m_mapUser = readUserData();
	}
	UserInf inf;
	auto it = m_mapUser.find(name);
	if (it == m_mapUser.end())
	{
		CCLOG("getLocalUserByName fail, can't find the user by the name %s", name.c_str());
	}
	else
	{
		inf = it->second;
	}
	return inf;
}

bool FileManager::addLocalUser(const UserInf& inf)
{
	if (m_mapUser.size() == 0)
	{
		m_mapUser = readUserData();
	}
	auto it = m_mapUser.find(inf.name);
	if (it != m_mapUser.end())
	{
		CCLOG("addLocalUser fail, user inf is exist by the name %s", inf.name.c_str());
		return false;
	}
	m_mapUser[inf.name] = inf;
	writeUserData(m_mapUser);
	return true;
}

bool FileManager::modifyLocalUser(const UserInf& inf)
{
	if (m_mapUser.size() == 0)
	{
		m_mapUser = readUserData();
	}
	auto it = m_mapUser.find(inf.name);
	if (it == m_mapUser.end())
	{
		CCLOG("modifyLocalUser fail, can't find the user by the name %s", inf.name.c_str());
		return false;
	}
	m_mapUser[inf.name] = inf;
	writeUserData(m_mapUser);
	return true;
}

void FileManager::deleteUserByName(const std::string& name)
{
	if (m_mapUser.size() == 0)
	{
		m_mapUser = readUserData();
	}
	auto it = m_mapUser.find(name);
	if (it == m_mapUser.end())
	{
		CCLOG("deleteUserByName fail, can't find the user by the name %s", name.c_str());
		return;
	}
	m_mapUser.erase(it);
	if (m_mapUser.size() == 0)
	{
		deleteFile(accFile);
	}
	else
	{
		writeUserData(m_mapUser);
	}
}

void FileManager::deleteAllUsers()
{
	if (m_mapUser.size() == 0)
	{
		m_mapUser = readUserData();
	}
	UserMap tmpMap = m_mapUser;
	for (auto it = tmpMap.begin(); it != tmpMap.end(); it++)
	{
		deleteUserByName(it->first);
	}
}

CommonCfg FileManager::getCommonCfg()
{
	ValueMap valueMap = readDataFromFile(commonCfgFile);
	if (valueMap.size() == 0)
	{
		// default false
		CommonCfg inf;
		inf.bMusic = false;
		inf.bEffect = false;
		if (modifyCommonCfg(inf))
		{
			return inf;
		}
	}
	CommonCfg inf;
	inf.bMusic = valueMap["bMusic"].asBool();
	inf.bEffect = valueMap["bEffect"].asBool();
	// add
	return inf;
}

bool FileManager::modifyCommonCfg(const CommonCfg& cfg)
{
	ValueMap valueMap;
	valueMap["bMusic"] = cfg.bMusic;
	valueMap["bEffect"] = cfg.bEffect;
	// add
	return writeDataToFile(valueMap, commonCfgFile);
}