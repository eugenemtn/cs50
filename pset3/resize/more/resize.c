/**
 * resize.c
 *
 * Harvard CS50 course
 * Problem Set 3
 *
 * Resize a bmp by a factor of n
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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
    float n = atof(argv[1]);

    // ensure n correctness
    if (n < 0.0 || n > 100.0) {
        fprintf(stderr, "The first argument supposed to be a positive float within 0...100\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    int BF_SIZE = sizeof(BITMAPFILEHEADER);
    int BI_SIZE = sizeof(BITMAPINFOHEADER);

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
    fread(&bf, BF_SIZE, 1, inptr);
    new_bf = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, new_bi;
    fread(&bi, BI_SIZE, 1, inptr);
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
    float new_height = bi.biHeight * n;
    float new_width = bi.biWidth * n;
    bool floor_height = new_height - floor(new_height) <= 0.5;
    bool floor_width = new_width - floor(new_width) <= 0.5;
    new_height = floor_height ? floor(new_height) : ceil(new_height);
    new_width = floor_width ? floor(new_width) : ceil(new_width);

    // set new image dimentions
    new_bi.biHeight = new_height;
    new_bi.biWidth = new_width;

     // determine ratio
    float width_ratio = (float) bi.biWidth / (float) new_width;
    float height_ratio = (float) bi.biHeight / (float) new_height;

    // determine padding for scanlines for both files
    int input_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int output_padding = (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // set new image sizes
    new_bi.biSizeImage = abs(new_bi.biHeight) * (new_bi.biWidth * sizeof(RGBTRIPLE) + output_padding);
    new_bf.bfSize = BF_SIZE + BI_SIZE + new_bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, BF_SIZE, 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, BI_SIZE, 1, outptr);

    // allocate memory for input bitmap line
    RGBTRIPLE in_line[sizeof(RGBTRIPLE) * bi.biWidth];
    int cached_row = -1;

    // for each row in a new file
    for (int i = 0; i < abs((int) new_height); i++)
    {
        int row = i * height_ratio;
        int start_read_at = (sizeof(RGBTRIPLE) * bi.biWidth + input_padding) * row;

        if (row != cached_row) {
            fseek(inptr, BF_SIZE + BI_SIZE + start_read_at, SEEK_SET);
            fread(in_line, sizeof(RGBTRIPLE), bi.biWidth, inptr);
            cached_row = row;
        }

        for (int j = 0; j < (int) new_width; j++)
        {
            int col = j * width_ratio;
            fwrite(&in_line[col], sizeof(RGBTRIPLE), 1, outptr);
        }

        // write output line padding
        for (int k = 0; k < output_padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    // success
    return 0;
}
