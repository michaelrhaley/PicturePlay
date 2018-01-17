/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit clue.bmp verdict.bmp\n");
        return 1;
    }

    // remember filenames
    // char *clue = argv[1];
    // char *verdict = argv[2];

    // open input file 
    FILE *clue = fopen(argv[1], "r");
    if (clue == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", "clue");
        return 2;
    }

    // open output file
    FILE *verdict = fopen(argv[2], "w");
    if (verdict == NULL)
    {
        fclose(clue);
        fprintf(stderr, "Could not create %s.\n", "verdict");
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, clue);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, clue);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(verdict);
        fclose(clue);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, verdict);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, verdict);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, clue);
            
            if(triple.rgbtRed == 255)
            {
                triple.rgbtRed = 255;
                triple.rgbtGreen = 255;
                triple.rgbtBlue = 255;
            }
            
            if(triple.rgbtRed == 204 & triple.rgbtGreen == 255 & triple.rgbtBlue == 255)
            {
                triple.rgbtRed = 255;
                triple.rgbtGreen = 0;
                triple.rgbtBlue = 0;
            }

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, verdict);
        }

        // skip over padding, if any
        fseek(clue, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, verdict);
        }
    }

    // close infile
    fclose(clue);

    // close outfile
    fclose(verdict);

    // success
    return 0;
}
