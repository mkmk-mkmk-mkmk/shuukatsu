#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"
#include "title.h"
#include "gameOver.h"
#include "game.h"
#include "backGround.h"
#include "map.h"
#include "box.h"
#include "camera.h"
#include "player.h"

void GameOver::Init()
{
	AddGameObject<BackGround>(0)->Init(3);

	//ÉJÅ[É\ÉãÇÃéÊìæ
	AddGameObject<Cursor>(1)->Init();
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