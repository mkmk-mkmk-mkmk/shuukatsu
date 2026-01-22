#pragma once
#include "enemy.h"

class Enemy_Ground : public Enemy
{
private:

	float m_JumpPower;		//ジャンプ力
	bool m_JumpStairs;		//段差をジャンプするか

public:

	void Init(Vector2 pos, Vector2 scale);
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

	void DeleteAnimation();

	void BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale);

	void CheckStairs();

};

