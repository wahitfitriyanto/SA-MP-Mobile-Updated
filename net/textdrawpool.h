#pragma once

#include "game/textdraw.h"

#define MAX_GLOBAL_TEXT_DRAWS 3072
#define MAX_PLAYER_TEXT_DRAWS 1024
#define MAX_TEXT_DRAWS MAX_GLOBAL_TEXT_DRAWS + MAX_PLAYER_TEXT_DRAWS

class CTextDrawPool
{
private:
    CTextDraw           *m_pTextDraw[MAX_TEXT_DRAWS];
    bool                 m_bSlotState[MAX_TEXT_DRAWS];
    bool                 m_bSelectState;
	uint32_t             m_dwHoverColor;
    uint16_t             m_wClickedTextDrawID;

public:
    CTextDrawPool();
    ~CTextDrawPool();

    CTextDraw *New(unsigned short wText, TEXT_DRAW_TRANSMIT *TextDrawTransmit, char *szText);
    void Delete(unsigned short wText);
    void Draw();

    bool OnTouchEvent(int type, bool multi, int x, int y);
    void SendClick();
    void SetSelectState(bool bState, uint32_t dwColor);

    bool IsValidTextDrawId(uint16_t textDrawId) {
        if (textDrawId >= 0 && textDrawId < MAX_TEXT_DRAWS) {
            return true;
        }
        return false;
    };

    void SnapshotProcess();

    CTextDraw *GetAt(unsigned short wText) {
        if(wText >= MAX_TEXT_DRAWS) return 0;
        if(!m_bSlotState[wText]) return 0;
        return m_pTextDraw[wText];
    }
};