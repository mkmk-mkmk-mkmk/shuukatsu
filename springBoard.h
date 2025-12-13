#pragma once

class SpringBoard : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

public:

	void Init(Vector2 boardLeftPos, Vector2 boardRightPos, float height);
	void Uninit();
	void Update();
	void Draw();

};
