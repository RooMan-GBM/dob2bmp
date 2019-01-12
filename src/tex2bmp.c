#include <main.h>
#include <stdio.h>
#include <stdlib.h>

int getPalettes(char *dobfile) {
    FILE *fp;
    int i, j, palnum = 0, colcount;
    unsigned int endPalette, currentadd;
    fp = fopen(dobfile, "rb");
    if (!fp) {
        return 0;
    }
    fseek(fp, 16, SEEK_SET);
    fread(&endPalette, 4, 1, fp);
    fseek(fp, (0x1c)+(256 * 256) / 2, SEEK_SET);
    printf("jumped to %x\n", (unsigned int) ftell(fp));


    pal = malloc(sizeof (PALETTE)*32);

    currentadd = ftell(fp);
    while (currentadd < endPalette + 24) {

        for (i = 0; i < 16; i++) {
            fread(&pal[palnum].pPal[i].R, 1, 1, fp);
            fread(&pal[palnum].pPal[i].G, 1, 1, fp);
            fread(&pal[palnum].pPal[i].B, 1, 1, fp);
            fread(&pal[palnum].pPal[i].X, 1, 1, fp);

            if (pal[palnum].pPal[i].X == 0) {
                break;
            } else {
                printf("palette %d %x %x %x\n",
                        palnum, pal[palnum].pPal[i].R, pal[palnum].pPal[i].G, pal[palnum].pPal[i].B);
                pal[palnum].pColors++;
            }
        }
        printf("palette %d had %d colors\n", palnum, pal[palnum].pColors);
        palnum++;

        currentadd = ftell(fp);

    }
    totalPalettes = palnum;
    fclose(fp);
    return 0;
}

getPaletteNames(FILE *fp) {

    return 0;
}