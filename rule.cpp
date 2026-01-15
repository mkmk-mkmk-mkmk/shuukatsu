#include "main.h"
#include "UI_Cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "rule.h"
#include "backGround.h"

void Rule::Init()
{
	AddGameObject<BackGround>(0)->Init(0);

	//ƒJ[ƒ\ƒ‹‚Ìæ“¾
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