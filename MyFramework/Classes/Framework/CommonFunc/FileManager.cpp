
#include "FileManager.h"

static const std::string ACCOUNTDATA = "accountData";

static CFileManager* _instance = NULL;

CFileManager::CFileManager()
{
}

CFileManager::~CFileManager()
{

}

CFileManager* CFileManager::instance()
{
	if(_instance == NULL)
	{
		_instance = new CFileManager();
	}

	return _instance;
}

void CFileManager::freeInstance()
{
	if(_instance != NULL)
	{
		delete _instance;
		_instance = NULL;
	}
}

bool CFileManager::checkFileIsExit(const std::string& fullPathName)
{
	return cocos2d::FileUtils::getInstance()->isFileExist(fullPathName);
}

AccountInf CFileManager::readAccountInf(const char* uid, const char* defaultName)
{
    FILE* fp = nullptr;
    AccountInf inf;
    
    std::string fullPath = cocos2d::FileUtils::getInstance()->getWritablePath();
    fullPath = fullPath.append(std::string(uid) + "/" + ACCOUNTDATA);
    
    if( !checkFileIsExit(fullPath) )
    {
        if (!writeAccountInf(uid, defaultName))
        {
            strcpy(inf.uid, uid);
            strcpy(inf.name, defaultName);
            return inf;
        }
    }
    
    if( (fp = fopen(fullPath.c_str(), "rb")) != NULL )
    {
        fread(&inf, sizeof(inf), 1, fp);
    }
    
    fclose(fp);
    
    return inf;
}

bool CFileManager::writeAccountInf(const char* uid, const char* name)
{
    AccountInf inf;
    strcpy(inf.uid, uid);
    strcpy(inf.name, name);
    
    FILE* fp;
    std::string fullPath = cocos2d::FileUtils::getInstance()->getWritablePath();
    fullPath = fullPath.append(std::string(uid) + "/" + ACCOUNTDATA);
    
    if( (fp = fopen(fullPath.c_str(), "wb")) != NULL && fwrite(&inf, sizeof(inf), 1, fp) != 0)
    {
        fclose(fp);
        return true;
    }
    
    fclose(fp);
    return false;
}

void CFileManager::saveUserInfo(string& name, string& pwd)
{
	UserDefault::getInstance()->setStringForKey("account", name);
	UserDefault::getInstance()->setStringForKey("pwd", pwd);
	UserDefault::getInstance()->flush();
}

vector<string> CFileManager::getUserInfo()
{
	auto acc = UserDefault::getInstance()->getStringForKey("account", "");
	auto pwd = UserDefault::getInstance()->getStringForKey("pwd", "");
	vector<string> vec;
	vec.push_back(acc);
	vec.push_back(pwd);
	return vec;
}