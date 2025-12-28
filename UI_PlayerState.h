#pragma once

#include "UI.h"
#include "player.h"

class UI_PlayerState : public UI
{
private:

	ID3D11ShaderResourceView* m_Texture[4];

	PlayerState m_PlayerState;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};