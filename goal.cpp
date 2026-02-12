#include "main.h"
#include "texture.h"
#include "scene.h"
#include "manager.h"

#include "goal.h"
#include "map.h"
#include "player.h"
#include "camera.h"
#include "stageClear.h"

void Goal::Init()
{
	//初期位置設定
	m_Position = Manager::GetScene()->GetGameObject<Map>()->m_GoalPosList.front();

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH * 3, MAPCHIP_HEIGHT * 3);

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\goal.png"));
}

void Goal::Uninit()
{
	UnInitSprite();
}

void Goal::Update()
{
	//プレイヤーのボックス当たり判定
	Vector2 playerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	Vector2 playerScale = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	if (Collision::BoxCollisionCommon(m_Position, m_Scale, playerPos, playerScale))
	{
		Manager::SetScene<StageClear>();
	}
}

void Goal::Draw()
{
	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	if (m_DrawPosition.x < -m_Scale.x || m_DrawPosition.x > screenWidth + m_Scale.x ||
		m_DrawPosition.y < -m_Scale.y || m_DrawPosition.y > screenHeight + m_Scale.y)
	{
		return; //画面外なら描画しない
	}

	DrawSprite(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
		XMFLOAT2(m_Scale.x, m_Scale.y), 0, 1.0f);
}
