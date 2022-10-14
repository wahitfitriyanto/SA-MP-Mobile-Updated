#include "main.h"
#include "gui/gui.h"
#include "game/game.h"
#include "net/netgame.h"
#include "settings.h"
#include "scoreboard.h"
#include "bkeys.h"

extern CGUI *pGUI;
extern CGame *pGame;
extern CNetGame *pNetGame;
extern CSettings *pSettings;
extern CScoreBoard *pScoreBoard;

CBKeys::CBKeys() {
	m_bIsActive = false;
	m_bIsItemShow = false;
}

CBKeys::~CBKeys() { }

void CBKeys::Show(bool bShow) {
	m_bIsActive = bShow;
}

void CBKeys::Render() {
	if(!m_bIsActive) return;

	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	if(pPlayerPed) {
		ImGuiIO &io = ImGui::GetIO();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.5f, 4.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

		ImGui::Begin("#ButtonKeys", nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings);

			ImVec2 vecButSize = ImVec2(
				ImGui::GetFontSize() * 3.5, 
				ImGui::GetFontSize() * 2.5);
				
		if(ImGui::Button(m_bIsItemShow ? "<<<" : ">>>", vecButSize))
		{
			if(!pScoreBoard->m_bToggle)
				m_bIsItemShow = !m_bIsItemShow;
		}
		ImGui::SameLine();
		if(ImGui::Button("TAB", vecButSize))
		{
			return pScoreBoard->Toggle();
		}
		ImGui::SameLine();
		CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
		if(pVehiclePool) 
		{
			uint16_t sNearestVehicleID = pVehiclePool->FindNearestToLocalPlayerPed();
			CVehicle *pVehicle = pVehiclePool->GetAt(sNearestVehicleID);
			if(pVehicle)
			{
				if(pVehicle->GetDistanceFromLocalPlayerPed() <0.0f) 
				{
					CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
					if(pPlayerPool) 
					{
						CLocalPlayer *pLocalPlayer = pPlayerPool->GetLocalPlayer();
						if(pLocalPlayer)
						{
							if(!pLocalPlayer->IsSpectating()) 
							{
								if(!pPlayerPed->IsInVehicle())
								{
									ImGui::SameLine();
									if(ImGui::Button("G", vecButSize))
									{
										pPlayerPed->EnterVehicle(pVehicle->m_dwGTAId, true);
										pLocalPlayer->SendEnterVehicleNotification(sNearestVehicleID, true);
									}
								}
							}
						}
					}
				}
			}
		}
		ImGui::SameLine();
		if(m_bIsItemShow) {
			if(ImGui::Button("F", vecButSize))
			{
				LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] = true;
			}
			ImGui::SameLine();
			if(ImGui::Button("H", vecButSize))
			{
				LocalPlayerKeys.bKeys[ePadKeys::KEY_CTRL_BACK] = true;
			}
			ImGui::SameLine();
			if(ImGui::Button("Y", vecButSize))
			{
				LocalPlayerKeys.bKeys[ePadKeys::KEY_YES] = true;
			}
			ImGui::SameLine();
			if(ImGui::Button("N", vecButSize))
			{
				LocalPlayerKeys.bKeys[ePadKeys::KEY_NO] = true;
			}
			ImGui::SameLine();
			if(ImGui::Button("ALT", vecButSize))
			{
				LocalPlayerKeys.bKeys[ePadKeys::KEY_WALK] = true;
			}
		}
		ImGui::SetWindowSize(ImVec2(-1, -1));

		ImVec2 size = ImGui::GetWindowSize();
		ImGui::SetWindowPos(ImVec2(pGUI->ScaleX(10), pGUI->ScaleY(345)));
		ImGui::End();

		ImGui::PopStyleVar();
	}
	return;
}
