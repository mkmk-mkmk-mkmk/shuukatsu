#pragma once
#include "gameObject.h"
#include "BehaviorTree/behaviorNode.h"
#include "BehaviorTree/compositeNode.h"

#include <list>
#include "random.h"

enum EnemyType
{
	Ground,
	Flying
};

//敵初期データ構造体
struct EnemyData
{
	Vector2 pos;
	Vector2 scale;
	EnemyType type;
};

//アニメーション状態
enum class AnimationState
{
	Patrol,		//徘徊
	Chase,		//追跡
	Attack,		//攻撃

	FindPlayer, //発見
	LookAround	//見回し
};

class Enemy : public GameObject
{
protected:

	EnemyType m_EnemyType;

	CompositeNode* m_RootNode = nullptr;

	AnimationState m_AnimationState;
	AnimationState m_OldAnimationState;

	Vector2 m_FirstPosition;		//初期座標

	Vector2 m_PlayerPos;			//プレイヤー座標(追跡用)
	Vector2 m_PlayerScale;			//プレイヤースケール
	std::list<Vector2> m_HitSideBoxPos; //左右で当たっている箱のリスト

	float m_Speed;			//移動速度
	Vector2 m_VisibleRange;	//発見範囲
	Vector2 m_AttackRange;	//攻撃範囲

	int m_MoveCount = 0;    //停止せずに移動した数
	int m_MoveCountMax = 3; //停止せずに移動する最大数

	int m_StopCount = 0;    //停止した数
	int m_StopCountMax = 2; //停止する最大数

	bool m_DrawHitBox = false;
	Vector2 m_AttackHitBoxPos = { 0.0f, 0.0f };	//攻撃判定の位置（デバッグ用）
	Vector2 m_AttackHitBoxDrawPos = { 0.0f, 0.0f };	//攻撃判定の描画位置（デバッグ用）

	bool m_VisiblePlayer = false;	//プレイヤーが見えているか
	bool m_InAttackRange = false;	//攻撃範囲内か
	bool m_AttackCoolTime = false;	//攻撃のクールタイム中か
	bool m_Attacking = false;		//攻撃中か

	bool m_AttackAnimationStarted = false;		//攻撃アニメーション開始したか
	bool m_AttackAnimationFinished = false;		//攻撃アニメーションが終了したか
	bool m_ChaseAnimationStarted = false;		//追跡アニメーション開始したか
	bool m_ChaseAnimationFinished = false;		//追跡アニメーションが終了したか
	bool m_PatrolAnimationStarted = false;		//徘徊アニメーション開始したか
	bool m_PatrolAnimationFinished = false;		//徘徊アニメーションが終了したか

	bool m_StopTick = false;		//tickを止める（攻撃などのアニメーション再生中に使用）
	bool m_HitOnce = false;			//自分の攻撃が多段ヒットしないように
	bool m_HitAttack = false;		//自分の攻撃が当たったか

	bool m_HitPlayerAttackOnce = false;	//プレイヤーの攻撃が多段ヒットしないように
	bool m_HitPlayerAttack = false;		//プレイヤーの攻撃が当たったか

	int m_EnemyInvincibleFrame = 20;	//無敵時間用フレームカウント
	bool m_EnemyInvincible = false;		//無敵状態か（当たり判定があるか）

	int m_RandomInt;		//乱数用
	float m_RandomFloat;	//乱数用

	//地上エネミー用
	float m_JumpPower;		//ジャンプ力
	bool m_JumpStairs;		//段差をジャンプするか

	int m_PatrolFrame = 150;	//徘徊状態のフレーム数
	int m_AttackFrame = 120;	//攻撃状態のフレーム数

public:

	void Init(Vector2 pos, Vector2 scale, EnemyType enemyType);
	void Uninit();
	void Update();
	void Draw();

	void DrawAttackHitBox(); //敵の当たり判定（デバッグ用）

	NodeStatus Patrol();
	NodeStatus Chase();
	NodeStatus Attack();
	NodeStatus OnlyAnimation(); //アニメーションのみ再生

	void UpdatePatrol();
	void UpdateChase();
	void UpdateAttack();
	void UpdateFind();
	void UpdateLookAround();

	AnimationState GetEnemyAnimState() { return m_AnimationState; }

	void DeleteAnimation() override;

	void BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale) override;

	void CheckStairs();

};

