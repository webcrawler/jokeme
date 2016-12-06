/**
*  @brief: 音频管理
*  @Created by fjut on 16-12-02
*/

#ifndef _H_AUDIO_MANAGER_H_
#define _H_AUDIO_MANAGER_H_

#include "cocos2d.h"
#include "FileManager.h"
#include "Game_Val_Define.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d;

class AudioManager
{
public:
	AudioManager();
	static AudioManager& getInstance();
	static void destroyInstance();

public:
	void playMusic(AudioTag tag);
	void playEffect(AudioTag tag);
	void pauseBgMsc();
	void resumeBgMsc();
	void setAudioCfgWithMusic(bool bOpen = true);
	void setAudioCfgWithEffect(bool bOpen = true);
	bool getMscIsOpen();
	bool getEffectIsOpen();

private:
	std::string getAudioFileNameByTag(AudioTag tag, bool bMusic, const std::string& type = "mp3");

private:
	CommonCfg m_cfg;
	AudioTag m_playingBgMscTag;
	bool m_bFirst;

};

#define g_audio AudioManager::getInstance()

#endif 

