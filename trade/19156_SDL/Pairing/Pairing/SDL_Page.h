#pragma once

#include <SDL.h>

class SDL_Page
{
public:
	SDL_Page(void);
	~SDL_Page(void);

	void SetBgColor(const Uint8 r, const Uint8 g, const Uint8 b);	// 배경색 지정
	Uint32 GetBgColor(SDL_Surface *screen);	// 배경색 가져오기(SDL_Window에서 호출)

private:
	//Uint32 bgColor;
	// 배경색
	Uint8 bg_r, bg_g, bg_b;
};
