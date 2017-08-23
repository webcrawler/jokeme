// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "string.h"
#include <iostream>  
#include <string>  
#include <io.h>  
#include<vector>
#include <direct.h>

using namespace std;

//获取特定格式的文件名  
void getAllFormatFiles(string path, vector<string>& files, string format)
{
	//文件句柄    
	long hFile = 0;
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
					getAllFormatFiles(p.assign(path).append("/").append(fileinfo.name), files, format);
				}
			}
			else files.push_back(p.assign(path).append("/").append(fileinfo.name));
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

string&  replace_all_distinct(string& str, const string& old_value, const string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

int main(int argc, char* argv[])
{
	char buffer[256];
	_getcwd(buffer, 256);
	string projDir = string(buffer);// +"\\BaLe\\";
	unsigned int pos = projDir.find_last_of("\\");
	projDir = projDir.substr(0, pos) + "\\BaLe";
	projDir = replace_all_distinct(projDir, "\\", "/");
	printf("The current directory is: %s \n", projDir.c_str());

	string filePath = projDir; // "E:/MyWork/trunk/client/BaLe/encrypt_res";
	string format = ".*";
	vector<string> files1;
	vector<string> files2;
	vector<string> files;
	getAllFormatFiles(filePath + "/res", files1, format);
	getAllFormatFiles(filePath + "/src", files2, format);
	files.insert(files.end(), files1.begin(), files1.end());
	files.insert(files.end(), files2.begin(), files2.end());
	int size = files.size();
	bool b = false;
	for (int i = 0; i < size; i++)
	{
		string str = files[i];
		size_t pos = str.find(" ");
		if (pos != string::npos)
		{
			printf("format_err = %s\n", str.c_str());
			b = true;
		}
	}
	if (!b)
	{
		printf("file format check success !");
	}
	else
	{
		printf("file format check fail !");
	}
	system("PAUSE");
    return 0;
}

