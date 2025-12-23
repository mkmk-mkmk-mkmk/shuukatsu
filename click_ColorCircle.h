#pragma once

#include "Effect.h"

class Click_ColorCircle : public Effect
{


public:
	void Init(Vector2 position);
	void Uninit();
	void Update();
	void Draw();
};