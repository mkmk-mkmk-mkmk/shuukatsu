#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "spring.h"
#include "map.h"
#include "player.h"
#include "camera.h"
#include "enemy.h"

void Spring::Init(Vector2 topLeftPos, Vector2 bottomLeftPos, Vector2 topRightPos, Vector2 bottomRightPos,
	float chainSplitLeft, float chainSplitRight, float chainWidth, float boardWidth, int gameObjectNumber)
{
	//チェーンオブジェクト作成
	//左側
	leftChain.Init(topLeftPos, bottomLeftPos, chainSplitLeft, chainWidth);

	//右側
	rightChain.Init(topRightPos, bottomRightPos, chainSplitRight, chainWidth);

	//ボードオブジェクト作成
	Manager::GetScene()->AddGameObject<SpringBoard>(gameObjectNumber)
		->Init(bottomLeftPos, bottomRightPos, boardWidth);
}

void Spring::Uninit()
{

}

void Spring::Update()
{

}

void Spring::Draw()
{

}
