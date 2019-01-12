#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#define dobpixbegin 0x1c // dob pixel data begins here

BMHEADER bm; //bitmap header
FILE *fp; //input file (dob)
FILE *op; //output file(bmp)

int imagew, imageh;
unsigned char *pxdata;
COLOR palette[16];
COLOR ph; //BGRs
PALETTE *pal; // array of palettes
int totalPalettes; // keep track of total palettes
char outputString[255]; // easier to under stand output file names

int writeBitmap(int palnumber) {  // writes the bitmap to file
    int i, j;
    op = fopen(outputString, "wb");
    //all this is explained elsewhere
    fwrite(&bm.bmMagic, 2, 1, op);
    fwrite(&bm.bmFileSize, 4, 1, op);
    fwrite(&bm.bmUnused1, 4, 1, op);
    fwrite(&bm.bmImageOffset, 4, 1, op);
    fwrite(&bm.dibHeaderSize, 4, 1, op);
    fwrite(&bm.dibImageWidth, 4, 1, op);
    fwrite(&bm.dibImageHeight, 4, 1, op);
    fwrite(&bm.dibColorPlanes, 2, 1, op);
    fwrite(&bm.dibBitsPerPix, 2, 1, op);
    fwrite(&bm.dibBitfields, 4, 1, op);
    fwrite(&bm.dibPixDataSize, 4, 1, op);
    fwrite(&bm.dibPrintResHor, 4, 1, op);
    fwrite(&bm.dibPrintResVer, 4, 1, op);
    fwrite(&bm.dibNoPalColors, 4, 1, op);
    fwrite(&bm.dibNoImportCol, 4, 1, op);
    fwrite(&ph, sizeof (COLOR), 1, op);
    fseek(op, 122, SEEK_SET); //jump to this address in the output file to begin writing palette data

    printf("size of pallete %d is : %d\n", palnumber, pal[palnumber].pColors); //debug

    for (i = 0; i < pal[palnumber].pColors; i++) { 
        fwrite(&pal[palnumber].pPal[i], 4, 1, op); // write the palette
    }
    fseek(op, bm.bmImageOffset, SEEK_SET);
    for (i = imageh - 1; i >= 0; i--) {
        fwrite(&pxdata[(imagew) * i], imagew, 1, op); //write the pixel data bottom to top used by BMP
    }

    fclose(op);
    return 0;
}

int main(int argc, char** argv) {

    bm.bmMagic = 19778; //'B' 'M'
    bm.bmFileSize = 32954; // precalculated file size
    bm.bmUnused1 = 0;
    bm.bmImageOffset = 186; // typical value
    bm.dibHeaderSize = 108; // agin this is a typical value
    bm.dibImageWidth = 256; // as far as i know this is always 256 in case of dob files
    bm.dibImageHeight = 256; // ditto
    bm.dibColorPlanes = 1; // default value
    bm.dibBitsPerPix = 4; // 4 bits perpixel
    bm.dibBitfields = 0; // no compresstion
    bm.dibPixDataSize = bm.bmFileSize - bm.bmImageOffset;// best not to hard code this
    bm.dibPrintResHor = 2835; // 72ppi default value
    bm.dibPrintResVer = 2835; // ditto
    bm.dibNoPalColors = 16; // this will change later
    bm.dibNoImportCol = 0; // 0= use all the colours avalable in pallete

    imagew = bm.dibImageWidth / 2; // the image width is half since only 4bpp
    imageh = bm.dibImageHeight; // this can stay the same
    
    //"header" for the palette
    ph.B = 'B'; 
    ph.G = 'G';
    ph.R = 'R';
    ph.X = 's';


    int i, j, z;
    fp = fopen(argv[1], "rb");
    if (!fp) {
        printf("error opening file\n");
        return 1;
    }


    pxdata = malloc(bm.dibImageWidth * bm.dibImageHeight / 2); // prep some memory
    fseek(fp, dobpixbegin, SEEK_SET); // jump to start of pixel array in the dob file
    fread(pxdata, bm.dibImageWidth * bm.dibImageHeight / 2, 1, fp); // read the pixel array
    getPalettes(argv[1]); // get the palettes as they come directly after, and passing the argv directly (naugty)

    for (z = 0; z < totalPalettes - 1; z++) { // prepare a file name before we write
        sprintf(outputString, "%s-%d.bmp", argv[1], z);
        printf("%s\n", outputString);
        //
        writeBitmap(z); // write the bitmap
    }

    fclose(fp); //close 
    return 0; //bye
}
