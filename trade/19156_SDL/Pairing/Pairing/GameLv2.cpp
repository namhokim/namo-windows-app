#include "GameLv2.h"
#include "RndGen.h"

namespace lv2 {
	const int Distance = 100;
	const int Selection_ID = 1;
	const int Size = 4;
	const int IndexMin = 0;
	const int IndexMax = (Size-1);
	const int SolveCount = ( (Size*Size)/2 );
	const int ElapTimeID = 3;
	const int CounterID = 4;
	const int ClearedID = 5;
	const int PreesAnyKeyID = 6;
	const int CandidateMax = 25;
}

GameLv2::GameLv2(SDL_Window* win, int pageID, int pageMenu)
{
	this->win = win;
	this->pageID = pageID;
	this->pageMenu = pageMenu;
	tID = NULL;
}

GameLv2::~GameLv2(void)
{
}

void GameLv2::StartGame()
{
	// 초기화
	initGame();

	// 그림 섞기
	Shuffle();

	// 타이머 시작
	startTimer();
}

void GameLv2::Endgame()
{
	// 타이머 정지
	stopTimer();
}

void GameLv2::GoMenuPage()
{
	win->SelectPage(pageMenu);
}

void GameLv2::CursorMove(SDLKey key)
{
	ResetFlips();
	switch(key) {
		case SDLK_UP:
			CursorUP();
			break;
		case SDLK_DOWN:
			CursorDown();
			break;
		case SDLK_LEFT:
			CursorLeft();
			break;
		case SDLK_RIGHT:
			CursorRight();
			break;
	}
	updateSelection();
}


void GameLv2::SpaceDown()
{
	if (remainedCount<=0) return;

	switch(stat) {
		case first:
			if (!IsFlipped(x, y)) return;
			else {
				Flip(x, y);
				openCount++;
				updateTryOpenCounter(openCount);
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

bool GameLv2::IsCleared()
{
	return (remainedCount==0);	// 남은 그림이 없을 때 완료
}

void GameLv2::UpdateElapTime()
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		TEXT_INFO* pI = page->GetTextInfo(lv2::ElapTimeID);
		if (pI!=NULL) {
			int t = (SDL_GetTicks() - startTime)/1000;
			sprintf_s(buffer_time, 20, "%d", t);
			pI->text = buffer_time;
			win->Refresh();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// private

SDL_Page* GameLv2::GetPage()
{
	if(win==NULL) return NULL;
	else return win->GetPage(pageID);
}

RECT_INFO* GameLv2::GetSelRect()
{
	SDL_Page* page = GetPage();
	if(page==NULL) return NULL;
	else return page->GetRectInfo(lv2::Selection_ID);
}

void GameLv2::updateSelection()
{
	RECT_INFO* sel = GetSelRect();
	if(sel==NULL) return;

	sel->x = sel->x_ori + (x*lv2::Distance);
	sel->y = sel->y_ori + (y*lv2::Distance);

	printf("(%d,%d)\n", sel->x, sel->y);
}

bool GameLv2::IsFlipped(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		IMAGE_INFO* pI = page->GetImageInfo(x + (y * lv2::Size));
		if (pI!=NULL)
		{
			return (pI->bFlip);
		}
	}
	return false;
}

void GameLv2::Flip(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page==NULL) return;

	IMAGE_INFO* pI = page->GetImageInfo(x + (y * lv2::Size));
	if (pI!=NULL) {
		pI->bFlip = !(pI->bFlip);
	}
}

void GameLv2::ResetFlips()
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

bool GameLv2::IsSameImage(int x, int y)
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

const char* GameLv2::GetImageName(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		IMAGE_INFO* pI = page->GetImageInfo(x + (y * lv2::Size));
		if (pI!=NULL) {
			return pI->file;
		}
	}
	return NULL;
}

void GameLv2::updateTryOpenCounter(int count)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		TEXT_INFO* pI = page->GetTextInfo(lv2::CounterID);
		if (pI!=NULL) {
			_itoa_s(openCount, buffer_counter, BUF_SIZE, 10);
			pI->text = buffer_counter;
		}
	}
}

void GameLv2::Shuffle()
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		// Generate
		RndGen gen(lv2::CandidateMax, lv2::SolveCount*2);
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

void GameLv2::displayClearedGame(bool bVisible)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		TEXT_INFO* pI = page->GetTextInfo(lv2::ClearedID);
		if (pI!=NULL) {
			pI->bDisplay = bVisible;
		}
		pI = page->GetTextInfo(lv2::PreesAnyKeyID);
		if (pI!=NULL) {
			pI->bDisplay = bVisible;
		}
	}

	// 타이머 중지
	stopTimer();
}

void GameLv2::initGame()
{
	this->x = 0;
	this->y = 0;
	updateSelection();
	displayClearedGame(false);
	openCount = 0;
	remainedCount = lv2::SolveCount;
	stat = first;
	firstX = firstY = secondX = secondY = -1;
	updateTryOpenCounter(openCount);
}

void GameLv2::startTimer()
{
	startTime = SDL_GetTicks();
	stopTimer();
	tID = SDL_AddTimer(1000, GameLv2::callbackTimeUpdate, this);

}

void GameLv2::stopTimer()
{
	if(tID!=NULL) {
		SDL_RemoveTimer(tID);
		tID = NULL;
	}
}

void GameLv2::CursorUP()
{
	y--;
	if (y<lv2::IndexMin) y = lv2::IndexMax;
}
void GameLv2::CursorDown()
{
	y++;
	if (y>lv2::IndexMax) y = lv2::IndexMin;
}
void GameLv2::CursorLeft()
{
	x--;
	if (x<lv2::IndexMin) x = lv2::IndexMax;
}
void GameLv2::CursorRight()
{
	x++;
	if (x>lv2::IndexMax) x = lv2::IndexMin;
}

Uint32 GameLv2::callbackTimeUpdate(Uint32 interval, void *param)
{
	printf("callback\n");

	GameLv2* game = (GameLv2*)param;
	game->UpdateElapTime();

	return(interval);	// re-do
}
