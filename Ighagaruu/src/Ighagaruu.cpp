//--------------------------------------------------------------------------------------
// Ighagaruu.cpp
// Main cpp file
// Contains main game loop and main network loop
//--------------------------------------------------------------------------------------

#include "AntCore/DXUTToAntCore.h"

//#include "DXUT.h"
//#include "DXUTgui.h"
//#include "DXUTguiIME.h"
//#include "DXUTcamera.h"
//#include "DXUTsettingsdlg.h"
//#include "SDKmesh.h"
//#include "SDKmisc.h"
//#include "SDKsound.h"
#include "resource.h"
#include "Tokenizer.h"
#include "ISFLoader.h"
#include "IIFLoader.h"
#include "IAFLoader.h"
#include "Character.h"
#include <vector>
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <sstream>
#include <math.h>

#include "Spell.h"
#include "Aura.h"


//--------------------------------------------------------------------------------------
// Useful Structs
//--------------------------------------------------------------------------------------

struct Vertex
{
	Vertex(){}
	Vertex(
		float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v; // texture coordinates

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

struct Ray
{
	POVector3 _o;
	POVector3 _d;
};

struct BoundingSphere
{
	//BoundingSphere();

	POVector3 _c;
	float	_r;
};
struct ARRFLOAT {
	char b[8];
	float f;
	float* ptr;
	ARRFLOAT(char b1, char b2, char b3, char b4, char b5, char b6, char b7, char b8 ){
		b[0]=b1;
		b[1]=b2;
		b[2]=b3;
		b[3]=b4;
		b[4]=b5;
		b[5]=b6;
		b[6]=b7;
		b[7]=b8;
		ptr=(float*)&b;
		f=*ptr;
	}
};
struct ARRINT {
	char b[4];
	int i;
	int* ptr;
	ARRINT(char b1, char b2, char b3, char b4){
		b[0]=b1;
		b[1]=b2;
		b[2]=b3;
		b[3]=b4;
		ptr=(int*)&b;
		i=*ptr;
	}
};
struct FLOATARR {
	char b[8];
	float f;
	char* ptr;
	FLOATARR(float inf){
		f=inf;
		ptr=(char*)&f;
		b[0]=(char)*ptr;
		b[1]=(char)*(ptr+1);
		b[2]=(char)*(ptr+2);
		b[3]=(char)*(ptr+3);
		b[4]=(char)*(ptr+4);
		b[5]=(char)*(ptr+5);
		b[6]=(char)*(ptr+6);
		b[8]=(char)*(ptr+7);
	}
};
struct INTARR {
	char b[4];
	int i;
	char* ptr;
	INTARR(int in){
		i=in;
		ptr=(char*)&i;
		b[0]=(char)*ptr;
		b[1]=(char)*(ptr+1);
		b[2]=(char)*(ptr+2);
		b[3]=(char)*(ptr+3);
	}
};
struct GAMESTATS {
	int blueteam;
	int redteam;
	float fo;
	float fx;
	float fy;
	float fz;
};

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
//ID3DXFont*              g_pFont = NULL;         // Font for drawing text
//ID3DXSprite*            g_pTextSprite = NULL;   // Sprite for batching draw text calls
//ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface
//ID3D10EffectTechnique*      g_pTechnique = NULL;
CDXUTXFileMesh          g_Mesh;                 // Background mesh
CDXUTXFileMesh          g_IntroMesh;
CDXUTXFileMesh          g_CharBackgroundMesh;
CDXUTXFileMesh          g_Avatar[8];
CDXUTXFileMesh          g_QuadMesh;
CDXUTXFileMesh          g_Flag;
CDXUTXFileMesh          g_Mesh3;
//D3DXMATRIXA16           g_mView;
//D3DXMATRIXA16           g_mMapWorld;
CModelViewerCamera      g_Camera;               // A model viewing camera
//CModelViewerCamera      g_StaticCamera;
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          // Device settings dialog
AntGUIDialog             g_TopUI;                  // dialog for all controls
AntGUIDialog             g_TargetUI;                  // dialog for all controls
AntGUIDialog             g_LoginUI;
AntGUIDialog             g_ScoreUI;
AntGUIDialog             g_OptionsUI;                  // dialog for options
AntGUIDialog             g_CharacterUI;             // dialog for character controls
AntGUIDialog             g_SpellInfoUI;
AntGUIDialog             g_CastBarUI;
AntGUIDialog				g_CharacterChooseUI;
AntGUIDialog				g_RedTextUI;
//CDXUTScrollBar			g_ChatScroll;
AntGUIDialog				g_LoginPopUp;
bool                    g_bShowHelp = true;     // If true, it renders the UI control text
bool                    g_bUseD3DXIntersect = true;      // Whether to use D3DXIntersect
bool                    g_bAllHits = false;      // Whether to just get the first "hit" or all "hits"
bool                    g_bRightMouseDown = false;
bool                    g_bFoward = false;
bool                    g_bBackward = false;
bool                    g_bLeft = false;
bool                    g_bRight = false;
bool                    g_bTurnLeft = false;
bool                    g_bTurnRight = false;
bool					loggedin = false;
bool					step =true;
bool					movementDisabled=false;
int						redTextTime=0;
int						redTextLength=3;
Character*				myChar;
Character*				target;
//IDirect3DVertexBuffer9* Quad;
std::vector <Character> characters;
std::vector <Item*> items;
std::vector <Spell*> spells;
std::vector <Aura*> auras;
//WSADATA wsaData;
//SOCKET ConnectSocket = INVALID_SOCKET;
double					lastTime;
//IDirect3DTexture9* Tex=0;
CSoundManager           g_SndMgr;
CSound*     pMusic;
CSound*     die;
CSound*     lFoot[8];
CSound*     rFoot[8];
CSound*     casterror[7];
CSound*     scores[7];
POVector3 cameraTrans[8];
POVector3 zoom[8];
float		yadjust[8];
//D3DXVECTOR3 zoom[8];
//HFONT font;
RECT icons[24];
RECT icons2[24];
GAMESTATS gamestats;
std::string serverip;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27115"

#define IDC_CHANGEDEVICE        1
#define IDC_QUIT				2
#define IDC_EDITBOX1            3
#define IDC_TEXTBOX1            4
#define IDC_HPSTATIC            5
#define IDC_MAGIKASTATIC        6
#define IDC_HATESTATIC          7
#define IDC_FOCUSSTATIC			8
#define IDC_ENERGYSTATIC        9
#define IDC_AVATARSPRITE        10
#define IDC_CLOTHESSPRITE		11
#define IDC_BAGSPRITE			12
#define IDC_SPELLSPRITE1		13
#define IDC_SCORESPRITE			14
#define IDC_TARGETSPRITE		15
#define IDC_STATESPRITE			16
#define IDC_AHPPERCSTATIC       17
#define IDC_AMAGIKAPERCSTATIC   18
#define IDC_AHATEPERCSTATIC     19
#define IDC_AFOCUSPERCSTATIC	20
#define IDC_AENERGYPERCSTATIC   21
#define IDC_LEVELSTATIC			22
#define IDC_XPSTATIC			23
#define IDC_MONEYSTATIC			24
#define IDC_ALEVELSTATIC		25
#define IDC_AXPSTATIC			26
#define IDC_AMONEYSTATIC		27
#define IDC_OPTIONSSTATIC		28
#define IDC_SPELL1				29
#define IDC_SPELL2				30
#define IDC_SPELL3				31
#define IDC_SPELL4				32
#define IDC_SPELL5				33
#define IDC_SPELL6				34
#define IDC_SPELL7				35
#define IDC_SPELL8				36
#define IDC_SPELL9				37
#define IDC_SPELL10				38
#define IDC_SPELL11				39
#define IDC_LOGINERR			40
#define IDC_USERNAME			41
#define IDC_PASSWORD			42
#define IDC_LOGIN				43
#define IDC_LOGOL				44
#define IDC_LOGOR				45
#define IDC_CLOSELOGINPOP		46
#define IDC_CHOOSECHAR			47
#define IDC_CREATENEWCHAR		48
#define IDC_CHARLIST			49
#define IDC_LOGOUT				50
#define IDC_EHPSTATIC           51
#define IDC_EMAGIKASTATIC       52
#define IDC_EHATESTATIC         53
#define IDC_EFOCUSSTATIC		54
#define IDC_EENERGYSTATIC       55
#define IDC_EAHPPERCSTATIC      56
#define IDC_EAMAGIKAPERCSTATIC  57
#define IDC_EAHATEPERCSTATIC    58
#define IDC_EAFOCUSPERCSTATIC	59
#define IDC_EAENERGYPERCSTATIC  60
#define IDC_ELEVELSTATIC		61
#define IDC_EALEVELSTATIC		62
#define IDC_ANAME				63
#define IDC_ENAME				64
#define IDC_SPELLCAST1			65
#define IDC_SPELLCAST2			66
#define IDC_SPELLCAST3			67
#define IDC_SPELLCAST4			68
#define IDC_SPELLCAST5			69
#define IDC_SPELLCAST6			70
#define IDC_SPELLCAST7			71
#define IDC_SPELLCAST8			72
#define IDC_SPELLCAST9			73
#define IDC_SPELLCAST10			74
#define IDC_SPELLCAST11			75
#define IDC_CASTBORDER			76
#define IDC_CASTBAR				77
#define IDC_CASTNUM				78
#define IDC_AVATAR				79
#define IDC_EAVATAR				90
#define IDC_HEAD				91
#define IDC_CHEST				92
#define IDC_WEAPON				93
#define IDC_LEGS				94
#define IDC_RING				95
#define IDC_CLOAK				96
#define IDC_INV1				97
#define IDC_INV2				98
#define IDC_INV3				99
#define IDC_INV4				100
#define IDC_INV5				101
#define IDC_INV6				102
#define IDC_SCORESTATIC			103
#define IDC_HAGARUU				104
#define IDC_CIVILIZED			105
#define IDC_HSCORE				106
#define IDC_CSCORE				107
#define IDC_REDSTATIC			108

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
//bool    IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
//bool    ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
//HRESULT OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
//HRESULT OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void OnResetDevice(const D3DSURFACE_DESC* pBackBufferSurfaceDesc);

void    OnFrameMove(float fTime);
void    CharacterFrameMove(float  fTime);

void    OnFrameRender(float fTime);
void    CharacterFrameRender(float  fTime);
void    ChooseFrameRender(float  fTime);
void    IntroFrameRender(float fTime);
bool 	Event(UINT msg, UINT p1, UINT p2);

void    KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CharacterKeyBoardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );

void    OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    LoginGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CharacterGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CharacterChooseGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

void    OnLostDevice( void* pUserContext );
void    OnDestroyDevice( void* pUserContext );

void    InitApp();
//HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );
void    RenderText();
void    Connect(const wchar_t* username, const wchar_t* password);
void	Send();
void	SendTick();
//DWORD Receive(LPVOID lpParam);
//DWORD UpdateThread(LPVOID lpParam);
void LoginPopUp(const wchar_t* error);
void ChosenChar(Character* chosen);
void Chat(const wchar_t* user, const wchar_t* message);
void Target(Character* target);
void Cast(int buttonId);
void SetSpells();
void SetInventory();
void LoadSpells();
void LoadItems();
void LoadAuras();
void LoadServerIP();

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------
int main(int argc, char* argv)
{
	AntWrapSetRenderCallBack(OnFrameRender);
	AntWrapSetUpdateCallBack(OnFrameMove);
	AntWrapSetMsgCallBack(Event);

	myChar=NULL;
	target=NULL;

	if(!AntWrapInit(&Renderer, Settings, L"TeapotRacer")) {
		fprintf(stderr,"DisplayInitFailed(CannotContinue)"); return -1;
		//MessageBox(0, L"DisplayInitFailed(CannotContinue)", 0, 0);
	}
	g_DialogResourceManager._renderer = Renderer;
	InitApp();
	D3DSURFACE_DESC surface = D3DSURFACE_DESC(Settings._width,Settings._height);
	OnResetDevice(&surface);

	WCHAR wszBounce[MAX_PATH];
	g_SndMgr.Initialize( DXUTGetHWND(), DSSCL_PRIORITY );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\dreameater.wav" );
	g_SndMgr.Create( &pMusic, wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\die.wav" );
	g_SndMgr.Create( &die, wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\rockslidel.wav" );
	g_SndMgr.Create( &lFoot[0], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\rockslider.wav" );
	g_SndMgr.Create( &rFoot[0], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\lfoot.wav" );
	g_SndMgr.Create( &lFoot[1], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\rfoot.wav" );
	g_SndMgr.Create( &rFoot[1], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\rockslidel.wav" );
	g_SndMgr.Create( &lFoot[2], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\rockslider.wav" );
	g_SndMgr.Create( &rFoot[2], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\rockslidel.wav" );
	g_SndMgr.Create( &lFoot[3], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\rockslider.wav" );
	g_SndMgr.Create( &rFoot[3], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\electl.wav" );
	g_SndMgr.Create( &lFoot[4], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\electr.wav" );
	g_SndMgr.Create( &rFoot[4], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\electl.wav" );
	g_SndMgr.Create( &lFoot[5], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\electr.wav" );
	g_SndMgr.Create( &rFoot[5], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\electl.wav" );
	g_SndMgr.Create( &lFoot[6], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\electr.wav" );
	g_SndMgr.Create( &rFoot[6], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\electl.wav" );
	g_SndMgr.Create( &lFoot[7], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\electr.wav" );
	g_SndMgr.Create( &rFoot[7], wszBounce, DSBCAPS_CTRLVOLUME );

	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\friendlyscore.wav" );
	g_SndMgr.Create( &scores[0], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\enemyscore.wav" );
	g_SndMgr.Create( &scores[1], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\clap.wav" );
	g_SndMgr.Create( &scores[2], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\boo.wav" );
	g_SndMgr.Create( &scores[3], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\enemyflag.wav" );
	g_SndMgr.Create( &scores[4], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\friendlyflag.wav" );
	g_SndMgr.Create( &scores[5], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\resetflag.wav" );
	g_SndMgr.Create( &scores[6], wszBounce, DSBCAPS_CTRLVOLUME );

	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\casterror0.wav" );
	g_SndMgr.Create( &casterror[0], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\casterror1.wav" );
	g_SndMgr.Create( &casterror[1], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\casterror2.wav" );
	g_SndMgr.Create( &casterror[2], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\casterror3.wav" );
	g_SndMgr.Create( &casterror[3], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\casterror4.wav" );
	g_SndMgr.Create( &casterror[4], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\casterror5.wav" );
	g_SndMgr.Create( &casterror[5], wszBounce, DSBCAPS_CTRLVOLUME );
	DXUTFindDXSDKMediaFileCch( wszBounce, MAX_PATH, L"media\\casterror6.wav" );
	g_SndMgr.Create( &casterror[6], wszBounce, DSBCAPS_CTRLVOLUME );

	pMusic->Reset();
	pMusic->Play( 0, 0, (LONG) 1 );
	Renderer->AddMesh(L"media\\introscene.X");
	AntWrapRun();
	AntWrapQuit(Renderer);

	return 0;
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
	// Initialize dialogs
	g_SettingsDlg.Init( &g_DialogResourceManager);
	g_TopUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_TargetUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_CharacterUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_RedTextUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_CastBarUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_SpellInfoUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_OptionsUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_ScoreUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );

	g_CharacterUI.SetTexture(1,L"media\\controls2.dds");
	g_ScoreUI.SetTexture(1,L"media\\controls2.dds");
	g_TopUI.SetTexture(1,L"media\\controls2.dds");
	g_TargetUI.SetTexture(1,L"media\\controls2.dds");
	g_OptionsUI.SetTexture(1,L"media\\controls2.dds");
	g_SpellInfoUI.SetTexture(1,L"media\\controls2.dds");
	g_CastBarUI.SetTexture(1,L"media\\controls2.dds");
	g_CharacterUI.SetTexture(2,L"media\\icons.dds");
	g_CharacterUI.SetTexture(3,L"media\\icons2.dds");
	g_CharacterUI.SetTexture(4,L"media\\controls3.dds");

	g_TopUI.SetCallback( OnGUIEvent );
	g_TargetUI.SetCallback( OnGUIEvent );

	g_OptionsUI.SetCallback(OnGUIEvent);
	g_OptionsUI.AddStatic( IDC_OPTIONSSTATIC, L"", -12,-53,149,200);
	RECT testrect2= {0,0,136,74};
	g_OptionsUI.GetStatic( IDC_OPTIONSSTATIC )->GetElement(0)->SetTexture(1,&testrect2,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_OptionsUI.AddButton( IDC_CHANGEDEVICE, L"Video Options", 0, 0, 125, 22);
	g_OptionsUI.AddButton( IDC_QUIT, L"QUIT", 0, 50, 125, 22);
	g_OptionsUI.SetVisible(false);

	g_LoginPopUp.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_LoginPopUp.SetTexture(1,L"media\\controls2.dds");
	g_LoginPopUp.SetCallback(OnGUIEvent);
	g_LoginPopUp.AddStatic( IDC_OPTIONSSTATIC, L"", -25,-20,200,100);
	g_LoginPopUp.GetStatic( IDC_OPTIONSSTATIC )->GetElement(0)->SetTexture(1,&testrect2,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_LoginPopUp.AddStatic( IDC_LOGINERR, L"", 0, 0, 150, 22);
	g_LoginPopUp.AddButton( IDC_CLOSELOGINPOP, L"Close", 12, 50, 125, 22);
	g_LoginPopUp.SetVisible(false);

	g_LoginUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_LoginUI.SetTexture(1,L"media\\controls2.dds");
	g_LoginUI.SetTexture(2,L"media\\logo.dds");
	g_LoginUI.SetCallback(OnGUIEvent);
	RECT logorectl= {0,0,283,176};
	RECT logorectr= {0,176,283,352};
	g_LoginUI.AddStatic( IDC_LOGOL, L"", -79,-176,141,88);
	g_LoginUI.GetStatic( IDC_LOGOL )->GetElement(0)->SetTexture(2,&logorectl,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_LoginUI.AddStatic( IDC_LOGOR, L"", 62,-176,141,88);
	g_LoginUI.GetStatic( IDC_LOGOR )->GetElement(0)->SetTexture(2,&logorectr,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_LoginUI.AddEditBox( IDC_USERNAME, L"", 0, 0, 125, 32);
	g_LoginUI.GetEditBox( IDC_USERNAME)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_LoginUI.AddEditBox( IDC_PASSWORD, L"", 0, 38, 125, 32);
	g_LoginUI.GetEditBox( IDC_PASSWORD)->SetTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_LoginUI.AddButton( IDC_LOGIN, L"LOGIN", 0, 76, 125, 22);

	g_CharacterChooseUI.Init( &g_DialogResourceManager, false, L"media\\controls.dds"  );
	g_CharacterChooseUI.SetTexture(1,L"media\\controls2.dds");
	g_CharacterChooseUI.SetCallback(OnGUIEvent);
	g_CharacterChooseUI.AddListBox( IDC_CHARLIST, 600, 0, 150, 578);
	g_CharacterChooseUI.AddButton( IDC_CHOOSECHAR, L"Choose", 237, 578, 125, 22);
	g_CharacterChooseUI.AddButton( IDC_CREATENEWCHAR, L"Create New Character", 600, 578, 125, 22);
	g_CharacterChooseUI.GetButton( IDC_CREATENEWCHAR)->SetVisible(false);
	g_CharacterChooseUI.AddButton( IDC_LOGOUT, L"LOGOUT", 12, 50, 125, 22);
	g_CharacterChooseUI.GetButton( IDC_LOGOUT)->SetVisible(false);

	g_CharacterUI.SetCallback( OnGUIEvent );
	g_CharacterUI.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
	g_CharacterUI.SetFont( 2, L"Courier New", 16, FW_NORMAL );

	//.AddStatic( IDC_MAGIKASTATIC, L"Magika:", 10,22,50,12);
	// Edit box
	g_CharacterUI.AddEditBox( IDC_EDITBOX1, L"", 142, 518, 412, 32 );
	g_CharacterUI.GetEditBox( IDC_EDITBOX1 )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) ); // Change color to black
	g_CharacterUI.GetEditBox( IDC_EDITBOX1 )->SetVisible(false);

	g_RedTextUI.AddStatic( IDC_REDSTATIC, L"", 0, 0, 800, 12);
	g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetTextColor(D3DCOLOR_ARGB(255, 255, 0, 0));

	RECT testrect= {0,0,136,74};
	g_TopUI.AddStatic( IDC_AVATARSPRITE, L"", 10, 10, 136, 74 );
	g_TopUI.GetStatic( IDC_AVATARSPRITE )->GetElement(0)->SetTexture(1,&testrect,D3DCOLOR_ARGB(255, 255, 255, 255));

	g_TargetUI.AddStatic( IDC_TARGETSPRITE, L"", 156, 10, 136, 74 );
	g_TargetUI.GetStatic( IDC_TARGETSPRITE )->GetElement(0)->SetTexture(1,&testrect,D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT tablespriterect= {136,98,256,256};
	g_CharacterUI.AddStatic( IDC_BAGSPRITE, L"", 674, 432, 120, 158 );
	g_CharacterUI.GetStatic( IDC_BAGSPRITE )->GetElement(0)->SetTexture(4,&tablespriterect,D3DCOLOR_ARGB(255, 255, 255, 255));

	g_CharacterUI.AddStatic( IDC_CLOTHESSPRITE, L"", 554, 432, 120, 158 );
	g_CharacterUI.GetStatic( IDC_CLOTHESSPRITE )->GetElement(0)->SetTexture(1,&tablespriterect,D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT statespriterect= {0,158,136,212};
	g_ScoreUI.SetSize(136,54);
	g_ScoreUI.SetLocation(654,10);
	g_ScoreUI.AddStatic( IDC_SCORESPRITE, L"", 0, 0, 136, 54 );
	g_ScoreUI.GetStatic( IDC_SCORESPRITE )->GetElement(0)->SetTexture(1,&statespriterect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_ScoreUI.AddStatic( IDC_SCORESTATIC, L"SCORE:", 0, 5, 136, 12 );
	g_ScoreUI.AddStatic( IDC_HAGARUU, L"HAGARUU:", -28, 20, 136, 12 );
	g_ScoreUI.AddStatic( IDC_CIVILIZED, L"CIVILIZED:", -28, 35, 136, 12 );
	g_ScoreUI.AddStatic( IDC_HSCORE, L"0", 50, 20, 136, 12 );
	g_ScoreUI.AddStatic( IDC_CSCORE, L"0", 50, 35, 136, 12 );

	g_CharacterUI.AddStatic( IDC_STATESPRITE, L"", 6, 536, 136, 54 );
	g_CharacterUI.GetStatic( IDC_STATESPRITE )->GetElement(0)->SetTexture(1,&statespriterect,D3DCOLOR_ARGB(255, 255, 255, 255));

	RECT leftspellrect= {20,216,59,256};
	RECT middlespellrect= {59,216,96,256};
	RECT rightspellrect= {96,216,136,256};

	g_CharacterUI.AddStatic( IDC_SPELL1, L"", 142, 550, 39, 40 );
	g_CharacterUI.GetStatic( IDC_SPELL1 )->GetElement(0)->SetTexture(1,&leftspellrect,D3DCOLOR_ARGB(255, 255, 255, 255));

	for (int i=0; i<9; i++) {
		g_CharacterUI.AddStatic( (IDC_SPELL2+i), L"", 181+(i*37), 550, 37, 40 );
		g_CharacterUI.GetStatic( (IDC_SPELL2+i) )->GetElement(0)->SetTexture(1,&middlespellrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	g_CharacterUI.AddStatic( IDC_SPELL11, L"", 514, 550, 40, 40 );
	g_CharacterUI.GetStatic( IDC_SPELL11 )->GetElement(0)->SetTexture(1,&rightspellrect,D3DCOLOR_ARGB(255, 255, 255, 255));

	for (int j=0; j<4; j++) {
		for (int i=0; i<6; i++) {
			RECT icon = {0+(i*37),0+(j*37),36+(i*36),36+(j*37)};
			icons[i+(6*j)] = icon;
		}
	}

	for (int j=0; j<5; j++) {
		for (int i=0; i<4; i++) {
			RECT icon = {0+(i*57),2+(j*50),56+(i*57),52+(j*50)};
			icons2[i+(4*j)] = icon;
		}
	}
	for (int i=0; i<11; i++) {
		g_CharacterUI.AddButton( IDC_SPELLCAST1+i, L"1", 145+(i*37), 552, 36, 36, 49+i);
		g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->GetElement(0)->SetTexture(2,&icons[0],D3DCOLOR_ARGB(255, 255, 255, 255));
		g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->SetEnabled(false);
		g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->SetVisible(false);
	}
	for (int j=0; j<3; j++) {
		for (int i=0; i<2; i++) {
			g_CharacterUI.AddButton( IDC_HEAD+i+(2*j), L"", 557+(i*56), 434+(j*50), 56, 50);
			g_CharacterUI.GetButton( IDC_HEAD+i+(2*j) )->GetElement(0)->SetTexture(3,&icons2[0],D3DCOLOR_ARGB(255, 255, 255, 255));
			g_CharacterUI.GetButton( IDC_HEAD+i+(2*j) )->SetEnabled(false);
			g_CharacterUI.GetButton( IDC_HEAD+i+(2*j) )->SetVisible(false);
		}
	}
	for (int j=0; j<3; j++) {
		for (int i=0; i<2; i++) {
			g_CharacterUI.AddButton( IDC_INV1+i+(2*j), L"", 677+(i*56), 434+(j*50), 56, 50);
			g_CharacterUI.GetButton( IDC_INV1+i+(2*j) )->GetElement(0)->SetTexture(3,&icons2[0],D3DCOLOR_ARGB(255, 255, 255, 255));
			g_CharacterUI.GetButton( IDC_INV1+i+(2*j) )->SetEnabled(false);
			g_CharacterUI.GetButton( IDC_INV1+i+(2*j) )->SetVisible(false);
		}
	}
	g_CharacterUI.AddListBox( IDC_TEXTBOX1, 142, 432, 412, 86 );
	g_CharacterUI.GetListBox( IDC_TEXTBOX1 )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
//	g_CharacterUI.GetListBox( IDC_TEXTBOX1 )->GetElement(0)->TextureColor.States[ DXUT_STATE_NORMAL ]=(D3DCOLOR_ARGB(150, 255, 255, 255));
	//g_CharacterUI.GetListBox( IDC_TEXTBOX1 )->SetBorderWidth(0);
	g_CharacterUI.GetListBox( IDC_TEXTBOX1 )->SetEnabled(false);
	//g_CharacterUI.GetListBox( IDC_TEXTBOX1 )->SetText(g_CharacterUI.GetEditBox( IDC_TEXTBOX1 )->GetText());

	g_TopUI.AddStatic( IDC_ANAME, L"...", 10,14,50,12);
	RECT emptyrect= {197,0,256,8};
	RECT healthrect= {201,8,256,12};
	RECT magikarect= {201,12,256,16};
	RECT focusrect= {201,16,256,20};
	RECT haterect= {201,20,256,24};
	RECT energyrect= {201,24,256,28};
	g_TopUI.AddStatic( IDC_HPSTATIC, L"", 75,16,59,8);
	g_TopUI.GetStatic( IDC_HPSTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_MAGIKASTATIC, L"", 75,26,59,8);
	g_TopUI.GetStatic( IDC_MAGIKASTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_FOCUSSTATIC, L"", 75,36,59,8);
	g_TopUI.GetStatic( IDC_FOCUSSTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_HATESTATIC, L"", 75,46,59,8);
	g_TopUI.GetStatic( IDC_HATESTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_ENERGYSTATIC, L"", 75,56,59,8);
	g_TopUI.GetStatic( IDC_ENERGYSTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_AHPPERCSTATIC, L"", 77,18,0,4);
	g_TopUI.GetStatic( IDC_AHPPERCSTATIC )->GetElement(0)->SetTexture(1,&healthrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_AMAGIKAPERCSTATIC, L"", 77,28,0,4);
	g_TopUI.GetStatic( IDC_AMAGIKAPERCSTATIC )->GetElement(0)->SetTexture(1,&magikarect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_AFOCUSPERCSTATIC, L"", 77,38,0,4);
	g_TopUI.GetStatic( IDC_AFOCUSPERCSTATIC )->GetElement(0)->SetTexture(1,&focusrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_AHATEPERCSTATIC, L"", 77,48,0,4);
	g_TopUI.GetStatic( IDC_AHATEPERCSTATIC )->GetElement(0)->SetTexture(1,&haterect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TopUI.AddStatic( IDC_AENERGYPERCSTATIC, L"", 77,58,0,4);
	g_TopUI.GetStatic( IDC_AENERGYPERCSTATIC )->GetElement(0)->SetTexture(1,&energyrect,D3DCOLOR_ARGB(255, 255, 255, 255));

	g_TargetUI.AddStatic( IDC_ENAME, L"...", 156,14,50,12);
	g_TargetUI.AddStatic( IDC_EHPSTATIC, L"", 221,16,59,8);
	g_TargetUI.GetStatic( IDC_EHPSTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EMAGIKASTATIC, L"", 221,26,59,8);
	g_TargetUI.GetStatic( IDC_EMAGIKASTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EFOCUSSTATIC, L"", 221,36,59,8);
	g_TargetUI.GetStatic( IDC_EFOCUSSTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EHATESTATIC, L"", 221,46,59,8);
	g_TargetUI.GetStatic( IDC_EHATESTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EENERGYSTATIC, L"", 221,56,59,8);
	g_TargetUI.GetStatic( IDC_EENERGYSTATIC )->GetElement(0)->SetTexture(1,&emptyrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EAHPPERCSTATIC, L"", 223,18,0,4);
	g_TargetUI.GetStatic( IDC_EAHPPERCSTATIC )->GetElement(0)->SetTexture(1,&healthrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EAMAGIKAPERCSTATIC, L"", 223,28,0,4);
	g_TargetUI.GetStatic( IDC_EAMAGIKAPERCSTATIC )->GetElement(0)->SetTexture(1,&magikarect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EAFOCUSPERCSTATIC, L"", 223,38,0,4);
	g_TargetUI.GetStatic( IDC_EAFOCUSPERCSTATIC )->GetElement(0)->SetTexture(1,&focusrect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EAHATEPERCSTATIC, L"", 223,48,0,4);
	g_TargetUI.GetStatic( IDC_EAHATEPERCSTATIC )->GetElement(0)->SetTexture(1,&haterect,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_TargetUI.AddStatic( IDC_EAENERGYPERCSTATIC, L"", 223,58,0,4);
	g_TargetUI.GetStatic( IDC_EAENERGYPERCSTATIC )->GetElement(0)->SetTexture(1,&energyrect,D3DCOLOR_ARGB(255, 255, 255, 255));

	g_CharacterUI.AddStatic( IDC_LEVELSTATIC, L"", 10,540,50,12);
	g_CharacterUI.AddStatic( IDC_XPSTATIC, L"X:", 10,552,50,12);
	g_CharacterUI.AddStatic( IDC_MONEYSTATIC, L"Z:", 10,564,50,12);
	g_CharacterUI.AddStatic( IDC_ALEVELSTATIC, L"", 50,540,50,12);
	g_CharacterUI.AddStatic( IDC_AXPSTATIC, L"", 50,552,50,12);
	g_CharacterUI.AddStatic( IDC_AMONEYSTATIC, L"", 50,564,50,12);

	g_SpellInfoUI.AddStatic( 0, L"", 0,0,400,200);
	g_SpellInfoUI.GetStatic( 0 )->GetElement(0)->SetTexture(1,&testrect2,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_SpellInfoUI.AddStatic( 1, L"Name", 0,10,149,12);
	g_SpellInfoUI.AddStatic( 2, L"10M/10F/10H/10E", 0,24,100,12);
	g_SpellInfoUI.AddStatic( 3, L"0-25 Yards", 100,24,100,12);
	g_SpellInfoUI.AddStatic( 4, L"Instant", 0,38,100,12);
	g_SpellInfoUI.AddStatic( 5, L"1min Cooldown", 60,38,100,12);
	g_SpellInfoUI.AddStatic( 6, L"Description", 0,44,400,100);
	g_SpellInfoUI.AddStatic( 7, L"Description 2", 0,56,400,100);
	g_SpellInfoUI.AddStatic( 8, L"Description 3", 0,68,400,100);
	g_SpellInfoUI.SetVisible(false);

	RECT castborder= {0,73,136,94};
	RECT castbar= {0,93,130,110};
	g_CastBarUI.AddStatic( IDC_CASTBORDER, L"", -68,100,136,20);
	g_CastBarUI.GetStatic( IDC_CASTBORDER )->GetElement(0)->SetTexture(1,&castborder,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_CastBarUI.AddStatic( IDC_CASTBAR, L"", -65,102,0,16);
	g_CastBarUI.GetStatic( IDC_CASTBAR )->GetElement(0)->SetTexture(1,&castbar,D3DCOLOR_ARGB(255, 255, 255, 255));
	g_CastBarUI.AddStatic( IDC_CASTNUM, L"", 68,100,30,12);
	g_CastBarUI.SetVisible(false);
	LoadSpells();
	LoadItems();
	LoadAuras();
	serverip="";
	LoadServerIP();
	gamestats.blueteam=0;
	gamestats.redteam=0;
}

/*
//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
								 D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	// Skip backbuffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object(); 
	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	// Must support pixel shader 2.0
	if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
		return false;

	return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

	HRESULT hr;
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	D3DCAPS9 caps;

	V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
		pDeviceSettings->d3d9.DeviceType,
		&caps ) );

	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
	// then switch to SWVP.
	if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
	{
		pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// Debugging vertex shaders requires either REF or software vertex processing 
	// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
	if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
	{
		pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;                            
		pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
#endif
#ifdef DEBUG_PS
	pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif

	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if( s_bFirstTime )
	{
		s_bFirstTime = false;
		if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
			DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
	}

	return true;
}*/

/*
//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
	V_RETURN( g_SettingsDlg.OnD3D9CreateDevice( pd3dDevice ) );
	// Initialize the font
	V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		L"Arial", &g_pFont ) );

	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif
#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	// Read the D3DX effect file
	//WCHAR str[MAX_PATH];
	//V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"media\\Ighagaruu.fx" ) );
	V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, L"media\\Ighagaruu.fx", NULL, NULL, dwShaderFlags, 
		NULL, &g_pEffect, NULL ) );
	D3DXCreateTextureFromFileEx(pd3dDevice,    // the device pointer
		L"media\\rings.dds",    // the new file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&Tex);    // load to sprite
	font=CreateFont(10,         //Height
		0,          //Width
		0,          //Escapement
		0,          //Orientation
		FW_NORMAL,  //Weight
		false,      //Italic
		false,      //Underline
		false,      //Strikeout
		DEFAULT_CHARSET,//Charset 
		OUT_DEFAULT_PRECIS,  //Output Precision
		CLIP_DEFAULT_PRECIS, //Clipping Precision
		DEFAULT_QUALITY,     //Quality
		DEFAULT_PITCH|FF_DONTCARE, //Pitch and Family
		L"Arial");

	g_IntroMesh.Create( pd3dDevice, L"media\\introscene.x" );
	//g_IntroMesh.Create( pd3dDevice, L"media\\main.x" );
	//g_QuadMesh.Create( pd3dDevice, L"media\\quad.x" );
	g_QuadMesh.Create( pd3dDevice, L"media\\target.x" );
	g_CharBackgroundMesh.Create( pd3dDevice, L"media\\charchoose.x" );
	g_Mesh.Create( pd3dDevice, L"media\\main.x" );

	g_Avatar[0].Create( pd3dDevice, L"media\\moai.x" );
	yadjust[0]=2.5f;
	g_Avatar[1].Create( pd3dDevice, L"media\\moai.x" );
	yadjust[1]=2.5f;
	g_Avatar[2].Create( pd3dDevice, L"media\\hagamel.x" );
	yadjust[2]=0.0f;
	g_Avatar[3].Create( pd3dDevice, L"media\\moai.x" );
	yadjust[3]=2.5f;

	g_Avatar[4].Create( pd3dDevice, L"media\\flagrage.x" );
	yadjust[4]=2.5f;
	g_Avatar[5].Create( pd3dDevice, L"media\\flagrage.x" );
	yadjust[5]=2.5f;
	g_Avatar[6].Create( pd3dDevice, L"media\\eye.x" );
	yadjust[6]=0.5f;
	g_Avatar[7].Create( pd3dDevice, L"media\\flagrage.x" );
	yadjust[7]=2.5f;

	g_Mesh3.Create( pd3dDevice, L"media\\tree.x" );
	g_Flag.Create( pd3dDevice, L"media\\flag.x" );
	// Setup the camera's view parameters
	D3DXVECTOR3 vecEye(0.0f, 1.2f, 5.0f);
	D3DXVECTOR3 vecAt (0.0f, 0.0f, -0.0f);
	g_Camera.SetViewParams( &vecEye, &vecAt );
	g_Camera.SetRadius(5.0f,1.0f,10.0f);
	//D3DXMatrixLookAtLH( &g_mView, &vecEye, &vecAt, &vecUp );
	D3DXVECTOR3 staticcvecEye(-20.0f, 3.0f, 10.0f);
	D3DXVECTOR3 staticcvecAt (0.0f, 0.0f, 0.0f);
	g_StaticCamera.SetViewParams( &staticcvecEye, &staticcvecAt );
	g_mMapWorld=*g_Camera.GetWorldMatrix(); 
	return S_OK;
}*/

/*
//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
							   const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr;

	V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );
	V_RETURN( g_SettingsDlg.OnD3D9ResetDevice() );

	if( g_pFont )
		V_RETURN( g_pFont->OnResetDevice() );
	if( g_pEffect )
		V_RETURN( g_pEffect->OnResetDevice() );

	// Create a sprite to help batch calls when drawing many lines of text
	V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pTextSprite ) );

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 5000.0f );
	g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	//float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_StaticCamera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 5000.0f );
	g_StaticCamera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	*/
void OnResetDevice(const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
	g_TopUI.SetLocation( 0, 0 );
	g_TopUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_CharacterChooseUI.SetLocation((((pBackBufferSurfaceDesc->Width-800)/2)), (((pBackBufferSurfaceDesc->Height-600)/2)));
	g_TargetUI.SetLocation( 0, 0 );
	g_TargetUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_OptionsUI.SetLocation( pBackBufferSurfaceDesc->Width/2 -62, pBackBufferSurfaceDesc->Height/2 -47);
	g_OptionsUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_SpellInfoUI.SetLocation( pBackBufferSurfaceDesc->Width/2 -62, pBackBufferSurfaceDesc->Height/2 -47);
	g_SpellInfoUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_CastBarUI.SetLocation( pBackBufferSurfaceDesc->Width/2, pBackBufferSurfaceDesc->Height/2);
	g_CastBarUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_LoginPopUp.SetLocation( pBackBufferSurfaceDesc->Width/2 -62, pBackBufferSurfaceDesc->Height/2 -47);
	g_LoginUI.SetLocation( pBackBufferSurfaceDesc->Width/2 -62, pBackBufferSurfaceDesc->Height/2 -47);
	g_CharacterUI.SetLocation((((pBackBufferSurfaceDesc->Width-800)/2)), pBackBufferSurfaceDesc->Height-600);
	g_CharacterUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	g_RedTextUI.SetLocation((((pBackBufferSurfaceDesc->Width-800)/2)), 100);
	g_RedTextUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

	g_ScoreUI.SetLocation( pBackBufferSurfaceDesc->Width-146, 10);
}
	/*g_Mesh.RestoreDeviceObjects( pd3dDevice );
	g_Avatar[0].RestoreDeviceObjects( pd3dDevice );
	g_Mesh3.RestoreDeviceObjects( pd3dDevice );
	g_Flag.RestoreDeviceObjects( pd3dDevice );
	return S_OK;
}*/


//--------------------------------------------------------------------------------------
// Handle updates to the scene.
//--------------------------------------------------------------------------------------
void OnFrameMove(float  fTime)
{
	if (myChar!=NULL) {
		CharacterFrameMove(fTime);
		g_Camera.Update(fTime);
	}
}

void CharacterFrameMove(float fTime) {
	float fElapsedTime = fTime;
	int timeDiff = (int)fTime - (int)lastTime;
	if (timeDiff>=1) {
			SetSpells();
			SetInventory();
			if (redTextTime>0) {
				redTextTime--;
			}
	}
	int timeDiff2 = (int)(fTime*1000 - lastTime*1000);
	myChar->tickCast(timeDiff2);
	lastTime=fTime;

	if (redTextTime<=0) {
		g_RedTextUI.GetStatic(IDC_REDSTATIC)->SetText(L"");
	}
	float totalspeedchange=100;
	for (unsigned int i=0; i<myChar->auras.size(); i++) {
		totalspeedchange+=(float)myChar->auras.at(i).aura->aura_speed;
	}
	if (totalspeedchange<0.0f) {totalspeedchange=0.0f;}
	float f1;
	float f2;
	float newY=myChar->pos.y;
	float newX=myChar->pos.x;
	float newZ=myChar->pos.z;
	float f3=3.0f*fElapsedTime;
	float f4=16.0f*fElapsedTime*(totalspeedchange/100);    
	if (g_bTurnRight ) {
		myChar->pos.orientation-=2.0f*fElapsedTime;
	} else if (g_bTurnLeft) {//
		myChar->pos.orientation+=2.0f*fElapsedTime;
	}
	newY=myChar->pos.y+f3;
	if (newY >= 0.0f) {
		myChar->pos.y=0.0f;
	} else if (newY <=-500.0f) {
		myChar->pos.y=-500.0f;
	} else {
		myChar->pos.y=newY;
	}
	if (g_bFoward || g_bBackward || g_bRight || g_bLeft) {
		int avint=myChar->info.model;
		if(!lFoot[avint]->IsSoundPlaying() && !rFoot[avint]->IsSoundPlaying() ) {
			if (step) {
				step=false;
				lFoot[avint]->Reset();
				lFoot[avint]->Play(0,0,(LONG) 1);
			} else {
				step=true;
				rFoot[avint]->Reset();
				rFoot[avint]->Play(0,0,(LONG) 1);
			}
		} 
		if (g_bFoward ) {
			f1=f4;
			myChar->info.casting=false;
			myChar->info.casttime=0;
		} else if (g_bBackward) {
			f1=-f4;
			myChar->info.casting=false;
			myChar->info.casttime=0;
		} else {
			f1=0;
		}
		if (g_bRight ) {
			f2=f4;
		} else if (g_bLeft) {//
			f2=-f4;
		} else {
			f2=0;
		}
		POVector3 vAvatar(f2,0,f1);
	//	D3DXMATRIX mRot;
	//	D3DXMatrixRotationY( &mRot, myChar->pos.orientation );
	//	D3DXMatrixInverse(&mRot, NULL, &mRot);
	//	D3DXVec3TransformCoord( &vAvatar, &vAvatar, &mRot );
		newX=myChar->pos.x+vAvatar._1;
		newZ=myChar->pos.z+vAvatar._3;

		if (newX >= 100.0f) {
			myChar->pos.x=100.0f;
		} else if (newX <= -100.0f) {
			myChar->pos.x=-100.0f;
		} else {
			myChar->pos.x=newX;
		}
		if (newZ >= 100.0f) {
			myChar->pos.z=100.0f;
		} else if (newZ <= -100.0f) {
			myChar->pos.z=-100.0f;
		} else {
			myChar->pos.z=newZ;
		}
	} else {
				int avint=myChar->info.model;
				lFoot[avint]->Stop();
				rFoot[avint]->Stop();
	}

	g_TopUI.GetStatic( IDC_ANAME)->SetText(myChar->getName());
	
	g_TopUI.GetStatic( IDC_AHPPERCSTATIC)->SetSize(((myChar->stats.health)*55)/(myChar->stats.maxhealth),4);
	g_TopUI.GetStatic( IDC_AMAGIKAPERCSTATIC)->SetSize(((myChar->stats.magika)*55)/(myChar->stats.maxmagika),4);
	g_TopUI.GetStatic( IDC_AFOCUSPERCSTATIC)->SetSize(((myChar->stats.focus)*55)/1000,4);
	g_TopUI.GetStatic( IDC_AHATEPERCSTATIC)->SetSize(((myChar->stats.hate)*55)/100,4);
	g_TopUI.GetStatic( IDC_AENERGYPERCSTATIC)->SetSize(((myChar->stats.energy)*55)/100,4);

	g_CharacterUI.GetStatic( IDC_AMONEYSTATIC)->SetText( Tokenizer::intToString((int)myChar->pos.x).c_str());
	g_CharacterUI.GetStatic( IDC_AXPSTATIC)->SetText( Tokenizer::intToString((int)myChar->pos.z).c_str());
	if (target!=NULL) {
		if (target->stats.health>0) {
			g_TargetUI.SetVisible(true);
			g_TargetUI.GetStatic( IDC_ENAME)->SetText(target->getName());
			g_TargetUI.GetStatic( IDC_EAHPPERCSTATIC)->SetSize(((target->stats.health)*55)/(target->stats.maxhealth),4);
			g_TargetUI.GetStatic( IDC_EAMAGIKAPERCSTATIC)->SetSize(((target->stats.magika)*55)/(target->stats.maxmagika),4);
			g_TargetUI.GetStatic( IDC_EAFOCUSPERCSTATIC)->SetSize(((target->stats.focus)*55)/1000,4);
			g_TargetUI.GetStatic( IDC_EAHATEPERCSTATIC)->SetSize(((target->stats.hate)*55)/100,4);
			g_TargetUI.GetStatic( IDC_EAENERGYPERCSTATIC)->SetSize(((target->stats.energy)*55)/100,4);
		} else {
			g_TargetUI.SetVisible(false);
			target=NULL;
		}
	} else {
		g_TargetUI.SetVisible(false);
	}
	for (int i=0; i<11; i++) {
		if (g_CharacterUI.GetButton(IDC_SPELLCAST1+i)->_mouseOver) {
			g_SpellInfoUI.GetStatic(1)->SetText(myChar->inv.spells[i]->getName());
			g_SpellInfoUI.GetStatic(2)->SetText(myChar->inv.spells[i]->getUsages());
			g_SpellInfoUI.GetStatic(3)->SetText(myChar->inv.spells[i]->getRange());
			g_SpellInfoUI.GetStatic(4)->SetText(myChar->inv.spells[i]->getCastTime());
			g_SpellInfoUI.GetStatic(5)->SetText(myChar->inv.spells[i]->getCooldown());
			g_SpellInfoUI.GetStatic(6)->SetText(myChar->inv.spells[i]->getDescription(0));
			g_SpellInfoUI.GetStatic(7)->SetText(myChar->inv.spells[i]->getDescription(1));
			g_SpellInfoUI.GetStatic(8)->SetText(myChar->inv.spells[i]->getDescription(2));
			g_SpellInfoUI.SetVisible(true);
			break;
		} else {
			g_SpellInfoUI.SetVisible(false);
		}
	}
	if (myChar->info.casting!=NULL) {
		g_CastBarUI.GetStatic(IDC_CASTBAR)->SetSize(((myChar->info.casting->spell_length-myChar->info.casttime)*130)/(myChar->info.casting->spell_length),16);
		g_CastBarUI.GetStatic(IDC_CASTNUM)->SetText(myChar->getCastText());
		g_CastBarUI.SetVisible(true);
	} else {
		g_CastBarUI.SetVisible(false);
	}
	g_ScoreUI.GetStatic(IDC_HSCORE)->SetText(Tokenizer::intToString(gamestats.redteam).c_str());
	g_ScoreUI.GetStatic(IDC_CSCORE)->SetText(Tokenizer::intToString(gamestats.blueteam).c_str());
}


//--------------------------------------------------------------------------------------
// Render the scene
//--------------------------------------------------------------------------------------
void CharacterFrameRender(float fTime) {}
void ChooseFrameRender(float fTime) {}
void OnFrameRender(float fTime)
{
	Renderer->BeginScene();
		if (myChar!=NULL) {
			CharacterFrameRender(fTime);
		} else {
			if (loggedin) {
				ChooseFrameRender(fTime);
			} else {
				IntroFrameRender(fTime);
			}
		}
	Renderer->EndScene();
}
float z;
void IntroFrameRender(float fTime) {
	z+=0.11f;
	float y = z;
	/*HRESULT hr;
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;
	// Get the projection & view matrix from the camera class
	mWorld = *g_StaticCamera.GetWorldMatrix();       
	mProj = *g_StaticCamera.GetProjMatrix();       
	mView = *g_StaticCamera.GetViewMatrix();
	D3DXMATRIXA16 mIntro;
	D3DXMATRIXA16 mRot;
	D3DXMATRIXA16 mRot2;
	D3DXMATRIXA16 mTrans;
	D3DXMATRIXA16 mAgg;
	D3DXMatrixTranslation( &mTrans, -100.0f,
		-10.0f,
		60.0f);
	float z = sin ((float)fTime/10)/8.0f;
	float y = sin ((float)fTime/10)/8.0f;
	D3DXMatrixRotationZ( &mRot2, z);     
	D3DXMatrixRotationY( &mRot, y+2.25f);      
	D3DXMatrixMultiply(&mTrans, &mTrans, &mRot2 );
	D3DXMatrixMultiply(&mAgg, &mTrans, &mRot );
	mWorld = *g_StaticCamera.GetWorldMatrix() *mAgg; 
	mWorldViewProjection = mWorld * mView * mProj;
	V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
	V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
	V( g_pEffect->SetFloat( "g_fTime", (float)fTime ) );

	g_pEffect->SetTechnique( "RenderScene" );

	UINT cPasses;
	g_pEffect->Begin( &cPasses, 0 );
	ID3DXMesh *pMesh = g_IntroMesh.GetMesh();
	pd3dDevice->SetFVF( g_IntroMesh.m_pMesh->GetFVF() );

	for( UINT p = 0; p < cPasses; ++p )
	{
		g_pEffect->BeginPass( p );
		for( UINT m = 0; m < g_IntroMesh.m_dwNumMaterials; ++m )
		{
			g_pEffect->SetTexture( "g_txScene", g_IntroMesh.m_pTextures[m] );
			g_pEffect->CommitChanges();
			pMesh->DrawSubset( m );
		}
		g_pEffect->EndPass();
	}
	g_pEffect->End();*/
	Renderer->SetProjection();
	Renderer->SetView(POVector3(10.0f, 13.0f, 60.0f),POVector3(0.0f,0.0f,0.0f));		// Set view matrix
	Renderer->DrawMesh(0,POVector3(-90.0f, 0.0f,0.0f), POVector3(0.0f,sin ((float)y/10)/8.0f,sin ((float)y/10)/8.0f));
	g_LoginUI.Render( fTime );
	g_LoginPopUp.Render( fTime );
}
/*
void ChooseFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext ) {
	HRESULT hr;
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;

	// Clear the render target and the zbuffer 

	// Get the projection & view matrix from the camera class
	mWorld = *g_StaticCamera.GetWorldMatrix();       
	mProj = *g_StaticCamera.GetProjMatrix();       
	mView = *g_StaticCamera.GetViewMatrix();

	mWorldViewProjection = mWorld * mView * mProj;

	V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
	V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
	V( g_pEffect->SetFloat( "g_fTime", (float)fTime ) );

	g_pEffect->SetTechnique( "RenderScene" );

	UINT cPasses;
	g_pEffect->Begin( &cPasses, 0 );
	ID3DXMesh *pMesh = g_CharBackgroundMesh.GetMesh();
	pd3dDevice->SetFVF( g_CharBackgroundMesh.m_pMesh->GetFVF() );

	for( UINT p = 0; p < cPasses; ++p )
	{
		g_pEffect->BeginPass( p );
		for( UINT m = 0; m < g_CharBackgroundMesh.m_dwNumMaterials; ++m )
		{

			g_pEffect->SetTexture( "g_txScene", g_CharBackgroundMesh.m_pTextures[m] );
			g_pEffect->CommitChanges();
			pMesh->DrawSubset( m );
		}
		g_pEffect->EndPass();
	}
	D3DXMATRIXA16 mRot=*g_StaticCamera.GetWorldMatrix();
	D3DXMatrixRotationY( &mRot, (float)fTime);      
	mWorld = *g_StaticCamera.GetWorldMatrix() *mRot; 
	mWorldViewProjection = mWorld * mView * mProj;
	V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
	V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );

	Character* chosenChar = (Character*)g_CharacterChooseUI.GetListBox(IDC_CHARLIST)->GetSelectedItem()->pData;
	int avint=chosenChar->info.model;
	
	
	pMesh = g_Avatar[avint].GetMesh();
	pd3dDevice->SetFVF( g_Avatar[avint].m_pMesh->GetFVF() );
	for( UINT p = 0; p < cPasses; ++p )
	{
		g_pEffect->BeginPass( p );
		for( UINT m = 0; m < g_Avatar[avint].m_dwNumMaterials; ++m )
		{
			g_pEffect->SetTexture( "g_txScene", g_Avatar[avint].m_pTextures[m] );
			g_pEffect->CommitChanges();
			pMesh->DrawSubset( m );
		}
		g_pEffect->EndPass();
	}
	g_pEffect->End();
	V( g_CharacterChooseUI.OnRender( fElapsedTime ) );
}

void CharacterFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext ) {
	HRESULT hr;
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;

	// Clear the render target and the zbuffer 

	// Get the projection & view matrix from the camera class
	D3DXMATRIXA16 mCameraTrans;
	D3DXMatrixTranslation( &mCameraTrans, 0.0f,
		-(yadjust[myChar->info.classs+(myChar->info.faction*4)]+0.6f),
		0.0f);
	mWorld = *g_Camera.GetWorldMatrix() *mCameraTrans;       
	mProj = *g_Camera.GetProjMatrix();       
	mView = *g_Camera.GetViewMatrix();

	mWorldViewProjection = mWorld * mView * mProj;

	// Update the effect's variables.  Instead of using strings, it would 
	// be more efficient to cache a handle to the parameter by calling 
	// ID3DXEffect::GetParameterByName
	V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
	V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
	V( g_pEffect->SetFloat( "g_fTime", (float)fTime ) );

	g_mMapWorld=*g_Camera.GetWorldMatrix();
	D3DXMATRIXA16 mRot;
	D3DXMATRIXA16 mTrans;
	D3DXMATRIXA16 mAgg;
	D3DXMatrixTranslation( &mTrans, myChar->pos.x,
		myChar->pos.y,
		myChar->pos.z);
	D3DXMatrixRotationY( &mRot, myChar->pos.orientation);      
	D3DXMatrixMultiply(&mAgg, &mTrans, &mRot );
	D3DXMatrixMultiply(&g_mMapWorld, &g_mMapWorld,&mAgg );
	mWorld = *g_Camera.GetWorldMatrix() *g_mMapWorld; 
	mWorldViewProjection = mWorld * mView * mProj;
	D3DXMATRIXA16 mWorldView = mWorld * mView;
	V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
	V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
	V( g_pEffect->SetFloat( "g_fTime", (float)fTime ) );

	g_pEffect->SetTechnique( "RenderScene" );
	UINT cPasses;
	g_pEffect->Begin( &cPasses, 0 );
	ID3DXMesh *pMesh= g_Mesh.GetMesh();;
	pd3dDevice->SetFVF( g_Mesh.m_pMesh->GetFVF() );

	for( UINT p = 0; p < cPasses; ++p )
	{
		g_pEffect->BeginPass( p );
		for( UINT m = 0; m < g_Mesh.m_dwNumMaterials; ++m )
		{

			g_pEffect->SetTexture( "g_txScene", g_Mesh.m_pTextures[m] );
			g_pEffect->CommitChanges();
			pMesh->DrawSubset( m );
		}
		g_pEffect->EndPass();
	}



	//

		D3DXMATRIXA16 mCharWorld=*g_Camera.GetWorldMatrix();
		D3DXMATRIXA16 mRot2;
		D3DXMATRIXA16 mTrans2;
		D3DXMATRIXA16 mAgg2;
		D3DXMatrixRotationY( &mRot2, -gamestats.fo);      
		D3DXMatrixTranslation( &mTrans2, -gamestats.fx,
			-gamestats.fy,
			-gamestats.fz);
		D3DXMatrixMultiply(&mAgg2, &mRot2,&mTrans2 );

		D3DXMatrixMultiply(&mCharWorld, &mCharWorld,&mAgg2 );

		D3DXMATRIXA16 mRot3;
		D3DXMATRIXA16 mTrans3;
		D3DXMATRIXA16 mAgg3;
		D3DXMatrixTranslation( &mTrans3, myChar->pos.x,
			myChar->pos.y,
			myChar->pos.z);
		D3DXMatrixRotationY( &mRot3, myChar->pos.orientation);      
		D3DXMatrixMultiply(&mAgg3, &mTrans3, &mRot3 );

		D3DXMatrixMultiply(&mCharWorld, &mCharWorld,&mAgg3 );

		mWorld = *g_Camera.GetWorldMatrix()* mCharWorld; 
		mWorldViewProjection = mWorld * mView * mProj;
		V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
		V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
		pMesh = g_Flag.GetMesh();
		pd3dDevice->SetFVF( g_Flag.m_pMesh->GetFVF() );
		for( UINT p = 0; p < cPasses; ++p )
		{
			g_pEffect->BeginPass( p );
			for( UINT m = 0; m < g_Flag.m_dwNumMaterials; ++m )
			{
				g_pEffect->SetTexture( "g_txScene", g_Flag.m_pTextures[m] );
				g_pEffect->CommitChanges();
				pMesh->DrawSubset( m );
			}
			g_pEffect->EndPass();
		}
	///
	for (unsigned int h=0; h < characters.size(); h++) {
		if (target!=NULL && characters.at(h).id==target->id){
			D3DXMATRIXA16 mCharWorld=*g_Camera.GetWorldMatrix();
			D3DXMATRIXA16 mTrans2;
			D3DXMatrixTranslation( &mTrans2, -characters.at(h).pos.x,
				-characters.at(h).pos.y+yadjust[characters.at(h).info.classs+(characters.at(h).info.faction*4)],
				-characters.at(h).pos.z);

			D3DXMatrixMultiply(&mCharWorld, &mCharWorld,&mTrans2 );

			D3DXMATRIXA16 mRot3;
			D3DXMATRIXA16 mTrans3;
			D3DXMATRIXA16 mAgg3;
			D3DXMatrixTranslation( &mTrans3, myChar->pos.x,
				myChar->pos.y,
				myChar->pos.z);
			D3DXMatrixRotationY( &mRot3, myChar->pos.orientation);      
			D3DXMatrixMultiply(&mAgg3, &mTrans3, &mRot3 );

			D3DXMatrixMultiply(&mCharWorld, &mCharWorld,&mAgg3 );

			mWorld = *g_Camera.GetWorldMatrix()* mCharWorld; 
			mWorldViewProjection = mWorld * mView * mProj;
			V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
			V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
			
			pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, true);
			pd3dDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
			pd3dDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
			pd3dDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
			pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
			pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			pd3dDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			pd3dDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
			pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
			pMesh = g_QuadMesh.GetMesh();
			pd3dDevice->SetFVF( g_QuadMesh.m_pMesh->GetFVF() );
			for( UINT p = 0; p < cPasses; ++p )
			{
				g_pEffect->BeginPass( p );
				for( UINT m = 0; m < g_QuadMesh.m_dwNumMaterials; ++m )
				{
					//g_pEffect->SetTexture( "g_txScene", Tex );
					g_pEffect->SetTexture( "g_txScene", g_QuadMesh.m_pTextures[m] );
					g_pEffect->CommitChanges();
					pMesh->DrawSubset( m );
				}
				g_pEffect->EndPass();
			}
			pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
			pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, false);
			//
		}

		D3DXMATRIXA16 mCharWorld=*g_Camera.GetWorldMatrix();
		D3DXMATRIXA16 mRot2;
		D3DXMATRIXA16 mTrans2;
		D3DXMATRIXA16 mAgg2;
		D3DXMatrixRotationY( &mRot2, -characters.at(h).pos.orientation);      
		D3DXMatrixTranslation( &mTrans2, -characters.at(h).pos.x,
			-characters.at(h).pos.y,
			-characters.at(h).pos.z);
		D3DXMatrixMultiply(&mAgg2, &mRot2,&mTrans2 );

		D3DXMatrixMultiply(&mCharWorld, &mCharWorld,&mAgg2 );

		D3DXMATRIXA16 mRot3;
		D3DXMATRIXA16 mTrans3;
		D3DXMATRIXA16 mAgg3;
		D3DXMatrixTranslation( &mTrans3, myChar->pos.x,
			myChar->pos.y,
			myChar->pos.z);
		D3DXMatrixRotationY( &mRot3, myChar->pos.orientation);      
		D3DXMatrixMultiply(&mAgg3, &mTrans3, &mRot3 );

		D3DXMatrixMultiply(&mCharWorld, &mCharWorld,&mAgg3 );

		mWorld = *g_Camera.GetWorldMatrix()* mCharWorld; 
		mWorldViewProjection = mWorld * mView * mProj;
		V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
		V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
		int avint=characters.at(h).info.model;
		pMesh = g_Avatar[avint].GetMesh();
		pd3dDevice->SetFVF( g_Avatar[avint].m_pMesh->GetFVF() );
		for( UINT p = 0; p < cPasses; ++p )
		{
			g_pEffect->BeginPass( p );
			for( UINT m = 0; m < g_Avatar[avint].m_dwNumMaterials; ++m )
			{
				g_pEffect->SetTexture( "g_txScene", g_Avatar[avint].m_pTextures[m] );
				g_pEffect->CommitChanges();
				pMesh->DrawSubset( m );
			}
			g_pEffect->EndPass();
		}

	}



	mWorld = *g_Camera.GetWorldMatrix() *mCameraTrans;       
	mProj = *g_Camera.GetProjMatrix();       
	mView = *g_Camera.GetViewMatrix();

	mWorldViewProjection = mWorld * mView * mProj;

	// Update the effect's variables.  Instead of using strings, it would 
	// be more efficient to cache a handle to the parameter by calling 
	// ID3DXEffect::GetParameterByName
	V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
	V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
	V( g_pEffect->SetFloat( "g_fTime", (float)fTime ) );
	int avint=myChar->info.model;
	pMesh = g_Avatar[avint].GetMesh();
	pd3dDevice->SetFVF( g_Avatar[avint].m_pMesh->GetFVF() );

	for( UINT p = 0; p < cPasses; ++p )
	{
		g_pEffect->BeginPass( p );
		for( UINT m = 0; m < g_Avatar[avint].m_dwNumMaterials; ++m )
		{

			g_pEffect->SetTexture( "g_txScene", g_Avatar[avint].m_pTextures[m] );
			g_pEffect->CommitChanges();
			pMesh->DrawSubset( m );
		}
		g_pEffect->EndPass();
	}

	////
	g_pEffect->End();
	V( g_RedTextUI.OnRender( fElapsedTime ) );
	V( g_TopUI.OnRender( fElapsedTime ) );
	V( g_ScoreUI.OnRender( fElapsedTime ) );
	V( g_OptionsUI.OnRender( fElapsedTime ) );
	V( g_CharacterUI.OnRender( fElapsedTime ) );
	V( g_TargetUI.OnRender( fElapsedTime ) );
	V( g_SpellInfoUI.OnRender( fElapsedTime ) );
	V( g_CastBarUI.OnRender( fElapsedTime ) );

}*/

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
bool Event(UINT msg, UINT p1, UINT p2)
{
	// Always allow dialog resource manager calls to handle global messages
	// so GUI state is updated correctly
	//*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	//if( *pbNoFurtherProcessing )
	//	return 0;

	//if( g_SettingsDlg.IsActive() )
	//{
	//	g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
	//	return 0;
	//}

	// Give the dialogs a chance to handle the message first
	if (myChar!=NULL) {
		if(g_TopUI.Event(msg, p1, p2))
			return false;
		if(g_OptionsUI.Event(msg, p1, p2))
			return false;
		if(g_CharacterUI.Event(msg, p1, p2))
			return false;

		// Pass all remaining windows messages to camera so it can respond to user input
		//g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );


		/*switch( uMsg )
		{
		case WM_LBUTTONDOWN:
			{
			ShowCursor(FALSE);
			break;
			}
		case WM_RBUTTONDOWN:
			g_bRightMouseDown = true;
			ShowCursor(FALSE);
			break;
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONUP:
			g_bRightMouseDown = false;
			ShowCursor(TRUE);
			break;
		case WM_LBUTTONUP:
			ShowCursor(TRUE);
			break;
		case WM_CAPTURECHANGED:
			break;
		}*/
	} else {
		if (loggedin) {
			if(g_CharacterChooseUI.Event(msg, p1, p2))
				return false;
		} else {
			if(g_LoginPopUp.Event(msg, p1, p2))
				return false;
			if( g_LoginUI.Event(msg, p1, p2))
				return false;
		}
	}

	return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if (myChar!=NULL) {
		CharacterKeyBoardProc( nChar, bKeyDown, bAltDown, pUserContext );
	} else {
		if (loggedin) {
		} else {
		}
	}
}
void CharacterKeyBoardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( bKeyDown )
	{
		switch( nChar )
		{
		case 27: if (g_CharacterUI.GetEditBox(IDC_EDITBOX1)->GetVisible()) {g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetVisible(false);g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetTextColor( D3DCOLOR_ARGB( 255, 0, 0, 0 ) );movementDisabled=false;} else {if (g_OptionsUI.GetVisible()) {g_OptionsUI.SetVisible(false); } else { g_OptionsUI.SetVisible(true); }}; break;
		case 191: if (!g_CharacterUI.GetEditBox(IDC_EDITBOX1)->GetVisible()) {g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetVisible(true); movementDisabled=true;g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetText(L"");g_CharacterUI.RequestFocus(g_CharacterUI.GetEditBox(IDC_EDITBOX1));}; break;
		case 87: {
			if (!movementDisabled) {g_bFoward=true;}
			//D3DXVECTOR3 vecEye(0.0f, 1.2f, 5.0f);
			//D3DXVECTOR3 vecAt (0.0f, 0.0f, -0.0f);
			//g_Camera.SetViewParams( &vecEye, &vecAt );
			//g_Camera.SetRadius(5.0f,1.0f,10.0f);
			break;
			}
		case 38: {
			if (!movementDisabled) {g_bFoward=true;}
			//D3DXVECTOR3 vecEye(0.0f, 1.2f, 5.0f);
			//D3DXVECTOR3 vecAt (0.0f, 0.0f, -0.0f);
			//g_Camera.SetViewParams( &vecEye, &vecAt );
			//g_Camera.SetRadius(5.0f,1.0f,10.0f);
			break;
			}
		case 65: if (!movementDisabled) {g_bLeft=true; }break;
		case 68: if (!movementDisabled) {g_bRight=true;} break;
		case 83: if (!movementDisabled) {g_bBackward=true; }break;
		case 40: if (!movementDisabled) {g_bBackward=true;} break;
		case 39: if (!movementDisabled) {g_bTurnRight=true; }break;
		case 37: if (!movementDisabled) {g_bTurnLeft=true; }break;
		//case 32: if (!movementDisabled) {char sendbuff[512]; sendbuff[0]=11;send( ConnectSocket, sendbuff, 512, 0 );}break; ///spacebar
		case 9: {
			if (characters.size()>0) {
				if (target==NULL) {
					Target(&characters.at(0));
				} else {
					bool found=false;
					for (unsigned int i=0; i<characters.size()&&!found; i++){
						if(characters.at(i).id==target->id) {
							found=true;
							i++;
							i=i%characters.size();
							Target(&characters.at(i));
						}
					}
				}
			}
			break;
				}
		}
	} else {
		switch( nChar )
		{
		case 87: g_bFoward=false; break;
		case 38: g_bFoward=false; break;
		case 65: g_bLeft=false; break;
		case 68: g_bRight=false; break;
		case 83: g_bBackward=false; break;
		case 40: g_bBackward=false; break;
		case 39: g_bTurnRight=false; break;
		case 37: g_bTurnLeft=false; break;
		}
	}

}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	if (myChar!=NULL) {
		CharacterGUIEvent( nEvent, nControlID, pControl, pUserContext );
	} else {
		if (loggedin) {
			CharacterChooseGUIEvent( nEvent, nControlID, pControl, pUserContext );
		} else {
			LoginGUIEvent( nEvent, nControlID, pControl, pUserContext );
		}
	}
}

void CharacterChooseGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext ) {
	switch( nControlID )
	{
	case IDC_CHOOSECHAR: {
		Character* chosenChar = (Character*)g_CharacterChooseUI.GetListBox(IDC_CHARLIST)->GetSelectedItem()->pData;
		ChosenChar(chosenChar);
		break;
						 }
	}
}


void LoginGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext ) {
	switch( nControlID )
	{
	//case IDC_LOGIN: Connect(g_LoginUI.GetEditBox(IDC_USERNAME)->GetText(),g_LoginUI.GetEditBox(IDC_PASSWORD)->GetText()); break;
	case IDC_CLOSELOGINPOP: g_LoginPopUp.SetVisible(false); break;
	}
}

void CharacterGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext ) {
	switch( nControlID )
	{
	//case IDC_CHANGEDEVICE: g_SettingsDlg.SetActive( !g_SettingsDlg ); break;
	case IDC_EDITBOX1: {
		std::wstring currentText = g_CharacterUI.GetEditBox(IDC_EDITBOX1)->GetText();
		wchar_t firstChar = currentText[0];
		switch (firstChar)
		{
		case L'/':
			{
				Tokenizer* toke = new Tokenizer(currentText,1, L' ');
				std::wstring next=toke->getNext();
				if (Tokenizer::toUppercase(next)==L"SAY") {
					std::wstring remaining=toke->getRemaining();
					const wchar_t* cwremaining=remaining.c_str();
					g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					//g_CharacterUI.GetEditBox(IDC_EDITBOX1) ->SetText(cwremaining);	
					if (nEvent==1537) {
						movementDisabled=false;
						Chat(L"#global",cwremaining);
						g_CharacterUI.GetEditBox(IDC_EDITBOX1)->ClearText();
						g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetVisible(false);
					}
				} else if (Tokenizer::toUppercase(next)==L"TELL") {
					if (toke->hasNext()) {
						next=toke->getNext();
						std::wstring remaining=toke->getRemaining();
						const wchar_t* cwremaining=remaining.c_str();
						g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetTextColor( D3DCOLOR_ARGB( 255, 128, 0, 255 ) );
						//g_CharacterUI.GetEditBox(IDC_EDITBOX1) ->SetText(cwremaining);
						if (nEvent==1537) {
							movementDisabled=false;
							Chat(next.c_str(),cwremaining);
							g_CharacterUI.GetEditBox(IDC_EDITBOX1)->ClearText();
							g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetVisible(false);
						}
					}
				}
				break;
			}
		case L'.':
			{
				g_CharacterUI.GetEditBox(IDC_EDITBOX1)->SetTextColor( D3DCOLOR_ARGB( 255, 255, 255, 0 ) );
				break;
			}
		}
		break;
					   }
	//case IDC_QUIT: DXUTShutdown(0); break;
	case IDC_SPELLCAST1: if (myChar->inv.spells[0]!=NULL) { Cast(0); } break;
	case IDC_SPELLCAST2: if (myChar->inv.spells[1]!=NULL) { Cast(1); } break;
	case IDC_SPELLCAST3: if (myChar->inv.spells[2]!=NULL) { Cast(2); } break;
	case IDC_SPELLCAST4: if (myChar->inv.spells[3]!=NULL) { Cast(3); } break;
	case IDC_SPELLCAST5: if (myChar->inv.spells[4]!=NULL) { Cast(4); } break;
	case IDC_SPELLCAST6: if (myChar->inv.spells[5]!=NULL) { Cast(5); } break;
	case IDC_SPELLCAST7: if (myChar->inv.spells[6]!=NULL) { Cast(6); } break;
	case IDC_SPELLCAST8: if (myChar->inv.spells[7]!=NULL) { Cast(7); } break;
	case IDC_SPELLCAST9: if (myChar->inv.spells[8]!=NULL) { Cast(8); } break;
	case IDC_SPELLCAST10: if (myChar->inv.spells[9]!=NULL) { Cast(9); } break;
	case IDC_SPELLCAST11: if (myChar->inv.spells[10]!=NULL) { Cast(10); } break;
	}
}
void Chat(const wchar_t* wuser, const wchar_t* wmessage) {fprintf(stderr,"Chat re-implement");}
void ChosenChar(Character* c) {fprintf(stderr,"ChosenChar re-implement");}
void Cast(int a) {fprintf(stderr,"Cast re-implement");}
void Target(Character* targetchar) {fprintf(stderr,"Cast re-implement");}
/*
//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D9LostDevice();
	g_SettingsDlg.OnD3D9LostDevice();
	g_Mesh.InvalidateDeviceObjects();
	g_Avatar[0].InvalidateDeviceObjects();
	g_Avatar[1].InvalidateDeviceObjects();
	g_Avatar[2].InvalidateDeviceObjects();
	g_Avatar[3].InvalidateDeviceObjects();
	g_Avatar[4].InvalidateDeviceObjects();
	g_Avatar[5].InvalidateDeviceObjects();
	g_Avatar[6].InvalidateDeviceObjects();
	g_Avatar[7].InvalidateDeviceObjects();
	g_Mesh3.InvalidateDeviceObjects();
	g_Flag.InvalidateDeviceObjects();
	if( g_pFont )
		g_pFont->OnLostDevice();
	if( g_pEffect )
		g_pEffect->OnLostDevice();
	SAFE_RELEASE(g_pTextSprite);
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D9DestroyDevice();
	g_SettingsDlg.OnD3D9DestroyDevice();
	g_Mesh.Destroy();
	g_IntroMesh.Destroy();
	g_CharBackgroundMesh.Destroy();
	g_Avatar[0].Destroy();
	g_Avatar[1].Destroy();
	g_Avatar[2].Destroy();
	g_Avatar[3].Destroy();
	g_Avatar[4].Destroy();
	g_Avatar[5].Destroy();
	g_Avatar[6].Destroy();
	g_Avatar[7].Destroy();
	g_Mesh3.Destroy();
	g_Flag.Destroy();
	g_QuadMesh.Destroy();
	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(Tex);
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

}*/

///////////
/*
void Connect(const wchar_t* wusername, const wchar_t* wpassword) {
	LPSTR username=new CHAR[wcslen(wusername)+1];
	LPSTR password=new CHAR[wcslen(wpassword)+1];
	WideCharToMultiByte(CP_ACP,0,wusername,-1,username,(int)wcslen(wusername)+1,0,0);
	WideCharToMultiByte(CP_ACP,0,wpassword,-1,password,(int)wcslen(wpassword)+1,0,0);

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	//char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(serverip.c_str(), DEFAULT_PORT, &hints, &result);

	// Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
			ptr->ai_protocol);
		unsigned long b=1;
		//ioctlsocket(ConnectSocket,FIONBIO,&b);
		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		//break;
	}
	if (iResult>=0) {
		freeaddrinfo(result);
		char sendbuf[512];
		sendbuf[0]=20;
		int bufint=1;
		for (unsigned int i=0; i<strlen(username); i++) {
			sendbuf[bufint]=username[i];
			bufint++;
		}
		sendbuf[bufint]=' ';
		bufint++;
		for (unsigned int i=0; i<strlen(password); i++) {
			sendbuf[bufint]=password[i];
			bufint++;
		}
		sendbuf[bufint]=' ';
		send(ConnectSocket,sendbuf,11,0);
		//
		char recvbuf[DEFAULT_BUFLEN];

		iResult = recv(ConnectSocket, recvbuf, 512, 0);

		if (iResult > 0) {
			char firstByte = recvbuf[0];
			switch (firstByte) {
			case 20:   {  //stats
				char secondByte = recvbuf[1];
				switch (secondByte) {
			case 0: {
				closesocket(ConnectSocket);
				WSACleanup();
				LoginPopUp(L"Username or password invalid.");
				break;
					}
			case 1: {
				closesocket(ConnectSocket);
				WSACleanup();
				LoginPopUp(L"Your Acccount has been suspended.");
				break;
					}
			case 2: {
				loggedin=true;
				char sendbuf[1];
				sendbuf[0]=21;
				send(ConnectSocket,sendbuf,1,0);
				iResult = recv(ConnectSocket, recvbuf, 512, 0);
				if (iResult > 0) {
					char firstByte = recvbuf[0];
					switch (firstByte) {
			case 21:   {
				std::vector <Character*> myChars;
				bool end = false;
				for (int i=1; i<iResult && !end; i=i+4){
					ARRINT c(recvbuf[i],recvbuf[i+1],recvbuf[i+2],recvbuf[i+3]);
					if (c.i==-1) { end=true; } else {
						sendbuf[0]=22;
						int bufint=1;
						for (int i=0; i<4; i++) {
							sendbuf[bufint]=c.b[i];
							bufint++;
						}		
						send(ConnectSocket,sendbuf,5,0);
						char recvbuf2[512];
						iResult = recv(ConnectSocket, recvbuf2, 512, 0);

						if (iResult > 0) {
							char firstByte = recvbuf2[0];
							if (firstByte==22) {
								ARRINT c2(recvbuf2[1],recvbuf2[2],recvbuf[3],recvbuf[4]);
								int bufpos=5;
								if (c.i==c2.i) {
									std::string charname="";
									while(recvbuf2[bufpos]!=' ') {
										charname=charname+recvbuf2[bufpos];
										bufpos++;
									}
									bufpos++;
									ARRINT f(recvbuf2[bufpos],recvbuf2[bufpos+1],recvbuf2[bufpos+2],recvbuf2[bufpos+3]);
									ARRINT cl(recvbuf2[bufpos+4],recvbuf2[bufpos+5],recvbuf2[bufpos+6],recvbuf2[bufpos+7]);
									Character* chara = new Character(c.i, charname, f.i, cl.i);
									myChars.push_back(chara);
								}
							}
						}

					}
				}
				g_CharacterChooseUI.GetListBox( IDC_CHARLIST )->RemoveAllItems();
				for (unsigned int k=0; k<myChars.size() && k<=10; k++) {					
					LPCSTR shortcname=myChars.at(k)->charname.c_str();
					LPWSTR widecname=new WCHAR[strlen(shortcname)+1];
					MultiByteToWideChar(0,0,shortcname,-1,widecname,(int)strlen(shortcname)+1);
					g_CharacterChooseUI.GetListBox( IDC_CHARLIST )->InsertItem(k,widecname,(void*)myChars.at(k));
				}
				if (myChars.size()>0) {
				g_CharacterChooseUI.GetListBox( IDC_CHARLIST )->SelectItem(0);
				}
				break;
					   }
			default: {
				loggedin=false;
				closesocket(ConnectSocket);
				WSACleanup();
				LoginPopUp(L"Unknown Error");
				break;
					 }

					}
				}

				break;
					}
			default: {
				closesocket(ConnectSocket);
				WSACleanup();
				LoginPopUp(L"Unknown Error");
				break;
					 }
				}
				break;
					   }
			default: {
				closesocket(ConnectSocket);
				WSACleanup();
				LoginPopUp(L"Unknown Error");
				break;
					 }
			}
		}
	} else {
		LoginPopUp(L"Cannot Connect To Server");
	}

}

DWORD Receive(LPVOID lpParam) {
	while(true) {
		SOCKET ConnectSocket = (SOCKET)lpParam;
		char recvbuf[512];
		int iResult = recv(ConnectSocket, recvbuf, 512, 0);

		//////
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			char firstByte = recvbuf[0];
			switch (firstByte) {
				case 0:   {  //stats
					if (iResult>1) {
						char secondByte = recvbuf[1];
						switch (secondByte)
						{
						case 0: {
							ARRFLOAT o(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5],recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
							myChar->pos.orientation=o.f;
							ARRFLOAT x(recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13],recvbuf[14],recvbuf[15],recvbuf[16],recvbuf[17]);
							myChar->pos.x=x.f;
							ARRFLOAT y(recvbuf[18],recvbuf[19],recvbuf[20],recvbuf[21],recvbuf[22],recvbuf[23],recvbuf[24],recvbuf[25]);
							myChar->pos.y=y.f;
							ARRFLOAT z(recvbuf[26],recvbuf[27],recvbuf[28],recvbuf[29],recvbuf[30],recvbuf[31],recvbuf[32],recvbuf[33]);
							myChar->pos.z=z.f;
							//position stats
							break;
								}
						case 1: {
							// health etc.
							ARRINT h(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
							myChar->stats.health=h.i;
							ARRINT m(recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
							myChar->stats.magika=m.i;
							ARRINT e(recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13]);
							myChar->stats.energy=e.i;
							ARRINT ha(recvbuf[14],recvbuf[15],recvbuf[16],recvbuf[17]);
							myChar->stats.hate=ha.i;
							ARRINT f(recvbuf[18],recvbuf[19],recvbuf[20],recvbuf[21]);
							myChar->stats.focus=f.i;
							ARRINT mh(recvbuf[22],recvbuf[23],recvbuf[24],recvbuf[25]);
							myChar->stats.maxhealth=mh.i;
							ARRINT mm(recvbuf[26],recvbuf[27],recvbuf[28],recvbuf[29]);
							myChar->stats.maxmagika=mm.i;
							break;
								}
						}

					}
					break;
						  }
				case 1: {
					bool end = false;
					for (int i=1; i<iResult && !end; i=i+4){
						ARRINT c(recvbuf[i],recvbuf[i+1],recvbuf[i+2],recvbuf[i+3]);
						if (c.i==-1) { end=true; } else {
							bool found=false;
							for (unsigned int j=0; j<characters.size() && !found; j++) {
								if (characters.at(j).id==c.i) {
									found=true;
								}
							}
							if (!found) {
								Character chara = *new Character(c.i);
								characters.push_back(chara);
							}
						}
					}
					break;
						}
				case 2: {
					char secondByte = recvbuf[1];
					switch (secondByte)
					{
					case 0: {
						ARRINT c(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
						bool found=false;
						if (c.i!=myChar->id) {
							for (unsigned int j=0; j<characters.size() && !found;j++) {
								if (characters.at(j).id==c.i) {
									found=true;
									ARRFLOAT o(recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9],recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13]);
									characters.at(j).pos.orientation=o.f;
									ARRFLOAT x(recvbuf[14],recvbuf[15],recvbuf[16],recvbuf[17],recvbuf[18],recvbuf[19],recvbuf[20],recvbuf[21]);
									characters.at(j).pos.x=x.f;
									ARRFLOAT y(recvbuf[22],recvbuf[23],recvbuf[24],recvbuf[25],recvbuf[26],recvbuf[27],recvbuf[28],recvbuf[29]);
									characters.at(j).pos.y=y.f;
									ARRFLOAT z(recvbuf[30],recvbuf[31],recvbuf[32],recvbuf[33],recvbuf[34],recvbuf[35],recvbuf[36],recvbuf[37]);
									characters.at(j).pos.z=z.f;
								}
							}
						}
						break;
							}
					case 1: {
						ARRINT c(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
						bool found=false;
						for (unsigned int j=0; j<characters.size() && !found;j++) {
							if (characters.at(j).id==c.i) {
								found=true;
								ARRINT h(recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
								characters.at(j).stats.health=h.i;
								ARRINT m(recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13]);
								characters.at(j).stats.magika=m.i;
								ARRINT e(recvbuf[14],recvbuf[15],recvbuf[16],recvbuf[17]);
								characters.at(j).stats.energy=e.i;
								ARRINT ha(recvbuf[18],recvbuf[19],recvbuf[20],recvbuf[21]);
								characters.at(j).stats.hate=ha.i;
								ARRINT f(recvbuf[22],recvbuf[23],recvbuf[24],recvbuf[25]);
								characters.at(j).stats.focus=f.i;
								ARRINT mh(recvbuf[26],recvbuf[27],recvbuf[28],recvbuf[29]);
								characters.at(j).stats.maxhealth=mh.i;
								ARRINT mm(recvbuf[30],recvbuf[31],recvbuf[32],recvbuf[33]);
								characters.at(j).stats.maxmagika=mm.i;
							}
						}
						break;
							}
					case 2: {
						ARRINT c(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
						bool found=false;
						for (unsigned int j=0; j<characters.size() && !found;j++) {
							if (characters.at(j).id==c.i) {
								int bufpos=6;
								found=true;
								std::string charname="";
								while(recvbuf[bufpos]!=' ') {
									charname=charname+recvbuf[bufpos];
									bufpos++;
								}
								characters.at(j).charname=charname;
								bufpos++;
								ARRINT m(recvbuf[bufpos],recvbuf[bufpos+1],recvbuf[bufpos+2],recvbuf[bufpos+3]);
								characters.at(j).info.model=m.i;
								ARRINT cl(recvbuf[bufpos+4],recvbuf[bufpos+5],recvbuf[bufpos+6],recvbuf[bufpos+7]);
								characters.at(j).info.classs=cl.i;
								ARRINT fa(recvbuf[bufpos+8],recvbuf[bufpos+9],recvbuf[bufpos+10],recvbuf[bufpos+11]);
								characters.at(j).info.faction=fa.i;
							}
						}
						break;
							}
					}

					break;
						}
				case 5: {
					ARRINT c(recvbuf[1],recvbuf[2],recvbuf[3],recvbuf[4]);
					bool found=false;
					for (unsigned int j=0; j<characters.size() && !found;j++) {
						if (characters.at(j).id==c.i) {
							found=true;
							target=&characters.at(j);
						}
					}
					break;
						}
				case 6: {
					char secondByte = recvbuf[1];
					switch (secondByte)
					{
					case 0: {
						bool found=false;
						ARRINT s(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
						for (unsigned int j=0; j<11 && !found;j++) {
							if (myChar->inv.spells[j]->spell_id==s.i) {
								found=true;
								myChar->info.casting=myChar->inv.spells[j];
								myChar->info.casttime=myChar->inv.spells[j]->spell_length-100;
							}
						}
						break;
							}
					case 1: {
						myChar->info.casting=NULL;
						break;
							}
					case 2: {
						ARRINT c(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
						ARRINT s(recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
						ARRINT a(recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13]);
						break;
							}
					case 3: {
						ARRINT c(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
						ARRINT s(recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
						ARRINT a(recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13]);
						break;
							}
					case 4: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"YOU ARE NOT FOCUSED!");
							redTextTime=redTextLength;
							casterror[0]->Reset();
							casterror[0]->Play(0,0,(LONG) 1);
							myChar->info.casting=NULL;
							break;
							}
					case 5: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"NOT ENOUGH HATE!");
							redTextTime=redTextLength;
							casterror[1]->Reset();
							casterror[1]->Play(0,0,(LONG) 1);
							myChar->info.casting=NULL;
							break;
							}
					case 6: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"NOT ENOUGH ENERGY");
							redTextTime=redTextLength;
							casterror[2]->Reset();
							casterror[2]->Play(0,0,(LONG) 1);
							myChar->info.casting=NULL;
							break;
							}
					case 7: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"NOT ENOUGH MAGIKA");
							redTextTime=redTextLength;
							casterror[3]->Reset();
							casterror[3]->Play(0,0,(LONG) 1);
							myChar->info.casting=NULL;
							break;
							}
					case 8: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"ENEMY TOO CLOSE");
							redTextTime=redTextLength;
							casterror[4]->Reset();
							casterror[4]->Play(0,0,(LONG) 1);
							myChar->info.casting=NULL;
							break;
							}
					case 9: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"ENEMY TOO FAR");
							redTextTime=redTextLength;
							casterror[5]->Reset();
							casterror[5]->Play(0,0,(LONG) 1);
							myChar->info.casting=NULL;
							break;
							}
					case 10: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"THAT ENEMY IS DEAD");
							redTextTime=redTextLength;
							casterror[6]->Reset();
							casterror[6]->Play(0,0,(LONG) 1);
							myChar->info.casting=NULL;
							break;
							}
					default: myChar->info.casting=NULL; break;
					}
					break;
						}
				case 8: {
					std::string msg;
					unsigned int m=2;
					while (m<512 && recvbuf[m]!=(char)1) {
						msg=msg+recvbuf[m];
						m++;
					}
					
					if (recvbuf[1]==0) {
						msg=msg+" says: ";
					} else {
						msg=msg+" whispers: ";
					}
					m++;
					while (m<512 && recvbuf[m]!=(char)1) {
						msg=msg+recvbuf[m];
						m++;
					}

					LPCSTR shorttext=msg.c_str();
					LPWSTR widetext=new WCHAR[strlen(shorttext)+1];
					MultiByteToWideChar(0,0,shorttext,-1,widetext,(int)strlen(shorttext)+1);
					g_CharacterUI.GetListBox( IDC_TEXTBOX1 )->InsertItem(0,widetext,NULL);
					break;
						}
				case 9: {
					bool end=false;
					int k=0;
					for (int i=1; i<iResult && i<11 && !end; i=i+4){
						ARRINT s(recvbuf[i],recvbuf[i+1],recvbuf[i+2],recvbuf[i+3]);
						if (s.i==-1) { end=true; } else {
							bool found=false;
							for (unsigned int j=0; j<spells.size() && !found; j++) {
								if (spells.at(j)->spell_id==s.i) {
									found=true;
									myChar->inv.spells[k]=spells.at(j);
									k++;
								} else {
									//myChar->inv.spells[j]=NULL;
								}
							}
						}
					}
					break;
						}
				case 10: {
					bool found=false;
					ARRINT g(recvbuf[3],recvbuf[4],recvbuf[5],recvbuf[6]);
					if (recvbuf[1]==0) {
						if (recvbuf[2]>=0 &&  recvbuf[2]<6) {
							if (g.i!=-1) {
								for (unsigned int j=0; j<items.size() &&!found;j++) {
									if (g.i==items.at(j)->item_id) {
										found=true;
										myChar->inv.gear[recvbuf[2]]=items.at(j);
									}
								}
							} else {
								myChar->inv.gear[recvbuf[2]]=NULL;
							}
						}
					} else if (recvbuf[1]==1) {
						if (recvbuf[2]>=0 &&  recvbuf[2]<6) {
							if (g.i!=-1) {
								for (unsigned int j=0; j<items.size() &&!found;j++) {
									if (g.i==items.at(j)->item_id) {
										found=true;
										myChar->inv.items[recvbuf[2]]=items.at(j);
									}
								}
							} else {
								myChar->inv.items[recvbuf[2]]=NULL;
							}
						}
					}
					break;
						 }
				case 12: {
					ARRINT h(recvbuf[1],recvbuf[2],recvbuf[3],recvbuf[4]);
					ARRINT c(recvbuf[5],recvbuf[6],recvbuf[7],recvbuf[8]);
					gamestats.blueteam=c.i;
					gamestats.redteam=h.i;
					break;
						}
				case 13: {
					switch(recvbuf[1]) {
						case 0: {
							Aura* aura=NULL;
							Character* chara=NULL;
							int timeout=0;
							ARRINT a(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
							bool found=false;
							for (unsigned int i=0; i<auras.size() &&!found; i++) {
								if (auras.at(i)->aura_id==a.i) {
									found=true;
									aura=auras.at(i);
								}
							}
							found=false;
							ARRINT c(recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
							for (unsigned int i=0; i<characters.size() &&!found; i++) {
								if (characters.at(i).id==c.i) {
									found=true;
									chara=&characters.at(i);
								}
							}
							ARRINT t(recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13]);
							if (aura!=NULL) {
								myChar->addAura(aura, chara, timeout);
							}
							break;
								}
						case 1: {
							ARRINT a(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
							ARRINT c(recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
							myChar->removeAura(a.i, c.i);
							break;
								}
					}
					break;
					  }
				case 15: {
					ARRFLOAT o(recvbuf[1],recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5],recvbuf[6],recvbuf[7],recvbuf[8]);
					ARRFLOAT x(recvbuf[9],recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13],recvbuf[14],recvbuf[15],recvbuf[16]);
					ARRFLOAT y(recvbuf[17],recvbuf[18],recvbuf[19],recvbuf[20],recvbuf[21],recvbuf[22],recvbuf[23],recvbuf[24]);
					ARRFLOAT z(recvbuf[25],recvbuf[26],recvbuf[27],recvbuf[28],recvbuf[29],recvbuf[30],recvbuf[31],recvbuf[32]);
					gamestats.fx=x.f;
					gamestats.fy=y.f;
					gamestats.fz=z.f;
					gamestats.fo=o.f;
					break;
						}
				case 17: {
					switch(recvbuf[1]) {
						case 0: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"YOU DIE!");
							redTextTime=redTextLength;
							die->Reset();
							die->Play(0,0,(LONG) 1);
							break;//you die 
						}
						case 1: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"YOUR TEAM SCORES :)");
							redTextTime=redTextLength;
							scores[0]->Reset();
							scores[0]->Play(0,0,(LONG) 1);
							break;//you score
								}
						case 2: {
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"ENEMY TEAM SCORES :(");
							redTextTime=redTextLength;
							scores[1]->Reset();
							scores[1]->Play(0,0,(LONG) 1);
							break;//enemy scores 
								}
						case 3: {//you win
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"YOUR TEAM WINS THE ROUND YOU HAVE BEEN AWARDED 10CASH :)");
							redTextTime=redTextLength;
							scores[2]->Reset();
							scores[2]->Play(0,0,(LONG) 1);
							break;
								}
						case 4: {//enemy wins
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"THE ENEMY WINS THE ROUND :(");
							redTextTime=redTextLength;
							scores[3]->Reset();
							scores[3]->Play(0,0,(LONG) 1);
							break;
								}
						case 5: {//friendly flag pickup
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"YOUR TEAM HAS THE FLAG :)");
							redTextTime=redTextLength;
							scores[5]->Reset();
							scores[5]->Play(0,0,(LONG) 1);
							break;
								}
						case 6: {//enemy flag pickup
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"THE ENEMY HAS THE FLAG :(");
							redTextTime=redTextLength;
							scores[4]->Reset();
							scores[4]->Play(0,0,(LONG) 1);
							break;
								}
						case 7: {//flag reset
							g_RedTextUI.GetStatic( IDC_REDSTATIC )->SetText(L"THE FLAG HAS RESET");
							redTextTime=redTextLength;
							scores[6]->Reset();
							scores[6]->Play(0,0,(LONG) 1);
							break;
								}
					}
					break;
						}
			}
		}
		///
		Sleep(1);
	}
}
void Send() {
	// Send an initial buffer
	char sendbuff[512];
	sendbuff[0]=2;  // poisition stats
	sendbuff[1]=0;  
	//char test=(char)chara->pos.orientation;
	int bufint=2;
	FLOATARR o(myChar->pos.orientation);
	for (int i=0; i<8; i++) {
		sendbuff[bufint]=o.b[i];
		bufint++;
	}
	FLOATARR x(myChar->pos.x);
	for (int i=0; i<8; i++) {
		sendbuff[bufint]=x.b[i];
		bufint++;
	}
	FLOATARR y(myChar->pos.y);
	for (int i=0; i<8; i++) {
		sendbuff[bufint]=y.b[i];
		bufint++;
	}
	FLOATARR z(myChar->pos.z);
	for (int i=0; i<8; i++) {
		sendbuff[bufint]=z.b[i];
		bufint++;
	}
	send( ConnectSocket, sendbuff, 512, 0 );
}

void SendTick() {
	// Send an initial buffer
	char sendbuff[512];
	sendbuff[0]=2;  // poisition stats
	sendbuff[1]=1;  // poisition stats
	//char test=(char)chara->pos.orientation;
	int bufint=2;
	INTARR h(myChar->stats.health);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=h.b[i];
		bufint++;
	}
	INTARR m(myChar->stats.magika);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=m.b[i];
		bufint++;
	}
	INTARR e(myChar->stats.energy);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=e.b[i];
		bufint++;
	}
	INTARR ha(myChar->stats.hate);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=ha.b[i];
		bufint++;
	}
	INTARR fo(myChar->stats.focus);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=fo.b[i];
		bufint++;
	}
	send( ConnectSocket, sendbuff, 512, 0 );
}

void Chat(const wchar_t* wuser, const wchar_t* wmessage) {
	LPSTR user=new CHAR[wcslen(wuser)+1];
	LPSTR message=new CHAR[wcslen(wmessage)+1];
	WideCharToMultiByte(CP_ACP,0,wuser,-1,user,(int)wcslen(wuser)+1,0,0);
	WideCharToMultiByte(CP_ACP,0,wmessage,-1,message,(int)wcslen(wmessage)+1,0,0);
	char sendbuf[512];
	sendbuf[0]=8;  // say
	int bufint=1;
	for (unsigned int i=0; i<strlen(user); i++) {
		sendbuf[bufint]=user[i];
		bufint++;
	}
	sendbuf[bufint]=(char)1;
	bufint++;
	for (unsigned int i=0; i<strlen(message); i++) {
		sendbuf[bufint]=message[i];
		bufint++;
	}
	sendbuf[bufint]=(char)1;
	send( ConnectSocket, sendbuf, 512, 0 );
}

void Target(Character* targetchar) {
	char sendbuf[512];
	sendbuf[0]=5;  // say
	int bufint=1;
	INTARR c(targetchar->id);
	for (int i=0; i<4; i++) {
		sendbuf[bufint]=c.b[i];
		bufint++;
	}
	send( ConnectSocket, sendbuf, 512, 0 );
}

void Cast(int buttonId) {
	char sendbuf[512];
	sendbuf[0]=6;  // cast
	int bufint=1;
	INTARR c(buttonId);
	for (int i=0; i<4; i++) {
		sendbuf[bufint]=c.b[i];
		bufint++;
	}
	send( ConnectSocket, sendbuf, 512, 0 );
}


DWORD UpdateThread(LPVOID lpParam) {
	int time=0;
	char sendbuff[512];
	sendbuff[0]=9;
	send( (SOCKET)lpParam, sendbuff, 512, 0 );
	sendbuff[0]=10;
	send( (SOCKET)lpParam, sendbuff, 512, 0 );
	while(true) {
		SOCKET ConnectSocket = (SOCKET)lpParam;
		char sendbuff[512];
		int iResult=0;
		Send();
		//sendbuff[0]=0;
		//iResult = send( ConnectSocket, sendbuff, 512, 0 );
		sendbuff[0]=1;
		iResult = send( ConnectSocket, sendbuff, 512, 0 );
		sendbuff[0]=3;
		iResult = send( ConnectSocket, sendbuff, 512, 0 );
		for (unsigned int j=0; j<characters.size(); j++) {
			sendbuff[0]=4;
			sendbuff[1]=0;
			int bufint =2;
			INTARR c(characters.at(j).id);
			for (int i=0; i<4; i++) {
				sendbuff[bufint]=c.b[i];
				bufint++;
			}
			iResult = send( ConnectSocket, sendbuff, 512, 0 );
			sendbuff[0]=4;
			sendbuff[1]=1;
			bufint =2;
			for (int i=0; i<4; i++) {
				sendbuff[bufint]=c.b[i];
				bufint++;
			}
			iResult = send( ConnectSocket, sendbuff, 512, 0 );
			sendbuff[0]=4;
			sendbuff[1]=2;
			bufint =2;
			for (int i=0; i<4; i++) {
				sendbuff[bufint]=c.b[i];
				bufint++;
			}
			iResult = send( ConnectSocket, sendbuff, 512, 0 );
		}
		sendbuff[0]=15;
		iResult = send( ConnectSocket, sendbuff, 512, 0 );
		if (time==1000) {
			time=0;
			sendbuff[0]=9;
			iResult = send( ConnectSocket, sendbuff, 512, 0 );
			for (int i=0; i<6; i++) {
				sendbuff[0]=10;
				sendbuff[1]=0;
				sendbuff[2]=i;
				iResult = send( ConnectSocket, sendbuff, 512, 0 );
			}
			for (int i=0; i<6; i++) {
				sendbuff[0]=10;
				sendbuff[1]=1;
				sendbuff[2]=i;
				iResult = send( ConnectSocket, sendbuff, 512, 0 );
			}
			sendbuff[0]=12;
			iResult = send( ConnectSocket, sendbuff, 512, 0 );
		} else {
			time++;
		}
		Sleep(1);
	}
	return 0;
}

void LoginPopUp(const wchar_t* error) {
	g_LoginPopUp.GetStatic(IDC_LOGINERR)->SetText(error);
	g_LoginPopUp.SetVisible(true);
}

void ChosenChar(Character* chosen) {
	char sendbuf[11];
	sendbuf[0]=24;
	int bufint=1;
	int iResult;
	INTARR c(chosen->id);
	for (int i=0; i<4; i++) {
		sendbuf[bufint]=c.b[i];
		bufint++;
	}		
	send(ConnectSocket,sendbuf,5,0);
	//
	char recvbuf[DEFAULT_BUFLEN];

	iResult = recv(ConnectSocket, recvbuf, 512, 0);

	if (iResult > 0) {
		char firstByte = recvbuf[0];
		switch (firstByte) {
			case 24:   {  //stats
				myChar=chosen;
				pMusic->Stop();
				POVector3 vecEye(0.0f, 1.2f, 5.0f);
				POVector3 vecAt (0.0f, -yadjust[myChar->info.classs+(myChar->info.faction*4)], -0.0f);
				g_Camera.SetViewParams( &vecEye, &vecAt );
				g_Camera.SetRadius(5.0f,1.0f,10.0f);
				DWORD threadId;
				sendbuf[0]=0;
				send( ConnectSocket, sendbuf, 512, 0 );
				CreateThread(0,0,Receive,(LPVOID)ConnectSocket,0,&threadId);
				sendbuf[0]=12;
				send(ConnectSocket,sendbuf,512,0);
				DWORD threadId1;
				CreateThread(0,0,UpdateThread,(LPVOID)ConnectSocket,0,&threadId1);
				break;
					   }
			default:   {
				break;
					   }
		}
	}
}*/

void SetSpells() {
	int i=0;
	for (i=0; i<11; i++) {
		if (myChar->inv.spells[i]!=NULL) {
			RECT current ={g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->GetElement(0)->GetTextureRect()->left, g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->GetElement(0)->GetTextureRect()->top, g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->GetElement(0)->GetTextureRect()->right, g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->GetElement(0)->GetTextureRect()->bottom};
			RECT newRect =icons[myChar->inv.spells[i]->spell_icon];
			if ((current.bottom!=newRect.bottom)||(current.top!=newRect.top)||(current.left!=newRect.left)||(current.right!=newRect.right)) {
				g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->GetElement(0)->SetTexture(2,&icons[myChar->inv.spells[i]->spell_icon],D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->SetEnabled(true);
			g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->SetVisible(true);
		} else {
			g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->SetEnabled(false);
			g_CharacterUI.GetButton( IDC_SPELLCAST1+i )->SetVisible(false);
		}
	}
}

void SetInventory() {
	int i=0;
	for (i=0; i<6; i++) {
		if (myChar->inv.gear[i]!=NULL) {
			RECT current ={g_CharacterUI.GetButton( IDC_HEAD+i )->GetElement(0)->GetTextureRect()->left, g_CharacterUI.GetButton( IDC_HEAD+i )->GetElement(0)->GetTextureRect()->top, g_CharacterUI.GetButton( IDC_HEAD+i )->GetElement(0)->GetTextureRect()->right, g_CharacterUI.GetButton( IDC_HEAD+i )->GetElement(0)->GetTextureRect()->bottom};
			RECT newRect =icons2[myChar->inv.gear[i]->item_icon];
			if ((current.bottom!=newRect.bottom)||(current.top!=newRect.top)||(current.left!=newRect.left)||(current.right!=newRect.right)) {
				g_CharacterUI.GetButton( IDC_HEAD+i )->GetElement(0)->SetTexture(3,&icons2[myChar->inv.gear[i]->item_icon],D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			g_CharacterUI.GetButton( IDC_HEAD+i )->SetEnabled(true);
			g_CharacterUI.GetButton( IDC_HEAD+i )->SetVisible(true);
		} else {
			g_CharacterUI.GetButton( IDC_HEAD+i )->SetEnabled(false);
			g_CharacterUI.GetButton( IDC_HEAD+i )->SetVisible(false);
		}
	}
	for (i=0; i<6; i++) {
		if (myChar->inv.items[i]!=NULL) {
			RECT current ={g_CharacterUI.GetButton( IDC_INV1+i )->GetElement(0)->GetTextureRect()->left, g_CharacterUI.GetButton( IDC_INV1+i )->GetElement(0)->GetTextureRect()->top, g_CharacterUI.GetButton( IDC_INV1+i )->GetElement(0)->GetTextureRect()->right, g_CharacterUI.GetButton( IDC_INV1+i )->GetElement(0)->GetTextureRect()->bottom};
			RECT newRect =icons2[myChar->inv.items[i]->item_icon];
			if ((current.bottom!=newRect.bottom)||(current.top!=newRect.top)||(current.left!=newRect.left)||(current.right!=newRect.right)) {
				g_CharacterUI.GetButton( IDC_INV1+i )->GetElement(0)->SetTexture(3,&icons2[myChar->inv.items[i]->item_icon],D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			g_CharacterUI.GetButton( IDC_INV1+i )->SetEnabled(true);
			g_CharacterUI.GetButton( IDC_INV1+i )->SetVisible(true);
		} else {
			g_CharacterUI.GetButton( IDC_INV1+i )->SetEnabled(false);
			g_CharacterUI.GetButton( IDC_INV1+i )->SetVisible(false);
		}
	}
}

void LoadSpells() {
	ISFLoader* load = new ISFLoader();
	load->LoadISF(&spells);
}

void LoadItems() {
	IIFLoader* load = new IIFLoader();
	load->LoadIIF(&items);
}

void LoadAuras() {
	IAFLoader* load = new IAFLoader();
	load->LoadIAF(&auras);
}


void LoadServerIP() {
	FILE* file;
	long lSize;
	char * buffer;
	size_t result=1;
	file = fopen("server.ip", "r" );
	if( file!= NULL )
	{
		fseek (file , 0 , SEEK_END);
		lSize = ftell (file);
		rewind (file);

		// allocate memory to contain the whole file:
		buffer = (char*) malloc (sizeof(char)*lSize);
		if (buffer == NULL) {exit (2);}

		// copy the file into the buffer:
		result = fread (buffer,1,lSize,file);
		//if (result != lSize) {exit (3);}


		//for (unsigned int i=0; i<result; i++) {
		for (unsigned int i=0; i<result; i++ ){
			serverip+=buffer[i];
		}
		//}
		fclose (file);
	}
}
