
#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "title_Cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "map.h"
#include "title_BreakableBox.h"
#include "boxBreakEffect.h"
#include "spring.h"
#include "springChain.h"
#include "springBoard.h"
#include "collision.h"


void Title_BreakableBox::Init()
{

	//初期位置設定
	m_Position = Vector2(float(random.RandomInt(screenWidth, screenWidth * 3)) * 0.1f,
		float(random.RandomInt(screenHeight, screenHeight * 3) * -0.1f));
	m_LeftorRight = random.RandomInt(0, 1);

	switch (m_LeftorRight)
	{
		case 0: //左側
			m_Position.x = float(random.RandomInt(screenWidth, screenWidth * 3)) * 0.1f;
			break;
		case 1: //右側
			m_Position.x = float(random.RandomInt(screenWidth * 7, screenWidth * 9)) * 0.1f;
			break;
	}

	//移動ベクトル設定
	m_Vector = Vector2(float(random.RandomInt(-5, 5)), 0.0f);

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\tile_Breakable.png"));
}

void Title_BreakableBox::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Title_BreakableBox::Update()
{
	if (!m_Breaking)
	{
		m_Vector.y += m_Gravity;

		m_Position += m_Vector;

		//if (m_Position.y > m_BreakHeight)
		//{
		//	m_Breaking = true;
		//	//エフェクト生成
		//	auto effect = std::make_unique<BoxBreakEffect>();
		//	effect->Init(m_Position);
		//	m_BoxBreakEffectList.push_back(std::move(effect));
		//}

		m_CursorPos = Manager::GetScene()->GetUIObject<Title_Cursor>()->GetPosition();
		m_CursorScale = Manager::GetScene()->GetUIObject<Title_Cursor>()->GetScale();

		m_HitCursor = BoxCollisionCommon(m_Position, m_Scale, m_CursorPos, m_CursorScale);

		if (m_HitCursor && !m_Breaking)
		{
			m_Breaking = true;
			//エフェクト生成
			auto effect = std::make_unique<BoxBreakEffect>();
			effect->Init(m_Position);
			m_BoxBreakEffectList.push_back(std::move(effect));
		}

	}
	else
	{
		for (auto it = m_BoxBreakEffectList.begin(); it != m_BoxBreakEffectList.end(); )
		{
			(*it)->Update();

			if ((*it)->GetDestroy())
			{
				it = m_BoxBreakEffectList.erase(it);
			}
			else
			{
				++it;
			}

			if (m_BoxBreakEffectList.empty())
			{
				this->SetDestroy();
			}
		}
	}
}

void Title_BreakableBox::Draw()
{

	if (!m_Breaking)
	{
		DrawSprite(XMFLOAT2(m_Position.x, m_Position.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), 0, 1.0f);

	}
	else
	{
		for (auto& p : m_BoxBreakEffectList)
		{
			p->Draw(m_DrawDiff);
		}
	}
}
