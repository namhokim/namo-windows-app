       #define DIB_HEADER_MAKER ((WORD)('M' << 8) | 'B')//BM MARKER
    // TODO: Add your command handler code here
    int width = 1300;//저장하려는 raw이미지 크기데로 설정하세요
    int height = 1030;
    int color = 256;//모노크롬은 256의 Gray level을 갖죠
    DWORD size,len; //파일 헨들링하는데 쓰려구

/*******BitmapInfo를 메모리에 로드합니다.*********************/
    int infosize;
    LPBITMAPINFO pBitmapInfo = NULL;
    if(pBitmapInfo)
        delete pBitmapInfo;
    infosize=sizeof(BITMAPINFO)+(sizeof(RGBQUAD)*color);
    //infosize=sizeof(BITMAPINFO);
    pBitmapInfo =(LPBITMAPINFO) new char[infosize];
/*************************************************************/
    
//Bitmap을 저장하려면 해더가 있어야죠
    BITMAPFILEHEADER bfHeader;
    
// 설명이 없는 부분을 MSDN이나 책을 참조하세요..............
    pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//Size of 
bmiHeader
    pBitmapInfo->bmiHeader.biWidth = width;
    pBitmapInfo->bmiHeader.biHeight = height;
    pBitmapInfo->bmiHeader.biPlanes = 1;
    
    //영상의 DEPTH 256 Color은 8로 설정 합니다.
    pBitmapInfo->bmiHeader.biBitCount = 8;
    //가로 길이 4배수의 나머지 값 얻기
    /*dumy = width%4;
    ((4-dumy)==4)?dumy = 0:dumy = 4-dumy;
    BitmapInfo.bmiHeader.biSizeImage=(width+dumy)*height;*/
//BMP는 가로길이 4배수로 저장해야 되는데 로우 이미지가 4배수라서 아래처럼 했슴당.
    pBitmapInfo->bmiHeader.biSizeImage=width*height;

    pBitmapInfo->bmiHeader.biCompression = BI_RGB;//Whether Compression or 
non compression
    pBitmapInfo->bmiHeader.biXPelsPerMeter = 0; // Meter per XPixel
    pBitmapInfo->bmiHeader.biYPelsPerMeter = 0; // Meter per YPixel
    pBitmapInfo->bmiHeader.biClrUsed = 256; //Real Color If 0 then use 
maximum colors
    pBitmapInfo->bmiHeader.biClrImportant = 0;
// 자 BITMAPINFOHEADER을 설정 했으니 BITMAPHEADER을 설정해볼까요....

    //Coding about BITMAPFILEHEADER
    bfHeader.bfType = DIB_HEADER_MAKER;//'BM'
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
        +((color)*sizeof(RGBQUAD));//Location of bitmap data
//혹시나 해서 위의 3개 구조체의 크기를넘어서면 그곳에 비트맵 데이타가 저장되죠 그 위치
를 갖습니다.
    bfHeader.bfSize = sizeof(bfHeader)+((color-1)*sizeof(RGBQUAD))
+pBitmapInfo->bmiHeader.biSizeImage;
    bfHeader.bfReserved1 = 0;
    bfHeader.bfReserved2 = 0;

    BYTE        *lpPal;

    lpPal = (BYTE *) pBitmapInfo;// 번지를 넘기죠 그 번지에 아래처럼 0-255까지
의 GRAY VALUE를 저장하죠.. 제가 이걸 몰라서 많이 고생했는데.. 이부분이 도움이 되신다
면 ㅜㅜ

    DWORD offDest = sizeof(BITMAPINFOHEADER);
    for(int i = 0; i < 256; i++)
    {
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = 0x00;
    }
//자 이제부터는 저장만 하면됩니다.
    HANDLE fd= CreateFile
(strFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,NULL);

    if(!WriteFile(fd,(LPSTR)&bfHeader,sizeof(bfHeader),&len,NULL))
        AfxMessageBox("Err1");//Header저장

    size=((color-1) * sizeof(RGBQUAD)) + sizeof(BITMAPINFO);
    if(!WriteFile(fd,pBitmapInfo,size,&len,NULL))
        AfxMessageBox("Err2");//InfoHeader 저장

    BYTE * Image = new BYTE[width*height];// 신경쓰지 마세요
    MbufGet2d(MilImage,0,0,width,height,Image);//신경쓰지 마세요.. ^^
    if(!WriteFile(fd,Image,pBitmapInfo->bmiHeader.biSizeImage,&len,NULL))
        AfxMessageBox("Err3");//Imgae는 RAW이미지인데 전 1300*1030 카메라
를 써서 MIL 라이브러리로 받았는데 여러분은 다르게 해도 됩니다. 암튼 Image는 raw이미지
의 버펍당...
    delete [] Image;
    delete pBitmapInfo;
    delete strFileName;
    CloseHandle(fd);
