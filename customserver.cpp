#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "vendor/imgui/imgui_internal.h"
#include "keyboard.h"
#include "settings.h"
#include "customserver.h"
#include "chatwindow.h"
#include "vendor/inih/cpp/INIReader.h"

#include <string.h>
#include <stdlib.h>

extern CGUI *pGUI;
extern CGame *pGame;
extern CNetGame *pNetGame;
extern CKeyBoard *pKeyBoard;
extern CChatWindow *pChatWindow;
extern CSettings *pSettings;

char szNickInputBuffer[100];
char utf8NickInputBuffer[100*3];

char szPasswordInputBuffer[100];
char utf8PasswordInputBuffer[100*3];

char szIPInputBuffer[100];
char utf8IPInputBuffer[100*3];

char szPortInputBuffer[100];
char utf8PortInputBuffer[100*3];

CCustomServer::CCustomServer()
{
	m_bIsActive = false;
	m_bFixer = false;
	m_bFixer2 = false;
	m_bLamb = false;
}

CCustomServer::~CCustomServer()
{

}

void CCustomServer::Show(bool bShow)
{
	m_bIsActive = bShow;
	m_bFixer = false;
}

void CCustomServer::Clear()
{
	m_bIsActive = false;
	m_bFixer = false;

	memset(szNickInputBuffer, 0, 100);
	memset(utf8NickInputBuffer, 0, 100*3);
	
	memset(szPasswordInputBuffer, 0, 100);
	memset(utf8PasswordInputBuffer, 0, 100*3);
	
	memset(szIPInputBuffer, 0, 100);
	memset(utf8IPInputBuffer, 0, 100*3);

	memset(szPortInputBuffer, 0, 100);
	memset(utf8PortInputBuffer, 0, 100*3);
}

void IPWindowInputHandler(const char* str)
{
	if(!str || *str == '\0') return;
	strcpy(szIPInputBuffer, str);
	cp1251_to_utf8(utf8IPInputBuffer, str);
}
void NickWindowInputHandler(const char* str)
{
	if(!str || *str == '\0') return;
	strcpy(szNickInputBuffer, str);
	cp1251_to_utf8(utf8NickInputBuffer, str);
}
void PasswordWindowInputHandler(const char* str)
{
	if(!str || *str == '\0') return;
	strcpy(szPasswordInputBuffer, str);
	cp1251_to_utf8(utf8PasswordInputBuffer, str);
}
void PortWindowInputHandler(const char* str)
{
	if(!str || *str == '\0') return;
	strcpy(szPortInputBuffer, str);
	cp1251_to_utf8(utf8PortInputBuffer, str);
}

void CCustomServer::Render()
{
	if(!m_bIsActive) return;

	ImGuiIO &io = ImGui::GetIO();
	
	ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));

	ImGui::Begin("Custom Server", nullptr,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoResize);

		ImGui::Text("Nama Pemain:");

		if(ImGui::Button(utf8NickInputBuffer, ImVec2(555, 50)))
		{
			pKeyBoard->Open(&NickWindowInputHandler);
		}

		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize()/2 + 5));
		
		ImGui::Text("IP Server:");

		if(ImGui::Button(utf8IPInputBuffer, ImVec2(555, 50)))
		{
			pKeyBoard->Open(&IPWindowInputHandler);
		}

		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize()/2 + 5));

		ImGui::Text("Port Server:");

		if(ImGui::Button(utf8PortInputBuffer, ImVec2(555, 50)))
		{
			pKeyBoard->Open(&PortWindowInputHandler);
		}

		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize()/2 + 5));

		ImGui::Text("Kata Sandi Server:");

		if(ImGui::Button(utf8PasswordInputBuffer, ImVec2(555, 50)))
		{
			pKeyBoard->Open(&PasswordWindowInputHandler);
		}

		ImGui::ItemSize(ImVec2(0, pGUI->GetFontSize()/2 + 5));
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 278 + ImGui::GetStyle().ItemSpacing.x) / 2);

		if(ImGui::Button("Hubungkan", ImVec2(125, 50)))
		{
			pNetGame->ShutDownForGameRestart();
			CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
			for(uint16_t playerId = 0; playerId < MAX_PLAYERS; playerId++)
			{
				CRemotePlayer *pPlayer = pPlayerPool->GetAt(playerId);
				if(pPlayer)
					pPlayerPool->Delete(playerId, 0);
			}
			nick = szNickInputBuffer;
			pass = szPasswordInputBuffer;
			address = szIPInputBuffer;
			char *po = szPortInputBuffer;
			port = atoi(po);

			pNetGame = new CNetGame(
				address,
				port,
				nick,
				pass
			);
			Show(false);
			
			if(pGame) 
            	pGame->FindPlayerPed()->TogglePlayerControllable(true);
		}

		ImGui::SameLine(0, pGUI->GetFontSize());
		
		if(ImGui::Button("Tutup", ImVec2(125, 50)))
		{
			Show(false);
		}

	ImGui::SetWindowSize(ImVec2(-1, -1));
	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(((io.DisplaySize.x - size.x)/2), ((io.DisplaySize.y - size.y)/2)));
	ImGui::End();

	ImGui::PopStyleVar();
}
