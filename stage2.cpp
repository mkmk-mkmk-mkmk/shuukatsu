#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "title.h"
#include "stage2.h"
#include "stageSelect.h"

#include "player.h"
#include "cursor.h"
#include "backGround.h"

#include "box.h"
#include "breakableBox.h"
#include "damageBox.h"
#include "untouchableBox.h"
#include "unvisualBox.h"

#include "goal.h"
#include "map.h"
#include "camera.h"
#include "enemy.h"

//#include "spring.h"

#include "UI_PlayerState.h"
#include "UI_PlayerLife.h"
#include "UI_TrailSecond.h"
#include "UI_EnemyState.h"


void Stage2::Init()
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
	AddGameObject<Map>(0)->Init(2);

	AddGameObject<BackGround>(0)->Init(1);

	//プレイヤーの追加
	AddGameObject<Player>(1)->Init();

	//カメラの追加
	AddGameObject<Camera>(2)->Init();

	////敵追加
	//int groundEnemyCount = GetGameObject<Map>()->m_GroundEnemyPosList.size();
	//for (int i = 0; i < groundEnemyCount; i++)
	//{
	//	//エネミーの位置、大きさ、タイプを入れて生成
	//	Vector2 enemyScale = { 100.0f, 100.0f };
	//	Vector2 enemyPos = { GetGameObject<Map>()->m_GroundEnemyPosList.front().x,
	//		GetGameObject<Map>()->m_GroundEnemyPosList.front().y - (MAPCHIP_HEIGHT + enemyScale.y * 0.5f) };
	//	m_EnemiesInitVal.push_back({ enemyPos, enemyScale, Ground });
	//	GetGameObject<Map>()->m_GroundEnemyPosList.pop_front();
	//}

	//int flyingEnemyCount = GetGameObject<Map>()->m_FlyingEnemyPosList.size();
	//for (int i = 0; i < flyingEnemyCount; i++)
	//{
	//	//エネミーの位置、大きさ、タイプを入れて生成
	//	Vector2 enemyScale = { 100.0f, 100.0f };
	//	Vector2 enemyPos = { GetGameObject<Map>()->m_FlyingEnemyPosList.front().x,
	//		GetGameObject<Map>()->m_FlyingEnemyPosList.front().y - (MAPCHIP_HEIGHT + enemyScale.y * 0.5f) };
	//	m_EnemiesInitVal.push_back({ enemyPos, enemyScale, Flying });
	//	GetGameObject<Map>()->m_FlyingEnemyPosList.pop_front();
	//}

	////追加した敵を生成
	//for (auto& enemies : m_EnemiesInitVal)
	//{
	//	//エネミーオブジェクト追加
	//	Enemy* enemy = AddGameObject<Enemy>(3);
	//	enemy->Init(enemies.pos, enemies.scale, enemies.type);

	//	//リストにも保存
	//	m_EnemyObjects.push_back(enemy);
	//}

	//箱の追加
	int m_BoxCount = GetGameObject<Map>()->m_BoxPosList.size(); //箱の数を保存しておく

	for (int i = 0; i < m_BoxCount; i++)
	{
		AddGameObject<Box>(4)->Init();
		GetGameObject<Map>()->m_BoxPosList.pop_front();
	}

	//破壊可能箱の追加
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

	//透明壁の追加
	int m_UnvisualBoxCount = GetGameObject<Map>()->m_UnvisualBoxPosList.size(); //透明壁の数を保存しておく
	for (int i = 0; i < m_UnvisualBoxCount; i++)
	{
		AddGameObject<UnVisualBox>(4)->Init();
		GetGameObject<Map>()->m_UnvisualBoxPosList.pop_front();
	}

	//ダメージブロックの追加
	int m_DamageBoxCount = GetGameObject<Map>()->m_DamageBoxPosList.size(); //ダメージブロックの数を保存しておく
	for (int i = 0; i < m_DamageBoxCount; i++)
	{
		AddGameObject<DamageBox>(4)->Init();
		GetGameObject<Map>()->m_DamageBoxPosList.pop_front();
	}

	//ゴールの追加
	int m_GoalCount = GetGameObject<Map>()->m_GoalPosList.size(); //ゴールの数を保存しておく

	for (int i = 0; i < m_GoalCount; i++)
	{
		AddGameObject<Goal>(0)->Init();
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

	////EnemyStateUIの追加
	//AddUIObject<UI_EnemyState>(0)->Init();
}

void Stage2::Uninit()
{
	m_Pause->Uninit();
	Scene::Uninit();
}

void Stage2::Update()
{
	//デバッグ用タイトルへ
	if (Input::GetKeyPress('i') || Input::GetKeyPress('I'))
	{
		Manager::SetScene<Title>();
	}

	//デバッグ用リスタート
	if (Input::GetKeyPress('p') || Input::GetKeyPress('P'))
	{
		Manager::SetScene<Stage2>();
	}

	//デバッグ用ステージセレクトへ
	if (Input::GetKeyPress('o') || Input::GetKeyPress('O'))
	{
		Manager::SetScene<StageSelect>();
	}

	//if (HitStop())
	//{
	//	//ヒットストップ中はカメラのみ更新
	//	GetGameObject<Camera>()->Update();
	//	return;
	//}

	m_Pause->Update();
	if (m_Pause->GetSceneStop())
	{
		GetGameObject<Camera>()->Update();	//カメラのみ更新
		return;
	}

	Scene::Update();
}

void Stage2::Draw()
{
	Scene::Draw();
	if (m_Pause->GetSceneStop())
	{
		m_Pause->Draw();
	}
}