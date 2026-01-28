#pragma once

#include "gameObject.h"

class BackGround : public GameObject
{
private:

public:
	void Init(int sceneType);
	void Uninit();
	void Update();
	void Draw();

};