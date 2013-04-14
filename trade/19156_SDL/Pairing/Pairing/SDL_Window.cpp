#include "SDL_Window.h"

#include <SDL_image.h>	// for imange
#include <SDL_ttf.h>	// for text

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDLmain.lib")

//////////////////////////////////////////////////////////////////////////

// Loads a font, nothing fancy.
// Put sensible error handling code in. If you pass the NULL in later on,
// SDL_ttf dies pretty horribly.
TTF_Font* loadfont(const char* file, int ptsize)
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, ptsize);
	if (tmpfont == NULL) {
		printf("Unable to load font: %s %s \n", file, TTF_GetError());
		// Handle the error here.
		return NULL;
	}
	return tmpfont;
}

//////////////////////////////////////////////////////////////////////////

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
	// Initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("%s\n", SDL_GetError());
		bInitialized = false;
		return false;
	}

	// Initialize SDL TTF
	if(TTF_Init()==-1)
	{
		printf("%s\n", TTF_GetError());
		bInitialized = false;
		return false;
	}
	
	//Create Title
	SDL_WM_SetCaption(title, NULL);

	//Create Window
	screen = SDL_SetVideoMode(width, height, 0, SDL_SWSURFACE);

	bInitialized = true;
	return true;
}

bool SDL_Window::IsInitialized() const
{
	return bInitialized;
};

int SDL_Window::AddPage(SDL_Page* page)
{
	pages.push_back(page);
	return (pages.size()-1);
}

bool SDL_Window::SelectPage(int pageID)
{
	if ( pageID > int(pages.size()-1) )	// 잛못된 Page ID 지정시
	{
		return false;
	}
	else
	{
		curr_page = pageID;
		return true;
	}
}

void SDL_Window::Refresh()
{
	int pageId = curr_page;
	if (pageId==PAGE_NOT_SELECTED)	// 페이지가 선택이 되지 않았으면
	{
		if (pages.empty()) return;	// 페이지가 없다
		else pageId = DEFAULT_PAGE;	// 기본페이지(첫 페이지)
	}

	// 페이지 정보가 있는 포인터 획득
	SDL_Page* page = pages[pageId];

	// 배경색 그린다.
	drawBackground(page);

	// 이미지 그린다.
	drawImages(page);

	// 텍스트 그린다.
	drawTexts(page);

	SDL_Flip(screen);	// 스크린 영역 전체를 새로고침
}


//////////////////////////////////////////////////////////////////////////
// private methods

void SDL_Window::drawBackground(SDL_Page* page)
{
	SDL_Rect screenRect = {0, 0, screen->w, screen->h};
	Uint32 bgColor = page->GetBgColor(screen);
	SDL_FillRect(screen, &screenRect, bgColor);
}

void SDL_Window::drawImages(SDL_Page* page)
{
	int nums = page->GetImagesNumber();
	for (int i=0; i<nums; i++)
	{
		IMAGE_INFO img;
		if(!page->GetImageInfo(i, &img)) continue;

		if(!img.bDisplay) continue;
		else drawImage(img);
	}
}

void SDL_Window::drawImage(const IMAGE_INFO& image_info)
{
	//We will display bitmap
	SDL_Surface *image;
	SDL_Surface *imageTmp;

	// Image Loading
	imageTmp = IMG_Load(image_info.file);
	if(imageTmp==NULL)	// 이미지 로드 실패
	{
		printf("%s - %s", IMG_GetError(), image_info.file);
		return;
	}

	// Get Image Info
	int w = imageTmp->w;
	int h = imageTmp->h;

	// Convert a surface to the display format
	image = SDL_DisplayFormat(imageTmp);
	SDL_FreeSurface(imageTmp);

	// Positioning and Display
	SDL_Rect dest = {image_info.x, image_info.y, };
	SDL_BlitSurface(image, NULL, screen, &dest);
	SDL_FreeSurface(image);
}

void SDL_Window::drawTexts(SDL_Page* page)
{
	int nums = page->GetTextsNumber();
	for (int i=0; i<nums; i++)
	{
		TEXT_INFO* pTxt = page->GetTextInfo(i);
		if(pTxt==NULL || (pTxt->bDisplay==false)) continue;
		else drawText(pTxt);
	}
}

void SDL_Window::drawText(const TEXT_INFO* text_info)
{
	TTF_Font* font = loadfont(text_info->font, text_info->size);
	if(font==NULL) return;	// 폰트 로드 실패

	SDL_Color color = {text_info->fg_r, text_info->fg_g, text_info->fg_b,};
	SDL_Surface* text = TTF_RenderText_Solid(font, text_info->text, color);
	SDL_Rect dest = {text_info->x, text_info->y, };
	SDL_BlitSurface(text, NULL, screen, &dest);

	SDL_FreeSurface(text);
	TTF_CloseFont(font);
}
