#pragma once

#include <SDL.h>
#include "SDL_Page.h"

// Background Colre Pre-define
#define SDL_BG_COLOR_WHILTE	0x00FFFFFF

// implement for SDL functions wrapping
class SDL_Window
{
public:
	SDL_Window(const char* title, int width, int height);
	~SDL_Window(void);

	bool Initialize();		// 초기화
	bool IsInitialized() const { return bInitialized; };// 초기화 여부 확인 함수

	void AddPage(int page_id, SDL_Page& page);	// 페이지 추가

private:
	const char* title;
	int width, height;
	bool bInitialized;

	SDL_Surface *screen;
};
