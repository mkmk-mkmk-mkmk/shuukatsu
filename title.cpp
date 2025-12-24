#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "manager.h"

#include "title_Cursor.h"
#include "input.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "backGround.h"
#include "spring.h"
#include "title_BreakableBox.h"
#include "UI_StartButton.h"

void Title::Init()
{
	//背景の追加
	AddGameObject<BackGround>(0)->Init(0);
	

	//スプリングの追加
	AddGameObject<Spring>(2)->
		Init(Vector2((float)screenWidth * 2 / 7, -(float)screenHeight / 5 * 3),
			Vector2((float)screenWidth * 5 / 7, -(float)screenHeight / 5 * 3),
			Vector2((float)screenWidth * 2 / 7, 0.0f),
			Vector2((float)screenWidth * 5 / 7, 0.0f),
			10.0f, 10.0f, screenHeight / 35, screenHeight * 2 / 5, 3);


	AddUIObject<UI_StartButton>(0)->Init();

	//カーソルの取得
	AddUIObject<Title_Cursor>(1)->Init();
}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	m_Frame++;
	if (m_Frame > 20)
	{
		//落ちてくる箱の追加
		AddGameObject<Title_BreakableBox>(1)->Init();

		m_Frame = 0;
	}

	Scene::Update();
}

void Title::Draw()
{
	Scene::Draw();
}