#include "main.h"
#include "player.h"
#include "texture.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "map.h"

#include "enemy_Ground.h"

#include "BehaviorTree/actionNode.h"
#include "BehaviorTree/conditionNode.h"
#include "BehaviorTree/sequenceNode.h"
#include "BehaviorTree/selectorNode.h"


void Enemy_Ground::Init(Vector2 pos, Vector2 scale)
{
	//敵キャラスタート位置
	m_Position = pos;

	//敵キャラの大きさ
	m_Scale = scale;
	
	//敵のHP（enmeyの種類に応じて体力設定）
	//switch(enemyType)
	m_Life = 1;

	//プレイヤーの位置と大きさも取得
	m_PlayerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	m_PlayerPos = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	//乱数を初期化しておく
	m_Random = random.RandomInt(0, 2);

	//動き始めのタイミングをランダムにする
	m_Frame = random.RandomInt(0, 100);

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\patrol.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\patrol.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\chase.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\attack.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\clickParticle.png"));

	m_TextureType = 0;

	//ビヘイビアツリー構築
	m_RootNode = new SelectorNode();

	//攻撃
	auto attackSeq = new SequenceNode();
	attackSeq->AddChild(new ConditionNode([this]
		{ return InRangeObject(m_Position, m_Scale, m_PlayerPos, m_PlayerScale, m_VisibleRange, m_Direction); }));
	attackSeq->AddChild(new ConditionNode([this]
		{ return InRangeObject(m_Position, m_Scale, m_PlayerPos, m_PlayerScale, m_AttackRange, m_Direction); }));
	attackSeq->AddChild(new ActionNode([this] { return Attack(); }));

	//追跡
	auto chaseSeq = new SequenceNode();
	chaseSeq->AddChild(new ConditionNode([this]
		{ return InRangeObject(m_Position, m_Scale, m_PlayerPos, m_PlayerScale, m_VisibleRange, m_Direction); }));
	chaseSeq->AddChild(new ActionNode([this] { return Chase(); }));

	//徘徊
	auto patrol = new ActionNode([this] { return Patrol(); });

	//ツリー構築
	m_RootNode->AddChild(attackSeq);
	m_RootNode->AddChild(chaseSeq);
	m_RootNode->AddChild(patrol);
}

void Enemy_Ground::Uninit()
{
	UnInitSprite();

	delete m_RootNode;
	m_RootNode = nullptr;
}

void Enemy_Ground::Update()
{
	//プレイヤーの位置と大きさ更新
	m_PlayerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	m_PlayerScale = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	if (m_HitSideBoxPos.size() > 0)
	{
		CheckStairs();
	}

	// ツリー実行
	if (m_RootNode)
	{
		//攻撃、発見、見回しなどのアニメーション中はツリーを止める
		if (!m_StopTick)
		{
			m_RootNode->Tick();
		}
	}

	//アニメーション名に応じた処理
	switch (m_AnimationState)
	{
	case AnimationState::Patrol:	//徘徊中
		UpdatePatrol();
		break;
	case AnimationState::Chase:		//追跡中
		UpdateChase();
		break;
	case AnimationState::Attack:	//攻撃
		UpdateAttack();
		break;
	case AnimationState::FindPlayer: //発見中
		UpdateFind();
		break;
	case AnimationState::LookAround: //見回し中
		UpdateLookAround();
		break;
	default:
		break;
	}

	//左右に力が働いている場合、重力を適用させる
	if (m_Vector.x != 0.0f)
	{
		m_OnGround = false;
	}

	//重力を適用
	if (!m_OnGround)
	{
		m_Vector.y += m_Gravity;
	}

	//位置更新
	m_Position += m_Vector;

	//プレイヤーの軌跡行動中に当たったらダメージ
	m_HitPlayerAttack = Collision::BoxCollisionCommon(m_Position, m_Scale, m_PlayerPos, m_PlayerScale);

	if (m_HitPlayerAttack && Manager::GetScene()->GetGameObject<Player>()->GetMoveTrail()
		&& !m_HitPlayerAttackOnce)
	{
		AddLife(-1);
		m_HitPlayerAttackOnce = true;
	}

	if (!Manager::GetScene()->GetGameObject<Player>()->GetMoveTrail())
	{
		m_HitPlayerAttackOnce = false;
	}

	//体力が0以下なら自身を消す
	if (m_Life = 0 || m_Life < 0)
	{
		m_Destroy = true;
	}
}

