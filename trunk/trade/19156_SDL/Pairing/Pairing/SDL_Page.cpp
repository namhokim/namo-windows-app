#include "SDL_Page.h"

SDL_Page::SDL_Page(void)
{
	// 검정 배경색
	bg_r = 0x00;
	bg_g = 0x00;
	bg_b = 0x00;
}

SDL_Page::~SDL_Page(void)
{
}

//////////////////////////////////////////////////////////////////////////
void SDL_Page::SetBgColor(const Uint8 r, const Uint8 g, const Uint8 b)
{
	bg_r = r;
	bg_g = g;
	bg_b = b;
}

Uint32 SDL_Page::GetBgColor(SDL_Surface *screen)
{
	Uint32 clearColor;
	clearColor = SDL_MapRGB(screen->format, bg_r, bg_g, bg_b);
	return clearColor;
}

//////////////////////////////////////////////////////////////////////////
int SDL_Page::AddFillRect(int x, int y, int width, int height,
				Uint8 r, Uint8 g, Uint8 b)
{
	RECT_INFO ri;
	ri.x = x;
	ri.y = y;
	ri.width = width;
	ri.height = height;
	ri.r = r;
	ri.g = g;
	ri.b = b;
	ri.bDisplay = true;

	rects.push_back(ri);
	return (rects.size()-1);
}

int SDL_Page::GetRectsNumber()
{
	return rects.size();
}
RECT_INFO* SDL_Page::GetRectInfo(int id)
{
	if ( id > int(rects.size()-1) )
	{
		return NULL;
	}
	else
	{
		return &(rects[id]);
	}
}


//////////////////////////////////////////////////////////////////////////
int SDL_Page::AddImage(const char* file, int x, int y)
{
	IMAGE_INFO image;
	image.file = file;
	image.x = x;
	image.y = y;
	image.bDisplay = true;	// 기본적으로 화면에 표시
	image.bFlip = false;	// 기본적으로 이미지를 표시(true시 흰색 뒤집기)

	imgs.push_back(image);
	return (imgs.size()-1);
}

int SDL_Page::GetImagesNumber()
{
	return imgs.size();
}

IMAGE_INFO* SDL_Page::GetImageInfo(int imageID)
{
	if ( imageID > int(imgs.size()-1) )
	{
		return NULL;
	}
	else
	{
		return &(imgs[imageID]);
	}
}

//////////////////////////////////////////////////////////////////////////
int SDL_Page::AddText(const char* text, int x, int y, int size,
					  Uint8 fg_r, Uint8 fg_g, Uint8 fg_b,
					  const char* font)
{
	TEXT_INFO t;
	t.text = text;
	t.x = x;
	t.y = y;
	t.size = size;
	t.font = font;
	t.bDisplay = true;	// 기본적으로 화면에 표시
	t.fg_r = fg_r;
	t.fg_g = fg_g;
	t.fg_b = fg_b;

	txts.push_back(t);
	return (txts.size()-1);
}

int SDL_Page::GetTextsNumber()
{
	return txts.size();
}

TEXT_INFO* SDL_Page::GetTextInfo(int textID)
{
	if ( textID > int(txts.size()-1) )
	{
		return NULL;
	}
	else
	{
		return &(txts[textID]);
	}
}
