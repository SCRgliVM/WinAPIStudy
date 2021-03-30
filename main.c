// compiled and worked in TDM-GCC 4.9.2 64-bit Profiling
#include <windows.h>
#include <stdio.h>

HMODULE MainModuleHandle;
LRESULT CALLBACK MainWindowProc(HWND, UINT, WPARAM, LPARAM);
HWND HandleMainWindow;
HMENU MainMenu;

int main () 
{
	MainModuleHandle = GetModuleHandle(NULL);
	ATOM AtomMainWindowClass = (ATOM)RegisterMainWindowClass(MainModuleHandle);
	CreateMainWindow(MainModuleHandle, SW_SHOW);
	// main message loop 
	MSG messageStruct;
	BOOL returnGetMessageinMainLoopMessageHandling;
	while(GetMessage(&messageStruct, (HWND)NULL, 0, 0))
	{
		TranslateMessage(&messageStruct);
		DispatchMessage(&messageStruct);
	}
	return 0;	
}

int RegisterMainWindowClass (HMODULE MainModuleHandle)
// Register main window class
{
	WNDCLASSEXA MainWindowClass;
	MainWindowClass.cbClsExtra = 0;
	MainWindowClass.cbSize = sizeof(MainWindowClass);
	MainWindowClass.cbWndExtra = 0;
	MainWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	MainWindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	MainWindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
	MainWindowClass.hIconSm = LoadImageA(MainModuleHandle, MAKEINTRESOURCE(5),
								IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),
								GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	MainWindowClass.hInstance = MainModuleHandle;
	MainWindowClass.lpfnWndProc = (WNDPROC)MainWindowProc;
	MainWindowClass.lpszClassName = "MainWClass";
	MainWindowClass.lpszMenuName = "MenuM";
	MainWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	
	return RegisterClassEx(&MainWindowClass);
}

int CreateMainWindow(HINSTANCE MainModuleHandle, int nCmdShow)
// Create main window
{
	HandleMainWindow = CreateWindowEx(
	WS_EX_LEFT,
	"MainWClass",
	"Крестики и нолики",
	WS_OVERLAPPEDWINDOW, 
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	(HWND)NULL,
	(HMENU)NULL,
	(HINSTANCE)MainModuleHandle,
	(LPVOID)NULL
	);
	int ResultShowWindowFunc = ShowWindow(HandleMainWindow, nCmdShow);
	int ResultUpdateWindowFunc = UpdateWindow(HandleMainWindow);
	// title-bar re-rendering
	SetWindowText(HandleMainWindow, "Крестики и нолики");
	MainMenu = LoadMenu(MainModuleHandle, "MenuM");
	SetMenu(HandleMainWindow, MainMenu);
} 

LRESULT CALLBACK MainWindowProc(HWND HandleMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam)
// main window proc
{
	switch(Msg){
		case WM_PAINT:
			{
				PAINTSTRUCT MainPaintStruct;
				HDC HandleDisplayDeviceContext = BeginPaint(HandleMainWindow, &MainPaintStruct);
				EndPaint(HandleMainWindow, &MainPaintStruct);
				return 0;	
			}
		case WM_COMMAND:
			if ((0 == lParam) & (0 == HIWORD(wParam))) // menu command
			{ 
			switch(LOWORD(wParam)){
				case 2: // EXIT COMMAND
				{
					PostQuitMessage(0);
					return 0;
				}
				return 0;
			}
			return 0;
			}			
		case WM_CREATE: 
			return 0;
		case WM_SIZE:
			return 0;
		case WM_NCCREATE:
			return 1;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(HandleMainWindow, Msg, wParam, lParam);
	}
	return 0;
}
