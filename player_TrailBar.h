#pragma once

#include "UI.h"

class PlayerTrailBar : public UI
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture[4];


	float m_TrailTime;		//æ“¾‚µ‚Ä‚¢‚éÄ¶‚Ì‘±ŠÔ
	float m_ReverseTime;	//æ“¾‚µ‚Ä‚¢‚é‹tÄ¶‚Ì‘±ŠÔ

	bool m_GettingTrail;	//‹OÕ‚ğæ“¾’†‚©‚Ç‚¤‚©
	bool m_GettingReverse;	//‹tÄ¶‚ğæ“¾’†‚©‚Ç‚¤‚©

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

};
