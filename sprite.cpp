
#include "main.h"
#include "sprite.h"


void Sprite::InitSprite()
{
	vertex[0].Position = XMFLOAT3( - 0.5f, -0.5, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3( -0.5f, 0.5f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


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
	material.Diffuse = { 1.0f, 1.0f, 1.0f, alpha };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}

void Sprite::DrawSpriteAnim(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale,
	int pattern, int cols, int rows, int texNum, float alpha, bool flip)
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
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, alpha };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	m_AnimationFrameCount++;
	if (m_AnimationFrameCount >= m_AnimationUpdateFrame)
	{
		m_AnimationFrameCount = 0;
		m_AnimationFrame++;
		if (m_AnimationFrame >= pattern)
		{
			m_AnimationFrame = 0;
		}
	}

	int x = m_AnimationFrame % cols;
	int y = m_AnimationFrame / cols;
	//一部のみを描画、アニメーション化（横二列）
	vertex[0].TexCoord = XMFLOAT2(1.0f / cols * x, 1.0f / rows * y);
	vertex[1].TexCoord = XMFLOAT2(1.0f / cols * (x + 1), 1.0f / rows * y);
	vertex[2].TexCoord = XMFLOAT2(1.0f / cols * x, 1.0f / rows * (y + 1));
	vertex[3].TexCoord = XMFLOAT2(1.0f / cols * (x + 1), 1.0f / rows * (y + 1));

	if (flip)
	{
		//左右反転
		m_SaveTexCoord[0] = vertex[0].TexCoord;
		m_SaveTexCoord[1] = vertex[2].TexCoord;

		vertex[0].TexCoord = vertex[1].TexCoord;
		vertex[1].TexCoord = m_SaveTexCoord[0];
		vertex[2].TexCoord = vertex[3].TexCoord;
		vertex[3].TexCoord = m_SaveTexCoord[1];
	}

	SetVertexSprite();

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//スプライト描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Sprite::DrawSpritePiece(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale,
	int pattern, int cols, int rows, int texNum, float alpha, bool flip)
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
	trans = XMMatrixTranslation(Position.x, Position.y, 0.0f);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, alpha };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	int x = pattern % cols;
	int y = pattern / cols;
	//一部のみを描画、アニメーション化（横二列）
	vertex[0].TexCoord = XMFLOAT2(1.0f / cols * x, 1.0f / rows * y);
	vertex[1].TexCoord = XMFLOAT2(1.0f / cols * (x + 1), 1.0f / rows * y);
	vertex[2].TexCoord = XMFLOAT2(1.0f / cols * x, 1.0f / rows * (y + 1));
	vertex[3].TexCoord = XMFLOAT2(1.0f / cols * (x + 1), 1.0f / rows * (y + 1));

	if (flip)
	{
		//左右反転
		m_SaveTexCoord[0] = vertex[0].TexCoord;
		m_SaveTexCoord[1] = vertex[2].TexCoord;

		vertex[0].TexCoord = vertex[1].TexCoord;
		vertex[1].TexCoord = m_SaveTexCoord[0];
		vertex[2].TexCoord = vertex[3].TexCoord;
		vertex[3].TexCoord = m_SaveTexCoord[1];
	}

	SetVertexSprite();

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//スプライト描画
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
		memcpy(mappedResource.pData, vertex, sizeof(VERTEX_3D) * 4);

		// アンマップ
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}
}