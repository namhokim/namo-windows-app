#include "SDL_Window.h"
#include "GameLv1.h"
#include "GameLv2.h"
#include "GameLv3.h"

#include <iostream>
using namespace std;

// ���� ȭ�� �ܰ�(����)
typedef enum {
	init, menu, level1, level2, level3, quit,
} game_state;

// ���Ӹ޴� - ���� ����
#define SEL_LV1		1
#define SEL_LV2		2
#define SEL_LV3		3

void makeInitPage(SDL_Page& page);	// ���� ȭ�� �ʱ�ȭ
void makeMenuPage(SDL_Page& page);	// �޴� ȭ�� �ʱ�ȭ
void makeLevel1(SDL_Page& page);	// ���� ���� 1 ȭ�� �ʱ�ȭ
void makeLevel2(SDL_Page& page);	// ���� ���� 2 ȭ�� �ʱ�ȭ
void makeLevel3(SDL_Page& page);	// ���� ���� 3 ȭ�� �ʱ�ȭ

void changeMenuSel(SDL_Page& page, int menu_sel);	// �޴� ���� ȭ�� ����
game_state MenuIdToState(int menuID);	// �޴� ���ý� ȭ�� ��ȯ�� ���� ���� ȹ��
void menuKeydownHandler(SDL_Window& window, SDL_Page& pageMenu, const int* pageIDs,
						SDLKey key, int& menu_sel, game_state& state);

void lv1KeydownHandler(GameLv1* game, SDLKey key, game_state& state);
void lv2KeydownHandler(GameLv2* game, SDLKey key, game_state& state);
void lv3KeydownHandler(GameLv3* game, SDLKey key, game_state& state);

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
	GameLv2 lv2(&win, pageIDs[level2], pageIDs[menu]);
	GameLv3 lv3(&win, pageIDs[level3], pageIDs[menu]);

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
							state = menu;					// �޴� ���·� ��ȯ
							win.SelectPage(pageIDs[state]);	// �޴� �������� ��ȯ
							break;
						case menu:
							menuKeydownHandler(win, pageMenu, pageIDs, evt.key.keysym.sym, menu_sel, state);
							switch(state) {
								case level1:
									lv1.StartGame();
									break;
								case level2:
									lv2.StartGame();
									break;
								case level3:
									lv3.StartGame();
									break;
							}
							break;
						case level1:
							lv1KeydownHandler(&lv1, evt.key.keysym.sym, state);
							break;
						case level2:
							lv2KeydownHandler(&lv2, evt.key.keysym.sym, state);
						case level3:
							lv3KeydownHandler(&lv3, evt.key.keysym.sym, state);
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

