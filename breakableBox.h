#pragma once
#include "gameObject.h"
#include "enemy_Ground.h"
#include "boxBreakEffect.h"

class BreakableBox : public GameObject
{
private:

	Vector2  m_FirstPos; //ボックス初期座標

	bool m_HitPlayerMoveTrail;	//プレイヤーの軌跡移動中に当たっているか

	bool m_Breaking = false;	//破壊時のエフェクト発生中

	std::vector<std::unique_ptr<BoxBreakEffect>> m_BoxBreakEffectList;

public:
	void Init();
	void Uninit();
	void Update(const std::list<Enemy_Ground*>& enemies);
	void Draw();

};