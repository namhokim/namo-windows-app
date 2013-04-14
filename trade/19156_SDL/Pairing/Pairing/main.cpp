#include "SDL_Window.h"

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
game_state MenuIdToState(int menuID);	// 메뉴 선택시 화면 전환할 게임 상태 획득

void makeLevel1(SDL_Page& page);	// 게임 레벨 1 화면 초기화
void makeLevel2(SDL_Page& page);	// 게임 레벨 2 화면 초기화
void makeLevel3(SDL_Page& page);	// 게임 레벨 3 화면 초기화

int main(int argc, char *argv[])
{
	SDL_Window win("Pairing Game", 640, 640);
	SDL_Page pageInit, pageMenu, pageLv1, pageLv2, pageLv3;
	int pageIDs[quit];

	if(!win.Initialize()) {
		return 1;
	} else {
		// init pages
		makeInitPage(pageInit);
		makeMenuPage(pageMenu);
		makeLevel1(pageLv1);
		makeLevel2(pageLv2);
		makeLevel3(pageLv3);

		// page add
		pageIDs[init] = win.AddPage(&pageInit);
		pageIDs[menu] = win.AddPage(&pageMenu);
		pageIDs[level1] = win.AddPage(&pageLv1);
		pageIDs[level2] = win.AddPage(&pageLv2);
		pageIDs[level3] = win.AddPage(&pageLv3);

		// first refresh
		win.Refresh();
	}

	game_state state = init;
	int menu_sel = SEL_LV1;
	changeMenuSel(pageMenu, menu_sel);

	while(state!=quit){
		SDL_Event evt;
		while( SDL_PollEvent(&evt) ){
			switch( evt.type) {
				case SDL_KEYDOWN:
					printf("%d\n", evt.key.keysym.sym);	// TODO: remove

					switch (state) {
				case init:
					state = menu;					// 메뉴 상태로 전환
					win.SelectPage(pageIDs[state]);	// 메뉴 페이지로 전환
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
						case SDLK_RETURN:
							state = MenuIdToState(menu_sel);
							win.SelectPage(pageIDs[state]);
							break;
					}
					break;
				case level1:
				case level2:
				case level3:
					switch(evt.key.keysym.sym) {
						case SDLK_ESCAPE:
							state = menu;		// 메뉴 상태로 전환
							win.SelectPage(pageIDs[state]);	// 메뉴 페이지로 전환
							break;
					}
					break;
					}
					win.Refresh();

					break;
				case SDL_QUIT:
					state = quit;
					break;
			}	// while( SDL_PollEvent(&evt) )
		}
		SDL_Delay(1);	// for save the CPU usage (thread yield)
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

game_state MenuIdToState(int menuID)
{
	switch(menuID) {
		case SEL_LV1:
			return level1;
		case SEL_LV2:
			return level2;
		case SEL_LV3:
			return level3;
		default:
			return menu;
	}
}

void makeLevel1(SDL_Page& page)
{
	int tileBegin, tileEnd;

	page.SetBgColor(0xff, 0xff, 0xff);	// white
	page.AddText("Level 1", 100, 30, 21);
	page.AddFillRect(160, 160, 315, 315, 0x00, 0x00, 0xff);	// blue
	page.AddFillRect(167, 167, 103, 103, 0xff, 0x00, 0x00);	// red

	page.AddImage("images\\LOL_Logo.jpg", 270, 270);

	tileBegin = page.AddImage("images\\1.jpg", 170, 170);
	page.AddImage("images\\2.jpg", 270, 170);
	page.AddImage("images\\3.jpg", 370, 170);
	page.AddImage("images\\4.jpg", 170, 270);
	// logo
	page.AddImage("images\\6.jpg", 370, 270);
	page.AddImage("images\\7.jpg", 170, 370);
	page.AddImage("images\\8.jpg", 270, 370);
	tileEnd = page.AddImage("images\\9.jpg", 370, 370);

	for (int id=tileBegin; id<=tileEnd; ++id) {
		page.GetImageInfo(id)->bFlip = true;
	}
}
void makeLevel2(SDL_Page& page)
{
	page.SetBgColor(0xff, 0xff, 0xff);	// image
	page.AddText("Level 2", 100, 30, 21);
}
void makeLevel3(SDL_Page& page)
{
	page.SetBgColor(0xff, 0xff, 0xff);	// image
	page.AddText("Level 3", 100, 30, 21);
	page.AddImage("images\\LOL_Logo.jpg", 270, 270);
}