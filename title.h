#pragma once

#include "scene.h"
#include "title_BreakableBox.h"

class Title : public Scene
{
private:

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};