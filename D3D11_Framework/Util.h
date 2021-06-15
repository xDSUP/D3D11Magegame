#pragma once
#include "stdafx.h"

#define w(c) CCharToWChar(c)
using namespace std;

// Convert DirectX error codes to exceptions.
inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		// Set a breakpoint on this line to catch DX API errors.
		throw hr;
	}
}

// тут утечки памяти)
inline wchar_t* CharToWChar(char* mbStr)
{
	int len = (int)strlen(mbStr) + 1;
	wchar_t* ucStr = new wchar_t[len];
	size_t outSize;
	mbstowcs_s(&outSize, ucStr, len, mbStr, len - 1);
	return ucStr;
}

// тут утечки памяти)
inline wchar_t* CCharToWChar(const char* mbStr)
{
	int len = (int)strlen(mbStr) + 1;
	wchar_t* ucStr = new wchar_t[len];
	size_t outSize;
	mbstowcs_s(&outSize, ucStr, len, mbStr, len - 1);
	return ucStr;
}

inline std::wstring intToStr(int i)
{
	wchar_t str[255];
	swprintf(str, 255, L"%d", i);
	return str; 
}

inline float convertDegreeToRad(float d)
{
	return d * 0.0174532925f;
}