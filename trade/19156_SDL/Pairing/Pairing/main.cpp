#include "SDL_Window.h"
#include "GameLv1.h"

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
void makeMenuPage(SDL_Page& page);	// 메뉴 화면 초기화
void makeLevel1(SDL_Page& page);	// 게임 레벨 1 화면 초기화
void makeLevel2(SDL_Page& page);	// 게임 레벨 2 화면 초기화
void makeLevel3(SDL_Page& page);	// 게임 레벨 3 화면 초기화

void changeMenuSel(SDL_Page& page, int menu_sel);	// 메뉴 선택 화면 변경
game_state MenuIdToState(int menuID);	// 메뉴 선택시 화면 전환할 게임 상태 획득
void menuKeydownHandler(SDL_Window& window, SDL_Page& pageMenu, const int* pageIDs,
						SDLKey key, int& menu_sel, game_state& state);

void lv2KeydownHandler(SDL_Window& window, SDL_Page& pageMenu, const int* pageIDs,
					   SDLKey key, int& x, int& y, game_state& state);
void lv3KeydownHandler(SDL_Window& window, SDL_Page& pageMenu, const int* pageIDs,
					   SDLKey key, int& x, int& y, game_state& state);

void lv1KeydownHandler(GameLv1* game, SDLKey key, game_state& state);

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

	GameLv1 lv1(&win, pageIDs[level1], pageIDs[menu]);

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
							menuKeydownHandler(win, pageMenu, pageIDs, evt.key.keysym.sym, menu_sel, state);
							break;
						case level1:
							lv1KeydownHandler(&lv1, evt.key.keysym.sym, state);
							break;
						/*case level2:
							lv1KeydownHandler(win, pageLv1, pageIDs, evt.key.keysym.sym, sel_x, sel_y, state);
						case level3:
							lv1KeydownHandler(win, pageLv1, pageIDs, evt.key.keysym.sym, sel_x, sel_y, state);
							break;*/
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

//////////////////////////////////////////////////////////////////////////
// 화면 구성

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

void makeLevel1(SDL_Page& page)
{
	int not_flip_id, fg_b, fg_e;

	page.SetBgColor(0xff, 0xff, 0xff);	// white
	page.AddText("Level 1", 100, 30, 21);
	page.AddFillRect(160, 160, 315, 315, 0x00, 0x00, 0xff);	// blue
	page.AddFillRect(167, 167, 103, 103, 0xff, 0x00, 0x00);	// red (always 2nd)

	fg_b = page.AddImage("images\\1.jpg", 170, 170);
	page.AddImage("images\\1.jpg", 270, 170);
	page.AddImage("images\\2.jpg", 370, 170);
	page.AddImage("images\\2.jpg", 170, 270);
	not_flip_id = page.AddImage("images\\LOL_Logo.jpg", 270, 270);	// logo
	page.AddImage("images\\3.jpg", 370, 270);
	page.AddImage("images\\3.jpg", 170, 370);
	page.AddImage("images\\4.jpg", 270, 370);
	fg_e = page.AddImage("images\\4.jpg", 370, 370);

	for (int id=fg_b; id<=fg_e; ++id) {
		if(id!=not_flip_id)
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


//////////////////////////////////////////////////////////////////////////
// 메뉴 선택 화면

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


// 메뉴 선택 핸들러
void menuKeydownHandler(SDL_Window& window, SDL_Page& pageMenu, const int* pageIDs,
						SDLKey key, int& menu_sel, game_state& state)
{
	int pageID;

	switch(key) {
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
			state = MenuIdToState(menu_sel);	// ID에 맞는 상태 획득
			pageID = pageIDs[state];			// 상태 -> 페이지 ID 획득
			window.SelectPage(pageID);			// 페이지 ID -> 화면(Page) 선택

			break;
	}
}

//////////////////////////////////////////////////////////////////////////

// 홀수인 경우 건너띄는 처리
inline void check_skip_postion(int& x, int& y, int skip_pos, bool bChangeX, bool bIncrease)
{
	if (x==skip_pos && y==skip_pos) {
		if(bChangeX) {
			if (bIncrease) x++;
			else x--;
		} else {
			if (bIncrease) y++;
			else y--;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 레벨1 게임 컨트롤 핸들러
void lv1KeydownHandler(GameLv1* game, SDLKey key, game_state& state)
{
	switch(key) {
		case SDLK_ESCAPE:
			state = menu;		// 메뉴 상태로 전환
			game->Reset();
			game->GoMenuPage();	// 메뉴 페이지로 전환
			break;
		case SDLK_UP:
			game->CursorUP();
			break;
		case SDLK_DOWN:
			game->CursorDown();
			break;
		case SDLK_LEFT:
			game->CursorLeft();
			break;
		case SDLK_RIGHT:
			game->CursorRight();
			break;
		case SDLK_SPACE:	// 선택
			game->SpaceDown();
			break;
	}
}

void lv2KeydownHandler(SDL_Window& window, SDL_Page& pageMenu, const int* pageIDs,
					   SDLKey key, int& x, int& y, game_state& state)
{
	switch(key) {
		case SDLK_ESCAPE:
			state = menu;		// 메뉴 상태로 전환
			window.SelectPage(pageIDs[state]);	// 메뉴 페이지로 전환
			break;
	}
}

void lv3KeydownHandler(SDL_Window& window, SDL_Page& pageMenu, const int* pageIDs,
					   SDLKey key, int& x, int& y, game_state& state)
{
	switch(key) {
		case SDLK_ESCAPE:
			state = menu;		// 메뉴 상태로 전환
			window.SelectPage(pageIDs[state]);	// 메뉴 페이지로 전환
			break;
	}
}
