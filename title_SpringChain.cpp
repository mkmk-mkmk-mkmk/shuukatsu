#include "main.h"
#include "texture.h"
#include "scene.h"
#include "manager.h"

#include "title_SpringChain.h"
#include "map.h"
#include "player.h"

void Title_SpringChain::Init(Vector2 topPos, Vector2 bottomPos, float chainSplit, float chainWidth)
{
	//分割数+1が点の数
	m_ChainPointCount = (int)chainSplit + 1;

	//一つ一つのチェーンの大きさ取得（絶対値で）
	Vector2 chainPieceScale = (bottomPos - topPos) / chainSplit;
	m_ChainLength = length(chainPieceScale);

	//リストをクリアしておく
	m_TitleChainPointList.clear();

	float swingPower = 3.0f;

	//点のリスト初期化
	for (int i = 0; i < m_ChainPointCount; i++)
	{
		Title_ChainPoint pointList;
		pointList.pos = topPos + (chainPieceScale * (float)i);

		float t = (float)i / (m_ChainPointCount - 1);
		float vx = sinf(t * PI) * swingPower; // 中央ほど強く

		pointList.oldPos = pointList.pos - Vector2(vx, 0.0f);
		pointList.acceleration = Vector2(0.0f, 0.0f);
		pointList.lock = false;			//後に一番上だけ固定する

		m_TitleChainPointList.push_back(pointList);
	}

	m_Scale = Vector2(chainWidth, chainPieceScale.y);

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\springChain.png"));

}

void Title_SpringChain::Uninit()
{
	UnInitSprite();
}

void Title_SpringChain::Update()
{
	//重力適用
	for (auto& point : m_TitleChainPointList)
	{
		if (!point.lock)
		{
			point.acceleration.y += m_Gravity;
		}
	}

	//位置更新
	for (auto& point : m_TitleChainPointList)
	{
		if (point.lock)
		{
			continue;
		}

		Vector2 oldPointPos = point.pos;

		// 位置更新
		point.pos += (point.pos - point.oldPos) + point.acceleration;
		point.oldPos = oldPointPos;
		point.acceleration = Vector2(0, 0);
	}

	//距離制約
	const int constraintLoop = 10;	//距離制約をフレームごとに何回解くか

	for (int n = 0; n < constraintLoop; n++)
	{
		for (int i = 0; i < m_ChainPointCount - 1; i++)
		{
			Title_ChainPoint& point1 = m_TitleChainPointList[i];
			Title_ChainPoint& point2 = m_TitleChainPointList[i + 1];

			Vector2 delta = point2.pos - point1.pos;
			float pointDistance = length(delta);
			if (pointDistance == 0.0f)
			{
				continue;
			}

			float stiffness = 0.9f;
			float diff = (pointDistance - m_ChainLength) / pointDistance;

			if (!point1.lock)
			{
				point1.pos += delta * 0.5f * diff * stiffness;
			}

			if (!point2.lock)
			{
				point2.pos -= delta * 0.5f * diff * stiffness;
			}
		}
	}

	if (m_TitleChainPointList.front().pos.y > -screenHeight / 4)
	{
		//一番上の点を固定
		m_TitleChainPointList.front().lock = true;
	}
}

void Title_SpringChain::Draw()
{
	for (int i = 0; i < m_ChainPointCount - 1; i++)
	{
		DrawPiece(i);
	}
}

void Title_SpringChain::DrawPiece(int count)
{
	Title_ChainPoint point1 = m_TitleChainPointList[count];
	Title_ChainPoint point2 = m_TitleChainPointList[count + 1];

	m_Position = point1.pos + ((point2.pos - point1.pos) * 0.5f);
	m_Rotate = PI * 0.5f - atan2(point2.pos.y - point1.pos.y,
		point2.pos.x - point1.pos.x);

	DrawSprite(XMFLOAT2(m_Position.x, m_Position.y), -m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y + 50.0f), 0, 1.0f);

}