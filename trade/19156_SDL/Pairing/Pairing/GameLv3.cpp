#include "GameLv3.h"
#include "RndGen.h"

namespace lv3 {
	const int Distance = 100;
	const int Selection_ID = 1;
	const int Size = 5;
	const int IndexMin = 0;
	const int IndexMax = (Size-1);
	const int SolveCount = ( (Size*Size)/2 );
	const int ElapTimeID = 3;
	const int CounterID = 4;
	const int ClearedID = 5;
	const int PreesAnyKeyID = 6;
	const int CandidateMax = 25;
}

GameLv3::GameLv3(SDL_Window* win, int pageID, int pageMenu)
{
	this->win = win;
	this->pageID = pageID;
	this->pageMenu = pageMenu;
	tID = NULL;
}

GameLv3::~GameLv3(void)
{
}

void GameLv3::StartGame()
{
	// 초기화
	initGame();

	// 그림 섞기
	Shuffle();

	// 타이머 시작
	startTimer();
}

void GameLv3::Endgame()
{
	// 타이머 정지
	stopTimer();
}

void GameLv3::GoMenuPage()
{
	win->SelectPage(pageMenu);
}

void GameLv3::CursorMove(SDLKey key)
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


void GameLv3::SpaceDown()
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

bool GameLv3::IsCleared()
{
	return (remainedCount==0);	// 남은 그림이 없을 때 완료
}

void GameLv3::UpdateElapTime()
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		TEXT_INFO* pI = page->GetTextInfo(lv3::ElapTimeID);
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

SDL_Page* GameLv3::GetPage()
{
	if(win==NULL) return NULL;
	else return win->GetPage(pageID);
}

RECT_INFO* GameLv3::GetSelRect()
{
	SDL_Page* page = GetPage();
	if(page==NULL) return NULL;
	else return page->GetRectInfo(lv3::Selection_ID);
}

void GameLv3::updateSelection()
{
	RECT_INFO* sel = GetSelRect();
	if(sel==NULL) return;

	sel->x = sel->x_ori + (x*lv3::Distance);
	sel->y = sel->y_ori + (y*lv3::Distance);

	printf("(%d,%d)\n", sel->x, sel->y);
}

bool GameLv3::IsFlipped(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		IMAGE_INFO* pI = page->GetImageInfo(x + (y * lv3::Size));
		if (pI!=NULL)
		{
			return (pI->bFlip);
		}
	}
	return false;
}

void GameLv3::Flip(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page==NULL) return;

	int offset = x + (y * lv3::Size);
	IMAGE_INFO* pI = page->GetImageInfo(offset);
	if (pI!=NULL) {
		pI->bFlip = !(pI->bFlip);
	}
}

void GameLv3::ResetFlips()
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

bool GameLv3::IsSameImage(int x, int y)
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

const char* GameLv3::GetImageName(int x, int y)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		IMAGE_INFO* pI = page->GetImageInfo(x + (y * lv3::Size));
		if (pI!=NULL) {
			return pI->file;
		}
	}
	return NULL;
}

void GameLv3::updateTryOpenCounter(int count)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		TEXT_INFO* pI = page->GetTextInfo(lv3::CounterID);
		if (pI!=NULL) {
			_itoa_s(openCount, buffer_counter, BUF_SIZE, 10);
			pI->text = buffer_counter;
		}
	}
}

void GameLv3::Shuffle()
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		// Generate
		RndGen gen(lv3::CandidateMax, lv3::SolveCount*2);
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

void GameLv3::displayClearedGame(bool bVisible)
{
	SDL_Page* page = GetPage();
	if(page!=NULL)
	{
		TEXT_INFO* pI = page->GetTextInfo(lv3::ClearedID);
		if (pI!=NULL) {
			pI->bDisplay = bVisible;
		}
		pI = page->GetTextInfo(lv3::PreesAnyKeyID);
		if (pI!=NULL) {
			pI->bDisplay = bVisible;
		}
	}

	// 타이머 중지
	stopTimer();
}

void GameLv3::initGame()
{
	this->x = 0;
	this->y = 0;
	updateSelection();
	displayClearedGame(false);
	openCount = 0;
	remainedCount = lv3::SolveCount;
	stat = first;
	firstX = firstY = secondX = secondY = -1;
	updateTryOpenCounter(openCount);
}

void GameLv3::startTimer()
{
	startTime = SDL_GetTicks();
	stopTimer();
	tID = SDL_AddTimer(1000, GameLv3::callbackTimeUpdate, this);

}

void GameLv3::stopTimer()
{
	if(tID!=NULL) {
		SDL_RemoveTimer(tID);
		tID = NULL;
	}
}

void GameLv3::CursorUP()
{
	y--;
	if (y<lv3::IndexMin) y = lv3::IndexMax;
}
void GameLv3::CursorDown()
{
	y++;
	if (y>lv3::IndexMax) y = lv3::IndexMin;
}
void GameLv3::CursorLeft()
{
	x--;
	if (x<lv3::IndexMin) x = lv3::IndexMax;
}
void GameLv3::CursorRight()
{
	x++;
	if (x>lv3::IndexMax) x = lv3::IndexMin;
}

Uint32 GameLv3::callbackTimeUpdate(Uint32 interval, void *param)
{
	printf("callback\n");

	GameLv3* game = (GameLv3*)param;
	game->UpdateElapTime();

	return(interval);	// re-do
}
