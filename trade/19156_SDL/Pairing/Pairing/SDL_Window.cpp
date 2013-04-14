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
	if (pageID+1 > (int)pages.size()) {	// ����� Page ID ����
		return false;
	} else {
		curr_page = pageID;
		return true;
	}
}

void SDL_Window::Refresh()
{
	int pageId = curr_page;
	if (pageId==PAGE_NOT_SELECTED) {// �������� ������ ���� �ʾ�����
		if (pages.empty()) return;	// �������� ����
		else pageId = DEFAULT_PAGE;	// �⺻������(ù ������)
	}

	// ������ ������ �ִ� ������ ȹ��
	SDL_Page* page = pages[pageId];

	// ���� �׸���.
	SDL_Rect screenRect = {0, 0, screen->w, screen->h};
	Uint32 bgColor = page->GetBgColor(screen);
 	SDL_FillRect(screen, &screenRect, bgColor);

	// �̹��� �׸���.
	//SDL_BlitSurface(image,NULL, screen, &dest);

	// �ؽ�Ʈ �׸���.

	SDL_Flip(screen);	// ��ũ�� ���� ��ü�� ���ΰ�ħ
}