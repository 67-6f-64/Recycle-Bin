#include"Hyado.h"

LRESULT CALLBACK TestWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			break;
		case WM_COMMAND:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}


void SetLayer(HWND hwnd, int invisible){
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, 0, (255 * invisible) / 100, LWA_ALPHA);
}

void WindowResize(HWND hwnd, int Width, int Height){
	RECT W_RECT, C_RECT;
	//�g���܂߂��E�B���h�E�̊p�̍��W���擾
	GetWindowRect(hwnd, &W_RECT);
	//�g���܂߂Ȃ�(�N���C�A���g�̈�)�̊p�̍��W���擾
	GetClientRect(hwnd, &C_RECT);
	//�w��T�C�Y�ɕύX
	SetWindowPos(hwnd, HWND_TOP, NULL, NULL, (Width + (W_RECT.right-W_RECT.left)-(C_RECT.right-C_RECT.left)), (Height + (W_RECT.bottom-W_RECT.top)-(C_RECT.bottom-C_RECT.top)), SWP_NOMOVE);
}

bool CreateSimpleWindow(HINSTANCE hInstance, char *Caption, int Width, int Height, WNDPROC lpfnWndProc, int invisible){
	WNDCLASSEXA wc;
	HWND hwnd;
	MSG lpMsg;
	ZeroMemory(&wc, sizeof(WNDCLASSEXA));
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = lpfnWndProc;
	wc.lpszClassName = Caption;
	wc.style = CS_HREDRAW|CS_VREDRAW;// | CS_DBLCLKS;

	//�E�B���h�E�o�^
	if(!RegisterClassExA(&wc)) return false;
	//�E�B���h�E�쐬
	hwnd = CreateWindowExA(NULL, Caption, Caption, (WS_EX_LAYERED | WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);
	WindowResize(hwnd, Width, Height);//�T�C�Y�ύX
	if(invisible) SetLayer(hwnd, invisible);
	ShowWindow(hwnd, SW_SHOW);//����

	//���鏈��������܂őҋ@�ł������ӂ�����܂��̂œǂ�ł�
	/*
		���{��ϊ��E�B���h�E��hwnd�Ƃ͕ʂȂ̂�
		�w�肵�Ă��܂��̂͂�낵���Ȃ��ł�(���{��ϊ��E�B���h�E����������Ȃ��Ȃ�)
		�p�ꂾ���̏ꍇ��hwnd�ŌŒ肵�Ă���Ă����Ǝv���܂���
	*/
	while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}

	return true;
}
