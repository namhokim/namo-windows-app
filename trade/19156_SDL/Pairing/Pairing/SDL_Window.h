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

	bool Initialize();				// �ʱ�ȭ
	bool IsInitialized() const;		// �ʱ�ȭ ���� Ȯ�� �Լ�

	int AddPage(SDL_Page* page);	// ������ �߰�, ��ȯ ��(������ id)
	bool SelectPage(int pageID);	// ������ ����(��ȯ)
	SDL_Page* GetPage(int pageID);	// ������ ���۷��� ȹ��

	void Refresh();					// ȭ�� ���ΰ�ħ

private:
	const char* title;
	int width, height;
	bool bInitialized;

	SDL_Surface *screen;

	// for Pages
	std::vector<SDL_Page*> pages;	// ��������
	int curr_page;					// ���õ� ������(������ 0����)

	// draws
	void drawBackground(SDL_Page* page);		// ����� �׸���
	void drawRects(SDL_Page* page);				// �簢���� �׸���
	void drawRect(const RECT_INFO* obj);		// �簢���� �׸�
	void drawImages(SDL_Page* page);			// �̹����� �׸���
	void drawImage(const IMAGE_INFO* obj);// �̹����� �׸�
	void drawTexts(SDL_Page* page);				// �ؽ�Ʈ�� �׸���
	void drawText(const TEXT_INFO* obj);// �ؽ�Ʈ�� �׸�

};
