#include "main.h"
#include "cmdprocs.h"
#include "chatwindow.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "settings.h"
#include "customserver.h"

extern CChatWindow *pChatWindow;
extern CGame *pGame;
extern CNetGame *pNetGame;
extern CGUI *pGUI;
extern CSettings *pSettings;
extern CCustomServer *pCustomServer;

void cmd_quit(const char *params)
{
    std::terminate();
    return;
}

void cmd_reconnect(const char *params) 
{
	pChatWindow->AddInfoMessage("Menghubungkan Kembali...");
	pNetGame->ShutDownForGameRestart();
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	for(uint16_t playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		CRemotePlayer *pPlayer = pPlayerPool->GetAt(playerId);
		if(pPlayer) pPlayerPool->Delete(playerId, 0);
	}
	pNetGame->SetGameState(GAMESTATE_WAIT_CONNECT);
}

void cmd_savepos(const char *params)
{
    CPlayerPed *pPlayer = pGame->FindPlayerPed();
    FILE *fileOut = nullptr;
    uint32_t dwVehicleID;
    float fZAngle = 0.0f;
    
    char ff[0xFF];
    sprintf(ff, "%sSAMP/savedpos.txt", g_pszStorage);
    fileOut = fopen(ff, "a");
    if(!fileOut)
        pChatWindow->AddInfoMessage("Cant open savedpos.txt");
        
    if(pPlayer->IsInVehicle())
    {
        //on vehicle savepos
        /*VEHICLE_TYPE *pVehicle = pPlayer->GetGtaVehicle();
        ScriptCommand(&get_car_z_angle, &fZAngle);
        fprintf(fileOut, "AddStaticVehicle(%u,%.4f,%.4f,%.4f,%.4f,%u,%u) //%s\n",
            pVehicle->entity.nModelIndex,
            pVehicle->entity.mat->pos.X, 
            pVehicle->entity.mat->pos.Y, 
            pVehicle->entity.mat->pos.Z, 
            fZAngle,
            pVehicle->byteColor1,
            pVehicle->byteColor2,
            params
        );*/
        pChatWindow->AddInfoMessage("On vehicle position saved.");
    }
    else
    {
    	//on foot savepos
    	PED_TYPE *pActor = pPlayer->GetGtaActor();
    	ScriptCommand(&get_actor_z_angle, pPlayer->m_dwGTAId, &fZAngle);
    	fprintf(fileOut,"AddPlayerClass(%u,%.4f,%.4f,%.4f,%.4f,0,0,0,0,0,0); // %s\n",
    		pPlayer->GetModelIndex(),
    		pActor->entity.mat->pos.X,
    		pActor->entity.mat->pos.Y,
    		pActor->entity.mat->pos.Z,
    		fZAngle,
    		params
    	);
    	pChatWindow->AddInfoMessage("On foot position saved.");
    }
    fclose(fileOut);
}

void cmd_rcon(const char *params)
{
    if(!params) return;
    uint8_t bytePacketID = ID_RCON_COMMAND;
    RakNet::BitStream bsCommand;
    bsCommand.Write(bytePacketID);
    uint32_t dwCmdLen = (uint32_t)strlen(params);
    bsCommand.Write(dwCmdLen);
    bsCommand.Write(params, dwCmdLen);
    pNetGame->GetRakClient()->Send(&bsCommand, HIGH_PRIORITY, RELIABLE, 0);
}

void cmd_customserver(const char *params)
{
	pCustomServer->Show(true);
	return;
}
void SetupCommands()
{
    pChatWindow->AddCmdProc("q", cmd_quit);
    pChatWindow->AddCmdProc("rec", cmd_reconnect);
    pChatWindow->AddCmdProc("save", cmd_savepos);
    pChatWindow->AddCmdProc("rcon", cmd_rcon);
    pChatWindow->AddCmdProc("customserver", cmd_customserver);
    Log("Commands initialized.");
}
