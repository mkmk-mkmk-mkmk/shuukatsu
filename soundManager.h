#pragma once

#include "miniaudio.h"

class SoundManager
{
protected:

	ma_engine m_MaEngine;

	float m_Volume;
	bool m_Stop;

public:

	virtual void Init();
	virtual void Uninit();
	virtual void Play();
};