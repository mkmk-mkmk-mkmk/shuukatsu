#include "main.h"
#include "texture.h"
#include "scene.h"
#include "manager.h"

#include "springBoard.h"
#include "map.h"
#include "player.h"
#include "camera.h"

void SpringBoard::Init(ChainPoint* leftPoint, ChainPoint* rightPoint, float height)
{
	// 左端
	m_LeftPoint = leftPoint;

	// 右端
	m_RightPoint = rightPoint;

	m_Position = m_LeftPoint->pos + (m_RightPoint->pos - m_LeftPoint->pos) * 0.5f;

	//大きさ
	float length = distance(rightPoint->pos, leftPoint->pos);
	m_Scale = Vector2(length, height);


	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\springBoard_1.png"));

}

void SpringBoard::Update()
{
	if (m_DrawPosition.x < -m_Scale.x || m_DrawPosition.x > screenWidth + m_Scale.x ||
		m_DrawPosition.y < -m_Scale.y || m_DrawPosition.y > screenHeight + m_Scale.y)
	{
		return; //画面外ならプレイヤーの当たり判定は更新しない
	}

	//プレイヤーの位置と大きさ更新
	Vector2 playerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	Vector2 playerScale = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	//プレイヤーのボックス当たり判定
	Manager::GetScene()->GetGameObject<Player>()->BoxCollision(playerPos, playerScale, m_Position, m_Scale);
	BoxCollision(playerPos, playerScale, m_Position, m_Scale);
	if (BoxCollisionCommon(playerPos, playerScale, m_Position, m_Scale))
	{
		Manager::GetScene()->GetGameObject<Player>()->SetJump(true);
	}
	else
	{
		Manager::GetScene()->GetGameObject<Player>()->SetJump(false);
	}

	m_OldPosition = m_Position;

	//位置、大きさ更新
	m_Position = m_LeftPoint->pos + (m_RightPoint->pos - m_LeftPoint->pos) * 0.5f;

	m_Rotate = atan2(m_RightPoint->pos.y - m_LeftPoint->pos.y,
		m_RightPoint->pos.x - m_LeftPoint->pos.x);

}

void SpringBoard::Draw()
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

void SpringBoard::BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale)
{
	Vector2 DiffPosition = m_Position - m_OldPosition;

	Manager::GetScene()->GetGameObject<Player>()->SetPosition(Manager::GetScene()->GetGameObject<Player>()->GetPosition() + DiffPosition);
	Manager::GetScene()->GetGameObject<Player>()->SetOnGround(false);
}