#pragma once

#include "SDL_Window.h"

#define IMAGE_MAX_LV1	9

namespace lv1 {
	extern const int Size;
}

extern const char* ImagePrefix;
extern const char* ImageSuffix;
extern const char* LogoImage;	// 홀수개로 만들기 위한 타일

class GameLv1
{
public:
	GameLv1(SDL_Window*, int pageID, int pageMenu);
	~GameLv1(void);

	void StartGame();				// 게임 시작
	void Endgame();					// 게임 종료
	void GoMenuPage();				// 메뉴로 이동(ESC)
	void CursorMove(SDLKey key);	// 커서이동
	void SpaceDown();				// 스페이스바

	bool IsCleared();	// 게임이 클리어되었다.
	void UpdateElapTime();	// 시간 표시 갱신

private:
	typedef enum {first, second, need_reset,cleared} g_status;	// 게임상태

	SDL_Window* win;			// 윈도우
	int pageID, pageMenu, x, y;	// 현재 페이지ID, 메인 페이지ID, 커서(x,y)

	int openCount, remainedCount;	// 열어본 카운터, 남은 카운터
	g_status stat;					// 게임상태
	int firstX, firstY, secondX, secondY;	// 열어본 이미지 좌표
	char buffer_counter[BUF_SIZE];			// 시도한 횟수 문자열 담을 버퍼
	char img_file[IMAGE_MAX_LV1][_MAX_PATH];	// 이미지 파일 경로 버퍼
	char buffer_time[20];					// 시간(문자열)
	Uint32 startTime;						// 시작 시간
	SDL_TimerID tID;						// 타이머 ID

	SDL_Page* GetPage();			// 현재 페이지 가져오기
	RECT_INFO* GetSelRect();		// 선택한 사각형 객체 가져오기
	void updateSelection();			// 커서의 위치를 화면에 반영
	bool IsFlipped(int x, int y);	// 돌려봤는지 확인
	void Flip(int x, int y);		// 돌리기
	void ResetFlips();				// 열어본 이미지 되돌리기
	bool IsSameImage(int x, int y);	// 동일한 이미지인지 확인
	const char* GetImageName(int x, int y);	// 이미지 이름 확인
	void updateTryOpenCounter(int count);	// 시도한 횟수 화면에 반영
	void Shuffle();							// 이미지를 섞기
	void displayClearedGame(bool bVisible);	// clear 여부 화면에 표시

	void initGame();		// 게임 초기화
	void startTimer();		// 타이머 시작
	void stopTimer();		// 타이머 정지

	void CursorUP();		// 커서 위 처리
	void CursorDown();		// 커서 위 처리
	void CursorLeft();		// 커서 위 처리
	void CursorRight();		// 커서 위 처리

	static Uint32 callbackTimeUpdate(Uint32 interval, void *param);
};
