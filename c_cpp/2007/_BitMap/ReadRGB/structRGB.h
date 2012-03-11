typedef unsigned char			BYTE;
#define	BIWIDTH		256
#define	BIHEIGHT	256
// user type
/* defined "WINGDI.H"
typedef struct tagRGBTRIPLE {
	BYTE	rgbtBlue;
	BYTE	rgbtGreen;
	BYTE	rgbtRed;
} RGBTRIPLE;
*/
typedef struct tagBITMAPLINE {
	RGBTRIPLE colors[BIWIDTH];
} BITMAPLINE;
