#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "cursor.h"
#include "scene.h"
#include "manager.h"
#include "sprite.h"
#include "player.h"

#include "player_TrailBar.h"

void PlayerTrailBar::Init()
{
	InitSprite();

	//m_TextureList.push_back(Texture::Load("asset\\texture\\cursor_icon.png"));
}

void PlayerTrailBar::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void PlayerTrailBar::Update()
{
	m_HaveTrail = Manager::GetScene()->GetGameObject<Player>()->GetHaveTrail();

	if (m_HaveTrail)
	{
		m_TrailTime = Manager::GetScene()->GetGameObject<Player>()->GetTrailTime();
		m_TrailType = Manager::GetScene()->GetGameObject<Player>()->GetTrailType();

	}
}

void PlayerTrailBar::Draw()
{

}