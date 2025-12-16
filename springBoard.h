#pragma once

#include "springChain.h"

class SpringBoard : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	ChainPoint* m_LeftPoint;
	ChainPoint* m_RightPoint;

	Vector2 m_PlayerPos;
	Vector2 m_PlayerScale;
	Vector2 m_PlayerVector;

	float m_BoardLength;

public:

	void Init(ChainPoint* leftPoint, ChainPoint* rightPoint, float height);
	void Uninit();
	//void Update();
	void Draw();

};
