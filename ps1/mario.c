#include <cs50.h>
#include <stdio.h>

void draw_pyramid(int h);

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    draw_pyramid(height);
}


void draw_pyramid(int h)
{
    // Draw the left part
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (j < h - i - 1)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("  ");

        // Draw the right part
        for (int j = 0; j <= i; j++)
        {
                printf("#");
        }
        printf("\n");
    }
}
