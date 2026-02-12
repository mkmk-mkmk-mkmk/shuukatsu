#include "main.h"
#include "player.h"
#include "texture.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "map.h"
#include "SE.h"

#include "enemy.h"

#include "BehaviorTree/actionNode.h"
#include "BehaviorTree/conditionNode.h"
#include "BehaviorTree/sequenceNode.h"
#include "BehaviorTree/selectorNode.h"

void Enemy::Init(Vector2 pos, Vector2 scale, EnemyType enemyType)
{
	//敵キャラスタート位置
	m_Position = pos;

	//敵キャラの大きさ
	m_Scale = scale;

	//敵キャラの種類
	m_EnemyType = enemyType;

	//エネミーの種類に応じて基礎初期値設定）
	switch (m_EnemyType)
	{
	case Ground:
		m_Life = 1;
		m_Speed = 2.0f;
		m_JumpPower = 6.0f;
		m_VisibleRange = { 300.0f, 100.0f };
		m_AttackRange = { 120.0f, 80.0f };

		InitSprite();
		m_TextureList.push_back(Texture::Load("asset\\texture\\enemy\\enemy_Ground\\enemy_Ground.png"));
		m_TextureList.push_back(Texture::Load("asset\\texture\\enemy\\enemy_Ground\\enemy_GroundAnimation.png"));
		m_TextureList.push_back(Texture::Load("asset\\texture\\chase.png"));
		m_TextureList.push_back(Texture::Load("asset\\texture\\attack.png"));
		m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\clickParticle.png"));

		break;

	case Flying:
		m_Life = 1;
		m_Speed = 2.0f;
		m_VisibleRange = { 300.0f, 100.0f };
		m_AttackRange = { 120.0f, 80.0f };

		InitSprite();
		m_TextureList.push_back(Texture::Load("asset\\texture\\enemy\\enemy_Flying\\enemy_FlyingAnimation.png"));
		m_TextureList.push_back(Texture::Load("asset\\texture\\enemy\\enemy_Flying\\enemy_FlyingAnimation.png"));
		m_TextureList.push_back(Texture::Load("asset\\texture\\chase.png"));
		m_TextureList.push_back(Texture::Load("asset\\texture\\attack.png"));
		m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\clickParticle.png"));

		break;

	default:
		break;
	}

	//プレイヤーの位置と大きさも取得
	m_PlayerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	m_PlayerPos = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	//乱数を初期化しておく
	m_RandomInt = random.RandomInt(0, 2);

	//動き始めのタイミングをランダムにする
	m_Frame = random.RandomInt(0, 100);

	//テクスチャタイプ初期化
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

void Enemy::Uninit()
{
	UnInitSprite();

	delete m_RootNode;
	m_RootNode = nullptr;
}

void Enemy::Update()
{
	switch (m_EnemyType)
	{
	case Ground:
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
			Manager::GetSE()->Play(SE_BreakBox);
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

		break;
	case Flying:
		//プレイヤーの位置と大きさ更新
		m_PlayerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
		m_PlayerScale = Manager::GetScene()->GetGameObject<Player>()->GetScale();

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

		break;
	default:
		break;
	}

}

void Enemy::Draw()
{
	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	if (m_DrawPosition.x < -m_Scale.x || m_DrawPosition.x > screenWidth + m_Scale.x ||
		m_DrawPosition.y < -m_Scale.y || m_DrawPosition.y > screenHeight + m_Scale.y)
	{
		return; //画面外なら描画しない
	}


	switch (m_EnemyType)
	{
	case Ground:
		switch (m_AnimationState)
		{
		case AnimationState::Patrol:
			if (m_Vector.x != 0.0f)
			{
				DrawSpriteAnim(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
					XMFLOAT2(m_Scale.x, m_Scale.y), 8, 8, 1, 1, 1.0f, m_Direction);
			}
			else
			{
				DrawSpriteAnim(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
					XMFLOAT2(m_Scale.x, m_Scale.y), 1, 1, 1, 0, 1.0f, m_Direction);
			}
			break;
		case AnimationState::Chase:
			DrawSpriteAnim(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
				XMFLOAT2(m_Scale.x, m_Scale.y), 8, 8, 1, 1, 1.0f, m_Direction);
			break;
		case AnimationState::Attack:
			DrawSpriteAnim(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
				XMFLOAT2(m_Scale.x, m_Scale.y), 8, 1, 1, 0, 1.0f, m_Direction);
			break;
		case AnimationState::FindPlayer:

			break;

		case AnimationState::LookAround:

			break;
		}
		break;
	case Flying:
		switch (m_AnimationState)
		{
		case AnimationState::Patrol:
			DrawSpriteAnim(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
				XMFLOAT2(m_Scale.x, m_Scale.y), 8, 8, 1, 1, 1.0f, m_Direction);
			break;
		case AnimationState::Chase:
			DrawSpriteAnim(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
				XMFLOAT2(m_Scale.x, m_Scale.y), 8, 8, 1, 1, 1.0f, m_Direction);
			break;
		case AnimationState::Attack:
			DrawSpriteAnim(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate,
				XMFLOAT2(m_Scale.x, m_Scale.y), 8, 1, 1, 0, 1.0f, m_Direction);
			break;
		case AnimationState::FindPlayer:

			break;

		case AnimationState::LookAround:

			break;
		}
		break;

	}

	if (m_DrawHitBox)
	{
		DrawAttackHitBox();
	}
}

void Enemy::DrawAttackHitBox()
{
	if (m_Direction) //右向き
	{
		m_AttackHitBoxPos = Vector2(m_Position.x + m_AttackRange.x / 2, m_Position.y);
	}
	else //左向き
	{
		m_AttackHitBoxPos = Vector2(m_Position.x - m_AttackRange.x / 2, m_Position.y);
	}

	//描画位置更新
	m_AttackHitBoxDrawPos =
		m_AttackHitBoxPos - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	DrawSprite(XMFLOAT2(m_AttackHitBoxDrawPos.x, m_AttackHitBoxDrawPos.y), m_Rotate,
		XMFLOAT2(m_AttackRange.x, m_AttackRange.y), 4, 1.0f);

}

NodeStatus Enemy::Patrol() //徘徊中
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

NodeStatus Enemy::Chase() //プレイヤーに向かって移動中
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

NodeStatus Enemy::Attack() //攻撃
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

NodeStatus Enemy::OnlyAnimation() //アニメーションのみ再生
{
	return NodeStatus::Success;
}

void Enemy::UpdatePatrol()
{
	//徘徊処理
	switch (m_EnemyType)
	{
	case Ground:
		m_Frame++;
		if (m_Frame > m_PatrolFrame)
		{
			switch (m_RandomInt)
			{
			case 0: //右移動
				m_Vector.x = m_Speed;
				m_RandomInt = random.RandomInt(0, 2);
				//ずっと移動しているのを防止
				m_MoveCount++;
				if (m_MoveCount >= m_MoveCountMax)
				{
					m_RandomInt = 2; //次は停止
					m_MoveCount = 0;
				}
				m_Direction = true;
				m_Frame = 0;

				break;
			case 1: //左移動
				m_Vector.x = -m_Speed;
				m_RandomInt = random.RandomInt(0, 2);
				//ずっと移動しているのを防止
				m_MoveCount++;
				if (m_MoveCount >= m_MoveCountMax)
				{
					m_RandomInt = 2; //次は停止
					m_MoveCount = 0;
				}
				m_Direction = false;
				m_Frame = 0;

				break;
			case 2: //停止
				m_Vector.x = 0.0f;
				m_RandomInt = random.RandomInt(0, 2);

				//ずっと停止しているのを防止
				m_StopCount++;
				if (m_StopCount >= m_StopCountMax)
				{
					m_RandomInt = random.RandomInt(0, 1); //次は移動
					m_StopCount = 0;
				}
				m_Frame = 90;

				break;
			}
		}

		break;
	case Flying:
		m_Frame++;
		if (m_Frame > m_PatrolFrame)
		{
			m_RandomFloat = random.RandomFloat(-1.0f, 1.0f);

			m_Vector.x = m_RandomFloat * m_Speed;

			if (m_Vector.x >= 0)
			{
				m_Direction = true; //右向き
			}
			else if (m_Vector.x < 0)
			{
				m_Direction = false; //左向き
			}


			switch (m_RandomInt)
			{
			case 0: //上向き
				m_Vector.y = m_Speed - modulus(m_Vector.x);
				m_RandomInt = random.RandomInt(0, 2);

				//ずっと移動しているのを防止
				m_MoveCount++;
				if (m_MoveCount >= m_MoveCountMax)
				{
					m_RandomInt = 2; //次は停止
					m_MoveCount = 0;
				}
				break;
			case 1: //下向き
				m_Vector.y = -m_Speed + modulus(m_Vector.x);
				m_RandomInt = random.RandomInt(0, 2);

				//ずっと移動しているのを防止
				m_MoveCount++;
				if (m_MoveCount >= m_MoveCountMax)
				{
					m_RandomInt = 2; //次は停止
					m_MoveCount = 0;
				}

				break;
			case 2: //停止
				m_Vector = { 0.0f, 0.0f };
				m_RandomInt = random.RandomInt(0, 2);

				//ずっと停止しているのを防止
				m_StopCount++;
				if (m_StopCount >= m_StopCountMax)
				{
					m_RandomInt = random.RandomInt(0, 1); //次は移動
					m_StopCount = 0;
				}

				break;
			}

			m_Frame = 0;
		}
		break;
	default:
		break;
	}
}

void Enemy::UpdateChase()
{
	//追跡処理

	switch (m_EnemyType)
	{
	case Ground:
		m_Vector.x = m_Direction ? m_Speed : -m_Speed; //敵の向きに応じて移動
		m_OnGround = false;

		break;
	case Flying:

		break;
	default:
		break;
	}

}

void Enemy::UpdateAttack()
{
	switch (m_EnemyType)
	{
	case Ground:
		//攻撃中は移動しない
		m_Vector.x = 0.0f;

		//攻撃アニメーション中はTickを止める
		m_StopTick = true;

		m_Frame++;

		//攻撃処理
		if (m_Frame > m_AttackFrame)	//攻撃アニメーションを継続するか判定
		{
			m_StopTick = false;
			m_HitAttack = false;
			m_DrawHitBox = false;

			m_Frame = 0;
		}
		else if (m_Frame > 50)	//攻撃判定が発生するタイミング
		{
			m_HitAttack = InRangeObject(m_Position, m_Scale, m_PlayerPos, m_PlayerScale, m_AttackRange, m_Direction);

			m_DrawHitBox = true;

			if (m_HitAttack)
			{
				if (!Manager::GetScene()->GetGameObject<Player>()->GetNoDamage())
				{
					Manager::GetScene()->GetGameObject<Player>()->AddLife(-1);
					Manager::GetScene()->GetGameObject<Player>()->SetNoDamage(true);
				}
			}
		}

		break;
	case Flying:

		break;
	default:
		break;
	}
}

void Enemy::UpdateFind()
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

void Enemy::UpdateLookAround()
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

void Enemy::DeleteAnimation()
{
	//消滅アニメーション再生

	//アニメーション再生時はTickを止める
	//m_StopTick = true;

	//m_DeleteAnimationFinishedをtrueにすると自動的に消える
	m_DeleteAnimationFinished = true;
}

void Enemy::BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale)
{
	switch (m_EnemyType)
	{
	case Ground:
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

		break;
	case Flying:
		if (m_Position.y < boxPos.y	//ボックスの上辺に当たっている場合
			&& m_Position.x + m_Scale.x * 0.25f >= boxPos.x - boxScale.x * 0.5f
			&& m_Position.x - m_Scale.x * 0.25f <= boxPos.x + boxScale.x * 0.5f)
		{
			m_Position.y = boxPos.y - boxScale.y * 0.5f - m_Scale.y * 0.5f; //位置をボックスの上に調整
			m_Vector.y = 0.0f; //下降速度リセット
		}
		else if (m_Position.y > boxPos.y	//ボックスの下辺に当たっている場合
			&& m_Position.x + m_Scale.x * 0.25f >= boxPos.x - boxScale.x * 0.5f
			&& m_Position.x - m_Scale.x * 0.25f <= boxPos.x + boxScale.x * 0.5f)
		{
			m_Position.y = boxPos.y + boxScale.y * 0.5f + m_Scale.y * 0.5f; //位置をボックスの下に調整

			if (m_Vector.y < 0)
			{
				m_Vector.y = 0.0f; //上昇速度リセット
			}
		}
		else if (m_Position.x < boxPos.x	//ボックスの左辺に当たっている場合
			&& m_Position.y + m_Scale.y * 0.25f >= boxPos.y - boxScale.y * 0.5f
			&& m_Position.y - m_Scale.y * 0.25f <= boxPos.y + boxScale.y * 0.5f)
		{
			m_Position.x = boxPos.x - boxScale.x * 0.5f - m_Scale.x * 0.5f; //位置をボックスの左に調整
		}
		else if (m_Position.x > boxPos.x	//ボックスの右辺に当たっている場合
			&& m_Position.y + m_Scale.y * 0.25f >= boxPos.y - boxScale.y * 0.5f
			&& m_Position.y - m_Scale.y * 0.25f <= boxPos.y + boxScale.y * 0.5f)
		{
			m_Position.x = boxPos.x + boxScale.x * 0.5f + m_Scale.x * 0.5f; //位置をボックスの右に調整
		}

		break;
	default:
		break;
	}

}

void Enemy::CheckStairs()
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
			if (!m_JumpStairs && m_Position.x < m_HitSideBoxPos.front().x)	//ボックスの左にいる
			{
				m_Frame = m_PatrolFrame;
				m_RandomInt = 1;
			}
			else if (!m_JumpStairs)									//ボックスの右にいる
			{
				m_Frame = m_PatrolFrame;
				m_RandomInt = 0;
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