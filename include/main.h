#ifndef __MAIN_H__
#define __MAIN_H__
#define dobbmpstart 0x1c
typedef struct{
//bitmap header
short	bmMagic;	//BM
int	bmFileSize;	//file size
int	bmUnused1;	//unused
int	bmImageOffset;	//start of pixel array
//dib header
int 	dibHeaderSize;	//size of the dib header
int	dibImageWidth;	//image width L2R
int	dibImageHeight;	//image height B2T
short	dibColorPlanes;	//number of colour planes being used
short 	dibBitsPerPix;	//number of bits per pixel
int	dibBitfields;	//bit fields for stuff
int	dibPixDataSize;	//size of raw bitmap data with padding
int	dibPrintResHor;	//print res of image
int	dibPrintResVer;	// ditto
int	dibNoPalColors;	//number of colours in  the pallete
int	dibNoImportCol;	//number of important colours
}BMHEADER;

typedef struct{
unsigned char B;
unsigned char G;
unsigned char R;
unsigned char X;
}COLOR; // this  is used to store a colour value

typedef struct{
int pColors;    //number of colours in this palette
COLOR pPal[16]; //array of 16 colours AKA palette
char *pName; // name of this palette (maybe unused)
}PALETTE; // this stores a palette

int getPalettes(char *dobfile);// 
extern PALETTE *pal; //stores the whole collection of palettes
extern int totalPalettes; 
#endif 
