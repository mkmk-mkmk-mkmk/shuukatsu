
#pragma once

#include "gameObject.h"
#include "title_SpringChain.h"
#include "title_SpringBoard.h"

class Title_Spring : public GameObject
{
private:

	Title_SpringChain leftChain;
	Title_SpringChain rightChain;

	Title_SpringBoard board;

public:

	void Init(Vector2 topLeftPos, Vector2 bottomLeftPos, Vector2 topRightPos, Vector2 bottomRightPos,
		float chainSplit, float chainSplitRight, float chainWidth, float boardHeight, int gameObjectNumber);
	void Uninit();
	void Update();
	void Draw();

};

/*作り方の流れ（最終的な形）
①端の４点と、チェーンの数（分割数）、チェーンの太さ、底辺の板のサイズを指定することで生成
②左側の２点と右側の２点でそれぞれ一つ一つのチェーンのつなぎ目となる部分の座標を取得（リスト作成）
③底辺の部分にもチェーンの要領で板を作る
④物理計算をして座標の更新をできるようにして描画

・物理計算について
chainTopPosの方を支点としてbottomの方のみに重力やVectorを適用させる
listのひとつ前のbottomを参照してその位置にtopを持ってくる。
一つ目の場合はtopPosで固定させる

・落ちてガシャンってなるアニメーションを作る場合
全ての点の位置を上にもっていかなきゃダメそう（それ用の関数ならまぁ楽に作れそう）
 ->全部真上にもっていくと多分なんか質素になるので、rotateを範囲内で乱数とってその回転分位置を補正できれば
 　めっちゃいい感じになるとは思う。多分まぁかなり大変

もしくは、作る形的にbottomの方が上にして作ればいい感じになるかもしれない
y軸もずらすことで質素にならないかも。rotateとかの補正は必要だけれども


・プレイヤーの当たり判定をとってその計算をしたい場合
プレイヤーのVectorをとってプレイヤーのVectorに減衰を入れつつspring自体にもVectorを加算すればいいが、
springの各点にVectorを与える際にどうするか
*/