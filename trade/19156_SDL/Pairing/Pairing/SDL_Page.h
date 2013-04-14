#pragma once

#include <SDL.h>

class SDL_Page
{
public:
	SDL_Page(void);
	~SDL_Page(void);

	void SetBgColor(const Uint8 r, const Uint8 g, const Uint8 b);	// ���� ����
	Uint32 GetBgColor(SDL_Surface *screen);	// ���� ��������(SDL_Window���� ȣ��)

private:
	//Uint32 bgColor;
	// ����
	Uint8 bg_r, bg_g, bg_b;
};
