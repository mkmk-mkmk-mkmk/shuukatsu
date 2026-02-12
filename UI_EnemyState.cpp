
#include "framework.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "camera.h"
#include "manager.h"
#include "scene.h"
#include "UI_EnemyState.h"

void UI_EnemyState::Init()
{
	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\enemy_LookAround.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\enemy_FindPlayer.png"));

	m_Scale = Manager::GetScene()->GetGameObject<Enemy>()->GetScale() * 0.6f;

}

void UI_EnemyState::Uninit()
{

}

void UI_EnemyState::Update()
{
	m_DrawEnemyPosition = Manager::GetScene()->GetGameObject<Enemy>()->GetPosition() -
		Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	m_Position = Vector2(
		m_DrawEnemyPosition.x,
		m_DrawEnemyPosition.y - m_Scale.y * 1.5f);
}

void UI_EnemyState::Draw()
{
	m_EnemyState = Manager::GetScene()->GetGameObject<Enemy>()->GetEnemyAnimState();

	//テクスチャセット
	switch (m_EnemyState)
	{
	case AnimationState::Patrol:

		m_TextureType = 0;
		break;

	case AnimationState::Chase:

		m_TextureType = 1;
		break;

	case AnimationState::Attack:

		m_TextureType = 1;
		break;
	}

	DrawSprite(XMFLOAT2(m_Position.x, m_Position.y), m_Rotate,
		XMFLOAT2(m_Scale.x, m_Scale.y), m_TextureType, 1.0f);

}