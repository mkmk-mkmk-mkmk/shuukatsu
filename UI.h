#pragma once

#include "Vector2.h"
#include "framework.h"

class UI : public Vector2
{
protected:

	int m_Frame = 0;
	bool m_Destroy = false;

	Vector2 m_Position{ 0.0f,0.0f };		//ワールド座標（判定などに使用）
	Vector2 m_DrawPosition{ 0.0f, 0.0f };	//描画座標（描画のみに使用）
	Vector2 m_Rotation{ 0.0f,0.0f };		//回転
	Vector2 m_Scale{ 1.0f,1.0f };			//スケール

	bool m_CanClick;						//クリック可能か（ボタンの役割を持つか）

public:

	virtual void Init(Vector2 position, Vector2 scale) {}
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
	Vector2 GetRotation() { return m_Rotation; }
	Vector2 GetScale() { return m_Scale; }

	void SetPosition(Vector2 Position) { m_Position = Position; }
	void SetDrawPosition(Vector2 drawPosition) { m_DrawPosition = drawPosition; }
	void SetRotation(Vector2 Rotation) { m_Position = Rotation; }
	void SetScale(Vector2 Scale) { m_Scale = Scale; }

};