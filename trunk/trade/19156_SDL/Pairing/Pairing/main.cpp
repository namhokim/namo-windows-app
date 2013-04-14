#include "SDL_Window.h"

#include <windows.h>
#include <iostream>
using namespace std;

void makeInitPage(SDL_Page& page);

int main(int argc, char *argv[])
{
	SDL_Window win("Pairing Game", 640, 640);
	SDL_Page pageInit, pageMenu;

	makeInitPage(pageInit);
	pageMenu.SetBgColor(0, 0, 0);		// black

	int idInit = win.AddPage(&pageInit);
	int idMenu = win.AddPage(&pageMenu);

	if(!win.Initialize()) {
		return 1;
	} else {
		win.Refresh();
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
					win.SelectPage(idInit);
					break;
				case SDLK_2:
					win.SelectPage(idMenu);
					break;
				case SDLK_LEFT:
					printf("<-\n");
					break;
				case SDLK_RIGHT:
					printf("->\n");
					break;
				}
				win.Refresh();
			}
		}
		Sleep(1);	// for save the CPU usage (yield)
	}

	return 0;
}


void makeInitPage(SDL_Page& page)
{
	page.SetBgColor(255, 255, 255);	// white
	page.AddImage("images\\Title.jpg", 125, 0);
	page.AddImage("images\\Jace.jpg", 75, 227);
	page.AddText("Press Any Key To Start", 200, 520, 21,  0, 112, 192);
	page.AddText("Made by SJS", 500, 600, 21, 112, 48, 160);
}