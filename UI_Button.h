#pragma once

#include "UI.h"

class UI_Button : public UI
{
protected:

	bool m_CursorOnButton = false;
	bool m_ClickButton = false;
	Vector2 m_CursorPos;
	Vector2 m_CursorScale;

	bool m_Action = false;

public:
	void Init(Vector2 position, Vector2 scale, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* onCursorTexture);
	void Uninit();
	void Update();
	void Draw();

	bool GetAction() { return m_Action; }
};