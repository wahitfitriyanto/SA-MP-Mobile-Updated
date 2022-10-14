#pragma once

#include "game/RW/RenderWare.h"
#include "../vendor/imgui/imgui.h"

enum eTouchType
{
	TOUCH_POP = 1,
	TOUCH_PUSH = 2,
	TOUCH_MOVE = 3
};

class CGUI
{
public:
	CGUI();
	~CGUI();

	void Render();

	float ScaleX(float x) { return m_vecScale.x * x; }
	float ScaleY(float y) { return m_vecScale.y * y; }
	ImFont* GetFont() { return m_pFont; }
	ImFont* GetSampFont() { return m_pSampFont; }
	float GetFontSize() { return m_fFontSize; }

	bool OnTouchEvent(int type, bool multi, int x, int y);

	void RenderText(ImVec2& pos, ImU32 col, bool bOutline, const char* text_begin, const char* text_end = nullptr, float font_size = 0.0f);
	void RenderOverlayText(ImVec2& pos, ImU32 col, bool bOutline, const char* text_begin, const char* text_end = nullptr);

	RwTexture* AltTexture   = nullptr;
	RwTexture* Ctrltexture  = nullptr;
	RwTexture* Tabtexture   = nullptr;
	RwTexture* Ftexture     = nullptr;
	RwTexture* Htexture     = nullptr;
	RwTexture* Ytexture     = nullptr;
	RwTexture* Ntexture     = nullptr;
	RwTexture* Gtexture     = nullptr;
	RwTexture* Twotexture   = nullptr;
	RwTexture* ScrlRtexture = nullptr;
	RwTexture* ScrlLtexture = nullptr;

    bool        m_bTabStatus;

private:
	void RenderRakNetStatistics();
	void RenderVersion();
	void SetupStyleColors();

private:
	ImFont* 	m_pFont;
	ImFont*		m_pSampFont;

	ImVec2		m_vecScale;
	float 		m_fFontSize;

	bool 		m_bMousePressed;
	ImVec2		m_vecMousePos;

	bool		m_bNeedClearMousePos;

};