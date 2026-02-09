
#include "SE.h"

void SE::Play(SEType type)
{
	const char* SEName = nullptr;

	switch (type)
	{
	case SE_PushButton:
		//SEName = "asset\\sound\\SE\\pushButton.wav";
		break;
	case SE_Jump:

		break;
	case SE_GetTrail:

		break;
	case SE_MoveTrail:

		break;
	case SE_BreakBox:

		break;
	}

	if (SEName != nullptr)
	{
		ma_engine_play_sound(&m_MaEngine, SEName, NULL);
	}
}