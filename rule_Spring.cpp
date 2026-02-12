
#include "rule_Spring.h"

void Rule_Spring::Init(Vector2 topLeftPos, Vector2 topRightPos, Vector2 bottomLeftPos, Vector2 bottomRightPos,
	float chainSplitLeft, float chainSplitRight, float chainWidth, float boardHeight, int gameObjectNumber)
{
	//チェーンオブジェクト作成
	//左側
	leftChain_Rule.Init(topLeftPos, bottomLeftPos, chainSplitLeft, chainWidth);

	//右側
	rightChain_Rule.Init(topRightPos, bottomRightPos, chainSplitRight, chainWidth);

	//ボードオブジェクト作成
	board.Init(&leftChain_Rule.GetEndPoint(), &rightChain_Rule.GetEndPoint(), boardHeight);
}

void Rule_Spring::Uninit()
{
	leftChain_Rule.Uninit();
	rightChain_Rule.Uninit();
}

void Rule_Spring::Update()
{
	leftChain_Rule.Update();
	rightChain_Rule.Update();

	board.Update();
}

void Rule_Spring::Draw()
{
	leftChain_Rule.Draw();
	rightChain_Rule.Draw();

	board.Draw();
}
