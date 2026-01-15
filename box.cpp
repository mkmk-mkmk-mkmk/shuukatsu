#include "main.h"
#include "texture.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "box.h"
#include "map.h"
#include "player.h"
#include "camera.h"
#include "enemy_Ground.h"

void Box::Init()
{

	//初期位置設定
	m_Position = Manager::GetScene()->GetGameObject<Map>()->m_BoxPosList.front();

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\tile.png"));
}

void Box::Uninit()
{
	UnInitSprite();
}

void Box::Update(const std::list<Enemy_Ground*>& enemies)
{
	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	//敵のボックス当たり判定
	for (auto enemy : enemies)
	{
		Vector2 enemyPos = enemy->GetPosition();
		Vector2 enemyScale = enemy->GetScale();
		enemy->BoxCollision(enemyPos, enemyScale, m_Position, m_Scale);
	}

	//プレイヤーの位置と大きさ更新
	Vector2 playerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	Vector2 playerScale = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	//プレイヤーのボックス当たり判定
	Manager::GetScene()->GetGameObject<Player>()->BoxCollision(playerPos, playerScale, m_Position, m_Scale);

}

void Box::Draw()
{

	if (m_DrawPosition.x < -m_Scale.x / 2 ||
		m_DrawPosition.x > screenWidth + m_Scale.x / 2 ||
		m_DrawPosition.y < -m_Scale.y / 2 ||
		m_DrawPosition.y > screenHeight + m_Scale.y / 2)
	{
		//画面外なら描画しない
		return;
	}

	DrawSprite(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), 0, 1.0f);

}
