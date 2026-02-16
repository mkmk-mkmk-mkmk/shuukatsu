#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "title.h"
#include "stage1.h"
#include "stageSelect.h"

#include "player.h"
#include "cursor.h"
#include "backGround.h"
#include "box.h"
#include "breakableBox.h"
#include "untouchableBox.h"
#include "goal.h"
#include "map.h"
#include "camera.h"
#include "enemy.h"

#include "spring.h"

#include "UI_PlayerState.h"
#include "UI_PlayerLife.h"
#include "UI_TrailSecond.h"


void Stage1::Init()
{
	//ポーズ状態を初期化
	m_Pause->Init();

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

	//AddGameObject<Spring>(0)->Init(Vector2(MAPCHIP_WIDTH * 10, MAPCHIP_HEIGHT * 10),
	//	Vector2(MAPCHIP_WIDTH * 13, MAPCHIP_HEIGHT * 10),
	//	Vector2(MAPCHIP_WIDTH * 10, MAPCHIP_HEIGHT * 13),
	//	Vector2(MAPCHIP_WIDTH * 13, MAPCHIP_HEIGHT * 13),
	//	3.0f, 3.0f, MAPCHIP_WIDTH, MAPCHIP_HEIGHT, 0);

	//プレイヤーの追加
	AddGameObject<Player>(1)->Init();

	//カメラの追加
	AddGameObject<Camera>(2)->Init();

	//敵追加
	int groundEnemyCount = GetGameObject<Map>()->m_GroundEnemyPosList.size();
	for (int i = 0; i < groundEnemyCount; i++)
	{
		//エネミーの位置、大きさ、タイプを入れて生成
		Vector2 enemyScale = { 100.0f, 100.0f };
		Vector2 enemyPos = { GetGameObject<Map>()->m_GroundEnemyPosList.front().x,
			GetGameObject<Map>()->m_GroundEnemyPosList.front().y  - (MAPCHIP_HEIGHT + enemyScale.y * 0.5f)};
		m_EnemiesInitVal.push_back({ enemyPos, enemyScale, Ground });
		GetGameObject<Map>()->m_GroundEnemyPosList.pop_front();
	}


	//追加した敵を生成
	for (auto& enemies : m_EnemiesInitVal)
	{
		//エネミーオブジェクト追加
		Enemy* enemy = AddGameObject<Enemy>(3);
		enemy->Init(enemies.pos, enemies.scale, enemies.type);

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
	int m_GoalCount = GetGameObject<Map>()->m_GoalPosList.size(); //ゴールの数を保存しておく

	for (int i = 0; i < m_GoalCount; i++)
	{
		AddGameObject<Goal>(4)->Init();
		GetGameObject<Map>()->m_GoalPosList.pop_front();
	}


	//カーソルの取得
	AddUIObject<Cursor>(1)->Init();

	//PlayerStateUIの追加
	AddUIObject<UI_PlayerState>(2)->Init();

	//PlayerLifeUIの追加
	AddUIObject<UI_PlayerLife>(2)->Init();

	//TrailSecondUIの追加
	AddUIObject<UI_TrailSecond>(2)->Init();

}

void Stage1::Uninit()
{
	m_Pause->Uninit();
	Scene::Uninit();
}

void Stage1::Update()
{
	//デバッグ用タイトルへ
	if (Input::GetKeyPress('i') || Input::GetKeyPress('I'))
	{
		Manager::SetScene<Title>();
	}

	//デバッグ用リスタート
	if (Input::GetKeyPress('p') || Input::GetKeyPress('P'))
	{
		Manager::SetScene<Stage1>();
	}

	//デバッグ用ステージセレクトへ
	if (Input::GetKeyPress('o') || Input::GetKeyPress('O'))
	{
		Manager::SetScene<StageSelect>();
	}

	m_Pause->Update();
	if (m_Pause->GetSceneStop())
	{
		GetGameObject<Camera>()->Update();	//カメラのみ更新
	}
	else
	{
		Scene::Update();
	}
}

void Stage1::Draw()
{
	Scene::Draw();
	if (m_Pause->GetSceneStop())
	{
		m_Pause->Draw();
	}

}