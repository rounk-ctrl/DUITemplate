#pragma once
#include "resource.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
void CALLBACK ParserErrorCB(const WCHAR* pszError, const WCHAR* pszToken, int dLine, void* pContext);
