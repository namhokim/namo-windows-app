#pragma once

#include "SDL_Window.h"

#define IMAGE_MAX	9
#define BUF_SIZE 10

extern const int Size;
extern const char* ImagePrefix;
extern const char* ImageSuffix;
extern const char* LogoImage;	// Ȧ������ ����� ���� Ÿ��

class GameLv2
{
public:
	GameLv2(SDL_Window*, int pageID, int pageMenu);
	~GameLv2(void);
};
