#include "main.h"
#include "UI_cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "gameOver.h"
#include "backGround.h"

void GameOver::Init()
{
	AddGameObject<BackGround>(0)->Init(3);

	//ƒJ[ƒ\ƒ‹‚Ìæ“¾
	AddUIObject<UI_Cursor>(1)->Init();
}

void GameOver::Uninit()
{
	Scene::Uninit();
}

void GameOver::Update()
{
	if (Input::GetKeyPress(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}
	Scene::Update();
}

void GameOver::Draw()
{
	Scene::Draw();
}