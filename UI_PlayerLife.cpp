
#include "framework.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "manager.h"
#include "scene.h"
#include "player.h"
#include "UI_PlayerLife.h"

void UI_PlayerLife::Init()
{
	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\Life.png"));

	m_Position = Vector2((float)screenWidth * 0.9f, (float)screenHeight * 0.12f);
	m_Scale = Vector2((float)screenWidth * 0.15f, (float)screenHeight * 0.075f);

}

void UI_PlayerLife::Uninit()
{

}

void UI_PlayerLife::Update()
{
}

void UI_PlayerLife::Draw()
{
	m_PlayerLife = Manager::GetScene()->GetGameObject<Player>()->GetLife();

	DrawSpritePiece(XMFLOAT2(m_Position.x, m_Position.y), m_Rotate,
		XMFLOAT2(m_Scale.x, m_Scale.y), 5 - m_PlayerLife, 1, 6, 0, 1.0f, false);

}