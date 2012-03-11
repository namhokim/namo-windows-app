#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN  // MFC 를 사용안함 
#endif

#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>

#define BMP_ID            0x4D42      // BMP 파일의 식별자(BM)
#define WINDOW_CLASS_NAME "WINCLASS"  // 윈도우 클래스 명칭의 정의 

#define WIDTH     640   // BMP 의 가로 넓이 
#define HEIGHT    480   // BMP 의 세로 길이 
#define PAL_COLOR 256   // 색상 수

LPDIRECTDRAW			lpDD = NULL;        // DirectDraw 객체
LPDIRECTDRAWSURFACE		lpDDSPrimary = NULL; // DirectDraw 기본표면 객체
LPDIRECTDRAWPALETTE		lpDDPal = NULL;     // DirectDraw  팔레트 객체
PALETTEENTRY			pal[256];           

BITMAPFILEHEADER bmpfileheader;   // BMP 파일의 헤더 
BITMAPINFOHEADER bmpinfoheader;   // BMP 파일의 정보 
PALETTEENTRY     bmp_pal[256];    // BMP 파일의 팔레트 
unsigned char    *bmp_buffer;     // BMP 그림 저장용 버퍼 

HWND saved_hwnd = NULL; 

//  윈도우즈클래스 만들기 및 윈도우 생성
int PASCAL WinMain(HINSTANCE hInstance,
       HINSTANCE hPrevInstance,
       LPSTR lpCmdLine,
       int nCmdShow)
{
   WNDCLASSEX winclass;  // 윈도우즈 클래스 선언
   HWND    hwnd;     // 윈도우 핸들
   MSG     msg;      // 메시지 선언

   winclass.cbSize     = sizeof(WNDCLASSEX);
   winclass.style  = CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
   winclass.lpfnWndProc = WindowProc;   // 메시지처리기의 명칭정의
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

// 윈도우즈 클래스의 등록
   if (!RegisterClassEx(&winclass))
       return(0);

// 윈도우의 생성
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
                         hInstance, // 응용프로그램의 인스턴스
                         NULL))) 

   return(0);

   saved_hwnd = hwnd;  // 윈도우즈 핸들의 저장 

   ProcessHere();   //  메인루틴

                           // 요놈이 일반적인 프로그램에서 안보이던놈이죠?
                          

 

// 메시지 루프
   while(GetMessage(&msg,NULL,0,0))
   { 
      TranslateMessage(&msg);

      // 메시지처리하기로 보낸다
      DispatchMessage(&msg);
   } 

   ReleaseObject(); // 요기서 해제할것들 죄다 해제해줍니다.
   return(msg.wParam);
} 
// 메시지 처리하기 함수의 정의 
long FAR PASCAL
WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:            
            ReleaseObject();  // DirectDraw 객체의 해제 
            PostQuitMessage(0);
            return 0L;

        case WM_KEYDOWN:            
            switch (wParam)
            {
                case VK_ESCAPE:  // ESC 나  
                case VK_F12:        // F12 라면 끝낸다 
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

// DirectDraw 객체생성, 표면생성, 팔레트생성 및 그림을 읽어오고 
// 기본표면에 출력한다 
int ProcessHere()
{
   DDSURFACEDESC2           ddsd;
   HRESULT                     hRet;
  
//  DirectDraw 객체의 생성
    hRet = DirectDrawCreateEx(NULL, (VOID**)&lpDD, 
         IID_IDirectDraw7, NULL); // 책에 설명은 나오고 컴파일도 잘되는데 MSDN에는 엄떠군요.

                                            // 하여간 DirectDrow7.0을 사용하기위한 객체생성부분
    if (hRet != DD_OK)
        return 0;

// 윈도우즈 화면을 전체 화면으로 설정
  hRet = lpDD->SetCooperativeLevel(saved_hwnd, DDSCL_EXCLUSIVE
            | DDSCL_FULLSCREEN);
   if (hRet != DD_OK)
 return 0;

// 윈도우즈 화면의 넓이, 높이, 색상(256) 의 정의 
    hRet = lpDD->SetDisplayMode(WIDTH, HEIGHT, 8, 0, 0);
    if (hRet != DD_OK)
        return 0;

// 기본표면의 사용을 위한 정의 
    ZeroMemory(&ddsd, sizeof(ddsd)); //메모리 초기화
    ddsd.dwSize = sizeof(ddsd); // 사이지 요만큼이라고 알려주고
    ddsd.dwFlags = DDSD_CAPS; //ddscaps멤버를 사용하는거라고 알려주는거래요.

                                               // 아마 밑에 하위 구조체 종류가 또 있나보죠. 선택적으루..
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    // 기본표면을 사용한다고 알려줍니다. 

// 기본표면 생성 
    hRet = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
    if (hRet != DD_OK)
        return 0;

    for (int color=0; color <= PAL_COLOR; color++)
    {
    // 검정색(0) 과 흰색(255) 을 포함한 팔레트정보 설정 
     pal[color].peRed   = color;
     pal[color].peGreen = color;
     pal[color].peBlue  = color;
     pal[color].peFlags = PC_NOCOLLAPSE; // 윈도우즈나 DX가 임의로 팔레트 바꾸는걸 방지해준대요.
    } 

// 팔레트 객체의 생성 (256)
  hRet = lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256
      | DDPCAPS_INITIALIZE, pal, &lpDDPal, NULL);
   if (hRet != DD_OK)
        return 0;

// 기본표면에 팔레트를 설정한다 
   hRet = lpDDSPrimary->SetPalette(lpDDPal);
   if (hRet != DD_OK)
    return 0;

// BMP 를 읽어 버퍼에 저장한다 
   if(!LoadBmp("backgrnd.bmp"))
     return(0);

// 팔레트를 DirectDraw 로 연계시킨다
   hRet = lpDDPal->SetEntries(0, 0, 256, bmp_pal);
   if (hRet != DD_OK)
    return(0);

// 기본표면을 잠근다  
 lpDDSPrimary->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR
     | DDLOCK_WAIT,NULL);

   unsigned char *dest = (unsigned char *)ddsd.lpSurface;    
   unsigned char *src  = bmp_buffer;

   for (int y=0; y < HEIGHT; y++)
    {
// 한 줄씩 버퍼에서 기본표면으로 복사한다 
       memcpy((void *)dest, (void *)src, WIDTH);
       
       dest+=ddsd.lPitch;    // Pitch 만큼 이동한다 
       src +=WIDTH;    // 그림의 넓이 만큼 이동한다 
    } 

