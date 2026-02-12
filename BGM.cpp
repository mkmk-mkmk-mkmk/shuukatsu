
#include "Windows.h"
#include "BGM.h"

void BGM::Init()
{
	m_Volume = m_MaxVolume;

	static const char* BGMName[BGMMaxNum] =
	{
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3",
		"asset\\sound\\BGM\\kyuumiraikuukan.mp3"
	};

	for (int i = 0; i < BGMMaxNum; i++)
	{
		ma_sound_init_from_file(
			m_pEngine,
			BGMName[i],
			MA_SOUND_FLAG_STREAM,
			NULL,
			NULL,
			&m_BGM[i]);

		ma_sound_set_looping(&m_BGM[i], MA_TRUE);

		ma_sound_set_volume(&m_BGM[i], m_Volume);
	}

}

void BGM::Uninit()
{
	for (int i = 0; i < BGMMaxNum; i++)
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