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

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	int m_ChainPieceCount;
	std::vector<ChainPoint> m_ChainPointList;

	float m_ChainLength;

public:

	void Init(Vector2 topPos, Vector2 bottomPos, float chainSplit, float chainWidth);
	void Uninit();
	void Update();
	void Draw();
	void DrawPiece(int count);

};