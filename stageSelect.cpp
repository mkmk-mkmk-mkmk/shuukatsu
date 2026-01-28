#include "main.h"
#include "UI_Cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "backGround.h"

#include "stageSelect.h"
#include "stageSelect_Button.h"

void StageSelect::Init()
{
	AddGameObject<BackGround>(0)->Init(0);

	//ステージ選択ボタン
	AddUIObject<StageSelectButton>(0)->Init();

	//カーソルの取得
	AddUIObject<UI_Cursor>(1)->Init();
}

void StageSelect::Uninit()
{
	Scene::Uninit();
}

void StageSelect::Update()
{
	Scene::Update();
}

void StageSelect::Draw()
{
	Scene::Draw();
}