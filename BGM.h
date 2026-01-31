#pragma once

#include "soundManager.h"

class BGM : public SoundManager
{
private:

public:
	void Init();
	void Uninit();
	void Play();
};