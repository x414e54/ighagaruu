//*****************************************************************************
//-----------------------------------------------------------------------------
// AntSettings.h 
//
// A settings class to read the settings from a file. The settings can then be
// saved or edited from an internal GUI or a created MFC or so on.
// Currently device types are hard coded in e.g. DX and GL, but this should be
// changed to allow for more or less choices.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H
#define DWORD unsigned int
#define UINT unsigned int
#include <vector>
#include <string>
enum ANTDEVICE_TYPE
{
	ANTDEVICE_DX,
	ANTDEVICE_GL,
};

struct Resolution
{
	DWORD _width;
	DWORD _height;
};

enum MULTISAMPLE
{
	MULTISAMPLE_NONE,
	MULTISAMPLE_2,
	MULTISAMPLE_4,
	MULTISAMPLE_8,
};

class AntSettings
{
public:
	ANTDEVICE_TYPE	_deviceType;
	DWORD			_width;
	DWORD			_height;
	DWORD			_multiSampleType;
	bool			_fullscreen;
	std::vector<Resolution> _resolutions;
	std::vector<ANTDEVICE_TYPE> _deviceTypes;
	std::vector<MULTISAMPLE> _multisampleTypes;
	std::wstring _programName;

	AntSettings();
	void Read();
	bool Write();
	Resolution* GetResolution(UINT index);
	ANTDEVICE_TYPE* GetDeviceType(UINT index);
	MULTISAMPLE* GetMultisampleType(UINT index);
	int GetNumberOfTypes();
	int GetNumberOfMultisampleTypes();
	int GetNumberOfResolutions();
	std::wstring GetResolutionString(UINT index);
	std::wstring GetMultisampleTypeString(UINT index);
	std::wstring GetDeviceTypeString(UINT index);
};

#endif
