#pragma once
#include "gameObject.h"
#include "BehaviorTree/behaviorNode.h"
#include "BehaviorTree/compositeNode.h"

//アニメーション状態
enum class AnimationState
{
	Patrol,		//徘徊
	Chase,		//追跡
	Attack,		//攻撃

	FindPlayer, //発見
	LookAround	//見回し
};

//敵の種類
enum class EnemyType
{
	Ground,
	Flying
};

class Enemy : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	CompositeNode* m_RootNode = nullptr;

	AnimationState m_AnimationState;

	Vector2 m_FirstPosition;		//初期座標
	Vector2 m_DrawPosition;			//描画位置、m_Positionは実際の位置を保存してこっちはカメラとの相対位置

	Vector2 m_PlayerPos;			//プレイヤー座標(追跡用)
	std::list<Vector2> m_HitSideBoxPos; //左右で当たっている箱のリスト

	float m_Speed = 2.0f;			//移動速度
	float m_JumpPower = 5.0f;		//ジャンプ力
	Vector2 m_VisibleRange = { 300.0f, 100.0f };//発見範囲
	Vector2 m_AttackRange = { 120.0f, 80.0f };	//攻撃範囲

	bool m_EnemyDirection = true;	//敵の向き(true:右 false:左)

	bool m_VisiblePlayer = false;	//プレイヤーが見えているかどうか
	bool m_InAttackRange = false;	//攻撃範囲内かどうか
	bool m_AttackCoolTime = false;	//攻撃のクールタイム中かどうか
	bool m_Attacking = false;		//攻撃中かどうか

	bool m_AttackAnimationStarted = false;		//攻撃アニメーション開始したかどうか
	bool m_AttackAnimationFinished = false;		//攻撃アニメーションが終了したかどうか
	bool m_ChaseAnimationStarted = false;		//追跡アニメーション開始したかどうか
	bool m_ChaseAnimationFinished = false;		//追跡アニメーションが終了したかどうか
	bool m_PatrolAnimationStarted = false;		//徘徊アニメーション開始したかどうか
	bool m_PatrolAnimationFinished = false;		//徘徊アニメーションが終了したかどうか

	int m_Random = 2;	//乱数用

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool InRangePlayer(Vector2 range, bool direction);

	NodeStatus Patrol();
	NodeStatus Chase();
	NodeStatus Attack();
	NodeStatus OnlyAnimation(); //アニメーションのみ再生

	void UpdatePatrol();
	void UpdateChase();
	void UpdateAttack();
	void UpdateFind();
	void UpdateLookAround();

	Vector2 GetEnemyPosition() { return m_Position; }
	void SetEnemyPosition(Vector2 pos)
	{
		m_Position = pos;
		m_DrawPosition = m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();
	}

	void CheckStairs();

protected:

	void BoxCollisionExtraStep(Vector2 boxPos, Vector2 boxScale) override
	{
		//衝突判定
		if (m_DrawPosition.x + m_Scale.x * 0.5f > boxPos.x - boxScale.x * 0.5f &&
			m_DrawPosition.x - m_Scale.x * 0.5f < boxPos.x + boxScale.x * 0.5f &&
			m_DrawPosition.y + m_Scale.x * 0.5f > boxPos.y - boxScale.y * 0.5f &&
			m_DrawPosition.y - m_Scale.x * 0.5f < boxPos.y + boxScale.y * 0.5f)
		{
			if (m_DrawPosition.y < boxPos.y	//ボックスの上に乗っている場合
				&& m_DrawPosition.x + m_Scale.x > boxPos.x - boxScale.x * 0.5f && m_DrawPosition.x - m_Scale.x < boxPos.x + boxScale.x * 0.5f)
			{
				m_DrawPosition.y = boxPos.y - boxScale.y * 0.5f - m_Scale.y * 0.5f; //位置をボックスの上に調整
				m_Position.y = m_DrawPosition.y + Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition().y;
				m_OnGround = true;
				m_Vector.y = 0.0f; //落下速度リセット
			}
			else if (m_DrawPosition.y > boxPos.y	//ボックスの下にいる場合
				&& m_DrawPosition.x - m_Scale.x > boxPos.x + boxScale.x * 0.5f && m_DrawPosition.x + m_Scale.x < boxPos.x - boxScale.x * 0.5f)
			{
				m_DrawPosition.y = boxPos.y + boxScale.y * 0.5f + m_Scale.y * 0.5f; //位置をボックスの下に調整
				m_Position.y = m_DrawPosition.y + Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition().y;
				m_Vector.y = 0.0f; //落下速度リセット
			}
			else if (m_DrawPosition.x < boxPos.x	//ボックスの左にいる場合
				&& m_DrawPosition.y + m_Scale.y > boxPos.y - boxScale.y * 0.5f && m_DrawPosition.y - m_Scale.y < boxPos.y + boxScale.y * 0.5f)
			{
				m_DrawPosition.x = boxPos.x - boxScale.x * 0.5f - m_Scale.x * 0.5f; //位置をボックスの左に調整
				m_Position.x = m_DrawPosition.x + Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition().x;
				m_Random = 1;

				m_HitSideBoxPos.push_back(boxPos); //当たっている箱の位置を保存
			}
			else if (m_DrawPosition.x > boxPos.x	//ボックスの右にいる場合
				&& m_DrawPosition.y + m_Scale .y > boxPos.y - boxScale.y * 0.5f && m_DrawPosition.y - m_Scale.y < boxPos.y + boxScale.y * 0.5f)
			{
				m_DrawPosition.x = boxPos.x + boxScale.x * 0.5f + m_Scale.x * 0.5f; //位置をボックスの右に調整
				m_Position.x = m_DrawPosition.x + Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition().x;
				m_Random = 0;

				m_HitSideBoxPos.push_back(boxPos); //当たっている箱の位置を保存
			}
		}
	}

};

