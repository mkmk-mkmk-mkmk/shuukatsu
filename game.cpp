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


void Game::Init()
{
	//追加する順番に注意
	//Player→Camera→Boxは固定(間に何か入っても大丈夫)
	//ゲームオブジェクトを追加するときはScene.cppとScene.hも編集すること

	AddGameObject<Map>(0)->Init(); //マップの追加

	AddGameObject<Player>(1)->Init(); //プレイヤーの追加
	AddGameObject<Enemy>(1)->Init();  //敵キャラの追加
	AddGameObject<Camera>(2)->Init(); //カメラの追加

	//箱の追加
	int m_BoxCount = Manager::GetScene()->GetGameObject<Map>()->m_BoxPosList.size(); //箱の数を保存しておく

	for (int i = 0; i < m_BoxCount; i++)
	{
		AddGameObject<Box>(3)->Init();
		Manager::GetScene()->GetGameObject<Map>()->m_BoxPosList.pop_front();
	}

	//透明箱の追加
	int m_TransparentBoxCount = Manager::GetScene()->GetGameObject<Map>()->m_TransparentBoxPosList.size(); //透明箱の数を保存しておく

	for (int i = 0; i < m_TransparentBoxCount; i++)
	{
		AddGameObject<TransparentBox>(4)->Init();
		Manager::GetScene()->GetGameObject<Map>()->m_TransparentBoxPosList.pop_front();
	}

	AddGameObject<Cursor>(5)->Init(); //カーソルの追加

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