//////////////////////////////////////////////////////////////////////////
// ȭ�� ����

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
	int id, not_flip_id, fg_b, fg_e;

	page.SetBgColor(0xff, 0xff, 0xff);	// white
	page.AddText("Level 1", 100, 30, 21);
	page.AddText("Elapsed Time(s) : ", 100, 530, 21);
	page.AddText("Try Counter     : ", 100, 565, 21);
	page.AddText("5", 300, 530, 21);					// time value
	page.AddText("1", 300, 565, 21);					// counter value
	id = page.AddText("Clear~ !!", 450, 30, 21, 0xff);	// red
	page.GetTextInfo(id)->bDisplay = false;
	id = page.AddText("Press Any Key.", 450, 530, 21);		// black
	page.GetTextInfo(id)->bDisplay = false;

	page.AddFillRect(160, 160, 315, 315, 0x00, 0x00, 0xff);	// blue
	page.AddFillRect(167, 167, 103, 103, 0xff, 0x00, 0x00);	// red (always 2nd)

	fg_b = page.AddImage("images\\1.jpg", 170, 170);
	page.AddImage("images\\1.jpg", 270, 170);
	page.AddImage("images\\2.jpg", 370, 170);
	page.AddImage("images\\2.jpg", 170, 270);
	not_flip_id = page.AddImage(LogoImage, 270, 270);	// logo
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
	int id, fg_b, fg_e;

	page.SetBgColor(0xff, 0xff, 0xff);	// white
	page.AddText("Level 2", 100, 30, 21);
	page.AddText("Elapsed Time(s) : ", 100, 530, 21);
	page.AddText("Try Counter     : ", 100, 565, 21);
	page.AddText("5", 300, 530, 21);					// time value
	page.AddText("1", 300, 565, 21);					// counter value
	id = page.AddText("Clear~ !!", 450, 30, 21, 0xff);	// red
	page.GetTextInfo(id)->bDisplay = false;
	id = page.AddText("Press Any Key.", 450, 530, 21);		// black
	page.GetTextInfo(id)->bDisplay = false;

	page.AddFillRect(110, 110, 417, 417, 0x00, 0x00, 0xff);	// blue
	page.AddFillRect(117, 117, 103, 103, 0xff, 0x00, 0x00);	// red (always 2nd)

	fg_b = page.AddImage("images\\1.jpg", 120, 120);
	page.AddImage("images\\1.jpg", 220, 120);
	page.AddImage("images\\2.jpg", 320, 120);
	page.AddImage("images\\2.jpg", 420, 120);
	page.AddImage("images\\3.jpg", 120, 220);
	page.AddImage("images\\3.jpg", 220, 220);
	page.AddImage("images\\4.jpg", 320, 220);
	page.AddImage("images\\4.jpg", 420, 220);
	page.AddImage("images\\5.jpg", 120, 320);
	page.AddImage("images\\5.jpg", 220, 320);
	page.AddImage("images\\6.jpg", 320, 320);
	page.AddImage("images\\6.jpg", 420, 320);
	page.AddImage("images\\7.jpg", 120, 420);
	page.AddImage("images\\7.jpg", 220, 420);
	page.AddImage("images\\8.jpg", 320, 420);
	fg_e = page.AddImage("images\\8.jpg", 420, 420);

	/*for (int id=fg_b; id<=fg_e; ++id) {
		page.GetImageInfo(id)->bFlip = true;
	}*/
}
void makeLevel3(SDL_Page& page)
{
	int id, not_flip_id, fg_b, fg_e;

	page.SetBgColor(0xff, 0xff, 0xff);	// white
	page.AddText("Level 3", 100, 30, 21);
	page.AddText("Elapsed Time(s) : ", 100, 600, 21);
	page.AddText("Try Counter : ", 400, 600, 21);
	page.AddText("5", 300, 600, 21);					// time value
	page.AddText("1", 520, 600, 21);					// counter value
	id = page.AddText("Clear~ !!", 450, 30, 21, 0xff);	// red
	page.GetTextInfo(id)->bDisplay = false;
	id = page.AddText("Press Any Key.", 450, 530, 21);		// black
	page.GetTextInfo(id)->bDisplay = false;

	page.AddFillRect(60, 60, 515, 515, 0x00, 0x00, 0xff);	// blue
	page.AddFillRect(68, 68, 103, 103, 0xff, 0x00, 0x00);	// red (always 2nd)

	fg_b = page.AddImage("images\\1.jpg", 70, 70);
	page.AddImage("images\\1.jpg", 170, 70);
	page.AddImage("images\\2.jpg", 270, 70);
	page.AddImage("images\\2.jpg", 370, 70);
	page.AddImage("images\\3.jpg", 470, 70);

	page.AddImage("images\\3.jpg", 70, 170);
	page.AddImage("images\\4.jpg", 170, 170);
	page.AddImage("images\\4.jpg", 270, 170);
	page.AddImage("images\\5.jpg", 370, 170);
	page.AddImage("images\\5.jpg", 470, 170);

	page.AddImage("images\\3.jpg", 70, 270);
	page.AddImage("images\\4.jpg", 170, 270);
	not_flip_id = page.AddImage(LogoImage, 270, 270);	// logo
	page.AddImage("images\\5.jpg", 370, 270);
	page.AddImage("images\\5.jpg", 470, 270);

	page.AddImage("images\\3.jpg", 70, 370);
	page.AddImage("images\\4.jpg", 170, 370);
	page.AddImage("images\\4.jpg", 270, 370);
	page.AddImage("images\\5.jpg", 370, 370);
	page.AddImage("images\\5.jpg", 470, 370);

	page.AddImage("images\\3.jpg", 70, 470);
	page.AddImage("images\\4.jpg", 170, 470);
	page.AddImage("images\\4.jpg", 270, 470);
	page.AddImage("images\\5.jpg", 370, 470);
	fg_e = page.AddImage("images\\5.jpg", 470, 470);

	for (int id=fg_b; id<=fg_e; ++id) {
		if(id!=not_flip_id)
			page.GetImageInfo(id)->bFlip = true;
	}
}


//////////////////////////////////////////////////////////////////////////
// �޴� ���� ȭ��

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


// �޴� ���� �ڵ鷯
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
			state = MenuIdToState(menu_sel);	// ID�� �´� ���� ȹ��
			pageID = pageIDs[state];			// ���� -> ������ ID ȹ��
			window.SelectPage(pageID);			// ������ ID -> ȭ��(Page) ����
			break;
	}
}

//////////////////////////////////////////////////////////////////////////

// Ȧ���� ��� �ǳʶ�� ó��
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
// ����1 ���� ��Ʈ�� �ڵ鷯
void lv1KeydownHandler(GameLv1* game, SDLKey key, game_state& state)
{
	if (game->IsCleared()) key = SDLK_ESCAPE;	// ���� Ŭ����� �ƹ�Ű��

	switch(key) {
		case SDLK_ESCAPE:
			state = menu;		// �޴� ���·� ��ȯ
			game->Endgame();
			game->GoMenuPage();	// �޴� �������� ��ȯ
			break;
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_LEFT:
		case SDLK_RIGHT:
			game->CursorMove(key);
			break;
		case SDLK_SPACE:	// ����
			game->SpaceDown();
			break;
	}
}

void lv2KeydownHandler(GameLv2* game, SDLKey key, game_state& state)
{
	if (game->IsCleared()) key = SDLK_ESCAPE;	// ���� Ŭ����� �ƹ�Ű��

	switch(key) {
		case SDLK_ESCAPE:
			state = menu;		// �޴� ���·� ��ȯ
			game->Endgame();
			game->GoMenuPage();	// �޴� �������� ��ȯ
			break;
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_LEFT:
		case SDLK_RIGHT:
			game->CursorMove(key);
			break;
		case SDLK_SPACE:	// ����
			game->SpaceDown();
			break;
	}
}
void lv3KeydownHandler(GameLv3* game, SDLKey key, game_state& state)
{
	if (game->IsCleared()) key = SDLK_ESCAPE;	// ���� Ŭ����� �ƹ�Ű��

	switch(key) {
		case SDLK_ESCAPE:
			state = menu;		// �޴� ���·� ��ȯ
			game->Endgame();
			game->GoMenuPage();	// �޴� �������� ��ȯ
			break;
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_LEFT:
		case SDLK_RIGHT:
			game->CursorMove(key);
			break;
		case SDLK_SPACE:	// ����
			game->SpaceDown();
			break;
	}
}