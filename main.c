// compiled and worked in TDM-GCC 4.9.2 64-bit Profiling
#include <windows.h>
#include <stdio.h>

HMODULE MainModuleHandle;
LRESULT CALLBACK MainWindowProc(HWND, UINT, WPARAM, LPARAM);
HWND HandleMainWindow;
HMENU MainMenu;
HPEN GridPenHandle;
int x, y, nWidth, nHeight, metricX, metricY;
LONG Height, Width;
HDC HandleDisplayDeviceContext;
int xRectangle, yRectangle;
LONG mHeight, mWidth;
PAINTSTRUCT MainPaintStruct;
struct RECORDINTABLE {
	int visible;
	int OorY; 
	int top;
	int bottom;
	int left;
	int right;
};
struct RECORDINTABLE MainTable[9];
int IndexArray[3][3];

int main () 
{
	IndexArray[0][0] = 0;
IndexArray[0][1] = 1;
IndexArray[0][2] = 2;
IndexArray[1][0] = 3;
IndexArray[1][1] = 4;
IndexArray[1][2] = 5;
IndexArray[2][0] = 6;
IndexArray[2][1] = 7;
IndexArray[2][2] = 8;
	int i1 = 0;
	while(i1<9){
		MainTable[i1].visible = 0;
		i1++;
	}
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
	GridPenHandle = CreatePen(PS_SOLID, 5, 0x00FF0000);
	GetMainWindowSize();
	HandleMainWindow = CreateWindowEx(
	WS_EX_LEFT,
	"MainWClass",
	"Крестики и нолики",
	WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 
	x,
	y,
	nWidth,
	nHeight,
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
				HandleDisplayDeviceContext = BeginPaint(HandleMainWindow, &MainPaintStruct);
				GetMetricWindow();
				PaintGrid();
				PaintSymbol();
				//char bufe[128];
				//wsprintf(bufe, "Width: %d\nHeight: %d", Width, Height);
				//MessageBox(HandleMainWindow, bufe,"C",MB_OK);
				//LineTo(HandleDisplayDeviceContext, 50, 50);
				EndPaint(HandleMainWindow, &MainPaintStruct);
				return 0;	
			}
		case WM_LBUTTONUP:
			{
			RECT ClientArea;
				GetClientRect(HandleMainWindow, &ClientArea);
				LONG ClientAreaHeight, ClientAreaWidth;
				ClientAreaHeight = ClientArea.bottom - ClientArea.top;
				ClientAreaWidth = ClientArea.right - ClientArea.left;
				WORD xLbuttonUp, yLbuttonUp;
				xLbuttonUp = LOWORD(lParam);
				yLbuttonUp = HIWORD(lParam);
				if (xLbuttonUp <= ClientAreaWidth/3){
					xRectangle = 1;
				}
				else if (xLbuttonUp <= 2*ClientAreaWidth/3){
					xRectangle = 2;
				}
				else{
					xRectangle = 3;
				}
				if (yLbuttonUp <= ClientAreaHeight/3){
						yRectangle = 1;
					}
					else if (yLbuttonUp <= 2*ClientAreaHeight/3){
						yRectangle = 2;
					}
					else{
						yRectangle = 3;
					}
				InputRecord();
				//char bufe[128];
				//wsprintf(bufe, "xRectangle: %d\nyRectangle: %d", xRectangle, yRectangle);
				//MessageBox(HandleMainWindow, bufe,"C",MB_OK);
				InvalidateRect(HandleMainWindow, NULL, 0);
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
				default:
					{
					char buf[256];
					wsprintf(buf, "Command code: %d", LOWORD(wParam));
					MessageBox(HandleMainWindow, buf, "Caption", MB_OK);
				}
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

int GetMainWindowSize()
{
 	//x = GetSystemMetrics(SM_CXSCREEN) / 4;	
 	y = GetSystemMetrics(SM_CYSCREEN) / 4;
 	x = y;
 	nWidth = 2*x+GetSystemMetrics(SM_CYBORDER);
	nHeight = 2*y+GetSystemMetrics(SM_CYMENU)+GetSystemMetrics(SM_CYCAPTION);
}

int PaintGrid(){
	SelectObject(HandleDisplayDeviceContext, GridPenHandle);
	if (Width >= Height){
		mHeight = (Height/3);
	}
	else{
		mHeight = (Width/3);
		Height = Width;
	}
		MoveToEx(HandleDisplayDeviceContext, metricX+mHeight, metricY, (LPPOINT)NULL);
		LineTo(HandleDisplayDeviceContext, metricX+mHeight, metricY+Height);
		MoveToEx(HandleDisplayDeviceContext, metricX+2*mHeight, metricY, (LPPOINT)NULL);
		LineTo(HandleDisplayDeviceContext, metricX+2*mHeight, metricY+Height);
		MoveToEx(HandleDisplayDeviceContext, metricX, metricY+mHeight, (LPPOINT)NULL);
		LineTo(HandleDisplayDeviceContext, metricX+Height, metricY+mHeight);
		MoveToEx(HandleDisplayDeviceContext, metricX, metricY+2*mHeight, (LPPOINT)NULL);
		LineTo(HandleDisplayDeviceContext, metricX+Height, metricY+2*mHeight);
		return 0;
}

int GetMetricWindow(){
	RECT Metric;
	GetClientRect(HandleMainWindow, &Metric);
	Width = Metric.right - Metric.left;
	Height = Metric.bottom - Metric.top;
	if (Width >= Height){
		metricY = 0;
		metricX = (int)((Width - Height)/2);
		return 0;
	}
	metricX = 0;
	metricY = (int)((Height - Width)/2);
	return 0;
}

int PaintSymbol(){
	int tmpIndex1 = 0;
	while (tmpIndex1 < 9){
		if (MainTable[tmpIndex1].visible == 1){
			if (MainTable[tmpIndex1].OorY == 0){
				int ResultEllipse = Ellipse(HandleDisplayDeviceContext, MainTable[tmpIndex1].left, MainTable[tmpIndex1].top,
				MainTable[tmpIndex1].right, MainTable[tmpIndex1].bottom);
				//char bufer1[256];
	   			//wsprintf(bufer1, "ResultEllipse: %d", ResultEllipse);
		  		//MessageBox(HandleMainWindow, bufer1, "Caption", MB_OK);
			}
		}
		tmpIndex1++;
	}
	return 0;
}



int InputRecord(){
	int tmpIndex = IndexArray[yRectangle-1][xRectangle-1];
	MainTable[tmpIndex].visible = 1;
	MainTable[tmpIndex].OorY = 0;
	MainTable[tmpIndex].bottom = (metricY+(yRectangle - 1)*mHeight) + 8*(mHeight/9);
	MainTable[tmpIndex].left = (metricX+(xRectangle - 1)*mHeight) + mHeight/9;
	MainTable[tmpIndex].right = (metricX+(xRectangle - 1)*mHeight) + 8*(mHeight/9);
	MainTable[tmpIndex].top = (metricY+(yRectangle - 1)*mHeight) + mHeight/9;
	
	//char bufer2[256];
	//wsprintf(bufer2, "left: %d\ntop: %d\nright: %d\nbottom: %d", MainTable[tmpIndex].left,
	//MainTable[tmpIndex].top,MainTable[tmpIndex].right,MainTable[tmpIndex].bottom);
	//MessageBox(HandleMainWindow, bufer2, "Caption", MB_OK);
	return 0;
}
