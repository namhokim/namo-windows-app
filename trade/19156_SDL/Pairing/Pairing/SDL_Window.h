#pragma once

#include <vector>	// for keep pages
#include "SDL_Page.h"

#define PAGE_NOT_SELECTED	(-1)
#define DEFAULT_PAGE		0		// FIRST PAGE

// implement for SDL functions wrapping
class SDL_Window
{
public:
	SDL_Window(const char* title, int width, int height);
	~SDL_Window(void);

	bool Initialize();				// 초기화
	bool IsInitialized() const;		// 초기화 여부 확인 함수

	int AddPage(SDL_Page* page);	// 페이지 추가, 반환 값(페이지 id)
	bool SelectPage(int pageID);	// 페이지 선택(전환)
	SDL_Page* GetPage(int pageID);	// 페이지 레퍼런스 획득

	void Refresh();					// 화면 새로고침

private:
	const char* title;
	int width, height;
	bool bInitialized;

	SDL_Surface *screen;

	// for Pages
	std::vector<SDL_Page*> pages;	// 페이지들
	int curr_page;					// 선택된 페이지(시작은 0부터)

	// draws
	void drawBackground(SDL_Page* page);		// 배경을 그린다
	void drawRects(SDL_Page* page);				// 사각형을 그린다
	void drawRect(const RECT_INFO* obj);		// 사각형을 그림
	void drawImages(SDL_Page* page);			// 이미지를 그린다
	void drawImage(const IMAGE_INFO* obj);// 이미지를 그림
	void drawTexts(SDL_Page* page);				// 텍스트를 그린다
	void drawText(const TEXT_INFO* obj);// 텍스트를 그림

};
