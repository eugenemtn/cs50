/**
 * resize.c
 *
 * Harvard CS50 course
 * Problem Set 3
 *
 * Resize (enlarge) a bmp by a factor of n
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper argument amount
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    // remember resize factor
    int n = atoi(argv[1]);

    // ensure n correctness
    if (n < 1 || n > 100) {
        fprintf(stderr, "The first argument supposed to be a positive integer within 1...100\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, new_bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    new_bf = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, new_bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    new_bi = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

     // set new size
    new_bi.biHeight = bi.biHeight * n;
    new_bi.biWidth = bi.biWidth * n;

    // determine padding for scanlines for both files
    int input_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int output_padding = (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // set new image sizes
    new_bi.biSizeImage = abs(new_bi.biHeight) * (new_bi.biWidth * sizeof(RGBTRIPLE) + output_padding);
    new_bf.bfSize = 54 + new_bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // read each line factor times
        for (int j = 0; j < n; j++)
        {
            // iterate over pixels in scanline
            for (int k = 0; k < bi.biWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int p = 0; p < n; p++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            for (int p = 0; p < output_padding; p++)
                fputc(0x00, outptr);

            if (j < n - 1)
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
        }

        // skip over padding, if any
        fseek(inptr, input_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    // success
    return 0;
}
