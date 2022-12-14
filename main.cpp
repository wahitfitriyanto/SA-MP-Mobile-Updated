#include "main.h"
#include "game/game.h"
#include "game/RW/RenderWare.h"
#include "net/netgame.h"
#include "gui/gui.h"
//#include "vendor/imgui/imgui_internal.h"
#include "chatwindow.h"
#include "spawnscreen.h"
#include "playertags.h"
#include "dialog.h"
#include "keyboard.h"
#include "settings.h"
#include "scoreboard.h"
#include "button.h"
#include "game/snapshothelper.h"
#include "game/world.h"
#include "customserver.h"
#include "bkeys.h"
#include "cmdprocs.h"
#include "GButton.h"
//#include "vendor/inih/cpp/INIReader.h"

#include "util/armhook.h"
#include "checkfilehash.h"

#include "str_obfuscator_no_template.hpp"

#include <dlfcn.h>
#include "game/audiostream.h"

uintptr_t g_libGTASA = 0;
uintptr_t g_libSCAnd = 0;

const char* g_pszStorage = nullptr;

const auto encHost = cryptor::create("tssnet.xyz", 24);// IP 
unsigned short Port = 7777;// PORT 
const auto encPass = cryptor::create("ONIC250+", 24);// PASSWORD 

constexpr auto unique_library_path = cryptor::create("libsamp.so",11);

CGame *pGame = nullptr;
CWorld *pWorld = nullptr;
CNetGame *pNetGame = nullptr;
CChatWindow *pChatWindow = nullptr;
CSpawnScreen *pSpawnScreen = nullptr;
CPlayerTags *pPlayerTags = nullptr;
CDialogWindow *pDialogWindow = nullptr;
CScoreBoard *pScoreBoard = nullptr;
CButton *pButton = nullptr;
CSnapShotHelper* pSnapShotHelper = nullptr;
CAudioStream *pAudioStream = nullptr;
CBKeys *pBKeys = nullptr;
CGButton *pGButton = nullptr;

CGUI *pGUI = nullptr;
CKeyBoard *pKeyBoard = nullptr;
CDebug *pDebug = nullptr;
CSettings *pSettings = nullptr;
CCustomServer *pCustomServer = nullptr;


void InitHookStuff();
void InstallSpecialHooks();
void InitRenderWareFunctions();
void ApplyInGamePatches();
void ApplyPatches_level0();
void MainLoop();
void InitInMenu();

void InitSAMP()
{
	Log("Initializing SAMP..");
	g_pszStorage = (const char*)(g_libGTASA+0x63C4B8);

	if(!g_pszStorage)
	{
		Log("Error: storage path not found!");
		std::terminate();
		return;
	}
	pSettings = new CSettings();

	Log("Checking samp files..");
	if(!FileCheckSum())
	{
		Log("SOME FILES HAVE BEEN MODIFIED. YOU NEED REINSTALL SAMP!");
		//std::terminate();
		return;
	}

	Log("SAMP library loaded! Build time: " __DATE__ " " __TIME__);
}

void InitInMenu()
{
	pGame = new CGame();
	pGame->InitInMenu();

	if(pSettings->Get().bDebug) {
		pDebug = new CDebug();
	}

	pGUI = new CGUI();
	pKeyBoard = new CKeyBoard();
	pChatWindow = new CChatWindow();
	pSpawnScreen = new CSpawnScreen();
	pPlayerTags = new CPlayerTags();
	pDialogWindow = new CDialogWindow();
	pScoreBoard = new CScoreBoard();
	pWorld = new CWorld();
	pGButton = new CGButton();
	pSnapShotHelper = new CSnapShotHelper();
	pAudioStream = new CAudioStream();
	pCustomServer = new CCustomServer();
	pBKeys = new CBKeys();
}

void InitInGame()
{
	static bool bGameInited = false;
	static bool bNetworkInited = false;

	if(!bGameInited)
	{
		pGame->InitInGame();
		pGame->SetMaxStats();
		pAudioStream->Initialize();

		SetupCommands();

		if(pDebug && !pSettings->Get().bOnline)
		{
			pDebug->SpawnLocalPlayer();
		}

		bGameInited = true;
		return;
	}

	if(!bNetworkInited && pSettings->Get().bOnline)
	{
		pNetGame = new CNetGame(
			encHost.decrypt(),
			Port,
			pSettings->Get().szNickName,
			encPass.decrypt()
		);
		bNetworkInited = true;
		return;
	}
}

void MainLoop()
{
	InitInGame();

	if(pDebug) pDebug->Process();
	if(pNetGame) pNetGame->Process();
	if(pAudioStream) pAudioStream->Process();
}

