
#include "rule_Spring.h"

void Rule_Spring::Init(Vector2 topLeftPos, Vector2 topRightPos, Vector2 bottomLeftPos, Vector2 bottomRightPos,
	float chainSplitLeft, float chainSplitRight, float chainWidth, float boardHeight, int gameObjectNumber)
{
	//チェーンオブジェクト作成
	//左側
	leftChain.Init(topLeftPos, bottomLeftPos, chainSplitLeft, chainWidth);

	//右側
	rightChain.Init(topRightPos, bottomRightPos, chainSplitRight, chainWidth);

	//ボードオブジェクト作成
	board.Init(&leftChain.GetEndPoint(), &rightChain.GetEndPoint(), boardHeight);
}

void Rule_Spring::Uninit()
{
	leftChain.Uninit();
	rightChain.Uninit();
}

void Rule_Spring::Update()
{
	leftChain.Update();
	rightChain.Update();

	board.Update();
}

void Rule_Spring::Draw()
{
	leftChain.Draw();
	rightChain.Draw();

	board.Draw();
}
