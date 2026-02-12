#pragma once

#include "UI_Button.h"

class GameOverButton : public UI_Button
{
private:
	std::vector<UI_Button> m_ButtonList;
	UI_Button button;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};