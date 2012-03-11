BOOL SetWindowPosition(HWND);					// 윈도우의 위치를 설정
BOOL GetAlternatePosition(HWND, RECT &);		// 기능 : 화면영역과 창영역을 구하여 대체 위치를 반환한다(시각적 가운데 정보반환)
BOOL GetVisuallyCentered(RECT, RECT, RECT &);	// 기능 : 화면영역과 창영역을 계산하여 시각적 가운데를 구한다.