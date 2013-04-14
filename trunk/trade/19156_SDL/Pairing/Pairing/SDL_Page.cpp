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

int SDL_Page::AddImage(const char* file, int x, int y)
{
	IMAGE_INFO image;
	image.file = file;
	image.x = x;
	image.y = y;
	image.bDisplay = true;	// 기본적으로 화면에 표시

	imgs.push_back(image);
	return (imgs.size()-1);
}

int SDL_Page::GetImagesNumber()
{
	return imgs.size();
}

bool SDL_Page::GetImageInfo(int imageID, IMAGE_INFO* imageInfo)
{
	if ( imageID > int(imgs.size()-1) )
	{
		return false;
	}
	else
	{
		(*imageInfo) = imgs[imageID];
		return true;
	}
}