#include "main.h"
#include "UI_cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "gameOver.h"
#include "backGround.h"
#include "gameOver_Button.h"

void GameOver::Init()
{
	AddGameObject<BackGround>(0)->Init(3);

	//タイトルへ戻るボタン追加
	AddUIObject<GameOverButton>(0)->Init();

	//カーソルの取得
	AddUIObject<UI_Cursor>(1)->Init();
}

void GameOver::Uninit()
{
	Scene::Uninit();
}

void GameOver::Update()
{
	Scene::Update();
}

void GameOver::Draw()
{
	Scene::Draw();
}