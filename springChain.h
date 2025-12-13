#pragma once

class SpringChain : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	std::list<Vector2> m_ChainPointList;

public:

	void Init(Vector2 topPos, Vector2 bottomPos, float chainSplit, float chainWidth);
	void Uninit();
	void Update();
	void Draw();
	void DrawPiece();

};