#include "main.h"
#include "texture.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "damageBox.h"
#include "map.h"
#include "player.h"
#include "camera.h"

void DamageBox::Init()
{
	//初期位置設定
	m_Position = Manager::GetScene()->GetGameObject<Map>()->m_DamageBoxPosList.front();

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\tile_Breakable.png"));
}

void DamageBox::Uninit()
{
	UnInitSprite();
}

void DamageBox::Update()
{

	////敵のボックス当たり判定
	//for (auto enemy : enemies)
	//{
	//	Vector2 enemyPos = enemy->GetPosition();
	//	Vector2 enemyScale = enemy->GetScale();
	//	enemy->BoxCollision(enemyPos, enemyScale, m_Position, m_Scale);
	//}

	if (m_DrawPosition.x < -m_Scale.x || m_DrawPosition.x > screenWidth + m_Scale.x ||
		m_DrawPosition.y < -m_Scale.y || m_DrawPosition.y > screenHeight + m_Scale.y)
	{
		return; //画面外ならプレイヤーのほうの更新はしない
	}

	//プレイヤーの位置と大きさ更新
	Vector2 playerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	Vector2 playerScale = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	//プレイヤーのボックス当たり判定
	Manager::GetScene()->GetGameObject<Player>()->BoxCollision(playerPos, playerScale, m_Position, m_Scale);

	if (BoxCollisionCommon(playerPos, playerScale, m_Position, m_Scale))
	{
		if (!Manager::GetScene()->GetGameObject<Player>()->GetNoDamage())
		{
			Manager::GetScene()->GetGameObject<Player>()->AddLife(-m_Damage);
			Manager::GetScene()->GetGameObject<Player>()->SetNoDamage(true);
		}
	}
}

void DamageBox::Draw()
{
	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	if (m_DrawPosition.x < -m_Scale.x / 2 ||
		m_DrawPosition.x > screenWidth + m_Scale.x / 2 ||
		m_DrawPosition.y < -m_Scale.y / 2 ||
		m_DrawPosition.y > screenHeight + m_Scale.y / 2)
	{
		//画面外なら描画しない
		return;
	}

	DrawSprite(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
		XMFLOAT2(m_Scale.x, m_Scale.y), 0, 1.0f);

}
