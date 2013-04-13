#include "main.h"

// Global variations
SDL_Surface *screen;


int main(int argc, char *argv[])
{
	if(!initialize("Paring Game", 640, 480)) {
		return 1;
	}

	//We will display bitmap
	SDL_Surface *image;
	SDL_Surface *image2;   //Using *SDL_image.h
	image2 = IMG_Load("images\\Title.jpg"); //Using *SDL_image.h
	if(image2==NULL){
		printf("Unable to load PNG: %s\n", IMG_GetError());
		return 1;
	}
	image = SDL_DisplayFormat(image2);
	SDL_FreeSurface(image2);

	//Positioning and Display
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;

	SDL_Rect imageRect = {0, 0, 402, 227};

	SDL_FillRect(screen , &imageRect , 0);
	SDL_BlitSurface(image,NULL, screen, &dest);
	SDL_Flip(screen);

	bool gameOn = true;
	SDL_Event evt;
	while(gameOn){
		while(SDL_PollEvent(&evt)){
			if(evt.type==SDL_KEYDOWN){
				switch(evt.key.keysym.sym)
				{
				case SDLK_a:
					printf("Pressing a");
					break;
				case SDLK_LEFT:
					dest.x -= 25;
					break;
				case SDLK_RIGHT:
					dest.x += 25;  
					break;
				case SDLK_q:
					gameOn=false;
					break;
				case SDLK_d:
					printf("pressed d");
					break;
				}
				SDL_FillRect(screen , NULL , 0);
				SDL_BlitSurface(image,NULL, screen, &dest);
				SDL_Flip(screen);
			}
		}
	}

	SDL_Quit();

	return 0;
}

bool initialize(const char* title, int width, int height)
{
	//Initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return false;
	}

	//Create Title
	SDL_WM_SetCaption(title, NULL);

	//Create Window
	screen = SDL_SetVideoMode(width, height, 0, SDL_SWSURFACE);

	// load images
	if(!load_files())
	{
		return false;
	}

	// Set Background Color
	SDL_Rect screenRect = {0, 0, screen->w, screen->h};

	Uint32 clearColor;
	clearColor = SDL_MapRGB(screen->format, 255, 255, 255);
	SDL_FillRect(screen, &screenRect, clearColor);

	return true;
}

bool load_files()
{
	return true;
}