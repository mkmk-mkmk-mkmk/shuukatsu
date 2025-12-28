#pragma once

#include "gameObject.h"
#include "boxBreakEffect.h"

class Title_BreakableBox : public GameObject
{
private:

	ID3D11ShaderResourceView* m_Texture;

	Vector2 m_CursorPos;	//カーソルの位置
	Vector2 m_CursorScale;	//カーソルの大きさ

	int m_LeftorRight;	 //生成場所が左側か右側か

	//float m_BreakHeight; //自動的に壊れる高さ

	bool m_HitCursor = false;	//カーソルに当たったかどうか

	bool m_Breaking = false;	//破壊時のエフェクト発生中

	std::vector<std::unique_ptr<BoxBreakEffect>> m_BoxBreakEffectList;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool GetHitCursor() { return m_HitCursor; }
};