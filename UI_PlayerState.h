#pragma once

#include "UI.h"
#include "player.h"

class UI_PlayerState : public UI
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture[4];

	PlayerState m_PlayerState;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};