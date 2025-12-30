
#include "framework.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "manager.h"
#include "scene.h"
#include "player.h"
#include "UI_PlayerState.h"

void UI_PlayerState::Init()
{
	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\playerState_Normal.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\playerState_GettingTrail.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\playerState_HaveTrail.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\playerState_MoveTrail.png"));

	m_Position = Vector2((float)screenWidth * 0.1f, (float)screenHeight * 0.125f);
	m_Scale = Vector2((float)screenWidth * 0.2f, (float)screenHeight * 0.25f);

}

void UI_PlayerState::Uninit()
{

}

void UI_PlayerState::Update()
{

}

void UI_PlayerState::Draw()
{
	m_PlayerState = Manager::GetScene()->GetGameObject<Player>()->GetPlayerState();

	//テクスチャセット
	switch (m_PlayerState)
	{
	case PlayerState::Player_Normal:

		m_TextureType = 0;
		break;

	case PlayerState::GettingTrail:

		m_TextureType = 1;
		break;

	case PlayerState::HaveTrail:

		m_TextureType = 2;
		break;

	case PlayerState::MoveTrail:

		m_TextureType = 3;
		break;
	}

	DrawSprite(XMFLOAT2(m_Position.x, m_Position.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), m_TextureType, 1.0f);

}