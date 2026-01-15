#include "framework.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "manager.h"
#include "scene.h"
#include "game.h"
#include "title_Cursor.h"
#include "collision.h"
#include "UI_Button.h"

void UI_Button::Init(Vector2 position, Vector2 scale,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* onCursorTexture)
{
	InitSprite();

	m_TextureList.push_back(texture);
	m_TextureList.push_back(onCursorTexture);

	m_Position = position;
	m_Scale = scale;
}

void UI_Button::Uninit()
{

}

void UI_Button::Update()
{
	m_CursorPos = Manager::GetScene()->GetUIObject<Title_Cursor>()->GetPosition();
	m_CursorScale = Manager::GetScene()->GetUIObject<Title_Cursor>()->GetScale();

	//ボタンとの当たり判定
	m_CursorOnButton = Collision::BoxCollisionCommon(m_CursorPos, m_CursorScale, m_Position, m_Scale);

	if (!m_CursorOnButton)
	{
		m_DrawPosition = m_Position;
	}
	else
	{
		m_DrawPosition = m_Position + Vector2(0.0f, -10.0f);
	}

	if (m_CursorOnButton && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_ClickButton = true;
	}
	if (m_ClickButton && !m_CursorOnButton)
	{
		m_ClickButton = false;
	}

	if (m_ClickButton && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		m_ClickButton = false;
		m_Action = true;
	}
}

void UI_Button::Draw()
{

	//テクスチャセット
	if (!m_CursorOnButton)
	{
		m_TextureType = 0;
	}
	else
	{
		m_TextureType = 1;
	}

	DrawSprite(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), m_TextureType, 1.0f);

}