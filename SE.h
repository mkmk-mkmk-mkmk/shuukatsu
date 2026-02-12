#pragma once

#include "soundManager.h"

enum SEType
{
	SE_PushButton,
	SE_Jump,
	SE_GetTrail,
	SE_MoveTrail,
	SE_BreakBox,
};

#define SEMaxNum 5

class SE : public SoundManager
{
private:
	ma_engine* m_pEngine;
	ma_sound m_SE[SEMaxNum];
	bool m_Playing[SEMaxNum];
public:
	void SetEngine(ma_engine* engine) { m_pEngine = engine; }

	void Init();
	void Play(SEType type);
};