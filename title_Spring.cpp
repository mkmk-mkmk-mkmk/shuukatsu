#include "main.h"
#include "scene.h"
#include "manager.h"

#include "title_Spring.h"

void Title_Spring::Init(Vector2 topLeftPos, Vector2 topRightPos, Vector2 bottomLeftPos, Vector2 bottomRightPos,
	float chainSplitLeft, float chainSplitRight, float chainWidth, float boardHeight, int gameObjectNumber)
{
	//チェーンオブジェクト作成
	//左側
	leftChain_Title.Init(topLeftPos, bottomLeftPos, chainSplitLeft, chainWidth);

	//右側
	rightChain_Title.Init(topRightPos, bottomRightPos, chainSplitRight, chainWidth);

	//ボードオブジェクト作成
	board.Init(&leftChain_Title.GetEndPoint(),&rightChain_Title.GetEndPoint(),boardHeight);
}

void Title_Spring::Uninit()
{
	leftChain_Title.Uninit();
	rightChain_Title.Uninit();
}

void Title_Spring::Update()
{
	leftChain_Title.Update();
	rightChain_Title.Update();

	board.Update();
}

void Title_Spring::Draw()
{
	leftChain_Title.Draw();
	rightChain_Title.Draw();

	board.Draw();
}
