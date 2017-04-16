// colorPciker.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ColorPicker.h"
#define MAX_LOADSTRING 100

#include "stdio.h"

// グローバル変数 :
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

static BOOLEAN s_bHex;
static HWND s_hDesktopWnd;
static HDC s_hDesktopDC;
static BOOLEAN s_bForceRepaint = false;


// このコード モジュールに含まれる関数の宣言を転送します :
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
bool displayColor(HWND hWnd, HDC hDC);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_COLORPCIKER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します。:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_COLORPCIKER);

	// メイン メッセージ ループ :
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
//  関数 : MyRegisterClass()
//
//  目的 : ウィンドウ クラスを登録します。
//
//  コメント :
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//     Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた 
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_COLORPCIKER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_COLORPCIKER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   関数 : InitInstance(HANDLE, int)
//
//   目的 : インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント :
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, 
					   szTitle, 
					   (WS_OVERLAPPEDWINDOW|WS_BORDER) &			 // 設定する属性
					   ~(WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX ), // はずす属性
					   CW_USEDEFAULT, 
					   0, 
					   CW_USEDEFAULT, 
					   0, 
					   NULL, 
					   NULL, 
					   hInstance, 
					   NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void CALLBACK TimerProc(
   HWND hWnd,      // handle of CWnd that called SetTimer
   UINT nMsg,      // WM_TIMER
   UINT nIDEvent,  // timer identification
   DWORD dwTime    // system time
){
	PostMessage(hWnd,WM_USER + 1,NULL,NULL);
}

int SelectTopMost(HWND hWnd,HMENU hMenu,BOOL bTopMost){
	HWND hWndInsertAfter;
	UINT uCheck;

	if(bTopMost){
		hWndInsertAfter = HWND_TOPMOST;
		uCheck = MF_CHECKED;
	}else{
		hWndInsertAfter = HWND_NOTOPMOST;
		uCheck = MF_UNCHECKED;
	}

	CheckMenuItem(
		hMenu,
		IDM_TOPMOST,
		uCheck);   

	SetWindowPos(
		hWnd,
		hWndInsertAfter,  // 配置順序のハンドル
		0,0,0,0,
		SWP_NOSIZE | SWP_NOMOVE);

	return 0;
}

void resizeWindow(HWND hWnd,HDC hDesktopDC,int width, int height)
{
	int desktopWidth;
	int desktopHeight;
	int windowWidth;
	int windowHeight;

	desktopWidth  = GetDeviceCaps(hDesktopDC, HORZRES);
	desktopHeight = GetDeviceCaps(hDesktopDC, VERTRES);

	windowWidth   = GetSystemMetrics(SM_CXFIXEDFRAME) * 2 +
					width;
	windowHeight  = GetSystemMetrics(SM_CYCAPTION) +
					GetSystemMetrics(SM_CYMENU) +
					GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
					height;

	MoveWindow(hWnd,
			   desktopWidth - windowWidth,
			   0,
			   windowWidth,
			   windowHeight,
			   true);

}

