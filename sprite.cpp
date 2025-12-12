
#include "sprite.h"

void InitSprite()
{
	ID3D11Device* pDevice = Renderer::GetDevice();

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//初期化
	g_Vertex[0].pos.x = -0.5f;
	g_Vertex[0].pos.y = -0.5f;
	g_Vertex[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[0].texCoord = XMFLOAT2(0.0f, 0.0f);

	g_Vertex[1].pos.x = 0.5f;
	g_Vertex[1].pos.y = -0.5f;
	g_Vertex[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[1].texCoord = XMFLOAT2(1.0f, 0.0f);

	g_Vertex[2].pos.x = -0.5f;
	g_Vertex[2].pos.y = 0.5f;
	g_Vertex[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[2].texCoord = XMFLOAT2(0.0f, 1.0f);

	g_Vertex[3].pos.x = 0.5f;
	g_Vertex[3].pos.y = 0.5f;
	g_Vertex[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_Vertex[3].texCoord = XMFLOAT2(1.0f, 1.0f);


	////頂点バッファ更新
	SetVertexSprite();

}

void UnInitSprite()
{
	//終了処理
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

}

void UpdateSprite()
{

}

void DrawSprite(XMFLOAT2 Pos, float Rotate, XMFLOAT2 Scale, float alpha)
{
	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(Scale.x, Scale.y, 1.0f);
	rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
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

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::GetDeviceContext()->Draw(4, 0);
}

void DrawSpriteAnim(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, float alpha, int pattern, int cols, int rows)
{
	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);


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
	g_Vertex[0].texCoord = XMFLOAT2(1.0f / cols * x, 1.0f / rows * y);
	g_Vertex[1].texCoord = XMFLOAT2(1.0f / cols * (x + 1), 1.0f / rows * y);
	g_Vertex[2].texCoord = XMFLOAT2(1.0f / cols * x, 1.0f / rows * (y + 1));
	g_Vertex[3].texCoord = XMFLOAT2(1.0f / cols * (x + 1), 1.0f / rows * (y + 1));

	SetVertexSprite();

	//スプライト描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void SetVertexSprite()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// 動的バッファ更新（WRITE_DISCARD）
	HRESULT hr = Renderer::GetDeviceContext()->Map(
		g_VertexBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);

	if (SUCCEEDED(hr))
	{
		// 頂点配列 → バッファへコピー
		memcpy(mappedResource.pData, g_Vertex, sizeof(VERTEX_3D) * NUM_VERTEX);

		// アンマップ
		Renderer::GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}
}