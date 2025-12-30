#pragma once

#include "random.h"
#include "vector2.h"

struct Particle
{
	Vector2 pos;
	float scale;
	Vector2 vec;
};

class ParticleTouch : public Sprite
{
private:

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