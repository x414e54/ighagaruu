//*****************************************************************************
//-----------------------------------------------------------------------------
// AntWrap.h 
//
// This wraps the creation of windows, and initalization of devices.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef ANTWRAP_H
#define ANTWRAP_H

#include "Ant.h"
#include "AntSettings.h"
#include "AntRenderer.h"

typedef void (* PRENDERCALLBACK)(float timeDelta);
typedef void (* PUPDATECALLBACK)(float timeDelta);
typedef bool (* PMSGCALLBACK)(UINT msg, UINT p1, UINT p2);

void AntWrapSetRenderCallBack(PRENDERCALLBACK callback);
void AntWrapSetMsgCallBack(PMSGCALLBACK callback);
void AntWrapSetUpdateCallBack(PUPDATECALLBACK callback);

bool AntWrapInit(AntRenderer** device, AntSettings settings, std::wstring wndName);

bool AntWrapQuit(AntRenderer* renderer);

int AntWrapRun();

int AntWrapRenderThread(void* data);
int AntWrapUpdateThread(void* data);
int AntWrapEventThread(void* data);

//LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
