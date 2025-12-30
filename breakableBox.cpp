#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "breakableBox.h"
#include "map.h"
#include "player.h"
#include "camera.h"
#include "enemy.h"
#include "boxBreakEffect.h"


void BreakableBox::Init()
{
	//初期位置設定
	m_Position = Manager::GetScene()->GetGameObject<Map>()->m_BreakableBoxPosList.front();

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\tile_Breakable.png"));
}

void BreakableBox::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void BreakableBox::Update(const std::list<Enemy*>& enemies)
{
	//プレイヤーのボックス当たり判定
	Vector2 playerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	Vector2 playerScale = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	//プレイヤーの軌跡行動中に当たったらエフェクト生成
	if (Manager::GetScene()->GetGameObject<Player>()->GetMoveTrail() && !m_Breaking)
	{
		m_HitPlayerMoveTrail = Collision::BoxCollisionCommon(m_Position, m_Scale, playerPos, playerScale);

		if (m_HitPlayerMoveTrail)
		{
			m_Breaking = true;
			//エフェクト生成
			auto effect = std::make_unique<BoxBreakEffect>();
			effect->Init(m_Position);
			m_BoxBreakEffectList.push_back(std::move(effect));

		}
	}

	if (!m_Breaking)
	{
		//それ以外なら普通のboxと同じ
		Manager::GetScene()->GetGameObject<Player>()->BoxCollision(playerPos, playerScale, m_Position, m_Scale);

		//敵のボックス当たり判定
		for (auto enemy : enemies)
		{
			Vector2 enemyPos = enemy->GetPosition();
			Vector2 enemyScale = enemy->GetScale();
			enemy->BoxCollision(enemyPos, enemyScale, m_Position, m_Scale);
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

void BreakableBox::Draw()
{
	//描画位置更新
	m_DrawDiff = Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	m_DrawPosition = m_Position - m_DrawDiff;

	if (m_DrawPosition.x < -m_Scale.x / 2 ||
		m_DrawPosition.x > screenWidth + m_Scale.x / 2 ||
		m_DrawPosition.y < -m_Scale.y / 2 ||
		m_DrawPosition.y > screenHeight + m_Scale.y / 2)
	{
		//画面外なら描画しない
		return;
	}

	if (!m_Breaking)
	{
		DrawSprite(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), 0, 1.0f);
	}
	else
	{
		for (auto& p : m_BoxBreakEffectList)
		{
			p->Draw(m_DrawDiff);
		}
	}

}
