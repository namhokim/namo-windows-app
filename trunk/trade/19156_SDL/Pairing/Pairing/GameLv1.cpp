#include "GameLv1.h"

const int Distance = 100;
const int Selection_ID = 1;
const int Size = 3;
const int IndexMin = 0;
const int IndexMax = (Size-1);
const int SolveCount = ( (Size*Size)/2 );

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

	openCount = 0;
	remainedCount = SolveCount;
	stat = first;
	firstX = firstY = secondX = secondY = -1;
}

void GameLv1::GoMenuPage()
{
	win->SelectPage(pageMenu);
}

void GameLv1::CursorUP()
{
	ResetFlips();
	y--;
	if (y<IndexMin) y = IndexMax;
	updateSelection();
}
void GameLv1::CursorDown()
{
	ResetFlips();
	y++;
	if (y>IndexMax) y = IndexMin;
	updateSelection();
}
void GameLv1::CursorLeft()
{
	ResetFlips();
	x--;
	if (x<IndexMin) x = IndexMax;
	updateSelection();
}
void GameLv1::CursorRight()
{
	ResetFlips();
	x++;
	if (x>IndexMax) x = IndexMin;
	updateSelection();
}


void GameLv1::SpaceDown()
{
	if (remainedCount<=0) return;

	switch(stat) {
		case first:
			if (!IsFlipped(x, y)) return;
			else {
				Flip(x, y);
				firstX = x;
				firstY = y;
				stat = second;
			}
			break;
		case second:
			if (!IsFlipped(x, y)) return;
			else {
				Flip(x, y);
				openCount++;

				if (IsSameImage(x,y)) {
					firstX = firstY = secondX = secondY = -1;
					--remainedCount;
					stat = first;
				} else {
					secondX = x;
					secondY = y;
					stat = need_reset;
				}
				
			}
			break;
		case need_reset:
			break;
	}

	printf("flip (%d, %d), openCount : %d, stat : %d, remainedCount: %d\n",
		x, y, openCount, stat, remainedCount);
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

bool GameLv1::IsFlipped(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		IMAGE_INFO* pI = page->GetImageInfo(x + (y * Size));
		if (pI!=NULL)
		{
			return (pI->bFlip);
		}
	}
	return false;
}

void GameLv1::Flip(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page==NULL) return;

	IMAGE_INFO* pI = page->GetImageInfo(x + (y * Size));
	if (pI!=NULL) {
		pI->bFlip = !(pI->bFlip);
	}
}

void GameLv1::ResetFlips()
{
	if (stat!=need_reset) return;

	if (firstX!=-1 && firstY!=-1 && !IsFlipped(firstX, firstY)) {
		Flip(firstX, firstY);
	}
	if (secondX!=-1 && secondY!=-1 && !IsFlipped(secondX, secondY)) {
		Flip(secondX, secondY);
	}
	firstX = firstY = secondX = secondY = -1;

	stat = first;
}

bool GameLv1::IsSameImage(int x, int y)
{
	if (stat!=second) return false;

	if (firstX!=-1 && firstY!=-1 && !IsFlipped(firstX, firstY)) {
		const char* n1 = GetImageName(firstX, firstY);
		const char* n2 = GetImageName(x, y);
		return (strcmp(n1, n2)==0);
	} else {
		return false;
	}
}

const char* GameLv1::GetImageName(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		IMAGE_INFO* pI = page->GetImageInfo(x + (y * Size));
		if (pI!=NULL) {
			return pI->file;
		}
	}
	return NULL;
}