#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "game.h"
#include "player.h"
#include "cursor.h"
#include "polygon.h"
#include "box.h"
#include "transparentbox.h"
#include "map.h"
#include "camera.h"
#include "enemy.h"

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
	//Player→Camera→Boxは固定(間に何か入っても大丈夫)
	//ゲームオブジェクトを追加するときはScene.cppとScene.hも編集すること

	//マップの追加
	AddGameObject<Map>(0)->Init();

	//プレイヤーの追加
	AddGameObject<Player>(1)->Init();

	//敵追加（変数は、pos、scale、enemyType。enemyTypeが0 : 地上の敵、1 : 飛んでいる敵）
	m_EnemiesInitVal.push_back({{0.0f, 0.0f}, {0.0f, 0.0f}, 0 });


	//追加した敵を生成
	for (auto& enemies : m_EnemiesInitVal)
	{
		//エネミーオブジェクト追加
		Enemy* enemy = AddGameObject<Enemy>(2);
		enemy->Init(enemies.pos, enemies.scale, enemies.enemyType);

		//リストにも保存
		m_EnemyObjects.push_back(enemy);
	}

	//カメラの追加
	AddGameObject<Camera>(3)->Init();

	//箱の追加
	int m_BoxCount = Manager::GetScene()->GetGameObject<Map>()->m_BoxPosList.size(); //箱の数を保存しておく

	for (int i = 0; i < m_BoxCount; i++)
	{
		AddGameObject<Box>(4)->Init();
		Manager::GetScene()->GetGameObject<Map>()->m_BoxPosList.pop_front();
	}

	//透明箱の追加
	int m_TransparentBoxCount = Manager::GetScene()->GetGameObject<Map>()->m_TransparentBoxPosList.size(); //透明箱の数を保存しておく

	for (int i = 0; i < m_TransparentBoxCount; i++)
	{
		AddGameObject<TransparentBox>(5)->Init();
		Manager::GetScene()->GetGameObject<Map>()->m_TransparentBoxPosList.pop_front();
	}

	//カーソルの追加
	AddGameObject<Cursor>(6)->Init();

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