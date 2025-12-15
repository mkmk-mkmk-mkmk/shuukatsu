#pragma once
#include "gameObject.h"
#include "enemy.h"

class BreakableBox : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	Vector2  m_FirstPos; //ボックス初期座標

public:
	void Init();
	void Uninit();
	void Update(const std::list<Enemy*>& enemies);
	void Draw();

	void SetPosition(Vector2 position) { m_Position = position; }
	Vector2 GetPosition() { return m_Position; }

	void SetScale(Vector2 scale) { m_Scale = scale; }
	Vector2 GetScale() { return m_Scale; }

};