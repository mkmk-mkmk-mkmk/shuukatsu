#include "main.h"
#include "texture.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "unvisualBox.h"
#include "map.h"
#include "player.h"
#include "camera.h"

void UnVisualBox::Init()
{
	//初期位置設定
	m_Position = Manager::GetScene()->GetGameObject<Map>()->m_UnvisualBoxPosList.front();

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);
}

void UnVisualBox::Uninit()
{
}

void UnVisualBox::Update()
{
	//m_DrawPosition =
	//	m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	//if (m_DrawPosition.x < -m_Scale.x || m_DrawPosition.x > screenWidth + m_Scale.x ||
	//	m_DrawPosition.y < -m_Scale.y || m_DrawPosition.y > screenHeight + m_Scale.y)
	//{
	//	return; //画面外ならプレイヤーのほうの更新はしない
	//}

	//プレイヤーの位置と大きさ更新
	Vector2 playerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	Vector2 playerScale = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	//プレイヤーのボックス当たり判定
	Manager::GetScene()->GetGameObject<Player>()->BoxCollision(playerPos, playerScale, m_Position, m_Scale);
}

void UnVisualBox::Draw()
{
	//描画無し
}
