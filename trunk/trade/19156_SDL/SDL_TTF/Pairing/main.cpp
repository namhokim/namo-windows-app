#include <SDL_ttf.h>

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDLmain.lib")

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_DEPTH 8

enum textquality {solid, shaded, blended};

// Loads a font, nothing fancy.
// Put sensible error handling code in. If you pass the NULL in later on,
// SDL_ttf dies pretty horribly.
TTF_Font* loadfont(char* file, int ptsize)
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, ptsize);
	if (tmpfont == NULL) {
		printf("Unable to load font: %s %s \n", file, TTF_GetError());
		// Handle the error here.
	}
	return tmpfont;
}

// SDL_ttf has a few modes you can draw with, check its documentation.
SDL_Surface *drawtext(TTF_Font* fonttodraw, Uint8 fgR, Uint8 fgG, Uint8 fgB, Uint8 fgA, 
					  Uint8 bgR, Uint8 bgG, Uint8 bgB, Uint8 bgA, char text[], textquality quality)
{
	SDL_Color tmpfontcolor = {fgR,fgG,fgB,fgA};
	SDL_Color tmpfontbgcolor = {bgR, bgG, bgB, bgA};
	SDL_Surface* resulting_text;

	if (quality == solid) {
		resulting_text = TTF_RenderText_Solid(fonttodraw, text, tmpfontcolor);
	}
	else if (quality == shaded) {
		resulting_text = TTF_RenderText_Shaded(fonttodraw, text, tmpfontcolor, tmpfontbgcolor);
	}
	else if (quality == blended) {
		resulting_text = TTF_RenderText_Blended(fonttodraw, text, tmpfontcolor);
	}

	return resulting_text;
}


int main(int argc, char** argv) {
	// Must call initialise first.
	TTF_Init();

	// This loads courier, but any font will do.
	TTF_Font* font = loadfont("C:/windows/fonts/cour.ttf", 10);
	SDL_Surface* surface = drawtext(font, 255, 255, 255, 70, 0, 0, 0, 0, "test", blended);

	// End of SDL_ttf stuff, this is just SDL to display the text we made.
	SDL_Surface* screen;
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_SWSURFACE);
	SDL_BlitSurface(surface, NULL, screen, NULL);
	SDL_Flip(screen);

	bool bRunning = true;	
	while(bRunning){
		SDL_Event evt;
		while(SDL_PollEvent(&evt)){
			if(evt.type==SDL_KEYDOWN){
				switch(evt.key.keysym.sym)
				{
				case SDLK_ESCAPE:	// when ESC key pressed
					bRunning=false;
					break;
				}
			}
		}
	}

	return 0;
}