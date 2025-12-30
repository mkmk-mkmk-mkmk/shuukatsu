
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
#include "UI_StartButton.h"

void UI_StartButton::Init()
{
	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\startButton.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\startButton_2.png"));


	m_Position = Vector2((float)screenWidth * 0.5f, (float)screenHeight * 0.8);
	m_Scale = Vector2((float)screenWidth * 0.3, (float)screenHeight * 0.2f);

}

void UI_StartButton::Uninit()
{

}

void UI_StartButton::Update()
{
	m_ButtonPos = Manager::GetScene()->GetUIObject<Title_Cursor>()->GetPosition();
	m_ButtonScale = Manager::GetScene()->GetUIObject<Title_Cursor>()->GetScale();

	//ボタンとの当たり判定
	m_CursorOnButton = Collision::BoxCollisionCommon(m_ButtonPos, m_ButtonScale, m_Position, m_Scale);

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
		Manager::SetScene<Game>();
	}
}

void UI_StartButton::Draw()
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