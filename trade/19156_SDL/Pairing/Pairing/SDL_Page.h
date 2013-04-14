#pragma once

#include <SDL.h>
#include <vector>	// for keep images, texts

#define DEFAULT_FONT	"fonts\\MALGUN.TTF"

typedef struct _RECT_INFO {
	int x, y, width, height;
	Uint8 r, g, b;
	bool bDisplay;
} RECT_INFO;

typedef struct _IMAGE_INFO {
	const char* file;
	int x, y;
	bool bDisplay;
	bool bFlip;		// 뒤집기
} IMAGE_INFO;

typedef struct _TEXT_INFO {
	const char* text;
	int x, y, size;
	const char* font;
	Uint8 fg_r, fg_g, fg_b;
	bool bDisplay;
} TEXT_INFO;

class SDL_Page
{
public:
	SDL_Page(void);
	~SDL_Page(void);

	// 배경색
	void SetBgColor(const Uint8 r, const Uint8 g, const Uint8 b);	// 배경색 지정
	Uint32 GetBgColor(SDL_Surface *screen);	// 배경색 가져오기(SDL_Window에서 호출)

	// 사각형
	int AddFillRect(int x, int y, int width, int height,
		Uint8 r=0x00, Uint8 g=0x00, Uint8 b=0x00);	// 채워진 사각형 추가
	int GetRectsNumber();								// 페이지의 지정된 이미지 개수를 구함
	RECT_INFO* GetRectInfo(int id);				// 사각형 정보 획득


	// 이미지
	int AddImage(const char* file, int x, int y);		// 이미지 추가
	int GetImagesNumber();								// 페이지의 지정된 이미지 개수를 구함
	IMAGE_INFO* GetImageInfo(int imageID);				// 이미지 정보 획득

	// 텍스트
	int AddText(const char* text, int x, int y, int size,
		Uint8 fg_r=0x00, Uint8 fg_g=0x00, Uint8 fg_b=0x00,
		const char* font = DEFAULT_FONT);				// 텍스트 추가
	int GetTextsNumber();								// 페이지의 지정된 텍스트 개수를 구함
	TEXT_INFO* GetTextInfo(int textID);					// 텍스트 정보 획득

private:
	// 배경색
	Uint8 bg_r, bg_g, bg_b;

	// 사각형
	std::vector<RECT_INFO> rects;	// 사각형들

	// 이미지
	std::vector<IMAGE_INFO> imgs;	// 이미지들

	// 텍스트
	std::vector<TEXT_INFO> txts;	// 텍스트들
};
