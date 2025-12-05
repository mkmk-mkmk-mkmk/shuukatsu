#pragma once

#include "scene.h"

class Title : public Scene
{
private:
	class Audio* m_BGM;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};