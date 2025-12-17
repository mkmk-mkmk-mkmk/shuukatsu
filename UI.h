#pragma once

#include "framework.h"

class UI : public Collision
{
protected:

	bool m_Destroy = false;

	Vector2 m_Position{ 0.0f,0.0f };		//ワールド座標
	Vector2 m_DrawPosition{ 0.0f, 0.0f };	//描画座標
	float	m_Rotate = 0.0f;				//回転
	Vector2 m_Scale{ 1.0f,1.0f };			//スケール
	int		m_Layer = 5;					//レイヤー

	bool m_Visible = true;			//表示・非表示
	bool m_Button = false;			//ボタンかどうか
	bool m_MoveWithCamera = false;	//カメラと一緒に動くかどうか

public:
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	void SetDestroy() { m_Destroy = true; }

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

	Vector2 GetPosition() { return m_Position; }
	Vector2 GetDrawPosition() { return m_DrawPosition; }
	float GetRotate() { return m_Rotate; }
	Vector2 GetScale() { return m_Scale; }

	void SetPosition(Vector2 position) { m_Position = position; }
	void SetDrawPosition(Vector2 drawPosition) { m_DrawPosition = drawPosition; }
	void SetRotate(float rotate) { m_Rotate = rotate; }
	void SetScale(Vector2 scale) { m_Scale = scale; }

};