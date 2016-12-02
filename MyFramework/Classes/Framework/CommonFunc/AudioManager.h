/**
*  @brief: 音频管理
*  @Created by fjut on 16-12-02
*/

#ifndef _H_AUDIO_MANAGER_H_
#define _H_AUDIO_MANAGER_H_

#include "cocos2d.h"
#include "FileManager.h"

using namespace cocos2d;

class AudioManager
{
public:
	AudioManager();
	static AudioManager& getInstance();
	static void destroyInstance();

public:
	void playMusic(const std::string& name);
	void playEffect(const std::string& name);

private:
	CommonCfg m_cfg;

};

#define g_audio AudioManager::getInstance()

#endif 

