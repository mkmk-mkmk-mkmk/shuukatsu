
#include "SE.h"

void SE::Init()
{
	const char* SEName[SEMaxNum] = 
	{
		"asset\\sound\\SE\\pushButton.mp3",
		"asset\\sound\\SE\\pushButton.mp3",
		"asset\\sound\\SE\\pushButton.mp3",
		"asset\\sound\\SE\\pushButton.mp3",
		"asset\\sound\\SE\\breakBox.mp3"
	};

	for (int i = 0; i < SEMaxNum; i++)
	{
		ma_sound_init_from_file(
			m_pEngine,
			SEName[i],
			0,
			NULL,
			NULL,
			&m_SE[i]);

		ma_sound_set_volume(&m_SE[i], 0.1f);
	}

}


void SE::Play(SEType type)
{
	ma_sound_start(&m_SE[type]);
	m_Playing[type] = true;
}
