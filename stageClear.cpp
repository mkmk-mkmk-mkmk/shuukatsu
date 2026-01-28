#include "main.h"
#include "UI_Cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "backGround.h"

#include "stageClear.h"

void StageClear::Init()
{
	AddGameObject<BackGround>(0)->Init(2);

	//ƒJ[ƒ\ƒ‹‚Ìæ“¾
	AddUIObject<UI_Cursor>(0)->Init();
}

void StageClear::Uninit()
{
	Scene::Uninit();
}

void StageClear::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}
}

void StageClear::Draw()
{
	Scene::Draw();
}