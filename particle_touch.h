#pragma once

#include "random.h"
#include "vector2.h"

struct Particle
{
	Vector2 pos;
	float scale;
	Vector2 vec;
};

class ParticleTouch
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	Random random;

	std::vector<Particle> m_ParticleList;

	Vector2 m_Position;
	float   m_Scale = 30.0f;

	bool	m_Destroy = false;

public:
	void Init(Vector2 firstPos);
	void Update();
	void Draw();
	void DrawParticle(int count);

	bool GetDestroy() { return m_Destroy; }
};