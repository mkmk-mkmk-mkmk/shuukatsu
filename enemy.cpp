#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "player.h"
#include "texture.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "map.h"
#include "random.h"

#include "enemy.h"

#include "BehaviorTree/actionNode.h"
#include "BehaviorTree/conditionNode.h"
#include "BehaviorTree/sequenceNode.h"
#include "BehaviorTree/selectorNode.h"

Random random;

void Enemy::Init(Vector2 pos, Vector2 scale, int enemyType)
{
	//敵キャラスタート位置
	m_Position = Vector2(screenWidth * 0.75f, screenHeight * 0.5f);
	//m_Position = pos;

	//敵キャラの大きさ
	m_Scale = { 100.0f, 100.0f };
	//m_Scale = scale;

	//敵のHP
	m_Life = 1;

	//プレイヤーの位置と大きさも取得
	m_PlayerPos = Manager::GetScene()->GetGameObject<Player>()->GetPosition();
	m_PlayerPos = Manager::GetScene()->GetGameObject<Player>()->GetScale();

	////敵のタイプ（地上or空中）
	//if (enemyType == 0)
	//{
	//	EnemyType::Ground;
	//}
	//else if (enemyType == 1)
	//{
	//	EnemyType::Flying;
	//}

	//乱数を初期化しておく
	//m_Random = random.RandomInt(0, 2);
	m_Random = 2;

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_Texture = Texture::Load("asset\\texture\\patrol.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

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
	m_Texture->Release();

	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	delete m_RootNode;
	m_RootNode = nullptr;
}

void Enemy::Update()
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

void Enemy::Draw()
{
	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, 1.0f);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, 0.0f);
	trans = XMMatrixTranslation(m_DrawPosition.x, m_DrawPosition.y, 0.0f);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::GetDeviceContext()->Draw(4, 0);

}


NodeStatus Enemy::Patrol() //徘徊中
{
	m_OldAnimationState = m_AnimationState;
	m_AnimationState = AnimationState::Patrol;
	if (m_AnimationState != m_OldAnimationState)
	{
		m_Frame = 0;
		m_Texture = Texture::Load("asset\\texture\\patrol.png");
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
		m_Texture = Texture::Load("asset\\texture\\chase.png");
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
		m_Texture = Texture::Load("asset\\texture\\attack.png");
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

void Enemy::UpdateChase()
{
	//追跡処理

	m_Vector.x = m_Direction ? m_Speed : -m_Speed; //敵の向きに応じて移動
	m_OnGround = false;

}

void Enemy::UpdateAttack()
{
	//攻撃中は移動しない
	m_Vector.x = 0.0f;

	//攻撃アニメーション中はTickを止める
	m_StopTick = true;

	m_Frame++;

	//攻撃処理
	if (m_Frame > 200)	//攻撃アニメーションを継続するか判定
	{
		m_AttackAnimationFinished = true;
		m_StopTick = false;
		m_HitAttack = false;

		m_Frame = 0;
	}
	else if (m_Frame > 100)	//攻撃判定が発生するタイミング
	{
		m_HitAttack = InRangeObject(m_Position, m_Scale, m_PlayerPos, m_PlayerScale, m_AttackRange, m_Direction);

		if (!m_HitOnce && m_HitAttack)
		{
			Manager::GetScene()->GetGameObject<Player>()->AddLife(-100);
			m_HitAttack = true;
		}
	}
}

void Enemy::UpdateFind()
{
	//発見アニメーション再生

	m_AnimationState = AnimationState::Chase;		//追跡へ
	m_ChaseAnimationStarted = false;
}

void Enemy::UpdateLookAround()
{
	//見回しアニメーション再生

	m_AnimationState = AnimationState::Patrol;		//巡回へ
	m_PatrolAnimationStarted = false;
}

void Enemy::BoxCollisionExtra(Vector2 boxPos, Vector2 boxScale)
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