void handler(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if(info->si_signo == SIGSEGV)
	{
		Log("SIGSEGV | Fault address: 0x%X", info->si_addr);
		Log("libGTASA base address: 0x%X", g_libGTASA);
		Log("register states:");

		Log("r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",
			context->uc_mcontext.arm_r0,
			context->uc_mcontext.arm_r1,
			context->uc_mcontext.arm_r2,
			context->uc_mcontext.arm_r3);
		Log("r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x",
			context->uc_mcontext.arm_r4,
			context->uc_mcontext.arm_r5,
			context->uc_mcontext.arm_r6,
			context->uc_mcontext.arm_r7);
		Log("r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x",
			context->uc_mcontext.arm_r8,
			context->uc_mcontext.arm_r9,
			context->uc_mcontext.arm_r10,
			context->uc_mcontext.arm_fp);
		Log("ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x",
			context->uc_mcontext.arm_ip,
			context->uc_mcontext.arm_sp,
			context->uc_mcontext.arm_lr,
			context->uc_mcontext.arm_pc);

		Log("backtrace:");
		Log("1: libGTASA.so + 0x%X", context->uc_mcontext.arm_pc - g_libGTASA);
		Log("2: libGTASA.so + 0x%X", context->uc_mcontext.arm_lr - g_libGTASA);

		Log("1: libsamp.so + 0x%X", context->uc_mcontext.arm_pc - FindLibrary("libsamp.so"));
		Log("2: libsamp.so + 0x%X", context->uc_mcontext.arm_lr - FindLibrary("libsamp.so"));

		exit(0);
	}

	return;
}

void *Init(void *p)
{
	ApplyPatches_level0();

	pthread_exit(0);
}
void(*CGameProcess)(void*);

void CGame__Process(void* thisptr)
{
	Log("%p %p", thisptr, (uint32_t)thisptr-g_libGTASA);
	CGameProcess(thisptr);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	g_libGTASA = FindLibrary("libGTASA.so");
	if (g_libGTASA == 0)
	{
		Log("ERROR: libGTASA.so address not found!");
		return 0;
	}
	Log("libGTASA.so image base address: 0x%X", g_libGTASA);

	g_libSCAnd = FindLibrary("libSCAnd.so");
	if (g_libSCAnd == 0)
	{
		Log("ERROR: libSCAnd.so address not found!");
		return 0;
	}
	Log("libSCAnd.so image base address: 0x%X", g_libSCAnd);

	//((void (*)())(g_libSCAnd + 0xBB448 + 1))();
	InitHookStuff();

	Log("Loading Bass Library");
	LoadBassLibrary();

	srand(time(0));

	InitRenderWareFunctions();
	InstallSpecialHooks();

	pthread_t thread;
	pthread_create(&thread, 0, Init, 0);

	struct sigaction act;
	act.sa_sigaction = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &act, 0);

	return JNI_VERSION_1_4;
}

int (*BASS_Init)(uint32_t, uint32_t, uint32_t);
int (*BASS_Free)(void);
int (*BASS_SetConfigPtr)(uint32_t, const char*);
int (*BASS_SetConfig)(uint32_t, uint32_t);
int (*BASS_ChannelStop)(uint32_t);
int (*BASS_StreamCreateURL)(char*, uint32_t, uint32_t, uint32_t);
int (*BASS_ChannelPlay)(uint32_t);
int *BASS_ChannelGetTags;
int *BASS_ChannelSetSync;
int *BASS_StreamGetFilePosition;
int (*BASS_StreamFree)(uint32_t);

void LoadBassLibrary()
{
	void* v0 = dlopen("/data/data/com.rockstargames.gtasa/lib/libbass.so", 1);
	if ( !v0 )
	{
		Log("%s", dlerror());
	}
	BASS_Init = (int (*)(uint32_t, uint32_t, uint32_t))dlsym(v0, "BASS_Init");
	BASS_Free = (int (*)(void))dlsym(v0, "BASS_Free");
	BASS_SetConfigPtr = (int (*)(uint32_t, const char*))dlsym(v0, "BASS_SetConfigPtr");
	BASS_SetConfig = (int (*)(uint32_t, uint32_t))dlsym(v0, "BASS_SetConfig");
	BASS_ChannelStop = (int (*)(uint32_t))dlsym(v0, "BASS_ChannelStop");
	BASS_StreamCreateURL = (int (*)(char*, uint32_t, uint32_t, uint32_t))dlsym(v0, "BASS_StreamCreateURL");
	BASS_ChannelPlay = (int (*)(uint32_t))dlsym(v0, "BASS_ChannelPlay");
	BASS_ChannelGetTags = (int *)dlsym(v0, "BASS_ChannelGetTags");
	BASS_ChannelSetSync = (int *)dlsym(v0, "BASS_ChannelSetSync");
	BASS_StreamGetFilePosition = (int *)dlsym(v0, "BASS_StreamGetFilePosition");
	BASS_StreamFree = (int (*)(uint32_t))dlsym(v0, "BASS_StreamFree");
}
