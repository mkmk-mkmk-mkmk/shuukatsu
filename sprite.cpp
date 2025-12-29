
#include "sprite.h"

Sprite::Sprite()
{
	vertex[0].Position = { -0.5f, -0.5 };
	vertex[0].Normal = { 0.0f, 0.0f };
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { 0.5f, -0.5f };
	vertex[1].Normal = { 0.0f, 0.0f };
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { -0.5f, 0.5f };
	vertex[2].Normal = { 0.0f, 0.0f };
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { 0.5f, 0.5f };
	vertex[3].Normal = { 0.0f, 0.0f };
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = { 1.0f, 1.0f };


	D3D11_BUFFER_DESC bd{};
	//bd.Usage = D3D11_USAGE_DEFAULT;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}

void Sprite::InitSprite()
{

}

void Sprite::UnInitSprite()
{
	m_VertexBuffer->Release();
	m_VertexShader->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
}

void Sprite::DrawSprite(XMFLOAT2 Pos, float Rotate, XMFLOAT2 Scale, int texNum, float alpha)
{
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureList[texNum]);

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(Scale.x, Scale.y, 1.0f);
	rot = XMMatrixRotationZ(Rotate);
	trans = XMMatrixTranslation(Pos.x, Pos.y, 0.0f);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//カーソル描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}

void Sprite::DrawSpriteAnim(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, int pattern, int cols, int rows, int texNum, float alpha)
{
	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	//プロジェクションマトリクス設定
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, screenWidth, screenHeight, 0.0f, 0.0f, 1.0f);
	Renderer::SetProjectionMatrix(projection);

	//ビューマトリクス設定
	XMMATRIX view;
	view = XMMatrixIdentity();
	Renderer::SetViewMatrix(view);

	//移動・回転マトリクス設定
	XMMATRIX trans, world, rot, scale;
	scale = XMMatrixScaling(Scale.x, Scale.y, 0.0f);

	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);
	rot = XMMatrixRotationZ(Rotate);	//ラジアン角
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定(半年後に登場)
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, alpha);
	Renderer::SetMaterial(material);

	int x = pattern % cols;
	int y = pattern / cols;
	//一部のみを描画、アニメーション化（横二列）
	vertex[0].TexCoord = XMFLOAT2(1.0f / cols * x, 1.0f / rows * y);
	vertex[1].TexCoord = XMFLOAT2(1.0f / cols * (x + 1), 1.0f / rows * y);
	vertex[2].TexCoord = XMFLOAT2(1.0f / cols * x, 1.0f / rows * (y + 1));
	vertex[3].TexCoord = XMFLOAT2(1.0f / cols * (x + 1), 1.0f / rows * (y + 1));

	SetVertexSprite();

	//スプライト描画
	Renderer::GetDeviceContext()->Draw(4, 0);



	//



	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_TextureList[texNum]);

	// ---------- シェーダ設定 ----------
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);

	// ---------- 行列設定（DrawSpriteと統一） ----------
	Renderer::SetWorldViewProjection2D();

	XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, 1.0f);
	XMMATRIX rot = XMMatrixRotationZ(Rotate);
	XMMATRIX trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);

	XMMATRIX world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	// ---------- マテリアル ----------
	MATERIAL material{};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, alpha);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// ---------- UV計算 ----------
	int x = pattern % cols;
	int y = pattern / cols;

	float u0 = (float)x / cols;
	float v0 = (float)y / rows;
	float u1 = (float)(x + 1) / cols;
	float v1 = (float)(y + 1) / rows;

	vertex[0].TexCoord = { u0, v0 };
	vertex[1].TexCoord = { u1, v0 };
	vertex[2].TexCoord = { u0, v1 };
	vertex[3].TexCoord = { u1, v1 };

	// ---------- 頂点バッファ更新（必要な時だけ） ----------
	SetVertexSprite();

	// ---------- 頂点バッファ設定 ----------
	UINT stride = sizeof(VERTEX_T);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(
		0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ---------- 描画 ----------
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Sprite::SetVertexSprite()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// 動的バッファ更新（WRITE_DISCARD）
	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_VertexBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);

	if (SUCCEEDED(hr))
	{
		// 頂点配列 → バッファへコピー
		memcpy(mappedResource.pData, vertex, sizeof(VERTEX_3D) * NUM_VERTEX);

		// アンマップ
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}
}