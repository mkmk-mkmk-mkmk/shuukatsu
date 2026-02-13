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

#define BGMMaxNum 5

class BGM : public SoundManager
{
private:
	ma_engine* m_pEngine;
	ma_sound m_BGM[BGMMaxNum];
	BGMType m_BGMType;

	float m_MaxVolume = 0.3f;
	float m_MinVolume = 0.0f;
	float m_FadeSpeed = 0.02f;

public:
	void SetEngine(ma_engine* engine) { m_pEngine = engine; }

	void Init();
	void Uninit();
	void Play(BGMType type);
	void Stop();

};