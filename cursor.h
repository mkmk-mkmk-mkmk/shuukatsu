#pragma once
#include "main.h"
#include <iostream>
#include <memory>

#include "framework.h"
#include "renderer.h"
#include "UI.h"

#include "particle_touch.h"
#include "click_ColorCircle.h"

enum CursorState
{
	Cursor_Normal,
	LeftClick,
	RightClick
};

class Cursor : public UI
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture[4];

	POINT pt;
	Click_ColorCircle m_ClickColorCircle;

	CursorState m_CursorState = CursorState::Cursor_Normal;
	CursorState m_OldCursorState = CursorState::Cursor_Normal;

	bool	m_DrawCurcle = false;	//円を描画
	bool	m_ReleaseClick = false;	//長押しが途切れたか

	bool	m_PlayerHaveTrail = false;	//プレイヤーがトレイルを持っているかどうか

	Vector2  m_CursorLockPos;		//カーソル位置固定座標


public:
	void Init();
	void Update();
	void Uninit();
	void Draw();

};

