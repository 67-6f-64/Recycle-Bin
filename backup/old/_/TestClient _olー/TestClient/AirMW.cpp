#include"AirWindow.h"

HCURSOR DefaultCursor = LoadCursorA(NULL, IDC_ARROW);

AirWindow::AirWindow(){
	hInstanceAW = NULL;
	hWndAW = NULL;
	IsBaseWindow = false;
	GuiControl = NULL;
}

//����
LRESULT CALLBACK AirWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch(msg){
		case WM_CREATE:
			//GuiControl(LOWORD(wParam));
			break;
		default:
			break;
	}
	return _DefWindowProcA(hwnd, msg, wParam, lParam);
}

//�}�E�X�J�[�\���C��
LRESULT WINAPI AirWindow::_DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){

	if(GetFocus() != hWnd){//�L�[�t�H�[�J�X�����C���E�B���h�E�ɖ���
		switch(Msg){
			case WM_MOUSEMOVE://�}�E�X�����C���E�B���h�E����ړ�
				SetCursor(DefaultCursor);//�J�[�\����ʏ�̃A�C�R���ɕύX
				break;

			case WM_LBUTTONUP://���C���E�B���h�E��ŃN���b�N�����ꂽ
				SetFocus(hWnd);
				break;

			default:
				break;
		}
	}

	switch(Msg){//����̖h�~(�T�u�E�B���h�E)
			case WM_DESTROY:
			case WM_CLOSE:

				if(IsBaseWindow){
					PostQuitMessage(0);
				}
				else{
					Msg = NULL;
					wParam = NULL;
					lParam = NULL;
					ShowWindow(hWnd, SW_HIDE);
				}
				break;

			default:
				break;
		}

	return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

//������
void AirWindow::SetLayer(BYTE bAlpha){
		//�X�^�C���t��
		SetWindowLongA(hWndAW, GWL_EXSTYLE, GetWindowLongA(hWndAW, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWndAW, 0, ((bAlpha * 0x100) / 0x64), LWA_ALPHA);
		return;
}

//�T�C�Y����
void AirWindow::AdjustWindowSize(int nWidth, int nHeight){
	RECT W_RECT, C_RECT;
	//�g���܂߂��E�B���h�E�̊p�̍��W���擾
	GetWindowRect(hWndAW, &W_RECT);
	//�g���܂߂Ȃ�(�N���C�A���g�̈�)�̊p�̍��W���擾
	GetClientRect(hWndAW, &C_RECT);
	//�w��T�C�Y�ɕύX
	SetWindowPos(hWndAW, HWND_TOP, NULL, NULL, (nWidth + (W_RECT.right-W_RECT.left)-(C_RECT.right-C_RECT.left)), (nHeight + (W_RECT.bottom-W_RECT.top)-(C_RECT.bottom-C_RECT.top)), SWP_NOMOVE);
	return;
}

//�E�B���h�E�쐬
bool AirWindow::CreateSimpleWindow(HINSTANCE hInstance, LPCSTR lpWindowName, int nWidth, int nHeight, void (*pFunction)(WORD), WNDPROC lpfnWndProc, bool bMain, BYTE bAlpha, bool bShow){
	WNDCLASSEXA wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEXA));
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;

	DWORD ndr;

	_asm{
		mov eax,AirWindow::WindowProc
		mov dword ptr [ndr],eax
	}
	
	if(!lpfnWndProc){
		wc.lpfnWndProc =  (WNDPROC)ndr;
	}
	else{
		wc.lpfnWndProc =  (WNDPROC)ndr;

	}

	wc.lpszClassName = lpWindowName;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = DefaultCursor;//�Ȃ�ƂȂ��J�[�\���̃A�C�R���ɂ��Ă݂�
	wc.hIconSm = DefaultCursor;

	//�E�B���h�E�o�^
	if(!RegisterClassExA(&wc)){
		return false;
	}

	//�֐��ݒ�
	GuiControl = pFunction;

	//�E�B���h�E�쐬
	hWndAW = CreateWindowExA(NULL, lpWindowName, lpWindowName, (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME), CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);
	
	AdjustWindowSize(nWidth, nHeight);//�T�C�Y�ύX

	//������
	if(bAlpha){
		SetLayer(bAlpha);
	}

	if(bShow){
		ShowWindow(hWndAW, SW_SHOW);//����
	}else{
		ShowWindow(hWndAW, SW_HIDE);
	}

	hInstanceAW = hInstance;

	if(bMain){
		IsBaseWindow = true;
	}

	return true;
}

//�������܂őҋ@
void AirWindow::WaitForQuit(){
	MSG lpMsg;

	while(GetMessageA(&lpMsg, NULL, NULL, NULL) > 0){
		TranslateMessage(&lpMsg);
		DispatchMessageA(&lpMsg);
	}

	return;
}