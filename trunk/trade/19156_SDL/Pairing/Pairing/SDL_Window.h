#pragma once

#include <SDL.h>

// Background Colre Pre-define
#define SDL_BG_COLOR_WHILTE	0x00FFFFFF

// implement for SDL functions wrapping
class SDL_Window
{
public:
	SDL_Window(const char* title, int width, int height);
	~SDL_Window(void);

	bool Initialize();
	bool IsInitialized();

private:
	const char* title;
	int width, height;
	bool bInitialized;

	SDL_Surface *screen;
};
