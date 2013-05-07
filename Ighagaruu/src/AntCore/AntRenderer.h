//*****************************************************************************
//-----------------------------------------------------------------------------
// AntRenderer.h 
//
// This is a base class for allowing abstract renderes to be used without the rest
// of the game having to be edited to change the renderer. E.g. from DirectX to
// OpenGL, or 3D to 2D.
// 
// All classes that require something to be rendered will call methods in this
// class.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef ANTRENDERER_H_
#define ANTRENDERER_H_
#define MAX_PATH 256
#include "PhysicsOne.h"
#include "AntSettings.h"
//#include <vector>
//#include <strsafe.h>

//-----------------------------------------------------------------------------
// Structs to allow use of generic types
//-----------------------------------------------------------------------------
struct AntFont
{
	std::wstring typeFace;
	unsigned int  h;
	unsigned int  w;
	void* font;
};

#define FW_NORMAL 0

struct AntTexture
{
	std::wstring fileName;
	void* texture;
	unsigned int w;
	unsigned int h;
};

struct AntMesh
{
	std::wstring fileName;
	void* mesh;
};

struct AntFontColorARGB
{
	int _A;
	int _R;
	int _G;
	int _B;
	AntFontColorARGB(int A=0, int R=0, int G=0, int B=0) : _A(A), _R(R), _G(G), _B(B) {}
};


class AntRenderer
{
public:
	virtual void DrawMesh(UINT meshId, POVector3* pos, POVector3* o) { return; }
	virtual void DrawMesh(UINT meshId, POVector3 pos, POVector3 o) { return DrawMesh(meshId, &pos, &o); }
	virtual void DrawMesh(UINT meshId, POVector3* pos, POVector3 o) { return DrawMesh(meshId, pos, &o); }
	virtual void DrawMesh(UINT meshId, POVector3 pos, POVector3* o) { return DrawMesh(meshId, &pos, o); }
	virtual void SetProjection() { return; }
	virtual void SetView(POVector3* pos, POVector3* target) { return; }
	virtual void SetView(POVector3 pos, POVector3 target) { return SetView(&pos, &target); }
	virtual void BeginScene() { return; }
	virtual void EndScene() { return; }
	virtual void DrawSprite(UINT textureId, const RECT& src, const RECT& dst, const POVector3& pos, float sx, float sw, float rotation) { return; }
	//virtual void DrawSprite(UINT textureId, const RECT& src, POVector3 pos, float sx, float sw, float rotation) { return DrawSprite(textureId, src, &pos, sx, sw, rotation); }
	virtual void DrawSprite(UINT textureID, const RECT& src, const RECT& dst, const POVector3& pos) { DrawSprite(textureID, src, dst, pos, 1.0f, 1.0f, 0.0f); }
	virtual void DrawText(UINT fontId, const std::wstring& string, const RECT& dst, const AntFontColorARGB& fontColor) { return; }
	virtual void DrawQuad(UINT textureID, RECT* rect, POVector3* pos, POVector3* Or) { return; }
	virtual void DrawQuad(UINT textureID, RECT* rect, POVector3 pos, POVector3 Or) { return DrawQuad(textureID, rect, &pos, &Or); }
	virtual bool Cleanup() { return false; }
	virtual bool Init(AntSettings settings) { return false; }
	virtual void AddFont(const std::wstring& typeFace, UINT width, UINT height) { return; }
	virtual void AddTexture(UINT textureID, const std::wstring& fileName) { return; }
	virtual void AddMesh(const std::wstring& fileName) { return; }
	AntFont* GetFont(UINT id) { return _fonts.at(id); }
	AntTexture* GetTexture(UINT id) { return _textures.at(id); }
	AntMesh* GetMesh(UINT id) { return _meshes.at(id); }

protected:
	std::vector<AntMesh*> _meshes;
	std::vector<AntFont*> _fonts;
	std::vector<AntTexture*> _textures;
	AntSettings _settings;
};

#endif

