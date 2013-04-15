#pragma once

#include "SDL_Window.h"

#define IMAGE_MAX_LV1	9

namespace lv1 {
	extern const int Size;
}

extern const char* ImagePrefix;
extern const char* ImageSuffix;
extern const char* LogoImage;	// Ȧ������ ����� ���� Ÿ��

class GameLv1
{
public:
	GameLv1(SDL_Window*, int pageID, int pageMenu);
	~GameLv1(void);

	void StartGame();				// ���� ����
	void Endgame();					// ���� ����
	void GoMenuPage();				// �޴��� �̵�(ESC)
	void CursorMove(SDLKey key);	// Ŀ���̵�
	void SpaceDown();				// �����̽���

	bool IsCleared();	// ������ Ŭ����Ǿ���.
	void UpdateElapTime();	// �ð� ǥ�� ����

private:
	typedef enum {first, second, need_reset,cleared} g_status;	// ���ӻ���

	SDL_Window* win;			// ������
	int pageID, pageMenu, x, y;	// ���� ������ID, ���� ������ID, Ŀ��(x,y)

	int openCount, remainedCount;	// ��� ī����, ���� ī����
	g_status stat;					// ���ӻ���
	int firstX, firstY, secondX, secondY;	// ��� �̹��� ��ǥ
	char buffer_counter[BUF_SIZE];			// �õ��� Ƚ�� ���ڿ� ���� ����
	char img_file[IMAGE_MAX_LV1][_MAX_PATH];	// �̹��� ���� ��� ����
	char buffer_time[20];					// �ð�(���ڿ�)
	Uint32 startTime;						// ���� �ð�
	SDL_TimerID tID;						// Ÿ�̸� ID

	SDL_Page* GetPage();			// ���� ������ ��������
	RECT_INFO* GetSelRect();		// ������ �簢�� ��ü ��������
	void updateSelection();			// Ŀ���� ��ġ�� ȭ�鿡 �ݿ�
	bool IsFlipped(int x, int y);	// �����ô��� Ȯ��
	void Flip(int x, int y);		// ������
	void ResetFlips();				// ��� �̹��� �ǵ�����
	bool IsSameImage(int x, int y);	// ������ �̹������� Ȯ��
	const char* GetImageName(int x, int y);	// �̹��� �̸� Ȯ��
	void updateTryOpenCounter(int count);	// �õ��� Ƚ�� ȭ�鿡 �ݿ�
	void Shuffle();							// �̹����� ����
	void displayClearedGame(bool bVisible);	// clear ���� ȭ�鿡 ǥ��

	void initGame();		// ���� �ʱ�ȭ
	void startTimer();		// Ÿ�̸� ����
	void stopTimer();		// Ÿ�̸� ����

	void CursorUP();		// Ŀ�� �� ó��
	void CursorDown();		// Ŀ�� �� ó��
	void CursorLeft();		// Ŀ�� �� ó��
	void CursorRight();		// Ŀ�� �� ó��

	static Uint32 callbackTimeUpdate(Uint32 interval, void *param);
};
