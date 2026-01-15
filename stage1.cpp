#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "stage1.h"
#include "player.h"
#include "cursor.h"
#include "polygon.h"
#include "backGround.h"
#include "box.h"
#include "breakableBox.h"
#include "untouchableBox.h"
#include "goal.h"
#include "map.h"
#include "camera.h"
#include "enemy_Ground.h"

//#include "spring.h"

#include "UI_PlayerState.h"

//エネミー用リスト
//初期データのみのリスト(Init用)
std::list<EnemyData> m_EnemiesInitVal;
//当たり判定のための、中身が更新されるリスト
std::list<Enemy_Ground*> m_EnemyObjects;


void Stage1::Init()
{
	//リスタート時用にリストをクリアしておく
	m_EnemiesInitVal.clear();
	m_EnemyObjects.clear();


	//追加する順番に注意
	//Map->Player→Camera→Enemyなど
	//Mapでマップサイズ取得->それを参照してPlayer位置初期化
	// ->さらにそれを参照してCameraの位置初期化->さらにそれを参照してEnemyなどの描画位置取得
	//ゲームオブジェクトを追加するときはScene.cppとScene.hも編集すること

	//マップの追加
	AddGameObject<Map>(0)->Init(1);

	AddGameObject<BackGround>(0)->Init(1);

	//プレイヤーの追加
	AddGameObject<Player>(1)->Init();

	//カメラの追加
	AddGameObject<Camera>(2)->Init();

	//敵追加
	int enemyCount = GetGameObject<Map>()->m_EnemyPosList.size();
	for (int i = 0; i < enemyCount; i++)
	{
		//エネミーの位置、大きさ、タイプを入れて生成（大きさとタイプは後々いじれるように）
		Vector2 enemyScale = { 100.0f, 100.0f };
		Vector2 enemyPos = { GetGameObject<Map>()->m_EnemyPosList.front().x,
			GetGameObject<Map>()->m_EnemyPosList.front().y  - (MAPCHIP_HEIGHT + enemyScale.y * 0.5f)};
		m_EnemiesInitVal.push_back({ enemyPos, enemyScale });
		GetGameObject<Map>()->m_EnemyPosList.pop_front();
	}

	//追加した敵を生成
	for (auto& enemies : m_EnemiesInitVal)
	{
		//エネミーオブジェクト追加
		Enemy_Ground* enemy = AddGameObject<Enemy_Ground>(3);
		enemy->Init(enemies.pos, enemies.scale);

		//リストにも保存
		m_EnemyObjects.push_back(enemy);
	}

	//箱の追加
	int m_BoxCount = GetGameObject<Map>()->m_BoxPosList.size(); //箱の数を保存しておく

	for (int i = 0; i < m_BoxCount; i++)
	{
		AddGameObject<Box>(4)->Init();
		GetGameObject<Map>()->m_BoxPosList.pop_front();
	}

	//透明箱の追加
	int m_BreakableBoxCount = GetGameObject<Map>()->m_BreakableBoxPosList.size(); //透明箱の数を保存しておく

	for (int i = 0; i < m_BreakableBoxCount; i++)
	{
		AddGameObject<BreakableBox>(4)->Init();
		GetGameObject<Map>()->m_BreakableBoxPosList.pop_front();
	}

	//当たり判定のない箱の追加
	int m_UntouchableBoxCount = GetGameObject<Map>()->m_UntouchableBoxPosList.size(); //当たり判定のない箱の数を保存しておく
	for (int i = 0; i < m_UntouchableBoxCount; i++)
	{
		AddGameObject<UntouchableBox>(4)->Init();
		GetGameObject<Map>()->m_UntouchableBoxPosList.pop_front();
	}

	//ゴールの追加
	int m_GoalCount = GetGameObject<Map>()->m_GoalPosList.size(); //箱の数を保存しておく

	for (int i = 0; i < m_GoalCount; i++)
	{
		AddGameObject<Goal>(4)->Init();
		GetGameObject<Map>()->m_GoalPosList.pop_front();
	}


	//カーソルの取得
	AddUIObject<Cursor>(0)->Init();

	//UIの追加
	AddUIObject<UI_PlayerState>(1)->Init();
}

void Stage1::Uninit()
{
	Scene::Uninit();
}

void Stage1::Update()
{
	Scene::Update();
}

void Stage1::Draw()
{
	Scene::Draw();
}