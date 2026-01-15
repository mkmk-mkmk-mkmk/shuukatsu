
#include "main.h"
#include "texture.h"
#include "manager.h"

#include "random.h"
#include "boxBreakEffect.h"

void BoxBreakEffect::Init(Vector2 position)
{
	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\breakableBox_piece.png"));

	for (int i = 0; i < m_PieceCount; i++)
	{
		m_PieceData.pos = position;
		m_PieceData.drawPos = position;
		m_PieceData.scale = float(random.RandomInt(15, 25));
		m_PieceData.rotate = float(random.RandomInt(0, 6));
		m_PieceData.vec = Vector2(float(random.RandomInt(-2, 2)), float(random.RandomInt(-2, 2)));

		m_EffectDataList.push_back(m_PieceData);
	}

}

void BoxBreakEffect::Update()
{
	for (int i = 0; i < m_EffectDataList.size(); i++)
	{
		//重力も適用する
		m_EffectDataList[i].vec.y += m_Gravity;

		m_EffectDataList[i].pos += m_EffectDataList[i].vec;
		m_EffectDataList[i].scale -= 0.2f;
		if (m_EffectDataList[i].scale < 0.0f)
		{
			m_EffectDataList.erase(m_EffectDataList.begin() + i);
			i--;
		}
	}

	if (m_EffectDataList.empty())
	{
		m_Destroy = true;
		//Destroy();
	}
}

void BoxBreakEffect::Draw(Vector2 drawDiff)
{
	for (int i = 0; i < m_EffectDataList.size(); i++)
	{
		DrawPiece(i, drawDiff);
	}
}

void BoxBreakEffect::DrawPiece(int count, Vector2 drawDiff)
{
	//ワールド座標と描画座標がずれることがあるので補正
	m_EffectDataList[count].drawPos = m_EffectDataList[count].pos - drawDiff;

	DrawSprite(XMFLOAT2(m_EffectDataList[count].drawPos.x, m_EffectDataList[count].drawPos.y), m_EffectDataList[count].rotate,
		XMFLOAT2(m_EffectDataList[count].scale, m_EffectDataList[count].scale), 0, 1.0f);

}