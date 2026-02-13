#pragma once

#include "sprite.h"

class Pause : public Sprite
{
private:

	float m_Alpha = 0.7f;

	bool m_SceneStop = false;

public:
	void Init();
	void Update();
	void Uninit();
	void Draw();

	bool GetSceneStop() { return m_SceneStop; }

};