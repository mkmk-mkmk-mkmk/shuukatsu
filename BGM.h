#pragma once

#include "soundManager.h"

enum BGMType
{
	BGM_Title,
	BGM_Stage1,
	BGM_Stage2,
	BGM_StageClear,
	BGM_GameOver,
};

class BGM : public SoundManager
{
private:
	ma_sound m_BGM[5];
	BGMType m_BGMType;

public:
	void Init();
	void Uninit();
	void Play(BGMType type);
	void Stop();

};