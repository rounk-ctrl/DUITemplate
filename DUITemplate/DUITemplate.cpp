// DUITemplate.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DUITemplate.h"

using namespace DirectUI;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

    if FAILED(CoInitializeEx(NULL, 0)) return FALSE;
    InitProcess(DUI_VERSION);
    InitThread(TSM_IMMERSIVE);

    // Perform application initialization:
    InitInstance(hInstance, nCmdShow);

    // this is a geneic message loop, doesnt support accelerators or custom messages
    StartMessagePump();

    // exit
    UnInitProcessPriv(HINST_THISCOMPONENT);
    return 0;
}


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    NativeHWNDHost* pWndHost;
    NativeHWNDHost::Create(
        szTitle, nullptr, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DUITEMPLATE)),
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        0, WS_OVERLAPPEDWINDOW, 0, &pWndHost);

    DUIXmlParser* pXmlParser;
    DUIXmlParser::Create(&pXmlParser, nullptr, nullptr, ParserErrorCB, nullptr);

    pXmlParser->SetXMLFromResource(IDR_MAINWND, hInst, hInst);

    Element* pElement;
    DWORD dwDefer;
    DirectUI::HWNDElement::Create(pWndHost->GetHWND(), true, 0, nullptr, &dwDefer, &pElement);

    Element* pHostElement;
    pXmlParser->CreateElement(L"main", pElement, nullptr, nullptr, &pHostElement);

    pHostElement->SetVisible(true);
    pHostElement->EndDefer(dwDefer);
    pWndHost->Host(pElement);
    pWndHost->ShowWindow(nCmdShow);

    // cleanup
    pXmlParser->Destroy();
    return TRUE;
}

void CALLBACK ParserErrorCB(const WCHAR* pszError, const WCHAR* pszToken, int dLine, void* pContext)
{
    if (pszError != nullptr)
    {
        MessageBox(nullptr, pszError, L"Error while parsing DirectUI", MB_ICONERROR);
        OutputDebugString(pszError);
        DebugBreak();
    }
}