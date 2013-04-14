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