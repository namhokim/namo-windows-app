#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN  // MFC �� ������ 
#endif

#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>

#define BMP_ID            0x4D42      // BMP ������ �ĺ���(BM)
#define WINDOW_CLASS_NAME "WINCLASS"  // ������ Ŭ���� ��Ī�� ���� 

#define WIDTH     640   // BMP �� ���� ���� 
#define HEIGHT    480   // BMP �� ���� ���� 
#define PAL_COLOR 256   // ���� ��

LPDIRECTDRAW			lpDD = NULL;        // DirectDraw ��ü
LPDIRECTDRAWSURFACE		lpDDSPrimary = NULL; // DirectDraw �⺻ǥ�� ��ü
LPDIRECTDRAWPALETTE		lpDDPal = NULL;     // DirectDraw  �ȷ�Ʈ ��ü
PALETTEENTRY			pal[256];           

BITMAPFILEHEADER bmpfileheader;   // BMP ������ ��� 
BITMAPINFOHEADER bmpinfoheader;   // BMP ������ ���� 
PALETTEENTRY     bmp_pal[256];    // BMP ������ �ȷ�Ʈ 
unsigned char    *bmp_buffer;     // BMP �׸� ����� ���� 

HWND saved_hwnd = NULL; 

//  ��������Ŭ���� ����� �� ������ ����
int PASCAL WinMain(HINSTANCE hInstance,
       HINSTANCE hPrevInstance,
       LPSTR lpCmdLine,
       int nCmdShow)
{
   WNDCLASSEX winclass;  // �������� Ŭ���� ����
   HWND    hwnd;     // ������ �ڵ�
   MSG     msg;      // �޽��� ����

   winclass.cbSize     = sizeof(WNDCLASSEX);
   winclass.style  = CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
   winclass.lpfnWndProc = WindowProc;   // �޽���ó������ ��Ī����
   winclass.cbClsExtra      = 0; 
   winclass.cbWndExtra    = 0;
   winclass.hInstance    = hInstance;
   winclass.hIcon    = LoadIcon(NULL, IDI_APPLICATION);
   winclass.hCursor    = LoadCursor(NULL, IDC_ARROW);
   winclass.hbrBackground  =(HBRUSH)GetStockObject
                                       (WHITE_BRUSH);  
   winclass.lpszMenuName = NULL;
   winclass.lpszClassName = WINDOW_CLASS_NAME;
   winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

// �������� Ŭ������ ���
   if (!RegisterClassEx(&winclass))
       return(0);

// �������� ����
   if (!(hwnd = CreateWindowEx(NULL, 
                        WINDOW_CLASS_NAME,   
                        NULL,
    WS_POPUP,
                         0,
                         0,
                         GetSystemMetrics(SM_CXSCREEN),
                         GetSystemMetrics(SM_CYSCREEN),
                         NULL,    
                         NULL,    
                         hInstance, // �������α׷��� �ν��Ͻ�
                         NULL))) 

   return(0);

   saved_hwnd = hwnd;  // �������� �ڵ��� ���� 

   ProcessHere();   //  ���η�ƾ

                           // ����� �Ϲ����� ���α׷����� �Ⱥ��̴�������?
                          

 

// �޽��� ����
   while(GetMessage(&msg,NULL,0,0))
   { 
      TranslateMessage(&msg);

      // �޽���ó���ϱ�� ������
      DispatchMessage(&msg);
   } 

   ReleaseObject(); // ��⼭ �����Ұ͵� �˴� �������ݴϴ�.
   return(msg.wParam);
} 
// �޽��� ó���ϱ� �Լ��� ���� 
long FAR PASCAL
WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:            
            ReleaseObject();  // DirectDraw ��ü�� ���� 
            PostQuitMessage(0);
            return 0L;

        case WM_KEYDOWN:            
            switch (wParam)
            {
                case VK_ESCAPE:  // ESC ��  
                case VK_F12:        // F12 ��� ������ 
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                    return 0L;
            }
            break;

        case WM_SETCURSOR:          
            SetCursor(NULL);     
            return TRUE;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

// DirectDraw ��ü����, ǥ�����, �ȷ�Ʈ���� �� �׸��� �о���� 
// �⺻ǥ�鿡 ����Ѵ� 
int ProcessHere()
{
   DDSURFACEDESC2           ddsd;
   HRESULT                     hRet;
  
//  DirectDraw ��ü�� ����
    hRet = DirectDrawCreateEx(NULL, (VOID**)&lpDD, 
         IID_IDirectDraw7, NULL); // å�� ������ ������ �����ϵ� �ߵǴµ� MSDN���� ��������.

                                            // �Ͽ��� DirectDrow7.0�� ����ϱ����� ��ü�����κ�
    if (hRet != DD_OK)
        return 0;

// �������� ȭ���� ��ü ȭ������ ����
  hRet = lpDD->SetCooperativeLevel(saved_hwnd, DDSCL_EXCLUSIVE
            | DDSCL_FULLSCREEN);
   if (hRet != DD_OK)
 return 0;

// �������� ȭ���� ����, ����, ����(256) �� ���� 
    hRet = lpDD->SetDisplayMode(WIDTH, HEIGHT, 8, 0, 0);
    if (hRet != DD_OK)
        return 0;

// �⺻ǥ���� ����� ���� ���� 
    ZeroMemory(&ddsd, sizeof(ddsd)); //�޸� �ʱ�ȭ
    ddsd.dwSize = sizeof(ddsd); // ������ �丸ŭ�̶�� �˷��ְ�
    ddsd.dwFlags = DDSD_CAPS; //ddscaps����� ����ϴ°Ŷ�� �˷��ִ°ŷ���.

                                               // �Ƹ� �ؿ� ���� ����ü ������ �� �ֳ�����. ����������..
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    // �⺻ǥ���� ����Ѵٰ� �˷��ݴϴ�. 

// �⺻ǥ�� ���� 
    hRet = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
    if (hRet != DD_OK)
        return 0;

    for (int color=0; color <= PAL_COLOR; color++)
    {
    // ������(0) �� ���(255) �� ������ �ȷ�Ʈ���� ���� 
     pal[color].peRed   = color;
     pal[color].peGreen = color;
     pal[color].peBlue  = color;
     pal[color].peFlags = PC_NOCOLLAPSE; // ������� DX�� ���Ƿ� �ȷ�Ʈ �ٲٴ°� �������ش��.
    } 

// �ȷ�Ʈ ��ü�� ���� (256)
  hRet = lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256
      | DDPCAPS_INITIALIZE, pal, &lpDDPal, NULL);
   if (hRet != DD_OK)
        return 0;

