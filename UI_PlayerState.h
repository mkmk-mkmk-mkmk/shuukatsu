#pragma once

#include "UI.h"
#include "player.h"

class UI_PlayerState : public UI
{
private:

	PlayerState m_PlayerState;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};