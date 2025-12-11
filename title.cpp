#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "game.h"
#include "backGround.h"
#include "map.h"
#include "box.h"
#include "camera.h"
#include "player.h"

void Title::Init()
{
	AddGameObject<BackGround>(0)->Init(0);
	
	AddGameObject<Map>(0)->Init(0);

	//プレイヤーの追加
	AddGameObject<Player>(1)->Init();

	//カメラの追加
	AddGameObject<Camera>(2)->Init();

	//箱の追加
	int m_BoxCount = Manager::GetScene()->GetGameObject<Map>()->m_BoxPosList.size(); //箱の数を保存しておく

	for (int i = 0; i < m_BoxCount; i++)
	{
		AddGameObject<Box>(3)->Init();
		Manager::GetScene()->GetGameObject<Map>()->m_BoxPosList.pop_front();
	}

	//カーソルの取得
	AddGameObject<Cursor>(4)->Init();
}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	Scene::Update();

	if (Input::GetKeyPress(VK_RETURN))
	{
		Manager::SetScene<Game>();
	}
}

void Title::Draw()
{
	Scene::Draw();
}