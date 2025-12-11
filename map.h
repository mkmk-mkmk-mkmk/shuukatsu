#pragma once
#include "gameObject.h"
#include <list>


//マップチップのサイズ（ここは指定）
#define MAPCHIP_WIDTH (50)
#define MAPCHIP_HEIGHT (50)


class Map : public GameObject
{
private:
	//ステージ縦横のブロック数
	int STAGEBLOCK_WIDTH;
	int STAGEBLOCK_HEIGHT;

	bool m_MapWidthGetFinish = false; //マップ横幅取得完了フラグ
	bool m_MapTextLineGetFinish = false; //マップテキスト行取得完了フラグ
	bool m_MapGetFinish = false; //マップ情報取得完了フラグ

public:
	void Init(int maptype);
	void Uninit();
	void Update();
	void Draw();


	std::list<Vector2> m_BoxPosList;			//箱の位置リスト
	std::list<Vector2> m_BreakableBoxPosList;	//壊せる箱の位置リスト
	std::list<Vector2> m_GoalPosList;			//ゴールの位置リスト

	int GetBlockWidth() const { return STAGEBLOCK_WIDTH; }
	int GetBlockHeight() const { return STAGEBLOCK_HEIGHT; }
};