#pragma once
#include "pause.h"

class Stage2 : public Scene
{
private:
	Pause* m_Pause = new Pause;

	//エネミー用リスト
	//初期データのみのリスト(Init用)
	std::list<EnemyData> m_EnemiesInitVal;
	//当たり判定のための、中身が更新されるリスト
	std::list<Enemy*> m_EnemyObjects;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};