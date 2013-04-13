#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_

#include <SDL.h>
#include <SDL_image.h>	// for imange
#include <SDL_ttf.h>	// for text

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDLmain.lib")

// Function declare
bool initialize(const char* title, int width, int height);
bool load_files();

// Background Colre Pre-define
#define SDL_BG_COLOR_WHILTE	0x00FFFFFF

#endif	/*_MAIN_H_*/