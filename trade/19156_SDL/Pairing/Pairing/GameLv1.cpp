#include "GameLv1.h"

const int Distance = 100;
const int Selection_ID = 1;

GameLv1::GameLv1(SDL_Window* win, int pageID, int pageMenu)
{
	this->win = win;
	this->pageID = pageID;
	this->pageMenu = pageMenu;

	Reset();
}

GameLv1::~GameLv1(void)
{
}

void GameLv1::Reset()
{
	this->x = 0;
	this->y = 0;
}

void GameLv1::GoMenuPage()
{
	win->SelectPage(pageMenu);
}

void GameLv1::CursorUP()
{
	y--;
	updateSelection();
}
void GameLv1::CursorDown()
{
	y++;
	updateSelection();
}
void GameLv1::CursorRight()
{
	x++;
	updateSelection();
}
void GameLv1::CursorLeft()
{
	x--;
	updateSelection();
}

void GameLv1::SpaceDown()
{
	printf("flip (%d, %d)\n", x, y);
}

//////////////////////////////////////////////////////////////////////////
// private

SDL_Page* GameLv1::GetPage()
{
	if(win==NULL) return NULL;
	else return win->GetPage(pageID);
}

RECT_INFO* GameLv1::GetSelRect()
{
	SDL_Page* page = GetPage();
	if(page==NULL) return NULL;
	else return page->GetRectInfo(Selection_ID);
}

void GameLv1::updateSelection()
{
	RECT_INFO* sel = GetSelRect();
	if(sel==NULL) return;

	sel->x = sel->x_ori + (x*Distance);
	sel->y = sel->y_ori + (y*Distance);

	printf("(%d,%d)\n", sel->x, sel->y);
}