void Enemy_Ground::Draw()
{
	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	DrawSprite(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), m_TextureType, 1.0f);

	if (m_DrawHitBox)
	{
		DrawAttackHitBox();
	}
}

void Enemy_Ground::DrawAttackHitBox()
{
	m_AttackHitBoxPos = m_Direction ?
		Vector2(m_Position.x + m_AttackRange.x / 2, m_Position.y) :
		Vector2(m_Position.x - m_AttackRange.x / 2, m_Position.y);

	//描画位置更新
	m_AttackHitBoxDrawPos =
		m_AttackHitBoxPos - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	DrawSprite(XMFLOAT2(m_AttackHitBoxDrawPos.x, m_AttackHitBoxDrawPos.y), m_Rotate,
		XMFLOAT2(m_AttackRange.x, m_AttackRange.y), 4, 1.0f);

}

NodeStatus Enemy_Ground::Patrol() //徘徊中
{
	m_OldAnimationState = m_AnimationState;
	m_AnimationState = AnimationState::Patrol;
	if (m_AnimationState != m_OldAnimationState)
	{
		m_Frame = 0;
		m_TextureType = 1;
	}
	return NodeStatus::Running;

}

NodeStatus Enemy_Ground::Chase() //プレイヤーに向かって移動中
{
	m_OldAnimationState = m_AnimationState;
	m_AnimationState = AnimationState::Chase;
	if (m_AnimationState != m_OldAnimationState)
	{
		m_Frame = 0;
		m_TextureType = 2;
	}
	return NodeStatus::Running;

}

NodeStatus Enemy_Ground::Attack() //攻撃
{
	m_OldAnimationState = m_AnimationState;
	m_AnimationState = AnimationState::Attack;
	if (m_AnimationState != m_OldAnimationState)
	{
		m_Frame = 0;
		m_TextureType = 3;
	}
	return NodeStatus::Running;

}

NodeStatus Enemy_Ground::OnlyAnimation() //アニメーションのみ再生
{
	return NodeStatus::Success;
}

void Enemy_Ground::UpdatePatrol()
{
	//徘徊処理

	m_Frame++;
	if (m_Frame > 150)
	{
		switch (m_Random)
		{
		case 0: //右移動
			m_Vector.x = m_Speed;
			m_Random = 2;
			m_Direction = true;
			m_Frame = 0;

			break;
		case 1: //左移動
			m_Vector.x = -m_Speed;
			m_Random = 2;
			m_Direction = false;
			m_Frame = 0;

			break;
		case 2: //停止
			m_Vector.x = 0.0f;
			m_Random = random.RandomInt(0, 1);
			m_Frame = 90;

			break;
		}
	}
}

void Enemy_Ground::UpdateChase()
{
	//追跡処理

	m_Vector.x = m_Direction ? m_Speed : -m_Speed; //敵の向きに応じて移動
	m_OnGround = false;

}

void Enemy_Ground::UpdateAttack()
{
	//攻撃中は移動しない
	m_Vector.x = 0.0f;

	//攻撃アニメーション中はTickを止める
	m_StopTick = true;

	m_Frame++;

	//攻撃処理
	if (m_Frame > 200)	//攻撃アニメーションを継続するか判定
	{
		m_StopTick = false;
		m_HitAttack = false;
		m_DrawHitBox = false;

		m_Frame = 0;
	}
	else if (m_Frame > 100)	//攻撃判定が発生するタイミング
	{
		m_HitAttack = InRangeObject(m_Position, m_Scale, m_PlayerPos, m_PlayerScale, m_AttackRange, m_Direction);

		m_DrawHitBox = true;

		if (!m_HitOnce && m_HitAttack)
		{
			Manager::GetScene()->GetGameObject<Player>()->AddLife(-100);
			m_HitAttack = true;
		}
	}
}

