#include "SDL_Window.h"

#include <SDL_image.h>	// for imange
#include <SDL_ttf.h>	// for text

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDLmain.lib")

SDL_Window::SDL_Window(const char* title, int width, int height)
:bInitialized(false)
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

void SDL_Window::AddPage(int page_id, SDL_Page& page)
{

}