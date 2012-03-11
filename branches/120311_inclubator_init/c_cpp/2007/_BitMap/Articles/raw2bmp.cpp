       #define DIB_HEADER_MAKER ((WORD)('M' << 8) | 'B')//BM MARKER
    // TODO: Add your command handler code here
    int width = 1300;//�����Ϸ��� raw�̹��� ũ�ⵥ�� �����ϼ���
    int height = 1030;
    int color = 256;//���ũ���� 256�� Gray level�� ����
    DWORD size,len; //���� ��鸵�ϴµ� ������

/*******BitmapInfo�� �޸𸮿� �ε��մϴ�.*********************/
    int infosize;
    LPBITMAPINFO pBitmapInfo = NULL;
    if(pBitmapInfo)
        delete pBitmapInfo;
    infosize=sizeof(BITMAPINFO)+(sizeof(RGBQUAD)*color);
    //infosize=sizeof(BITMAPINFO);
    pBitmapInfo =(LPBITMAPINFO) new char[infosize];
/*************************************************************/
    
//Bitmap�� �����Ϸ��� �ش��� �־����
    BITMAPFILEHEADER bfHeader;
    
// ������ ���� �κ��� MSDN�̳� å�� �����ϼ���..............
    pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//Size of 
bmiHeader
    pBitmapInfo->bmiHeader.biWidth = width;
    pBitmapInfo->bmiHeader.biHeight = height;
    pBitmapInfo->bmiHeader.biPlanes = 1;
    
    //������ DEPTH 256 Color�� 8�� ���� �մϴ�.
    pBitmapInfo->bmiHeader.biBitCount = 8;
    //���� ���� 4����� ������ �� ���
    /*dumy = width%4;
    ((4-dumy)==4)?dumy = 0:dumy = 4-dumy;
    BitmapInfo.bmiHeader.biSizeImage=(width+dumy)*height;*/
//BMP�� ���α��� 4����� �����ؾ� �Ǵµ� �ο� �̹����� 4����� �Ʒ�ó�� �߽���.
    pBitmapInfo->bmiHeader.biSizeImage=width*height;

    pBitmapInfo->bmiHeader.biCompression = BI_RGB;//Whether Compression or 
non compression
    pBitmapInfo->bmiHeader.biXPelsPerMeter = 0; // Meter per XPixel
    pBitmapInfo->bmiHeader.biYPelsPerMeter = 0; // Meter per YPixel
    pBitmapInfo->bmiHeader.biClrUsed = 256; //Real Color If 0 then use 
maximum colors
    pBitmapInfo->bmiHeader.biClrImportant = 0;
// �� BITMAPINFOHEADER�� ���� ������ BITMAPHEADER�� �����غ����....

    //Coding about BITMAPFILEHEADER
    bfHeader.bfType = DIB_HEADER_MAKER;//'BM'
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
        +((color)*sizeof(RGBQUAD));//Location of bitmap data
//Ȥ�ó� �ؼ� ���� 3�� ����ü�� ũ�⸦�Ѿ�� �װ��� ��Ʈ�� ����Ÿ�� ������� �� ��ġ
�� �����ϴ�.
    bfHeader.bfSize = sizeof(bfHeader)+((color-1)*sizeof(RGBQUAD))
+pBitmapInfo->bmiHeader.biSizeImage;
    bfHeader.bfReserved1 = 0;
    bfHeader.bfReserved2 = 0;

    BYTE        *lpPal;

    lpPal = (BYTE *) pBitmapInfo;// ������ �ѱ��� �� ������ �Ʒ�ó�� 0-255����
�� GRAY VALUE�� ��������.. ���� �̰� ���� ���� ����ߴµ�.. �̺κ��� ������ �ǽŴ�
�� �̤�

    DWORD offDest = sizeof(BITMAPINFOHEADER);
    for(int i = 0; i < 256; i++)
    {
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = 0x00;
    }
//�� �������ʹ� ���常 �ϸ�˴ϴ�.
    HANDLE fd= CreateFile
(strFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,NULL);

    if(!WriteFile(fd,(LPSTR)&bfHeader,sizeof(bfHeader),&len,NULL))
        AfxMessageBox("Err1");//Header����

    size=((color-1) * sizeof(RGBQUAD)) + sizeof(BITMAPINFO);
    if(!WriteFile(fd,pBitmapInfo,size,&len,NULL))
        AfxMessageBox("Err2");//InfoHeader ����

    BYTE * Image = new BYTE[width*height];// �Ű澲�� ������
    MbufGet2d(MilImage,0,0,width,height,Image);//�Ű澲�� ������.. ^^
    if(!WriteFile(fd,Image,pBitmapInfo->bmiHeader.biSizeImage,&len,NULL))
        AfxMessageBox("Err3");//Imgae�� RAW�̹����ε� �� 1300*1030 ī�޶�
�� �Ἥ MIL ���̺귯���� �޾Ҵµ� �������� �ٸ��� �ص� �˴ϴ�. ��ư Image�� raw�̹���
�� �����...
    delete [] Image;
    delete pBitmapInfo;
    delete strFileName;
    CloseHandle(fd);
