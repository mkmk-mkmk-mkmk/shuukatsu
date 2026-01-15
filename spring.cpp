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

void Spring::Init(Vector2 topLeftPos, Vector2 topRightPos, Vector2 bottomLeftPos, Vector2 bottomRightPos,
	float chainSplitLeft, float chainSplitRight, float chainWidth, float boardHeight, int gameObjectNumber)
{
	//チェーンオブジェクト作成
	//左側
	leftChain.Init(topLeftPos, bottomLeftPos, chainSplitLeft, chainWidth);

	//右側
	rightChain.Init(topRightPos, bottomRightPos, chainSplitRight, chainWidth);

	//ボードオブジェクト作成
	board.Init(&leftChain.GetEndPoint(),&rightChain.GetEndPoint(),boardHeight);
}

void Spring::Uninit()
{
	leftChain.Uninit();
	rightChain.Uninit();
}

void Spring::Update()
{
	leftChain.Update();
	rightChain.Update();

	//board.Update();
}

void Spring::Draw()
{
	leftChain.Draw();
	rightChain.Draw();

	board.Draw();
}
