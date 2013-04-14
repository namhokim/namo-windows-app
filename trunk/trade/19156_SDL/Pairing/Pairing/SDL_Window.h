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

	bool Initialize();		// �ʱ�ȭ
	bool IsInitialized() const { return bInitialized; };// �ʱ�ȭ ���� Ȯ�� �Լ�

	int AddPage(SDL_Page* page);	// ������ �߰�, ��ȯ ��(������ id)
	bool SelectPage(int pageID);	// ������ ����(��ȯ)

	void Refresh();					// ȭ�� ���ΰ�ħ

private:
	const char* title;
	int width, height;
	bool bInitialized;

	SDL_Surface *screen;

	// for Pages
	std::vector<SDL_Page*> pages;	// ��������
	int curr_page;					// ���õ� ������(������ 0����)

};