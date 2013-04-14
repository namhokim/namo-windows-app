#include "SDL_Window.h"

#include <windows.h>
#include <iostream>
using namespace std;

// 게임 화면 단계(상태)
typedef enum {
	init, menu, level1, level2, level3, quit,
} game_state;

// 게임메뉴 - 선택 레벨
#define SEL_LV1		1
#define SEL_LV2		2
#define SEL_LV3		3

void makeInitPage(SDL_Page& page);	// 최초 화면 초기화
void makeMenuPage(SDL_Page& page);	// 최초 화면 초기화
void changeMenuSel(SDL_Page& page, int menu_sel);	// 메뉴 선택 화면 변경

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
	INT menu_sel = SEL_LV1;
	changeMenuSel(pageMenu, menu_sel);

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
						case SDLK_UP:
							--menu_sel;
							if (menu_sel<SEL_LV1) menu_sel = SEL_LV3;
							changeMenuSel(pageMenu, menu_sel);
							break;
						case SDLK_DOWN:
							++menu_sel;
							if (menu_sel>SEL_LV3) menu_sel = SEL_LV1;
							changeMenuSel(pageMenu, menu_sel);
							break;
					}
					break;
				case level1:
				case level2:
				case level3:
					switch(evt.key.keysym.sym) {
						case SDLK_ESCAPE:
							state = menu;		// 메뉴 상태로 전환
							win.SelectPage(pageIDs[menu]);	// 메뉴 페이지로 전환
							break;
					}
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
	page.SetBgColor(255, 255, 255);	// white
	page.AddText("Select Level", 230, 100, 30);

	page.AddText(">", 180, 200, 25, 0xff);	// id : 1
	page.AddText(">", 180, 270, 25, 0xff);	// id : 2
	page.AddText(">", 180, 340, 25, 0xff);	// id : 3

	page.AddText("Level 1   :   3 x 3", 210, 200, 25);
	page.AddText("Level 2   :   4 x 4", 210, 270, 25);
	page.AddText("Level 3   :   5 x 5", 210, 340, 25);
	page.AddText("ESC : go back or exit game", 330, 570, 23);
}

void changeMenuSel(SDL_Page& page, int menu_sel)
{
	switch(menu_sel) {
		case SEL_LV1:
			page.GetTextInfo(1)->bDisplay = true;
			page.GetTextInfo(2)->bDisplay = false;
			page.GetTextInfo(3)->bDisplay = false;
			break;
		case SEL_LV2:
			page.GetTextInfo(1)->bDisplay = false;
			page.GetTextInfo(2)->bDisplay = true;
			page.GetTextInfo(3)->bDisplay = false;
			break;
		case SEL_LV3:
			page.GetTextInfo(1)->bDisplay = false;
			page.GetTextInfo(2)->bDisplay = false;
			page.GetTextInfo(3)->bDisplay = true;
			break;
	}
}