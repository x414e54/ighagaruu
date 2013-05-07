#pragma once
#ifndef ANT_H
#define ANT_H

#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#define UINT unsigned int
#ifndef NULL
#define NULL 0
#endif
#define WORD short
#define DWORD unsigned int
#define LONG long
#define SETDWORD(h,l) (((DWORD)(h) << 16) | l);
#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))

enum ANTGUI_EVENT
{
	ANTGUI_EVENT_KEYDOWN,
	ANTGUI_EVENT_LBUTTONDOWN,
	ANTGUI_EVENT_LBUTTONUP,
	ANTGUI_EVENT_MBUTTONDOWN,
	ANTGUI_EVENT_MBUTTONUP,
	ANTGUI_EVENT_RBUTTONDOWN,
	ANTGUI_EVENT_RBUTTONUP,
	ANTGUI_EVENT_MOUSEMOVE,
	ANTGUI_EVENT_KEYUP,
	ANTGUI_EVENT_QUIT,
	ANTGUI_EVENT_CHAR,
	ANTGUI_EVENT_KEY_UP,
	ANTGUI_EVENT_KEY_DOWN,
	ANTGUI_EVENT_KEY_LEFT,
	ANTGUI_EVENT_KEY_RIGHT,
	ANTGUI_EVENT_KEY_BACK,
	ANTGUI_EVENT_KEY_ESCAPE,
	ANTGUI_EVENT_KEY_RETURN,
	ANTGUI_EVENT_KEY_SPACE,
	ANTGUI_EVENT_KEY_SHIFT,
};

std::string inline Convert(const std::wstring& str)
{
	int bufferlen = str.length()+1;
	char buffer[bufferlen];
	int ret = wcstombs( buffer, str.c_str(), bufferlen);
	if (ret == bufferlen-1)
	{
		buffer[bufferlen]=0;
		return std::string(buffer);
	} else {
		return std::string(""); // error here
	}
}

#endif
