#pragma once

#include "framework.h"
#include "random.h"
#include "Vector2.h"
#include <vector>

struct EffectData
{
	Vector2 pos;
	Vector2 drawPos;
	float	scale;
	float	rotate;
	Vector2 vec;
	int effectType = 0;
};

class Effect
{
protected:

	float m_Gravity = 0.3f;

	bool m_Destroy = false;

	Vector2 m_DrawDiff;		//描画する際の補正用

	Vector2 m_Position{ 0.0f,0.0f };		//ワールド座標
	Vector2 m_DrawPosition{ 0.0f, 0.0f };	//描画座標
	Vector2 m_Vector{ 0.0f,0.0f };			//移動ベクトル
	float	m_Rotate = 0.0f;				//回転
	Vector2 m_Scale{ 1.0f,1.0f };			//スケール
	int		m_Layer = 5;					//レイヤー

	Random random;

	std::vector<EffectData> m_EffectDataList;

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture[5];

private :

public:
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	bool GetDestroy() { return m_Destroy; }
	void SetDestroy()
	{ 
		m_Destroy = true;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	//Vector2 GetPosition() { return m_Position; }
	//Vector2 GetDrawPosition() { return m_DrawPosition; }
	//float GetRotate() { return m_Rotate; }
	//Vector2 GetScale() { return m_Scale; }

	//void SetPosition(Vector2 position) { m_Position = position; }
	//void SetDrawPosition(Vector2 drawPosition) { m_DrawPosition = drawPosition; }
	//void SetRotate(float rotate) { m_Rotate = rotate; }
	//void SetScale(Vector2 scale) { m_Scale = scale; }

};