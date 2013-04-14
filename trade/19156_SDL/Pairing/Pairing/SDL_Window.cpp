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
	if ( pageID > int(pages.size()-1) )	// ����� Page ID ������
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
	if (pageId==PAGE_NOT_SELECTED)	// �������� ������ ���� �ʾ�����
	{
		if (pages.empty()) return;	// �������� ����
		else pageId = DEFAULT_PAGE;	// �⺻������(ù ������)
	}

	// ������ ������ �ִ� ������ ȹ��
	SDL_Page* page = pages[pageId];

	// ���� �׸���.
	drawBackground(page);

	// �簢�� �׸���.
	drawRects(page);

	// �̹��� �׸���.
	drawImages(page);

	// �ؽ�Ʈ �׸���.
	drawTexts(page);

	SDL_Flip(screen);	// ��ũ�� ���� ��ü�� ���ΰ�ħ
}


//////////////////////////////////////////////////////////////////////////
// private methods

void SDL_Window::drawBackground(SDL_Page* page)
{
	SDL_Rect screenRect = {0, 0, screen->w, screen->h};
	Uint32 bgColor = page->GetBgColor(screen);
	SDL_FillRect(screen, &screenRect, bgColor);
}

void SDL_Window::drawRects(SDL_Page* page)
{
	int nums = page->GetRectsNumber();
	for (int id=0; id<nums; id++)
	{
		RECT_INFO* info = page->GetRectInfo(id);
		if (info==NULL || (info->bDisplay==false)) continue;
		else {
			drawRect(info);
		}
	}

}
void SDL_Window::drawRect(const RECT_INFO* obj)
{
	SDL_Rect dest = {obj->x, obj->y, obj->width, obj->height};
	Uint32 color = SDL_MapRGB(screen->format, obj->r, obj->g, obj->b);
	SDL_FillRect(screen, &dest, color);
}

void SDL_Window::drawImages(SDL_Page* page)
{
	int nums = page->GetImagesNumber();
	for (int i=0; i<nums; i++)
	{
		IMAGE_INFO* img = page->GetImageInfo(i);
		if(img==NULL || (img->bDisplay==false)) continue;
		else drawImage(img);
	}
}

void SDL_Window::drawImage(const IMAGE_INFO* obj)
{
	//We will display bitmap
	SDL_Surface *image;
	SDL_Surface *imageTmp;

	// Image Loading
	imageTmp = IMG_Load(obj->file);
	if(imageTmp==NULL)	// �̹��� �ε� ����
	{
		printf("%s - %s", IMG_GetError(), obj->file);
		return;
	}

	// Get Image Info
	int w = imageTmp->w;
	int h = imageTmp->h;
	
	if (obj->bFlip) {	// FLIP check
		SDL_Rect dest = {obj->x, obj->y, w, h};
		Uint32 colorWhite = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
		SDL_FillRect(screen, &dest, colorWhite);
	} else {			// normal image
		// Convert a surface to the display format
		image = SDL_DisplayFormat(imageTmp);

		// Positioning and Display
		SDL_Rect dest = {obj->x, obj->y, };
		SDL_BlitSurface(image, NULL, screen, &dest);
		SDL_FreeSurface(image);
	}
	
	SDL_FreeSurface(imageTmp);
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

void SDL_Window::drawText(const TEXT_INFO* obj)
{
	TTF_Font* font = loadfont(obj->font, obj->size);
	if(font==NULL) return;	// ��Ʈ �ε� ����

	SDL_Color color = {obj->fg_r, obj->fg_g, obj->fg_b,};
	SDL_Surface* text = TTF_RenderText_Solid(font, obj->text, color);
	SDL_Rect dest = {obj->x, obj->y, };
	SDL_BlitSurface(text, NULL, screen, &dest);

	SDL_FreeSurface(text);
	TTF_CloseFont(font);
}
