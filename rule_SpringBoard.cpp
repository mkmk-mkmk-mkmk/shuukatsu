#include "main.h"
#include "texture.h"
#include "scene.h"
#include "manager.h"

#include "rule_SpringBoard.h"
#include "map.h"
#include "player.h"

void Rule_SpringBoard::Init(Rule_ChainPoint* leftPoint, Rule_ChainPoint* rightPoint, float height)
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

	m_TextureList.push_back(Texture::Load("asset\\texture\\ruleBoard.png"));

}

void Rule_SpringBoard::Update()
{
	//位置、大きさ更新
	m_Position = m_LeftPoint->pos + (m_RightPoint->pos - m_LeftPoint->pos) * 0.5f;

	m_Rotate = atan2(m_RightPoint->pos.y - m_LeftPoint->pos.y,
		m_RightPoint->pos.x - m_LeftPoint->pos.x);


}

void Rule_SpringBoard::Draw()
{

	DrawSprite(XMFLOAT2(m_Position.x, m_Position.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), 0, 1.0f);

}