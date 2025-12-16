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
#include "spring.h"

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

	//スプリングの追加
	AddGameObject<Spring>(4)->
		Init(Vector2((float)screenWidth / 6, -(float)screenHeight / 5 * 2),
			Vector2((float)screenWidth * 5 / 6, -(float)screenHeight / 5 * 2),
			Vector2((float)screenWidth / 6, (float)screenHeight / 5),
			Vector2((float)screenWidth * 5 / 6, (float)screenHeight / 5),
			5.0f, 5.0f, 50.0f, 70.0f, 3);

	//カーソルの取得
	AddGameObject<Cursor>(5)->Init();
}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	if (Input::GetKeyPress(VK_RETURN))
	{
		Manager::SetScene<Game>();
	}
	Scene::Update();
}

void Title::Draw()
{
	Scene::Draw();
}