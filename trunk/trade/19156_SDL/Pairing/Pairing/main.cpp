#include "SDL_Window.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	SDL_Window w("Pairing Game", 640, 480);

	SDL_Page pageInit;
	pageInit.SetBgColor(255, 255, 255);	// white

	SDL_Page pageMenu;
	pageMenu.SetBgColor(0, 0, 0);		// black

	int idInit = w.AddPage(&pageInit);
	int idMenu = w.AddPage(&pageMenu);

	if(!w.Initialize()) {
		return 1;
	} else {
		w.Refresh();
	}

	bool bRunning = true;	
	while(bRunning){
		SDL_Event evt;
		while(SDL_PollEvent(&evt)){
			if(evt.type==SDL_KEYDOWN){
				printf("%d\n", evt.key.keysym.sym);	// TODO: remove
				switch(evt.key.keysym.sym)
				{
				case SDLK_ESCAPE:	// when ESC key pressed
					bRunning=false;
					break;
				case SDLK_1:
					w.SelectPage(idInit);
					break;
				case SDLK_2:
					w.SelectPage(idMenu);
					break;
				case SDLK_LEFT:
					printf("<-\n");
					break;
				case SDLK_RIGHT:
					printf("->\n");
					break;
				}
				w.Refresh();
			}
		}
	}

	////We will display bitmap
	//SDL_Surface *image;
	//SDL_Surface *image2;   //Using *SDL_image.h
	//image2 = IMG_Load("images\\Title.jpg"); //Using *SDL_image.h
	//if(image2==NULL){
	//	printf("Unable to load PNG: %s\n", IMG_GetError());
	//	return 1;
	//}
	//image = SDL_DisplayFormat(image2);
	//SDL_FreeSurface(image2);

	////Positioning and Display
	//SDL_Rect dest;
	//dest.x = 0;
	//dest.y = 0;

	//SDL_Rect imageRect = {0, 0, 402, 227};

	//SDL_FillRect(screen , &imageRect , 0);
	//SDL_BlitSurface(image,NULL, screen, &dest);
	//SDL_Flip(screen);

	//bool gameOn = true;
	//SDL_Event evt;
	//while(gameOn){
	//	while(SDL_PollEvent(&evt)){
	//		if(evt.type==SDL_KEYDOWN){
	//			switch(evt.key.keysym.sym)
	//			{
	//			case SDLK_a:
	//				printf("Pressing a");
	//				break;
	//			case SDLK_LEFT:
	//				dest.x -= 25;
	//				break;
	//			case SDLK_RIGHT:
	//				dest.x += 25;  
	//				break;
	//			case SDLK_q:
	//				gameOn=false;
	//				break;
	//			case SDLK_d:
	//				printf("pressed d");
	//				break;
	//			}
	//			SDL_FillRect(screen , NULL , 0);
	//			SDL_BlitSurface(image,NULL, screen, &dest);
	//			SDL_Flip(screen);
	//		}
	//	}
	//}

	return 0;
}
