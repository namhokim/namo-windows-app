#pragma once

#include "SDL_Window.h"

class GameLv1
{
public:
	GameLv1(SDL_Window*, int pageID, int pageMenu);
	~GameLv1(void);
	void Reset();
	void GoMenuPage();

	void CursorUP();
	void CursorDown();
	void CursorRight();
	void CursorLeft();
	void SpaceDown();

private:
	SDL_Window* win;
	int pageID, pageMenu, x, y;

	SDL_Page* GetPage();
	RECT_INFO* GetSelRect();
	void updateSelection();
};
