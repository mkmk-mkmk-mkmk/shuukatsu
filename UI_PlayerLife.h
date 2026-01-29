#pragma once

#include "UI.h"
#include "player.h"

class UI_PlayerLife : public UI
{
private:
	int m_PlayerLife;	//プレイヤーのライフ数

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};