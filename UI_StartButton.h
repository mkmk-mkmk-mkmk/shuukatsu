#pragma once

#include "UI.h"

class UI_StartButton : public UI
{
private:

	ID3D11ShaderResourceView* m_Texture[2];

	bool m_CursorOnButton = false;
	bool m_ClickButton = false;
	Vector2 m_ButtonPos;
	Vector2 m_ButtonScale;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};