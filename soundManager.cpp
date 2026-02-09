
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "soundManager.h"
#include "Windows.h"

void SoundManager::Init()
{
	ma_result result = ma_engine_init(NULL, &m_MaEngine);
	if (result != MA_SUCCESS)
	{
		MessageBox(
			NULL,
			"オーディオエンジンの初期化に失敗",
			"Sound Error",
			MB_OK | MB_ICONERROR);
		
		return;
	}

	m_Volume = 1.0f;
	m_Stop = false;
	ma_engine_set_volume(&m_MaEngine, m_Volume);
}

void SoundManager::Uninit()
{
	ma_engine_uninit(&m_MaEngine);
}


//int main()
//{
//    ma_engine engine;
//    ma_engine_init(NULL, &engine);
//
//    ma_sound bgm;
//
//    ma_sound_init_from_file(
//        &engine,
//        "bgm.mp3",
//        MA_SOUND_FLAG_STREAM,   // 長い曲は必須
//        NULL,
//        NULL,
//        &bgm);
//
//    ma_sound_set_looping(&bgm, MA_TRUE);
//    ma_sound_start(&bgm);
//
//    std::cout << "BGM再生中 Enterで終了\n";
//    std::cin.get();
//
//    ma_sound_uninit(&bgm);
//    ma_engine_uninit(&engine);
//}
