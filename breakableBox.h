#pragma once
#include "gameObject.h"
#include "enemy.h"
#include "boxBreakEffect.h"

class BreakableBox : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	Vector2  m_FirstPos; //ボックス初期座標

	bool m_HitPlayerMoveTrail;	//プレイヤーの軌跡移動中に当たっているか

	bool m_Breaking = false;	//破壊時のエフェクト発生中

	std::vector<std::unique_ptr<BoxBreakEffect>> m_BoxBreakEffectList;

public:
	void Init();
	void Uninit();
	void Update(const std::list<Enemy*>& enemies);
	void Draw();

};