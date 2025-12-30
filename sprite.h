#pragma once

#include "renderer.h"
#include <memory>

class Sprite
{
protected:

	ID3D11Buffer*			m_VertexBuffer = nullptr;
	ID3D11VertexShader*		m_VertexShader = nullptr;
	ID3D11InputLayout*		m_VertexLayout = nullptr;
	ID3D11PixelShader*		m_PixelShader = nullptr;

	VERTEX_3D vertex[4];

	std::vector<ID3D11ShaderResourceView*> m_TextureList;

	int m_TextureType;

public:
	Sprite();

	void InitSprite();
	void UnInitSprite();
	void DrawSprite(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, int texNum, float alpha = 1.0f);
	void DrawSpriteAnim(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, int pattern, int cols, int rows, int texNum, float alpha);

	void SetVertexSprite();
};
//static int g_AnimPattern = 0;
//static int g_AnimFrame = 0;
//
//void InitSprite();
//void UnInitSprite();
//void UpdateSprite();
//void DrawSprite(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, float alpha = 1.0f);
//void DrawSpriteAnim(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, float alpha, int pattern, int cols, int rows);
//void DrawSprite(const ObjectManager& obj, bool EnableShake = true, TextureCoord Texcoord = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) });
//void DrawSpriteColor(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, XMFLOAT4 col, bool EnableShake = true, TextureCoord Texcoord = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) });
//void DrawSpriteColor(ObjectManager& obj, bool EnableShake = true, XMFLOAT2 ExtraDrawDiff = XMFLOAT2(0.0f, 0.0f), TextureCoord Texcoord = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) });
////void DrawSpriteBar(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, float alpha, float end);
//void DrawSpriteAnim(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, int AnimCols, int AnimRows, int AnimPattern, float alpha = 1.0f, bool EnableShake = true);
//void DrawSpriteAnim(ObjectManager& obj, int AnimCols, int AnimRows, int AnimPattern, XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f }, bool EnableShake = true);
//void SetVertexSprite();
