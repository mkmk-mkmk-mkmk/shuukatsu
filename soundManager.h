#pragma once

enum SoundType
{
	BGM,
	SE
};

class SoundManager
{
protected:

	int m_Volume;
	bool m_Mute;

public:

	virtual void Init();
	virtual void Uninit();
	virtual void Play();
};