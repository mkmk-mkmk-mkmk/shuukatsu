#pragma once
#include "main.h"
#include <iostream>

#include "framework.h"
#include "renderer.h"
#include "gameObject.h"

class Cursor : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;


	Vector2  m_Position; //カーソル座標
	Vector2  m_Scale;	//カーソルサイズ

	bool	m_LeftButtonUse = false;	//左ボタン使用中
	bool	m_RightButtonUse = false;	//右ボタン使用中

	float m_CursorPlayerDistance;	//カーソルとプレイヤーの距離

	Vector2  m_CursorLockPos;		//カーソル位置固定座標

	Vector2  m_TextureSplit;		//テクスチャの分割数

	int m_Frame;

public:
	void Init();
	void Update();
	void Uninit();
	void Draw();

	void SetPosition(Vector2 pos) { m_Position = pos; }
	Vector2 GetPosition() { return m_Position; }

};

