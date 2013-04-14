#include "GameLv1.h"
#include "RndGen.h"

const int Distance = 100;
const int Selection_ID = 1;
const int Size = 3;
const int IndexMin = 0;
const int IndexMax = (Size-1);
const int SolveCount = ( (Size*Size)/2 );
const int CounterID = 4;
const int ClearedID = 5;
const int PreesAnyKeyID = 6;
const int CandidateMax = 25;

const char* ImagePrefix = "images\\";
const char* ImageSuffix = ".jpg";
const char* LogoImage = "images\\LOL_Logo.jpg";

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
	updateSelection();
	displayClearedGame(false);

	openCount = 0;
	remainedCount = SolveCount;
	stat = first;
	firstX = firstY = secondX = secondY = -1;

	updateTryOpenCounter(openCount);
	Shuffle();
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
				openCount++;
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
				updateTryOpenCounter(openCount);

				if (IsSameImage(x,y)) {
					firstX = firstY = secondX = secondY = -1;
					--remainedCount;
					if (IsCleared()) {
						displayClearedGame(true);
						stat = cleared;
					} else {
						stat = first;
					}
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

bool GameLv1::IsCleared()
{
	return (remainedCount==0);	// 남은 그림이 없을 때 완료
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

void GameLv1::updateTryOpenCounter(int count)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		TEXT_INFO* pI = page->GetTextInfo(CounterID);
		if (pI!=NULL) {
			_itoa_s(openCount, buffer_counter, BUF_SIZE, 10);
			pI->text = buffer_counter;
		}
	}
}

void GameLv1::Shuffle()
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		// Generate
		RndGen gen(CandidateMax, SolveCount*2);
		gen.generate();

		int nums = page->GetImagesNumber();
		for(int id=0; id<nums; ++id)
		{
			IMAGE_INFO* pI = page->GetImageInfo(id);

			// 에러처리
			if (pI==NULL
				|| strncmp(pI->file, ImagePrefix, strlen(ImagePrefix))!=0
				|| strcmp(pI->file, LogoImage)==0) continue;

			sprintf_s(img_file[id], _MAX_PATH, "%s%d%s",
				ImagePrefix, gen.next(), ImageSuffix);
			pI->file = img_file[id];// 파일 이름 설정
			pI->bFlip = true;		// 그림을 뒤집는다
		}
	}
}

void GameLv1::displayClearedGame(bool bVisible)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		TEXT_INFO* pI = page->GetTextInfo(ClearedID);
		if (pI!=NULL) {
			pI->bDisplay = bVisible;
		}
		pI = page->GetTextInfo(PreesAnyKeyID);
		if (pI!=NULL) {
			pI->bDisplay = bVisible;
		}
	}
}