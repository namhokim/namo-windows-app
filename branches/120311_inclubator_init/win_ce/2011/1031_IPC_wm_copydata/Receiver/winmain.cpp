#pragma comment(linker, "/subsystem:windows")
#include <windows.h>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR          lpCmdLine, int nShowCmd )
{
	ATOM atom;
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground= (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor            = LoadCursor( 0, IDC_ARROW );
	wc.hIcon              = LoadIcon( 0, IDI_APPLICATION);
	wc.hInstance  = hInstance;
	wc.lpfnWndProc  = WndProc; 
	wc.lpszClassName= TEXT("Receiver");
	wc.lpszMenuName = 0;
	wc.style              = 0;

	atom = RegisterClass( &wc);

	if ( atom == 0 ) 
	{
		MessageBox( 0, TEXT("Fail To RegisterClass"), TEXT("Error"), MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx( 0, TEXT("Receiver"), TEXT("Receiver"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT,0, 0, 0, 
		hInstance, 0); 
	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );

	while ( GetMessage( &msg, 0, 0, 0) ) 
	{
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{        
	case WM_CREATE:
		return 0;
	case WM_COPYDATA:
		{
			COPYDATASTRUCT *p = (COPYDATASTRUCT*)lParam;

			if(p->dwData == 1)        // 식별자 확인
			{
				char *temp = (char*)p->lpData;
				MessageBoxA(0, temp, "도착한 data", MB_OK);
			}
		}
		return 0;

		// B를 먼저 실행하세요..
		// 그리고 A를 실행한 후에 문자열을 넣고 Enter...
	case WM_LBUTTONDOWN: 
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}