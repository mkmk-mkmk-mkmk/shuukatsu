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


void Title::Init()
{
	AddGameObject<BackGround>(1)->Init();
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
}

void Title::Draw()
{
	Scene::Draw();
}