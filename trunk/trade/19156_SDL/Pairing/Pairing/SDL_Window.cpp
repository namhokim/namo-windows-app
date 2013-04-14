#include "SDL_Window.h"

#include <SDL_image.h>	// for imange
#include <SDL_ttf.h>	// for text

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDLmain.lib")

SDL_Window::SDL_Window(const char* title, int width, int height)
:bInitialized(false), curr_page(PAGE_NOT_SELECTED)
{
	this->title = title;
	this->width = width;
	this->height = height;
}

SDL_Window::~SDL_Window(void)
{
	if(bInitialized)
	{
		SDL_Quit();
	}
}

bool SDL_Window::Initialize()
{
	//Initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		bInitialized = false;
	}
	else {
		//Create Title
		SDL_WM_SetCaption(title, NULL);

		//Create Window
		screen = SDL_SetVideoMode(width, height, 0, SDL_SWSURFACE);

		bInitialized = true;
	}

	return bInitialized;
}

int SDL_Window::AddPage(SDL_Page* page)
{
	pages.push_back(page);
	return pages.size()-1;
}

bool SDL_Window::SelectPage(int pageID)
{
	if (pageID+1 > (int)pages.size()) {	// 잛못된 Page ID 지정
		return false;
	} else {
		curr_page = pageID;
		return true;
	}
}

void SDL_Window::Refresh()
{
	int pageId = curr_page;
	if (pageId==PAGE_NOT_SELECTED) {// 페이지가 선택이 되지 않았으면
		if (pages.empty()) return;	// 페이지가 없다
		else pageId = DEFAULT_PAGE;	// 기본페이지(첫 페이지)
	}

	// 페이지 정보가 있는 포인터 획득
	SDL_Page* page = pages[pageId];

	// 배경색 그린다.
	SDL_Rect screenRect = {0, 0, screen->w, screen->h};
	Uint32 bgColor = page->GetBgColor(screen);
 	SDL_FillRect(screen, &screenRect, bgColor);

	// 이미지 그린다.
	//SDL_BlitSurface(image,NULL, screen, &dest);

	// 텍스트 그린다.

	SDL_Flip(screen);	// 스크린 영역 전체를 새로고침
}