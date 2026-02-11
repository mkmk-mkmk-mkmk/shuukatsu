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
	ma_engine* m_pEngine;
	ma_sound m_BGM[5];
	BGMType m_BGMType;

public:
	void SetEngine(ma_engine* engine) { m_pEngine = engine; }

	void Init();
	void Uninit();
	void Play(BGMType type);
	void Stop();

};