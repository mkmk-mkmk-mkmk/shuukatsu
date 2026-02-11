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

class SE : public SoundManager
{
private:

	ma_engine* m_pEngine;


public:
	void SetEngine(ma_engine* engine) { m_pEngine = engine; }

	void Play(SEType type);
};