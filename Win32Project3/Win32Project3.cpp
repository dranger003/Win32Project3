// Win32Project3.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Project3.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT3));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT3));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT3);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      200, 200, 320, 320, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

BOOL SnapUnsnapRect(
    PRECT prc,      // Window
    PPOINT ppt,     // Cursor
    PRECT prcce,    // Cursor to window edge
    PRECT prcst,    // Snap to
    PRECT prcsr     // Snap range
)
{
    BOOL bRes = FALSE;

    // Left
    if (prc->left < prcst->left + prcsr->left)
    {
        // Unsnap
        if (ppt->x + prcce->left >= prcst->left + prcsr->left)
            ::OffsetRect(prc, ppt->x + prcce->left - prc->left, 0);
        else // Snap
            ::OffsetRect(prc, -prc->left + prcst->left, 0);

        bRes = TRUE;
    }

    // Top
    if (prc->top < prcst->top + prcsr->top)
    {
        if (ppt->y + prcce->top >= prcst->top + prcsr->top)
            ::OffsetRect(prc, 0, ppt->y + prcce->top - prc->top);
        else
            ::OffsetRect(prc, 0, -prc->top + prcst->top);

        bRes = TRUE;
    }

    // Right
    if (prc->right > prcst->right - prcsr->right)
    {
        if (ppt->x + prcce->right <= prcst->right - prcsr->right)
            ::OffsetRect(prc, -prc->right + ppt->x + prcce->right, 0);
        else
            ::OffsetRect(prc, prcst->right - prc->right, 0);

        bRes = TRUE;
    }

    // Bottom
    if (prc->bottom > prcst->bottom - prcsr->bottom)
    {
        if (ppt->y + prcce->bottom <= prcst->bottom - prcsr->bottom)
            ::OffsetRect(prc, 0, -prc->bottom + ppt->y + prcce->bottom);
        else
            ::OffsetRect(prc, 0, prcst->bottom - prc->bottom);

        bRes = TRUE;
    }

    return bRes;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

    static RECT rcce = { 0 };
    static RECT rcsz = { 0 };
    static RECT rcsd = { 200, 200, 200, 200 };

    switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case WM_NCLBUTTONDOWN:
        {
            POINT pt = { 0 };
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);

            RECT rc = { 0 };
            ::GetWindowRect(hWnd, &rc);

            // Cursor to edge
            rcce.left = rc.left - pt.x;
            rcce.top = rc.top - pt.y;
            rcce.right = rc.right - pt.x;
            rcce.bottom = rc.bottom - pt.y;

            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_ENTERSIZEMOVE:
        {
            // Snap zone = Work area
            ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcsz, 0);
        }
        break;
    case WM_EXITSIZEMOVE:
        break;
    case WM_MOVING:
        {
            LPRECT lprc = (LPRECT)lParam;

            POINT pt = { 0 };
            ::GetCursorPos(&pt);

            ::SnapUnsnapRect(lprc, &pt, &rcce, &rcsz, &rcsd);
        }
        break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
