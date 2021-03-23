#pragma once
#include "stdafx.h"

#define w(c) CCharToWChar(c)
using namespace std;

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