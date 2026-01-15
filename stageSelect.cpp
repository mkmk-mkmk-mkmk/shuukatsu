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

#include "stageSelect.h"

void StageSelect::Init()
{
	AddGameObject<BackGround>(0)->Init(2);

	//ÉJÅ[É\ÉãÇÃéÊìæ
	AddUIObject<Cursor>(0)->Init();
}

void StageSelect::Uninit()
{
	Scene::Uninit();
}

void StageSelect::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}
}

void StageSelect::Draw()
{
	Scene::Draw();
}