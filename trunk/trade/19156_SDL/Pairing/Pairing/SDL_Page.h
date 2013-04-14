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

	// ����
	void SetBgColor(const Uint8 r, const Uint8 g, const Uint8 b);	// ���� ����
	Uint32 GetBgColor(SDL_Surface *screen);	// ���� ��������(SDL_Window���� ȣ��)

	// �̹���
	int AddImage(const char* file, int x, int y);	// �̹��� �߰�
	int GetImagesNumber();								// �������� ������ �̹��� ������ ����
	bool GetImageInfo(int imageID, IMAGE_INFO* imageInfo);

private:
	// ����
	Uint8 bg_r, bg_g, bg_b;

	// �̹���
	std::vector<IMAGE_INFO> imgs;	// �̹�����
};
