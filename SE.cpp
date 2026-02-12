
#include "SE.h"

void SE::Init()
{
	m_Volume = 0.1f;

	const char* SEName[SEMaxNum] = 
	{
		"asset\\sound\\SE\\pushButton.mp3",
		"asset\\sound\\SE\\pushButton.mp3",
		"asset\\sound\\SE\\pushButton.mp3",
		"asset\\sound\\SE\\pushButton.mp3",
		"asset\\sound\\SE\\breakBox.mp3"
	};

	for (int i = 0; i < SEMaxNum - 1; i++)
	{
		ma_sound_init_from_file(
			m_pEngine,
			SEName[i],
			0,
			NULL,
			NULL,
			&m_SE[i]);

		ma_sound_set_volume(&m_SE[i], m_Volume);
	}

	//‰ó‚ê‚éSE‚Í•¡”“¯ŽžÄ¶‚Å‚«‚é‚æ‚¤‚É‚·‚é
	for (int i = 0; i < 8; i++)
	{
		ma_sound_init_from_file(
			m_pEngine,
			SEName[SEMaxNum - 1],
			0,
			NULL,
			NULL,
			&m_BreakBoxSE[i]);
		ma_sound_set_volume(&m_BreakBoxSE[i], m_Volume);
	}
}


void SE::Play(SEType type)
{
	ma_sound_start(&m_SE[type]);
}

void SE::PlayBreakBox()
{
	for (int i = 0; i < 8; i++)
	{
		if (!ma_sound_is_playing(&m_BreakBoxSE[i]))
		{
			ma_sound_start(&m_BreakBoxSE[i]);
			break;
		}
	}
}
