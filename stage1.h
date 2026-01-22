#pragma once
#include "pause.h"

class Stage1 : public Scene
{
private:
	class Audio* m_BGM;
	Pause* m_Pause = new Pause;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};