#include "main.h"
#include "UI_Cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "backGround.h"

#include "stageClear.h"
#include "stageClear_Button.h"

void StageClear::Init()
{
	AddGameObject<BackGround>(0)->Init(2);

	//ステージクリアボタンの追加
	AddUIObject<StageClearButton>(0)->Init();

	//カーソルの取得
	AddUIObject<UI_Cursor>(1)->Init();
}

void StageClear::Uninit()
{
	Scene::Uninit();
}

void StageClear::Update()
{
	Scene::Update();
}

void StageClear::Draw()
{
	Scene::Draw();
}