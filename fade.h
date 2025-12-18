#pragma once

#include "Vector2.h"
#include "framework.h"

class Fade
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	float m_FadeSpeed = 0.02f;
	float m_Alpha = 0.0f;
	bool m_Fade = false;
	bool m_FadeInFinished = false;
	bool m_FadeOutFinished = false;

public:
	void Init();
	void FadeIn();
	void FadeOut();
	void Uninit();
	void Draw();

	bool GetFade() { return m_Fade; }
	void SetFade(bool fade)
	{
		m_Fade = fade;
	}

	bool GetFadeInFinished() { return m_FadeInFinished; }
	void SetFadeInFinished(bool finish)
	{
		m_FadeInFinished = finish;
	}

	bool GetFadeOutFinished() { return m_FadeOutFinished; }
	void SetFadeOutFinished(bool finish)
	{
		m_FadeOutFinished = finish;
	}

};