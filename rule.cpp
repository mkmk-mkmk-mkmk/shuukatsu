#include "main.h"
#include "UI_Cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "rule.h"
#include "backGround.h"
#include "rule_Spring.h"
#include "rule_Button.h"

void Rule::Init()
{
	AddGameObject<BackGround>(0)->Init(0);

	//スプリングの追加
	AddGameObject<Rule_Spring>(2)->
		Init(Vector2((float)screenWidth / 9, 0.0f),
			Vector2((float)screenWidth * 8 / 9, 0.0f),
			Vector2((float)screenWidth / 9, (float)screenHeight * 3 / 7),
			Vector2((float)screenWidth * 8 / 9, (float)screenHeight * 3 / 7),
			5.0f, 5.0f, screenHeight / 35, screenHeight * 5 / 7, 2);

	//ボタンの追加
	AddUIObject<RuleButton>(0)->Init();

	//カーソルの取得
	AddUIObject<UI_Cursor>(1)->Init();
}

void Rule::Uninit()
{
	Scene::Uninit();
}

void Rule::Update()
{
	if (Input::GetKeyPress(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}
	Scene::Update();
}

void Rule::Draw()
{
	Scene::Draw();
}