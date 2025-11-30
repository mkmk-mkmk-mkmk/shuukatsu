#pragma once

#include "Vector2.h"
#include "framework.h"

class GameObject : public Vector2
{
protected:

	int m_Frame = 0;

	bool m_Destroy = false;

	Vector2 m_Position{ 0.0f,0.0f };//位置
	Vector2 m_Rotation{ 0.0f,0.0f };//回転
	Vector2 m_Scale{ 1.0f,1.0f };	//スケール
	Vector2 m_Vector{ 0.0f,0.0f };	//移動ベクトル

	bool m_OnGround = false;	//地面に接地しているかどうか

	void BoxCollisionCommonStep(Vector2 boxPos, Vector2 boxScale)
	{

	}

	virtual void BoxCollisionExtraStep(Vector2 boxPos, Vector2 boxScale) {} //派生先で追加の処理を行う場合に使用

public:
	float m_Gravity = 0.3f;

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
	Vector2 GetRotation() { return m_Rotation; }
	Vector2 GetScale(){ return m_Scale; }

	void SetPosition(Vector2 Position) { m_Position = Position; }
	void SetRotation(Vector2 Rotation) { m_Position = Rotation; }
	void SetScale(Vector2 Scale) { m_Scale = Scale; }

	//距離取得
	float GetDistance(Vector2 Position)
	{
		Vector2 direction = m_Position - Position;
		return direction.length();
	}

	//ボックス当たり判定
	void BoxCollision(Vector2 boxPos, Vector2 boxScale)
	{
		BoxCollisionCommonStep(boxPos, boxScale);
		BoxCollisionExtraStep(boxPos, boxScale);
	}

};