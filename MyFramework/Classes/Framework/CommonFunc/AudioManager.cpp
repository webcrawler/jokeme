/**
*  @brief: 音频管理
*  @Created by fjut on 16-12-02
*/

#include "AudioManager.h"

AudioManager::AudioManager()
{
	m_cfg = g_file.getCommonCfg();
	m_playingBgMscTag = music_none;
	m_bFirst = true;
}

AudioManager& AudioManager::getInstance()
{
	static AudioManager _instance;
	return _instance;
}

void AudioManager::destroyInstance()
{

}

std::string AudioManager::getAudioFileNameByTag(AudioTag tag, bool bMusic, const std::string& type)
{
	std::string path = StringUtils::format("%d", (int)tag) + "." + type;
	path = ( (bMusic) ? "audio/music_" : "audio/effect_" ) + path;
	return path;
}

void AudioManager::playMusic(AudioTag tag)
{
	if (!m_cfg.bMusic || tag == music_none)
	{
		m_playingBgMscTag = tag;
		return;
	}
	std::string audioPath = getAudioFileNameByTag(tag, true);
	if (m_playingBgMscTag == music_none)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(audioPath.c_str(), true);
		m_playingBgMscTag = tag;
		m_bFirst = false;
		return;
	}
	if (m_playingBgMscTag == tag && !m_bFirst) // && SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		return;
	}
	SimpleAudioEngine::getInstance()->playBackgroundMusic(audioPath.c_str(), true);
	m_playingBgMscTag = tag;
	m_bFirst = false;
}

void AudioManager::playEffect(AudioTag tag)
{
	if (!m_cfg.bEffect)
	{
		return;
	}
	std::string audioPath = getAudioFileNameByTag(tag, false);
	SimpleAudioEngine::getInstance()->playEffect(audioPath.c_str());
}

void AudioManager::pauseBgMsc()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AudioManager::resumeBgMsc()
{
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AudioManager::setAudioCfgWithMusic(bool bOpen)
{
	m_cfg.bMusic = bOpen;
	g_file.modifyCommonCfg(m_cfg);
}

void AudioManager::setAudioCfgWithEffect(bool bOpen)
{
	m_cfg.bEffect = bOpen;
	g_file.modifyCommonCfg(m_cfg);
}

bool AudioManager::getMscIsOpen()
{
	return m_cfg.bMusic;
}

bool AudioManager::getEffectIsOpen()
{
	return m_cfg.bEffect;
}

