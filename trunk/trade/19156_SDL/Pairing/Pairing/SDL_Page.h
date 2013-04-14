#pragma once

#include <SDL.h>
#include <vector>	// for keep images, texts

typedef struct _IMAGE_INFO {
	const char* file;
	int x, y;
	bool bDisplay;
} IMAGE_INFO;

class SDL_Page
{
public:
	SDL_Page(void);
	~SDL_Page(void);

	// 배경색
	void SetBgColor(const Uint8 r, const Uint8 g, const Uint8 b);	// 배경색 지정
	Uint32 GetBgColor(SDL_Surface *screen);	// 배경색 가져오기(SDL_Window에서 호출)

	// 이미지
	int AddImage(const char* file, int x, int y);	// 이미지 추가
	int GetImagesNumber();								// 페이지의 지정된 이미지 개수를 구함
	bool GetImageInfo(int imageID, IMAGE_INFO* imageInfo);

private:
	// 배경색
	Uint8 bg_r, bg_g, bg_b;

	// 이미지
	std::vector<IMAGE_INFO> imgs;	// 이미지들
};
