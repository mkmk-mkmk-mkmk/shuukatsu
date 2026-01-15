#include "cursor.h"  
#include "texture.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "player.h"


void Cursor::Init()
{
	//マウス初期位置
	m_CursorLockPos = { 0.0f, 0.0f };
	SetCursorPos(m_CursorLockPos.x, m_CursorLockPos.y);

	if (GetCursorPos(&pt))
	{
		SetPosition(Vector2{ static_cast<float>(pt.x), static_cast<float>(pt.y) });
	}

	//マウスを取得できない場合
	if (!GetCursorPos(&pt))
	{
		//エラーメッセージ
		MessageBox(nullptr, "Cursor position could not be retrieved.", "Error", MB_OK | MB_ICONERROR);
	}

	m_Scale = { 80.0f, 80.0f }; //カーソルのスケールを初期化

	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\cursor.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\cursor_GettingReverse.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\cursor_GettingTrail.png"));

	//マウス初期位置変更後再移動
	m_CursorLockPos = { screenWidth * 0.5f, screenHeight * 0.5f };
	SetCursorPos(m_CursorLockPos.x, m_CursorLockPos.y);

	//マウスカーソル非表示
	ShowCursor(FALSE);


	////カーソル用テクスチャの分割数を設定
	//cursorInstance.m_TextureSplit = { 1, 1 };
}

void Cursor::Uninit()
{
	UnInitSprite();
}

void Cursor::Update()
{
	if (GetCursorPos(&pt))
	{
		SetPosition(Vector2{ static_cast<float>(pt.x), static_cast<float>(pt.y) });
	}

	//前回の状態を保存
	m_OldCursorState = m_CursorState;

	//プレイヤーが軌跡を持っているかどうか
	if (Manager::GetScene()->GetGameObject<Player>() != nullptr)
	{
		m_PlayerHaveTrail = Manager::GetScene()->GetGameObject<Player>()->GetHaveTrail();
	}

	//クリック検出
	if (!m_PlayerHaveTrail)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			m_CursorState = CursorState::LeftClick;
		}
		else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			m_CursorState = CursorState::RightClick;
		}
		else
		{
			m_CursorState = CursorState::Cursor_Normal;
		}
	}
	else
	{
		m_CursorState = CursorState::Cursor_Normal;
	}


	if (m_OldCursorState != CursorState::Cursor_Normal && m_CursorState == CursorState::Cursor_Normal)
	{
		m_ReleaseClick = true;
	}

	if (!m_DrawCurcle && m_CursorState != CursorState::Cursor_Normal)
	{
		m_ClickColorCircle.Init(GetPosition(), (m_CursorState == CursorState::LeftClick) ? 0 : 1);
		m_DrawCurcle = true;
	}

	if (m_DrawCurcle)
	{
		m_ClickColorCircle.Update(m_ReleaseClick);
		if (m_ClickColorCircle.GetEndDraw())
		{
			m_DrawCurcle = false;
			m_ReleaseClick = false;
			m_ClickColorCircle.Uninit();
		}
	}
}

void Cursor::Draw()
{
	if (m_DrawCurcle)
	{
		m_ClickColorCircle.Draw();
	}

	switch (m_CursorState)
	{
	case CursorState::Cursor_Normal:
		m_TextureType = 0;
		break;

	case CursorState::LeftClick:
		m_TextureType = 1;
		break;

	case CursorState::RightClick:
		m_TextureType = 2;
		break;
	}

	DrawSprite(XMFLOAT2(m_Position.x, m_Position.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), m_TextureType, 1.0f);

}
