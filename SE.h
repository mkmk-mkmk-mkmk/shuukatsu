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

public:
	void Play(SEType type);
};