#pragma once

#include "SDL_Window.h"

#define BUF_SIZE 10

class GameLv1
{
public:
	GameLv1(SDL_Window*, int pageID, int pageMenu);
	~GameLv1(void);
	void Reset();
	void GoMenuPage();

	void CursorUP();
	void CursorDown();
	void CursorLeft();
	void CursorRight();
	void SpaceDown();	// 스페이스바

private:
	typedef enum {
		first, second, need_reset,
	} g_status;	// 게임상태
	SDL_Window* win;
	int pageID, pageMenu, x, y;

	int openCount, remainedCount;
	g_status stat;
	int firstX, firstY, secondX, secondY;

	SDL_Page* GetPage();
	RECT_INFO* GetSelRect();
	void updateSelection();
	bool IsFlipped(int x, int y);
	void Flip(int x, int y);
	void ResetFlips();
	bool IsSameImage(int x, int y);
	const char* GetImageName(int x, int y);
	void updateTryOpenCounter(int count);
	char buffer_counter[BUF_SIZE];
};
