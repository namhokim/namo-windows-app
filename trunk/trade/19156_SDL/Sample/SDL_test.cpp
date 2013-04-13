#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char* argv[]){
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	//Create Title
	SDL_WM_SetCaption("Game One", "Game One");
	//Create Window
	SDL_Surface *screen = SDL_SetVideoMode(640,480,0,0);

	//We will display bitmap
	SDL_Surface *image;
	SDL_Surface *temp;
	SDL_Surface *image2;   //Using *SDL_image.h

	temp = SDL_LoadBMP("flower.bmp");
	image2 = IMG_Load("flower.png"); //Using *SDL_image.h

	if(image2==NULL){
		printf("Unable to load PNG: %s\n", IMG_GetError());
		return 1;
	}

	if (temp == NULL) {
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		return 1;
	}
	image = SDL_DisplayFormat(image2);
	SDL_FreeSurface(temp);
	SDL_FreeSurface(image2);

	//Positioning and Display
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;

	SDL_FillRect(screen , NULL , 0);
	SDL_BlitSurface(image,NULL, screen, &dest);
	SDL_Flip(screen);
	//SDL_FreeSurface(image);
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