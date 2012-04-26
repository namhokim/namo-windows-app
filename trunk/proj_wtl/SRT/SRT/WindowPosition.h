#define RESTORE	0
#define BACKUP	1
//#define MAX_REGISTRY_NAME	512

// 주의사항! : swName은 레지스트리에 저장할 S/W 이름이다.
//			   사용하는 프로그램의 이름은 모두 다르게 지정을 해야 잘못된 위치에 나타나지 않는다.
BOOL WindowPosition(HWND hWnd, LPCTSTR swName, int actionCode);		// 윈도우의 위치를 설정(복귀/백업)

BOOL RestoreWindowPosition(HWND hWnd, LPCTSTR swName);	// 윈도우 정보 -> 윈도우 조정
BOOL BackupWindowPosition(HWND hWnd, LPCTSTR swName);		// 최종 윈도우 정보 -> 정보 기억

// 기능 : 화면영역과 창영역을 구하여 대체 위치를 반환한다(시각적 가운데 정보반환)
BOOL GetAlternatePosition(HWND hWnd, RECT &getRect);

// 기능 : 화면영역과 창영역을 계산하여 시각적 가운데를 구한다.
BOOL GetVisuallyCentered(RECT desktop, RECT currWnd, RECT &infoVC);	// 시각적으로 가운데위치 구하기


// 차후 개선사항
// 1. 성능상 : 레지스트리 읽기를 한번에 하고 (구분자; 예) 공백)으로 나누어서 split하는 것이 좋다.
// 2. 해상도 변경(고->저) 시, 개발자 환경보다 사용자 해상도가 낮을 때, 윈도우 통제가 불가능 존재.
//	BOOL CheckValidPosition(hWnd, rt);