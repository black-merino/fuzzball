// colorPciker.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ColorPicker.h"
#define MAX_LOADSTRING 100

#include "stdio.h"

// �O���[�o���ϐ� :
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

static BOOLEAN s_bHex;
static HWND s_hDesktopWnd;
static HDC s_hDesktopDC;
static BOOLEAN s_bForceRepaint = false;


// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂� :
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
 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_COLORPCIKER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂��B:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_COLORPCIKER);

	// ���C�� ���b�Z�[�W ���[�v :
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
//  �֐� : MyRegisterClass()
//
//  �ړI : �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g :
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//     Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ 
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
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
//   �֐� : InitInstance(HANDLE, int)
//
//   �ړI : �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g :
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(szWindowClass, 
					   szTitle, 
					   (WS_OVERLAPPEDWINDOW|WS_BORDER) &			 // �ݒ肷�鑮��
					   ~(WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX ), // �͂�������
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
		hWndInsertAfter,  // �z�u�����̃n���h��
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
//  �֐� : WndProc(HWND, unsigned, WORD, LONG)
//
//  �ړI :  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
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

			// mouse�ʒu���ς���Ă��Ȃ�������Ȃɂ����Ȃ��Ă悢
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

        // window�̃��T�C�Y
        resizeWindow(hWnd,s_hDesktopDC,c_clientWidth,c_clientHeight);

        // 16�i�ł͂Ȃ�
		s_bHex = false;

        // �ʒu�͕\�����Ȃ�
        s_bPosDisplay = false;

		s_hMenu = GetMenu(hWnd);

		CheckMenuRadioItem(
			s_hMenu,		// ���j���[�̃n���h��
			IDM_HEX,		// �ŏ��̍��ڂ̎��ʎq�܂��͈ʒu
			IDM_DECI,		// �Ō�̍��ڂ̎��ʎq�܂��͈ʒu
			IDM_DECI,		// ���j���[���ڂ̎��ʎq�܂��͈ʒu
			MF_BYCOMMAND    // �֐��̃I�v�V����
		);

		CheckMenuRadioItem(
			s_hMenu,		// ���j���[�̃n���h��
			IDM_MOUSEPOS_ON,		// �ŏ��̍��ڂ̎��ʎq�܂��͈ʒu
			IDM_MOUSEPOS_OFF,		// �Ō�̍��ڂ̎��ʎq�܂��͈ʒu
			IDM_MOUSEPOS_OFF,		// ���j���[���ڂ̎��ʎq�܂��͈ʒu
			MF_BYCOMMAND    // �֐��̃I�v�V����
		);

		SelectTopMost(hWnd,s_hMenu,s_bTopMost);

		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// �I�����ꂽ���j���[�̉�� :
		switch (wmId)
		{
		case IDM_EXIT:
			PostQuitMessage(0);
			break;
		case IDM_HEX:
			CheckMenuRadioItem(
				s_hMenu,		// ���j���[�̃n���h��
				IDM_HEX,		// �ŏ��̍��ڂ̎��ʎq�܂��͈ʒu
				IDM_DECI,		// �Ō�̍��ڂ̎��ʎq�܂��͈ʒu
				IDM_HEX,		// ���j���[���ڂ̎��ʎq�܂��͈ʒu
				MF_BYCOMMAND    // �֐��̃I�v�V����
			);
			s_bHex = true;
			break;
		case IDM_DECI:
			CheckMenuRadioItem(
				s_hMenu,		// ���j���[�̃n���h��
				IDM_HEX,		// �ŏ��̍��ڂ̎��ʎq�܂��͈ʒu
				IDM_DECI,		// �Ō�̍��ڂ̎��ʎq�܂��͈ʒu
				IDM_DECI,		// ���j���[���ڂ̎��ʎq�܂��͈ʒu
				MF_BYCOMMAND    // �֐��̃I�v�V����
			);
			s_bHex = false;
			break;
		case IDM_MOUSEPOS_ON:
			CheckMenuRadioItem(
				s_hMenu,		// ���j���[�̃n���h��
				IDM_MOUSEPOS_ON,		// �ŏ��̍��ڂ̎��ʎq�܂��͈ʒu
				IDM_MOUSEPOS_OFF,		// �Ō�̍��ڂ̎��ʎq�܂��͈ʒu
				IDM_MOUSEPOS_ON,		// ���j���[���ڂ̎��ʎq�܂��͈ʒu
				MF_BYCOMMAND    // �֐��̃I�v�V����
			);
            // window�̃��T�C�Y
            resizeWindow(hWnd,s_hDesktopDC,c_clientWidth,c_clientMouseHeight);

			s_bPosDisplay = true;
			break;
		case IDM_MOUSEPOS_OFF:
			CheckMenuRadioItem(
				s_hMenu,		// ���j���[�̃n���h��
				IDM_MOUSEPOS_ON,		// �ŏ��̍��ڂ̎��ʎq�܂��͈ʒu
				IDM_MOUSEPOS_OFF,		// �Ō�̍��ڂ̎��ʎq�܂��͈ʒu
				IDM_MOUSEPOS_OFF,		// ���j���[���ڂ̎��ʎq�܂��͈ʒu
				MF_BYCOMMAND    // �֐��̃I�v�V����
			);
            // window�̃��T�C�Y
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

        // �`�悵�Ă�������
        s_bForceRepaint = true;

		break;
	case WM_PAINT:
		{
			PAINTSTRUCT paint;

			BeginPaint(hWnd, &paint);

			if ( displayColor(hWnd, s_myDC) && s_bPosDisplay == TRUE)
			{
				//////////////////////////
				// �N���C�A���g�̈���̃}�E�X�ʒu�擾
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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
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

    // mouse�ʒu���ς���Ă��Ȃ�������Ȃɂ����Ȃ��Ă悢
    GetCursorPos(&mousePoint);

    // ���͕`�悵�Ȃ��Ă����ł�
    s_bForceRepaint = false;

    s_mouse_x = mousePoint.x;
    s_mouse_y = mousePoint.y;


	GetClientRect(hWnd,&rect);

    // �N���A
//    ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, "", 0, NULL );
/*	
	HBRUSH old_brush = (HBRUSH)SelectObject(hDC , CreateSolidBrush(0XFF << 16));
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	DeleteObject( SelectObject(hDC , old_brush) );
*/
	GetCursorPos(&pt);
	// TODO: �`��R�[�h�������ɒǉ����Ă�������...

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