// 기본표면을 풀어준다 
   hRet = lpDDSPrimary->Unlock(NULL);
   if (hRet != DD_OK)  
    return(0);

   free(bmp_buffer);

   return 1;
}
 

// BMP 파일을 식별하여 읽고 그림을 바로 잡는다
int LoadBmp(char *filename)
{
   FILE *fp;
   int i;          
   int temp_color;

   unsigned char *buffer = NULL; 

// 2진 파일 열기 
  if ((fp = fopen(filename, "rb"))==NULL)
       return(0);

// 비트맵 파일헤더 읽기 
   fread(&bmpfileheader, sizeof(BITMAPFILEHEADER), 1, fp);

// BMP 식별자를 판단하기 
   if (bmpfileheader.bfType!=BMP_ID)
   {
    fclose(fp);
    return(0);
   } 

// 비트맵 정보헤더 읽기
   fread(&bmpinfoheader, sizeof(BITMAPINFOHEADER), 1, fp);

// 256색상의 그림인지 판단하여 컬러팔레트 읽기 
   if (bmpinfoheader.biBitCount == 8)
   {
      fread(bmp_pal, 256*sizeof(PALETTEENTRY), 1, fp);

       for (i=0; i < 256; i++)
       {
       //  peRed 와 peBlue 를 서로 바꾼다 

       // RGBQUAD 와 PALETTEENTRY가 반대 구조로 되어있음
    temp_color        = bmp_pal[i].peRed;
    bmp_pal[i].peRed  = bmp_pal[i].peBlue;
    bmp_pal[i].peBlue = temp_color;
       
    bmp_pal[i].peFlags = PC_NOCOLLAPSE;
       } 
   } 

// 그림 데이터로의 위치이동 
   fseek(fp, -(int)(bmpinfoheader.biSizeImage), SEEK_END);

   if (bmpinfoheader.biBitCount==8)     
   {
    // 그림 데이터를 읽기위한 메모리 할당 
      if (!(bmp_buffer = (UCHAR *)malloc(bmpinfoheader.biSizeImage)))
       {
      //  메모리가 없으므로 닫는다 
          fclose(fp);
       return 0;
       } 

    // 그림 데이터 읽기 
    // 거꾸로 된 그림을 바로 잡는다  
    for (i=0; i < HEIGHT; i++)
     fread(&bmp_buffer[((HEIGHT-1) - i)*WIDTH],    
                        WIDTH, 1, fp);
   } 
   else
   {
    return 0;
   } 


// 비트맵 파일 닫기 
   fclose(fp);

    return(1);
} 


// DirectDraw 객체를 종료한다
static void ReleaseObject(void)
{
    if (lpDD != NULL)
    {
        if (lpDDSPrimary != NULL)           
        {
            lpDDSPrimary->Release();   // 기본표면의 해제  
   lpDDSPrimary = NULL;
        }
        if (lpDDPal != NULL)
        {
            lpDDPal->Release();        // 기본표면 팔레트의 해제
            lpDDPal = NULL;
        }
        lpDD->Release();
        lpDD = NULL;
    }
}