//
//  関数 : WndProc(HWND, unsigned, WORD, LONG)
//
//  目的 :  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	static HDC s_myDC;
	static BOOLEAN s_bTopMost = true;
	static BOOLEAN s_bPosDisplay = false;
    
	static HMENU s_hMenu;
	const int c_clientWidth = 250;
	const int c_clientHeight = 45;
	const int c_clientMouseHeight = c_clientHeight + 50;

	switch (message) 
	{
	case WM_USER + 1:
		{

			static int s_mouse_x = 0;
			static int s_mouse_y = 0;
			POINT mousePoint;

			// mouse位置が変わっていなかったらなにもしなくてよい
			GetCursorPos(&mousePoint);

			if(mousePoint.x == s_mouse_x &&
			   mousePoint.y == s_mouse_y && 
			   s_bForceRepaint != TRUE)
			{
				return 0;
			}

			InvalidateRect(hWnd, NULL, false);


		}
		break;
	case WM_CREATE:
        s_hDesktopWnd = GetDesktopWindow();
		s_hDesktopDC = GetDC(s_hDesktopWnd);
		s_myDC = GetDC(hWnd);
		SetTimer(hWnd,1,100,TimerProc);

        // windowのリサイズ
        resizeWindow(hWnd,s_hDesktopDC,c_clientWidth,c_clientHeight);

        // 16進ではない
		s_bHex = false;

        // 位置は表示しない
        s_bPosDisplay = false;

		s_hMenu = GetMenu(hWnd);

		CheckMenuRadioItem(
			s_hMenu,		// メニューのハンドル
			IDM_HEX,		// 最初の項目の識別子または位置
			IDM_DECI,		// 最後の項目の識別子または位置
			IDM_DECI,		// メニュー項目の識別子または位置
			MF_BYCOMMAND    // 関数のオプション
		);

		CheckMenuRadioItem(
			s_hMenu,		// メニューのハンドル
			IDM_MOUSEPOS_ON,		// 最初の項目の識別子または位置
			IDM_MOUSEPOS_OFF,		// 最後の項目の識別子または位置
			IDM_MOUSEPOS_OFF,		// メニュー項目の識別子または位置
			MF_BYCOMMAND    // 関数のオプション
		);

		SelectTopMost(hWnd,s_hMenu,s_bTopMost);

		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// 選択されたメニューの解析 :
		switch (wmId)
		{
		case IDM_EXIT:
			PostQuitMessage(0);
			break;
		case IDM_HEX:
			CheckMenuRadioItem(
				s_hMenu,		// メニューのハンドル
				IDM_HEX,		// 最初の項目の識別子または位置
				IDM_DECI,		// 最後の項目の識別子または位置
				IDM_HEX,		// メニュー項目の識別子または位置
				MF_BYCOMMAND    // 関数のオプション
			);
			s_bHex = true;
			break;
		case IDM_DECI:
			CheckMenuRadioItem(
				s_hMenu,		// メニューのハンドル
				IDM_HEX,		// 最初の項目の識別子または位置
				IDM_DECI,		// 最後の項目の識別子または位置
				IDM_DECI,		// メニュー項目の識別子または位置
				MF_BYCOMMAND    // 関数のオプション
			);
			s_bHex = false;
			break;
		case IDM_MOUSEPOS_ON:
			CheckMenuRadioItem(
				s_hMenu,		// メニューのハンドル
				IDM_MOUSEPOS_ON,		// 最初の項目の識別子または位置
				IDM_MOUSEPOS_OFF,		// 最後の項目の識別子または位置
				IDM_MOUSEPOS_ON,		// メニュー項目の識別子または位置
				MF_BYCOMMAND    // 関数のオプション
			);
            // windowのリサイズ
            resizeWindow(hWnd,s_hDesktopDC,c_clientWidth,c_clientMouseHeight);

			s_bPosDisplay = true;
			break;
		case IDM_MOUSEPOS_OFF:
			CheckMenuRadioItem(
				s_hMenu,		// メニューのハンドル
				IDM_MOUSEPOS_ON,		// 最初の項目の識別子または位置
				IDM_MOUSEPOS_OFF,		// 最後の項目の識別子または位置
				IDM_MOUSEPOS_OFF,		// メニュー項目の識別子または位置
				MF_BYCOMMAND    // 関数のオプション
			);
            // windowのリサイズ
            resizeWindow(hWnd,s_hDesktopDC,c_clientWidth,c_clientHeight);

			s_bPosDisplay = true;
			break;
		case IDM_TOPMOST:
			/* topmost */
			s_bTopMost = !s_bTopMost;	// set new state

			SelectTopMost(hWnd,s_hMenu,s_bTopMost);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

        // 描画してください
        s_bForceRepaint = true;

		break;
	case WM_PAINT:
		{
			PAINTSTRUCT paint;

			BeginPaint(hWnd, &paint);

			if ( displayColor(hWnd, s_myDC) && s_bPosDisplay == TRUE)
			{
				//////////////////////////
				// クライアント領域内のマウス位置取得
				POINT org_point, point;
				HWND hTargetWnd;
				char szPos[256];

				GetCursorPos(&org_point);

				hTargetWnd = WindowFromPoint(org_point);

				point = org_point;

				MapWindowPoints(HWND_DESKTOP, hTargetWnd, &point, 1);
				if( point.x < 0 || point.y < 0 )
				{
#if 0
					RECT targetRect;
					GetWindowRect(hTargetWnd, &targetRect);
					point.x = org_point.x - targetRect.left;
					point.y = org_point.y - targetRect.top;
#else
					hTargetWnd = 0;
					point = org_point;
#endif
				}

				sprintf_s(szPos,sizeof(szPos),"hWnd = 0x%08x    ",
						hTargetWnd);

				TextOut(s_myDC, 1, 37, (LPCSTR)szPos, (int)strlen(szPos));

				sprintf_s(szPos,sizeof(szPos),"pos  = %03d,%03d    ",
						point.x,
						point.y);

				TextOut(s_myDC, 1, 58, (LPCSTR)szPos, (int)strlen(szPos));
			}

			EndPaint(hWnd, &paint);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ReleaseDC(s_hDesktopWnd,s_hDesktopDC);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラです。
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}


bool displayColor(HWND hWnd, HDC hDC)
{
	POINT pt;
	char szPos[256];
	COLORREF myColor;
	BYTE r,g,b;
	RECT rect;
    static int s_mouse_x = 0;
    static int s_mouse_y = 0;
    POINT mousePoint;

    // mouse位置が変わっていなかったらなにもしなくてよい
    GetCursorPos(&mousePoint);

    // 次は描画しなくていいです
    s_bForceRepaint = false;

    s_mouse_x = mousePoint.x;
    s_mouse_y = mousePoint.y;


	GetClientRect(hWnd,&rect);

    // クリア
//    ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, "", 0, NULL );
/*	
	HBRUSH old_brush = (HBRUSH)SelectObject(hDC , CreateSolidBrush(0XFF << 16));
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	DeleteObject( SelectObject(hDC , old_brush) );
*/
	GetCursorPos(&pt);
	// TODO: 描画コードをここに追加してください...

	StretchBlt(
				hDC,
				1, 1, 30, 30, 				// dst l,t,r,b
				s_hDesktopDC,				// src
				pt.x, pt.y, 1, 1,			// src l,t,r,b
				SRCCOPY);					// ope code

	myColor = GetPixel(hDC,10,10);

	r = GetRValue(myColor);
	g = GetGValue(myColor);
	b = GetBValue(myColor);

	
//	sprintf(szPos,"                               ");
//	TextOut(hDC, 40, 5, (LPCSTR)szPos, (int)strlen(szPos));

	if(s_bHex == false){
		sprintf_s(szPos,sizeof(szPos),"color = [%03d %03d %03d]     ",
				r,g,b);
	}else{
		sprintf_s(szPos,sizeof(szPos),"color=[0x%02X 0x%02X 0x%02X]     ",
				r,g,b);
	}

	TextOut(hDC, 40, 5, (LPCSTR)szPos, (int)strlen(szPos));

	return true;
}
