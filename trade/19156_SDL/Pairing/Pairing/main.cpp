#include "SDL_Window.h"

#include <windows.h>
#include <iostream>
using namespace std;

// 게임 화면 단계(상태)
typedef enum {
	init, menu, level1, level2, level3, quit,
} game_state;

void makeInitPage(SDL_Page& page);	// 최초 화면 초기화
void makeMenuPage(SDL_Page& page);	// 최초 화면 초기화

int main(int argc, char *argv[])
{
	SDL_Window win("Pairing Game", 640, 640);
	SDL_Page pageInit, pageMenu;
	int pageIDs[quit];

	if(!win.Initialize()) {
		return 1;
	} else {
		// init pages
		makeInitPage(pageInit);
		makeMenuPage(pageMenu);

		// page add
		pageIDs[init] = win.AddPage(&pageInit);
		pageIDs[menu] = win.AddPage(&pageMenu);

		// first refresh
		win.Refresh();
	}

	game_state state = init;

	while(state!=quit){
		SDL_Event evt;
		while( SDL_PollEvent(&evt) && (evt.type==SDL_KEYDOWN) ){

			printf("%d\n", evt.key.keysym.sym);	// TODO: remove

			switch (state) {
				case init:
					state = menu;					// 메뉴 상태로 전환
					win.SelectPage(pageIDs[menu]);	// 메뉴 페이지로 전환
					break;
				case menu:
					switch(evt.key.keysym.sym) {
						case SDLK_ESCAPE:
							state = quit;
							break;
					}
					break;
				case level1:
				case level2:
				case level3:
					break;
			}
			win.Refresh();
		}
		Sleep(1);	// for save the CPU usage (thread yield)
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

void makeMenuPage(SDL_Page& page)
{

}