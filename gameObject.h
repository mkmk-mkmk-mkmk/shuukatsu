#pragma once

#include "Vector2.h"
#include "collision.h"
#include "framework.h"
#include "random.h"
#include <list>

class GameObject : public Vector2, public Collision
{
protected:

	int m_Frame = 0;

	bool m_Destroy = false;

	int m_Life;

	Vector2 m_Position{ 0.0f,0.0f };		//ワールド座標（判定などに使用）
	Vector2 m_DrawPosition{ 0.0f, 0.0f };	//描画座標（描画のみに使用）
	Vector2 m_DrawDiff{ 0.0f,0.0f };		//描画する際の補正用
	Vector2 m_OldPosition{ 0.0f, 0.0f };	//1フレーム前のワールド座標
	float	m_Rotate = 0.0f;				//回転
	Vector2 m_Scale{ 1.0f,1.0f };			//スケール
	Vector2 m_Vector{ 0.0f,0.0f };			//移動ベクトル
	float   m_Weight = 1.0f;				//重さ
	int		m_Layer = 0;					//レイヤー
	
	bool m_OnGround = false;	//地面に接地しているかどうか
	bool m_Direction = true;	//体の向き true：右向き、false：左向き

	Vector2 m_AnimationSplit = { 1.0f, 1.0f };
	bool m_DeleteAnimationFinished = false;

	Random random;

public:
	float PI = 3.1415;

	float m_Gravity = 0.3f;

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	virtual void DeleteAnimation() {}

	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			this->DeleteAnimation();

			if (!m_DeleteAnimationFinished)
			{
				Uninit();
				delete this;
			}
			return m_DeleteAnimationFinished;
		}
		else
		{
			return false;
		}
	}

	Vector2 GetPosition() { return m_Position; }
	Vector2 GetDrawPosition() { return m_DrawPosition; }
	float GetRotate() { return m_Rotate; }
	Vector2 GetScale(){ return m_Scale; }
	Vector2 GetVector() { return m_Vector; }
	int GetLife() { return m_Life; }

	void SetPosition(Vector2 position) { m_Position = position; }
	void SetDrawPosition(Vector2 drawPosition) { m_DrawPosition = drawPosition; }
	void SetRotate(float rotate) { m_Rotate = rotate; }
	void SetScale(Vector2 scale) { m_Scale = scale; }
	void SetVector(Vector2 vector) { m_Vector = vector; }
	void AddLife(int add) { m_Life += add; }


	bool InRangeObject(Vector2 mainObjPos, Vector2 mainObjScale,
		Vector2 hitObjPos, Vector2 hitObjScale, Vector2 range, bool direction) 
		//directionがtrueなら右方向、falseなら左方向
	{
		//範囲内にいるかどうかの判定処理（主に）
		float dx = hitObjPos.x - mainObjPos.x;
		float dy = modulus(hitObjPos.y - mainObjPos.y);	//y軸のみ絶対値を使う

		if (dy > range.y)
		{
			return false;
		}

		if (direction) //右方向
		{
			if (dx <= range.x && dx >= 0)
			{
				return true;
			}
		}
		else //左方向
		{
			if (dx >= -range.x && dx <= 0)
			{
				return true;
			}
		}

		return false;
	}

};