// �⺻ǥ�鿡 �ȷ�Ʈ�� �����Ѵ� 
   hRet = lpDDSPrimary->SetPalette(lpDDPal);
   if (hRet != DD_OK)
    return 0;

// BMP �� �о� ���ۿ� �����Ѵ� 
   if(!LoadBmp("backgrnd.bmp"))
     return(0);

// �ȷ�Ʈ�� DirectDraw �� �����Ų��
   hRet = lpDDPal->SetEntries(0, 0, 256, bmp_pal);
   if (hRet != DD_OK)
    return(0);

// �⺻ǥ���� ��ٴ�  
 lpDDSPrimary->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR
     | DDLOCK_WAIT,NULL);

   unsigned char *dest = (unsigned char *)ddsd.lpSurface;    
   unsigned char *src  = bmp_buffer;

   for (int y=0; y < HEIGHT; y++)
    {
// �� �پ� ���ۿ��� �⺻ǥ������ �����Ѵ� 
       memcpy((void *)dest, (void *)src, WIDTH);
       
       dest+=ddsd.lPitch;    // Pitch ��ŭ �̵��Ѵ� 
       src +=WIDTH;    // �׸��� ���� ��ŭ �̵��Ѵ� 
    } 

// �⺻ǥ���� Ǯ���ش� 
   hRet = lpDDSPrimary->Unlock(NULL);
   if (hRet != DD_OK)  
    return(0);

   free(bmp_buffer);

   return 1;
}
 

// BMP ������ �ĺ��Ͽ� �а� �׸��� �ٷ� ��´�
int LoadBmp(char *filename)
{
   FILE *fp;
   int i;          
   int temp_color;

   unsigned char *buffer = NULL; 

// 2�� ���� ���� 
  if ((fp = fopen(filename, "rb"))==NULL)
       return(0);

// ��Ʈ�� ������� �б� 
   fread(&bmpfileheader, sizeof(BITMAPFILEHEADER), 1, fp);

// BMP �ĺ��ڸ� �Ǵ��ϱ� 
   if (bmpfileheader.bfType!=BMP_ID)
   {
    fclose(fp);
    return(0);
   } 

// ��Ʈ�� ������� �б�
   fread(&bmpinfoheader, sizeof(BITMAPINFOHEADER), 1, fp);

// 256������ �׸����� �Ǵ��Ͽ� �÷��ȷ�Ʈ �б� 
   if (bmpinfoheader.biBitCount == 8)
   {
      fread(bmp_pal, 256*sizeof(PALETTEENTRY), 1, fp);

       for (i=0; i < 256; i++)
       {
       //  peRed �� peBlue �� ���� �ٲ۴� 

       // RGBQUAD �� PALETTEENTRY�� �ݴ� ������ �Ǿ�����
    temp_color        = bmp_pal[i].peRed;
    bmp_pal[i].peRed  = bmp_pal[i].peBlue;
    bmp_pal[i].peBlue = temp_color;
       
    bmp_pal[i].peFlags = PC_NOCOLLAPSE;
       } 
   } 

// �׸� �����ͷ��� ��ġ�̵� 
   fseek(fp, -(int)(bmpinfoheader.biSizeImage), SEEK_END);

   if (bmpinfoheader.biBitCount==8)     
   {
    // �׸� �����͸� �б����� �޸� �Ҵ� 
      if (!(bmp_buffer = (UCHAR *)malloc(bmpinfoheader.biSizeImage)))
       {
      //  �޸𸮰� �����Ƿ� �ݴ´� 
          fclose(fp);
       return 0;
       } 

    // �׸� ������ �б� 
    // �Ųٷ� �� �׸��� �ٷ� ��´�  
    for (i=0; i < HEIGHT; i++)
     fread(&bmp_buffer[((HEIGHT-1) - i)*WIDTH],    
                        WIDTH, 1, fp);
   } 
   else
   {
    return 0;
   } 


// ��Ʈ�� ���� �ݱ� 
   fclose(fp);

    return(1);
} 


// DirectDraw ��ü�� �����Ѵ�
static void ReleaseObject(void)
{
    if (lpDD != NULL)
    {
        if (lpDDSPrimary != NULL)           
        {
            lpDDSPrimary->Release();   // �⺻ǥ���� ����  
   lpDDSPrimary = NULL;
        }
        if (lpDDPal != NULL)
        {
            lpDDPal->Release();        // �⺻ǥ�� �ȷ�Ʈ�� ����
            lpDDPal = NULL;
        }
        lpDD->Release();
        lpDD = NULL;
    }
}
