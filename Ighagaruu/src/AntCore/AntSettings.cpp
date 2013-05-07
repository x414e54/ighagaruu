#include "AntSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
AntSettings::AntSettings()
{
	/* Default settings */
	_deviceType				= ANTDEVICE_GL;
	_width					= 640;
	_height					= 480;
	_multiSampleType		= 0;
	_fullscreen				= false;
	
	//DEVMODE devMode;
	//devMode.dmSize = sizeof(DEVMODE);

	/* Add capable display resolutions */
	//for (int i=0; EnumDisplaySettings(NULL, i, &devMode) == 1; i++)
	//{
		//bool notFound=true;
		//for (unsigned int j=0; j<_resolutions.size() && notFound; j++)
		//{
//			if (devMode.dmPelsHeight==_resolutions.at(j)._height && devMode.dmPelsWidth==_resolutions.at(j)._width) { notFound=false; }
	//	}
//		if (notFound)
	//	{
			//Resolution res = {devMode.dmPelsWidth,devMode.dmPelsHeight};
			//_resolutions.push_back(res);
		//}
	//}

	/* Add device types */
	_deviceTypes.push_back(ANTDEVICE_DX);
	_deviceTypes.push_back(ANTDEVICE_GL);

	/* Add the multisample types */
	_multisampleTypes.push_back(MULTISAMPLE_NONE);
	_multisampleTypes.push_back(MULTISAMPLE_2);
	_multisampleTypes.push_back(MULTISAMPLE_4);
	_multisampleTypes.push_back(MULTISAMPLE_8);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void AntSettings::Read()
{
	std::string line;
	std::ifstream settings("settings.ini");
	if (settings.is_open())
	{
		while (settings.good())
		{
			std::getline(settings,line);
	    }
		settings.close();
	}
  //HANDLE inF = CreateFile(L"settings.dat", GENERIC_READ, 0, NULL,
//    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  //if (inF == INVALID_HANDLE_VALUE) { MessageBox(0, L"ReadSettingsFileError(UsingDefaults)", 0, 0); return; }

  //DWORD bytesRead;

  //ReadFile(inF, &_deviceType, 1, &bytesRead, NULL);
  //ReadFile(inF, &_width, sizeof(DWORD), &bytesRead, NULL);
  //ReadFile(inF, &_height, sizeof(DWORD), &bytesRead, NULL);
  //ReadFile(inF, &_multiSampleType, sizeof(DWORD), &bytesRead, NULL);
  //ReadFile(inF, &_fullscreen, 1, &bytesRead, NULL);

  //CloseHandle(inF);
  //fprintf(stderr,"SettingsReadFailed(CannotContinue)");
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool AntSettings::Write()
{
  /*HANDLE inF = CreateFile(L"settings.dat", GENERIC_WRITE, 0, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  if (inF == INVALID_HANDLE_VALUE) { MessageBox(0, L"WriteSettingsFileError", 0, 0); return false; }

  DWORD bytesWritten;

  WriteFile(inF, &_deviceType, 1, &bytesWritten, NULL);
  WriteFile(inF, &_width, sizeof(DWORD), &bytesWritten, NULL);
  WriteFile(inF, &_height, sizeof(DWORD), &bytesWritten, NULL);
  WriteFile(inF, &_multiSampleType, sizeof(DWORD), &bytesWritten, NULL);
  WriteFile(inF, &_fullscreen, 1, &bytesWritten, NULL);

  CloseHandle(inF);*/
  return true;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
Resolution* AntSettings::GetResolution(UINT index)
{
	if (index<_resolutions.size())
	{
		Resolution* res= &_resolutions.at(index);
		return res;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int AntSettings::GetNumberOfResolutions()
{
	return _resolutions.size();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
std::wstring AntSettings::GetResolutionString(UINT index)
{
	if (index<_resolutions.size()) { 
		std::wstringstream w;
		std::wstring out;
		w<<_resolutions.at(index)._width;
		w<<L"x";
		w<<_resolutions.at(index)._height;
		w>>out;
		return out;
	}
	return L"";
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
ANTDEVICE_TYPE* AntSettings::GetDeviceType(UINT index)
{
	if (index<_deviceTypes.size())
	{
		ANTDEVICE_TYPE* type= &_deviceTypes.at(index);
		return type;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int AntSettings::GetNumberOfTypes()
{
	return _deviceTypes.size();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
std::wstring AntSettings::GetDeviceTypeString(UINT index)
{
	if (index<_deviceTypes.size()) { 
		std::wstring out;
		switch(_deviceTypes.at(index))
		{
			case ANTDEVICE_DX:
				out=L"DirectX";
			break;
			case ANTDEVICE_GL:
				out=L"OpenGL";
			break;
			default :
				out=L"";
			break;
		}
		return out;
	}
	return L"";
}

MULTISAMPLE* AntSettings::GetMultisampleType(UINT index)
{
	if (index<_multisampleTypes.size())
	{
		MULTISAMPLE* type= &_multisampleTypes.at(index);
		return type;
	}
	return NULL;
}

int AntSettings::GetNumberOfMultisampleTypes()
{
	return _multisampleTypes.size();
}

std::wstring AntSettings::GetMultisampleTypeString(UINT index)
{
	if (index<_multisampleTypes.size()) { 
		std::wstring out;
		switch(_multisampleTypes.at(index))
		{
			case MULTISAMPLE_NONE:
				out=L"NONE";
			break;
			case MULTISAMPLE_2:
				out=L"2xAA";
			break;
			case MULTISAMPLE_4:
				out=L"4xAA";
			break;
			case MULTISAMPLE_8:
				out=L"8xAA";
			break;
			default :
				out=L"";
			break;
		}
		return out;
	}
	return L"";
}
