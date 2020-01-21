// https://cs50.harvard.edu/x/2020/psets/4/filter/less/

#include <stdbool.h>
#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    RGBTRIPLE pixel;
    float middle;
    bool floor_middle;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixel = image[i][j];
            middle = (pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0f;
            floor_middle = middle - floor(middle) < 0.5;
            pixel.rgbtBlue = pixel.rgbtRed = pixel.rgbtGreen = floor_middle ? floor(middle) : ceil(middle);
            image[i][j] = pixel;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    bool floor_value;
    RGBTRIPLE pixel;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixel = image[i][j];
            int red = round(.393 * pixel.rgbtRed + .769 * pixel.rgbtGreen + .189 * pixel.rgbtBlue);
            int green = round(.349 * pixel.rgbtRed + .686 * pixel.rgbtGreen + .168 * pixel.rgbtBlue);
            int blue = round(.272 * pixel.rgbtRed + .534 * pixel.rgbtGreen + .131 * pixel.rgbtBlue);
            image[i][j].rgbtRed = red <= 255 ? red : 255;
            image[i][j].rgbtGreen = green <= 255 ? green : 255;
            image[i][j].rgbtBlue = blue <= 255 ? blue : 255;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    RGBTRIPLE original_line[width];

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            original_line[j] = image[i][j];
        }
        for (j = 0; j < width; j++)
        {
            image[i][j] = original_line[width - 1 - j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j, k, l, count_pixels;
    int red, green, blue;
    red = green = blue = 0;
    struct BoxBorders
    {
        int start_x;
        int end_x;
        int start_y;
        int end_y;
    } edges;
    RGBTRIPLE original_image[height][width];

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            original_image[i][j] = image[i][j];
        }
    }

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            count_pixels = 0;
            edges.start_x = j - 1 < 0 ? 0 : j - 1;
            edges.end_x = j + 1 > width - 1 ? width - 1 : j + 1;
            edges.start_y = i - 1 < 0 ? 0 : i - 1;
            edges.end_y = i + 1 > height - 1 ? height - 1 : i + 1;
            red = green = blue = 0;
            for (k = edges.start_y; k <= edges.end_y; k++)
            {
                for (l = edges.start_x; l <= edges.end_x; l++)
                {
                    count_pixels++;
                    red += original_image[k][l].rgbtRed;
                    green += original_image[k][l].rgbtGreen;
                    blue += original_image[k][l].rgbtBlue;
                }
            }
            image[i][j].rgbtRed = round((float) red / count_pixels);
            image[i][j].rgbtGreen = round((float) green / count_pixels);
            image[i][j].rgbtBlue = round((float) blue / count_pixels);
        }
    }
    return;
}

