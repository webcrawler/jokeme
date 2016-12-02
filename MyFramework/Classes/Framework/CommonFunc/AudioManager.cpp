/**
*  @brief: 音频管理
*  @Created by fjut on 16-12-02
*/

#include "AudioManager.h"

AudioManager::AudioManager()
{
	m_cfg = g_file.getCommonCfg();
}

AudioManager& AudioManager::getInstance()
{
	static AudioManager _instance;
	return _instance;
}

void AudioManager::destroyInstance()
{

}

void AudioManager::playMusic(const std::string& name)
{

}

void AudioManager::playEffect(const std::string& name)
{

}