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
#include "map.h"
#include "box.h"
#include "camera.h"
#include "player.h"

#include "stageClear.h"

void StageClear::Init()
{
	AddGameObject<BackGround>(0)->Init(2);

	//ÉJÅ[É\ÉãÇÃéÊìæ
	AddGameObject<Cursor>(1)->Init();
}

void StageClear::Uninit()
{
	Scene::Uninit();
}

void StageClear::Update()
{
	Scene::Update();

	if (Input::GetKeyPress(VK_SHIFT))
	{
		Manager::SetScene<Title>();
	}
}

void StageClear::Draw()
{
	Scene::Draw();
}