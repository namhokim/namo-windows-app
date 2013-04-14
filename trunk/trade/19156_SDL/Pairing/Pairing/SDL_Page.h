#pragma once

#include <SDL.h>
#include <vector>	// for keep images, texts

#define DEFAULT_FONT	"fonts\\MALGUN.TTF"

typedef struct _IMAGE_INFO {
	const char* file;
	int x, y;
	bool bDisplay;
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

	// �̹���
	int AddImage(const char* file, int x, int y);		// �̹��� �߰�
	int GetImagesNumber();								// �������� ������ �̹��� ������ ����
	bool GetImageInfo(int imageID, IMAGE_INFO* imageInfo);// �̹��� ���� ȹ��

	// �ؽ�Ʈ
	int AddText(const char* text, int x, int y, int size,
		Uint8 fg_r=0x00, Uint8 fg_g=0x00, Uint8 fg_b=0x00,
		const char* font = DEFAULT_FONT);				// �ؽ�Ʈ �߰�
	int GetTextsNumber();								// �������� ������ �ؽ�Ʈ ������ ����
	TEXT_INFO* GetTextInfo(int textID);					// �ؽ�Ʈ ���� ȹ��

private:
	// ����
	Uint8 bg_r, bg_g, bg_b;

	// �̹���
	std::vector<IMAGE_INFO> imgs;	// �̹�����

	// �ؽ�Ʈ
	std::vector<TEXT_INFO> txts;	// �ؽ�Ʈ��
};
