
// ��P�O�Q�� �E�B���h�E�^�C�g����ύX���� 



//        mcap02.cpp

#define _WIN32_WINNT 0x0500

#define STRICT
#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

char szClassName[] = "Seethrough";      //�E�B���h�E�N���X
BOOL bCap = FALSE;
HINSTANCE hInst; //�C���X�^���X�n���h��

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
                   LPSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    
    if (!InitApp(hCurInst))
        return FALSE;
    if (!InitInstance(hCurInst, nCmdShow)) 
        return FALSE;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}


//�E�B���h�E�E�N���X�̓o�^

BOOL InitApp(HINSTANCE hInst)
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;    //�v���V�[�W����
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //�C���X�^���X
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = NULL;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = "MYMENU";    //���j���[��
    wc.lpszClassName = (LPCSTR)szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    return (RegisterClassEx(&wc));
}



//�E�B���h�E�̐���

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
            "Seethrough",//�^�C�g���o�[�ɂ��̖��O���\������܂�
			(WS_OVERLAPPEDWINDOW|WS_BORDER) &			 // �ݒ肷�鑮��
			~(WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX ), // �͂�������
            CW_USEDEFAULT,    //�w���W
            CW_USEDEFAULT,    //�x���W
            150,    //��
            160,    //����
            NULL,//�e�E�B���h�E�̃n���h���A�e�����Ƃ���NULL
            NULL,//���j���[�n���h���A�N���X���j���[���g���Ƃ���NULL
            hInst,//�C���X�^���X�n���h��
            NULL);
    if (!hWnd)
        return FALSE;
    hInst = hInstance; //�C���X�^���X�n���h����ۑ�
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}



//�E�B���h�E�v���V�[�W��

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    POINTS pts;
    POINT pt;
    static HWND s_hTarget = NULL;
    HDC hdc, hdc_mem;
    PAINTSTRUCT ps;
    HBITMAP hBit;
    BITMAP bmp_info;
    static int wx, wy;
    RECT rc;
	LONG lStyle;
	static LONG s_prevLStyle;
	static BYTE bAlpha;

    switch (msg) {
        case WM_PAINT:
            if (!bCap) {
                GetClientRect(hWnd, &rc);
                hdc = BeginPaint(hWnd, &ps);
                hBit = LoadBitmap(hInst, "MYBMP");
                GetObject(hBit, sizeof(BITMAP), &bmp_info);
                wx = bmp_info.bmWidth;
                wy = bmp_info.bmHeight;
                hdc_mem = CreateCompatibleDC(hdc);
                SelectObject(hdc_mem, hBit);
                BitBlt(hdc, (rc.right - wx)/2,
                    (rc.bottom-wy)/2, wx, wy, hdc_mem, 0, 0, SRCCOPY);
                DeleteDC(hdc_mem);
                DeleteObject(hBit);
                EndPaint(hWnd, &ps);
            } 
            break;
        case WM_COMMAND:
            switch(LOWORD(wp)) {
                case IDM_END:
                    SendMessage(hWnd, WM_CLOSE, 0, 0L);
                    break;
            }
            break;
        case WM_LBUTTONDOWN:
            bCap = TRUE;
            SetCapture(hWnd);
            SetCursor(LoadCursor(hInst, "MYCURSOR"));
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case WM_MOUSEMOVE:
            if (bCap) {
                SetCursor(LoadCursor(hInst, "MYCURSOR"));
            } else
                SetCursor(LoadCursor(NULL, IDC_ARROW));
            break;
        case WM_LBUTTONUP:
            SetCursor(LoadCursor(NULL, IDC_ARROW));

			if(s_hTarget == NULL)
			{
				pts = MAKEPOINTS(lp);
				pt.x = pts.x;
				pt.y = pts.y;
				ClientToScreen(hWnd, &pt);
				s_hTarget = WindowFromPoint(pt);
				if (s_hTarget == NULL) {
					MessageBox(hWnd, "Fail To Get..", "Fail", MB_OK);
					return (DefWindowProc(hWnd, msg, wp, lp));
				}

				if(s_hTarget != hWnd){
/*
					HWND hParent;
					HWND hDesk = GetDesktopWindow();

					hParent = (HWND)GetWindowLongPtr(s_hTarget, GWL_HWNDPARENT);
					
					while(hParent)
					{
						s_hTarget = hParent;
						hParent = (HWND)GetWindowLongPtr(s_hTarget, GWL_HWNDPARENT);

					}
*/
					// �����͓����ɂ��Ȃ�
					s_prevLStyle = GetWindowLong(s_hTarget, GWL_EXSTYLE);
					lStyle = s_prevLStyle | WS_EX_LAYERED;
					SetWindowLong(s_hTarget, GWL_EXSTYLE, lStyle);
					bAlpha = 128;
					SetLayeredWindowAttributes(s_hTarget, 0, bAlpha, LWA_ALPHA);
				}else{
					/* �������w�肳�ꂽ�ꍇ�́ANULL��ݒ肵�ă��g���C */
					s_hTarget = NULL;
				}
			}else{
	            SetLayeredWindowAttributes(s_hTarget, 0, 255, LWA_ALPHA);
				SetWindowLong(s_hTarget, GWL_EXSTYLE, s_prevLStyle);
				s_hTarget = NULL;
			}

            ReleaseCapture();
            bCap = FALSE;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case WM_KEYDOWN:
			if(s_hTarget != NULL){
				RECT rect;

				GetWindowRect(s_hTarget,&rect);

				switch (wp)
				{
					case VK_RIGHT:
						rect.left += 1;
						rect.right += 1;
						break;
					case VK_LEFT:
						rect.left -= 1;
						rect.right -= 1;
						break;
					case VK_UP:
						rect.top -= 1;
						rect.bottom -= 1;
						break;
					case VK_DOWN:
						rect.top += 1;
						rect.bottom += 1;
						break;
					default:
						break;
				}

				MoveWindow(s_hTarget,
						rect.left,
						rect.top,
						rect.right - rect.left,
						rect.bottom - rect.top,
						TRUE);
			}

			break;
        case WM_CREATE:
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            break;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
			if(s_hTarget != NULL){
	            SetLayeredWindowAttributes(s_hTarget, 0, 255, LWA_ALPHA);
				lStyle = GetWindowLong(s_hTarget, GWL_EXSTYLE);
				lStyle ^= WS_EX_LAYERED;
				SetWindowLong(s_hTarget, GWL_EXSTYLE, WS_EX_LAYERED);
			}
            PostQuitMessage(0);
            break;
        default:
            return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0L;
}

