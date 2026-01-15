#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "rule.h"
#include "game.h"
#include "backGround.h"
#include "map.h"
#include "box.h"
#include "camera.h"
#include "player.h"

void Rule::Init()
{
	AddGameObject<BackGround>(0)->Init(0);

	//ÉJÅ[É\ÉãÇÃéÊìæ
	AddUIObject<Cursor>(1)->Init();
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