void Enemy_Ground::UpdateFind()
{
	//発見アニメーション再生


	//アニメーション再生時はTickを止める
	//m_StopTick = true;

	//発見アニメーション終了後にstate変更
	//if()
	{
		//m_StopTick = false;
		m_AnimationState = AnimationState::Chase;		//追跡へ
		m_ChaseAnimationStarted = false;
	}
}

void Enemy_Ground::UpdateLookAround()
{
	//見回しアニメーション再生

	//アニメーション再生時はTickを止める
	//m_StopTick = true;

	//発見アニメーション終了後にstate変更
	//if()
	{
		//m_StopTick = false;
		m_AnimationState = AnimationState::Patrol;		//巡回へ
		m_PatrolAnimationStarted = false;
	}
}

void Enemy_Ground::DeleteAnimation()
{
	//消滅アニメーション再生

	//アニメーション再生時はTickを止める
	//m_StopTick = true;

	//m_DeleteAnimationFinishedをtrueにすると自動的に消える
	m_DeleteAnimationFinished = true;
}

void Enemy_Ground::BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale)
{

	if (m_Position.y < boxPos.y	//ボックスの上に乗っている場合
		&& m_Position.x + m_Scale.x * 0.25f >= boxPos.x - boxScale.x * 0.5f
		&& m_Position.x - m_Scale.x * 0.25f <= boxPos.x + boxScale.x * 0.5f)
	{
		m_Position.y = boxPos.y - boxScale.y * 0.5f - m_Scale.y * 0.5f; //位置をボックスの上に調整
		m_OnGround = true;
		m_Vector.y = 0.0f; //落下速度リセット
	}
	else if (m_Position.y > boxPos.y	//ボックスの下にいる場合
		&& m_Position.x + m_Scale.x * 0.25f >= boxPos.x - boxScale.x * 0.5f
		&& m_Position.x - m_Scale.x * 0.25f <= boxPos.x + boxScale.x * 0.5f)
	{
		m_Position.y = boxPos.y + boxScale.y * 0.5f + m_Scale.y * 0.5f; //位置をボックスの下に調整

		if (m_Vector.y < 0)
		{
			m_Vector.y = 0.0f; //落下速度リセット
		}
	}
	else if (m_Position.x < boxPos.x	//ボックスの左にいる場合
		&& m_Position.y + m_Scale.y * 0.25f >= boxPos.y - boxScale.y * 0.5f
		&& m_Position.y - m_Scale.y * 0.25f <= boxPos.y + boxScale.y * 0.5f)
	{
		m_Position.x = boxPos.x - boxScale.x * 0.5f - m_Scale.x * 0.5f; //位置をボックスの左に調整

		m_HitSideBoxPos.push_back(boxPos); //当たっている箱の位置を保存
	}
	else if (m_Position.x > boxPos.x	//ボックスの右にいる場合
		&& m_Position.y + m_Scale.y * 0.25f >= boxPos.y - boxScale.y * 0.5f
		&& m_Position.y - m_Scale.y * 0.25f <= boxPos.y + boxScale.y * 0.5f)
	{
		m_Position.x = boxPos.x + boxScale.x * 0.5f + m_Scale.x * 0.5f; //位置をボックスの右に調整

		m_HitSideBoxPos.push_back(boxPos); //当たっている箱の位置を保存
	}

}

void Enemy_Ground::CheckStairs()
{
	m_JumpStairs = true;

	//側面で当たっているboxの数だけループ
	for (int i = 0; i < m_HitSideBoxPos.size(); i++)
	{
		//体の半分より上に箱がある場合は飛べない
		if (m_Position.y > m_HitSideBoxPos.front().y - MAPCHIP_HEIGHT * 0.5f)
		{
			m_JumpStairs = false;

			//ずっと壁に直進しないようにする
			if (m_Position.x < m_HitSideBoxPos.front().x)	//ボックスの左にいる
			{
				m_Random = 1;
			}
			else											//ボックスの右にいる
			{
				m_Random = 0;
			}
		}
		m_HitSideBoxPos.pop_front();
	}

	//階段上り処理
	if (m_OnGround && m_JumpStairs)
	{
		m_Vector.y += -m_JumpPower; //上に移動
		m_OnGround = false;
	}

}