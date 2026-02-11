
#include "Windows.h"
#include "BGM.h"

void BGM::Init()
{
	static const char* BGMName[5] =
	{
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
	};

	for (int i = 0; i < 5; i++)
	{
		ma_sound_init_from_file(
			m_pEngine,
			BGMName[i],
			MA_SOUND_FLAG_STREAM,
			NULL,
			NULL,
			&m_BGM[i]);

		ma_sound_set_looping(&m_BGM[i], MA_TRUE);
	}

}

void BGM::Uninit()
{
	for (int i = 0; i < 5; i++)
	{
		ma_sound_uninit(&m_BGM[i]);
	}
}

void BGM::Play(BGMType type)
{
	ma_sound_start(&m_BGM[type]);
}

void BGM::Stop()
{
	for (int i = 0; i < 5; i++)
	{
		ma_sound_stop(&m_BGM[i]);
	}
}