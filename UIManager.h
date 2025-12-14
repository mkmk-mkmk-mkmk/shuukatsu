#pragma once

#include "gameObject.h"
#include "framework.h"

class UIManager : public GameObject
{
private:

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

};