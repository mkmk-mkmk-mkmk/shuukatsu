
#include "main.h"
#include "texture.h"
#include "input.h"
#include "manager.h"

#include "particle_touch.h"


void ParticleTouch::Init(Vector2 firstPos)
{
	//“_‚ÌƒŠƒXƒg‰Šú‰»
	for (int i = 0; i < 7; i++)
	{
		Particle particle;
		particle.pos = firstPos;
		particle.scale = float(random.RandomInt(15, 25));
		particle.vec = Vector2(float(random.RandomInt(-2, 2)), float(random.RandomInt(-2, 2)));

		m_ParticleList.push_back(particle);
	}

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\clickParticle.png"));
}

void ParticleTouch::Update()
{
	for (int i = 0; i < m_ParticleList.size(); i++)
	{
		Particle& particle = m_ParticleList[i];
		particle.pos += particle.vec;
		particle.scale -= 0.6f;
		if (particle.scale < 0.0f)
		{
			m_ParticleList.erase(m_ParticleList.begin() + i);
			i--;
		}
	}

	if (m_ParticleList.empty())
	{
		m_Destroy = true;
	}
}

void ParticleTouch::Draw()
{
	for (int i = 0; i < m_ParticleList.size(); i++)
	{
		DrawParticle(i);
	}
}

void ParticleTouch::DrawParticle(int count)
{
	Particle particle = m_ParticleList[count];
	m_Position = particle.pos;

	DrawSprite(XMFLOAT2(particle.pos.x, particle.pos.y), 0.0f, XMFLOAT2(particle.scale, particle.scale), 0, 1.0f);

}