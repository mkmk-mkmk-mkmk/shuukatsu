#pragma once
#include "gameObject.h"

class Goal : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	Vector2  m_FirstPos; //ボックス初期座標

	Vector2 m_Piece;
	Vector2 m_PieceNumber;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPiece(Vector2 piece) { m_Piece = piece; }
};