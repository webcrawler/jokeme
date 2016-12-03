/**
*  @brief: 音频管理
*  @Created by fjut on 16-12-02
*/

#include "AudioManager.h"

AudioManager::AudioManager()
{
	m_cfg = g_file.getCommonCfg();
	m_playingBgMscTag = music_none;
}

AudioManager& AudioManager::getInstance()
{
	static AudioManager _instance;
	return _instance;
}

void AudioManager::destroyInstance()
{

}

std::string getAudioFileNameByTag(AudioTag tag, const std::string& type)
{
	std::string path = StringUtils::format("audio/music_%d", (int)tag) + "." + type;
	return path;
}

void AudioManager::playMusic(AudioTag tag)
{
	if (!m_cfg.bMusic)
	{
		m_playingBgMscTag = tag;
		return;
	}
	std::string audioPath = getAudioFileNameByTag(tag);
	if (m_playingBgMscTag == music_none)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(audioPath.c_str());
	}
	else if (m_playingBgMscTag == tag)
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	else
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(audioPath.c_str());
	}
}

void AudioManager::playEffect(AudioTag tag)
{

}