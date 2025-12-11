#pragma once

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include <memory>

class ObjectManager;

//------------------------
// マクロ定義
//------------------------
#define NUM_VERTEX (4)	//必要な頂点の数
			//原点が中心
#define P_POSX (-100.0f)	//スプライトのデフォルト位置x
#define P_POSY (-200.0f)	//スプライトのデフォルト位置y

//------------------------
// プロトタイプ宣言
//------------------------

//------------------------
// グローバル変数
//------------------------
static ID3D11Buffer* g_VertexBuffer = NULL;		//線用の頂点情報

struct VERTEX_T {
	XMFLOAT2		pos;		//位置ベクトル
	XMFLOAT2		vel;		//速度ベクトル
	XMFLOAT4		color;		//頂点カラー
	XMFLOAT2		texCoord;	//テクスチャ座標
};

struct TextureCoord
{
	XMFLOAT2 pos[4];
};

static VERTEX_T g_Vertex[NUM_VERTEX];
static int g_AnimPattern = 0;
static int g_AnimFrame = 0;

void InitSprite();
void UnInitSprite();
void UpdateSprite();
void DrawSprite(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, float alpha = 1.0f);
void DrawSpriteAnim(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, float alpha, int pattern, int cols, int rows);
//void DrawSprite(const ObjectManager& obj, bool EnableShake = true, TextureCoord Texcoord = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) });
//void DrawSpriteColor(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, XMFLOAT4 col, bool EnableShake = true, TextureCoord Texcoord = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) });
//void DrawSpriteColor(ObjectManager& obj, bool EnableShake = true, XMFLOAT2 ExtraDrawDiff = XMFLOAT2(0.0f, 0.0f), TextureCoord Texcoord = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) });
////void DrawSpriteBar(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, float alpha, float end);
//void DrawSpriteAnim(XMFLOAT2 Position, float Rotate, XMFLOAT2 Scale, int AnimCols, int AnimRows, int AnimPattern, float alpha = 1.0f, bool EnableShake = true);
//void DrawSpriteAnim(ObjectManager& obj, int AnimCols, int AnimRows, int AnimPattern, XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f }, bool EnableShake = true);
void SetVertexSprite();
