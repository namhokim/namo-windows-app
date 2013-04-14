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
	bool bFlip;		// ������
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

	// ����
	void SetBgColor(const Uint8 r, const Uint8 g, const Uint8 b);	// ���� ����
	Uint32 GetBgColor(SDL_Surface *screen);	// ���� ��������(SDL_Window���� ȣ��)

	// �簢��
	int AddFillRect(int x, int y, int width, int height,
		Uint8 r=0x00, Uint8 g=0x00, Uint8 b=0x00);	// ä���� �簢�� �߰�
	int GetRectsNumber();								// �������� ������ �̹��� ������ ����
	RECT_INFO* GetRectInfo(int id);				// �簢�� ���� ȹ��


	// �̹���
	int AddImage(const char* file, int x, int y);		// �̹��� �߰�
	int GetImagesNumber();								// �������� ������ �̹��� ������ ����
	IMAGE_INFO* GetImageInfo(int imageID);				// �̹��� ���� ȹ��

	// �ؽ�Ʈ
	int AddText(const char* text, int x, int y, int size,
		Uint8 fg_r=0x00, Uint8 fg_g=0x00, Uint8 fg_b=0x00,
		const char* font = DEFAULT_FONT);				// �ؽ�Ʈ �߰�
	int GetTextsNumber();								// �������� ������ �ؽ�Ʈ ������ ����
	TEXT_INFO* GetTextInfo(int textID);					// �ؽ�Ʈ ���� ȹ��

private:
	// ����
	Uint8 bg_r, bg_g, bg_b;

	// �簢��
	std::vector<RECT_INFO> rects;	// �簢����

	// �̹���
	std::vector<IMAGE_INFO> imgs;	// �̹�����

	// �ؽ�Ʈ
	std::vector<TEXT_INFO> txts;	// �ؽ�Ʈ��
};
