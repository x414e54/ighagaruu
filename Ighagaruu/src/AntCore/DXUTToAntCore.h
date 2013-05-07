//*****************************************************************************
//-----------------------------------------------------------------------------
// DXUTToAntCore.h
//
// Provides a drop in abstraction for my DXUT projects to run on the AntCore
// engine allowing them to be run on Linux.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef DXUTTOANTCORE_H
#define DXUTTOANTCORE_H

// Ant Core Includes
#include "AntSettings.h"
#include "AntWrap.h"
#include "AntGUI.h"
#include "AntActor.h"
#include "AntCamera.h"
#include "AntMath.h"
#include "AntSound.h"
#include "GLRenderer.h"
#include <wchar.h>

#define D3DCOLOR_ARGB AntFontColorARGB
#define CDXUTDialogResourceManager AntResourceManager
#define CDXUTDialog AntGUIDialog
#define CD3DSettingsDlg AntGUIDialog
#define CDXUTControl AntGUIComponent
#define CModelViewerCamera AntCameraFollower
#define CSound AntSound
#define CDXUTXFileMesh AntMesh
#define CSoundManager AntSoundManager
#define WCHAR wchar_t
#define DSSCL_PRIORITY 0
#define DSBCAPS_CTRLVOLUME 0
#define DXUTGetHWND() 0
#define D3DFVF_XYZ 0
#define D3DFVF_NORMAL 0
#define D3DFVF_TEX1 0
#define	D3DXMATRIX POMatrix4;
	//	D3DXMatrixRotationY( &mRot, myChar->pos.orientation );
#define	D3DXMatrixInverse POInvertMatrix;
	//	D3DXVec3TransformCoord( &vAvatar, &vAvatar, &mRot );

AntRenderer*			Renderer;
AntSettings				Settings;

void DXUTCreateDevice(bool a, UINT w, UINT h) {}
void DXUTMainLoop(){ AntWrapRun(); }

void DXUTCreateWindow(std::wstring name)
{
	if(!AntWrapInit(&Renderer, Settings, name))
	{
		fprintf(stderr,"DisplayInitFailed(CannotContinue)"); return;
	}

}
void DXUTInit(bool a, bool b) {}
void DXUTSetHotkeyHandling(bool a, bool b, bool c) {}

void DXUTFindDXSDKMediaFileCch(WCHAR* bounce, UINT max, std::wstring path)
{
	path.replace(path.find(L"\\"), 1, L"/");
	wcsncpy(bounce,path.c_str(),(long int)max);
}

//DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );
//DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
//DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
//DXUTSetCallbackD3D9FrameRender( OnFrameRender );
//DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
//DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
//DXUTSetCallbackMsgProc( MsgProc );
//DXUTSetCallbackKeyboard( KeyboardProc );
//DXUTSetCallbackFrameMove( OnFrameMove );
//DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

//DXUTSetCursorSettings( true, true );

#endif
