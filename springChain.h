#pragma once

struct ChainPoint
{
	Vector2 pos;
	Vector2 oldPos;
	Vector2 acceleration;
	bool lock;
};

class SpringChain : public GameObject
{
private:
	std::vector<ChainPoint> m_ChainPointList;

	int m_ChainPointCount;

	float m_ChainLength;

public:

	void Init(Vector2 topPos, Vector2 bottomPos, float chainSplit, float chainWidth);
	void Uninit();
	void Update();
	void Draw();
	void DrawPiece(int count);


	//チェーンの終端の点を取得
	ChainPoint& GetEndPoint()
	{
		return m_ChainPointList.back();
	}

};