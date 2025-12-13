#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "game.h"
#include "player.h"
#include "cursor.h"
#include "polygon.h"
#include "backGround.h"
#include "box.h"
#include "breakableBox.h"
#include "goal.h"
#include "map.h"
#include "camera.h"
#include "enemy.h"

#include "spring.h"

//エネミー用リスト
//初期データのみのリスト(Init用)
std::list<EnemyData> m_EnemiesInitVal;
//当たり判定のための、中身が更新されるリスト
std::list<Enemy*> m_EnemyObjects;


void Game::Init()
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

	//敵追加（変数は、pos、scale、enemyType。enemyTypeが0 : 地上の敵、1 : 飛んでいる敵）
	m_EnemiesInitVal.push_back({{0.0f, 0.0f}, {0.0f, 0.0f}, 0 });


	//追加した敵を生成
	for (auto& enemies : m_EnemiesInitVal)
	{
		//エネミーオブジェクト追加
		Enemy* enemy = AddGameObject<Enemy>(3);
		enemy->Init(enemies.pos, enemies.scale, enemies.enemyType);

		//リストにも保存
		m_EnemyObjects.push_back(enemy);
	}

	//箱の追加
	int m_BoxCount = Manager::GetScene()->GetGameObject<Map>()->m_BoxPosList.size(); //箱の数を保存しておく

	for (int i = 0; i < m_BoxCount; i++)
	{
		AddGameObject<Box>(4)->Init();
		GetGameObject<Map>()->m_BoxPosList.pop_front();
	}

	//透明箱の追加
	int m_BreakableBoxCount = Manager::GetScene()->GetGameObject<Map>()->m_BreakableBoxPosList.size(); //透明箱の数を保存しておく

	for (int i = 0; i < m_BreakableBoxCount; i++)
	{
		AddGameObject<BreakableBox>(4)->Init();
		GetGameObject<Map>()->m_BreakableBoxPosList.pop_front();
	}

	//ゴールの追加
	int m_GoalCount = Manager::GetScene()->GetGameObject<Map>()->m_GoalPosList.size(); //箱の数を保存しておく

	for (int i = 0; i < m_GoalCount; i++)
	{
		AddGameObject<Goal>(4)->Init();
		GetGameObject<Map>()->m_GoalPosList.pop_front();
	}


	//カーソルの取得
	AddGameObject<Cursor>(5)->Init();

}

void Game::Uninit()
{
	Scene::Uninit();
}

void Game::Update()
{

	Scene::Update();
}

void Game::Draw()
{
	Scene::Draw();
}