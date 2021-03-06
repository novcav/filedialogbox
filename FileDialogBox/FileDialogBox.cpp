//
// Display open file and save as file dialog box.
// 

#include "stdafx.h"
#include "filedialogbox.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void OnPaint(HWND hWnd);
void OnDestroy(HWND hWnd);

void OnFileOpen(HWND hWnd, HINSTANCE hInst);
void OnFileSaveAs(HWND hWnd, HINSTANCE hInst);
void OnFileExit(HWND hWnd);


// main window
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FILEDIALOGBOX, szWindowClass, MAX_LOADSTRING);

    //  Registers the window class.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILEDIALOGBOX));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FILEDIALOGBOX);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    // Store instance handle in our global variable.
    hInst = hInstance; 

    // Create the main program window.
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    // Display the main program window.
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILEDIALOGBOX));

    MSG msg;

    // Main message loop
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//  Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_OPEN:   OnFileOpen(hWnd, hInst);				break;
		case IDM_SAVEAS: OnFileSaveAs(hWnd, hInst);				break;
		case IDM_EXIT:   OnFileExit(hWnd);						break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
        break;
	case WM_PAINT:   OnPaint(hWnd);									break;
	case WM_DESTROY: OnDestroy(hWnd);								break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;

	hDC = BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
}

//
void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

// It displays a warning in a message box if invalid filename is entered
void OnFileOpen(HWND hWnd, HINSTANCE hInst)
{
	wchar_t szFile[MAX_PATH] = L"";
    OPENFILENAME fn;

	// initialize variable fn to zero
	ZeroMemory(&fn,sizeof(OPENFILENAME));

	// set the variable fn
	fn.lStructSize = sizeof(OPENFILENAME);
	fn.hwndOwner = hWnd;
	fn.hInstance = hInst;
	fn.lpstrFilter = _T("All Files\0*.*\0");
	fn.lpstrFile = szFile;
	fn.nMaxFile = MAX_PATH;
	fn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// display open file dialog box
	if (!GetOpenFileName(&fn)) return;

    // add filename to window title
	wchar_t szText[MAX_PATH];
    wcscpy_s(szText,MAX_PATH,szTitle);
    wcscat_s(szText,MAX_PATH,L" - ");
    wcscat_s(szText,MAX_PATH, szFile);
    SetWindowText(hWnd,szText);
}

// It displays a warning in a message box if the selected file already exists.
// The user must confirm whether to overwrite the file.
void OnFileSaveAs(HWND hWnd, HINSTANCE hInst)
{
	wchar_t szFile[MAX_PATH] = L"";
    OPENFILENAME fn;

	// initialize variable fn to zero
	ZeroMemory(&fn,sizeof(OPENFILENAME));

	// set the variable fn
	fn.lStructSize = sizeof(OPENFILENAME);
	fn.hwndOwner = hWnd;
	fn.hInstance = hInst;
	fn.lpstrFilter = _T("All Files\0*.*\0");
	fn.lpstrFile = szFile;
	fn.nMaxFile = MAX_PATH;
	fn.Flags =  OFN_OVERWRITEPROMPT;

	// display save as file dialog box
	if(!GetSaveFileName(&fn)) return;

    // add filename to window title
	wchar_t szText[MAX_PATH];
    wcscpy_s(szText,MAX_PATH,szTitle);
    wcscat_s(szText,MAX_PATH,L" - ");
    wcscat_s(szText,MAX_PATH, szFile);
    SetWindowText(hWnd,szText);
}

//
void OnFileExit(HWND hWnd)
{
	DestroyWindow(hWnd);
}
