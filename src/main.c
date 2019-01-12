#include <main.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#define dobpixbegin 0x1c

BMHEADER bm; //bitmap header
FILE *fp; //input file (dob)
FILE *op; //output file(bmp)

int imagew, imageh;
unsigned char *pxdata;
COLOR palette[16];
COLOR ph; //BGRs
PALETTE *pal;
int totalPalettes;
char outputString[255];

int writeBitmap(int palnumber) {
    int i, j;
    op = fopen(outputString, "wb");
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
    fseek(op, 122, SEEK_SET);

    printf("size of pallete %d is : %d\n", palnumber, pal[palnumber].pColors);

    for (i = 0; i < pal[palnumber].pColors; i++) {
        fwrite(&pal[palnumber].pPal[i], 4, 1, op);
    }
    fseek(op, bm.bmImageOffset, SEEK_SET);
    for (i = imageh - 1; i >= 0; i--) {
        fwrite(&pxdata[(imagew) * i], imagew, 1, op);
    }


    //fclose(fp);
    fclose(op);
    return 0;
}

int main(int argc, char** argv) {

    bm.bmMagic = 19778;
    bm.bmFileSize = 32954;
    bm.bmUnused1 = 0;
    bm.bmImageOffset = 186;
    bm.dibHeaderSize = 108;
    bm.dibImageWidth = 256;
    bm.dibImageHeight = 256;
    bm.dibColorPlanes = 1;
    bm.dibBitsPerPix = 4;
    bm.dibBitfields = 0;
    bm.dibPixDataSize = bm.bmFileSize - bm.bmImageOffset;
    bm.dibPrintResHor = 2835;
    bm.dibPrintResVer = 2835;
    bm.dibNoPalColors = 16;
    bm.dibNoImportCol = 0;

    imagew = bm.dibImageWidth / 2;
    imageh = bm.dibImageHeight;

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


    pxdata = malloc(bm.dibImageWidth * bm.dibImageHeight / 2);
    fseek(fp, dobpixbegin, SEEK_SET);
    fread(pxdata, bm.dibImageWidth * bm.dibImageHeight / 2, 1, fp);
    getPalettes(argv[1]);

    for (z = 0; z < totalPalettes - 1; z++) {
        sprintf(outputString, "%s-%d.bmp", argv[1], z);
        printf("%s\n", outputString);
        //
        writeBitmap(z);
    }

    fclose(fp);
    return 0;
}
