#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// alias for C/C++ primitive data types
// https://msdn.microsoft.com/en-us/library/cc230309.aspx
typedef uint8_t  BYTE;

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // ensure proper argument amount
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file_image\n");
        return 1;
    }

    // remember image file
    char *image_file = argv[1];

    // open image file
    FILE *imageptr = fopen(image_file, "r");

    //allocate memory for resulting image name
    char filename[8];

    if (imageptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", image_file);
        return 2;
    }

    int reading_file = 0;
    int file_counter = 0;
    int bytes_readed = 512;
    BYTE buffer[BLOCK_SIZE];
    FILE *img = NULL;

    while (fread(buffer, 1, BLOCK_SIZE, imageptr) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (reading_file == 1) {
                fclose(img);
            }
            reading_file = 1;

            // printf to filename string
            sprintf(filename, "%03i%s", file_counter, ".jpg");
            // get a new image file pointer
            img = fopen(filename, "w");
            // update counter
            file_counter++;
            // write first chunk to img stream
            fwrite(buffer, BLOCK_SIZE, 1, img);
        } else {
            if (reading_file == 1)
            {
                fwrite(buffer, BLOCK_SIZE, 1, img);
            }
        }
    }

    // close a file if any opened
    if (img != NULL)
    {
        fclose(img);
    }

    return 0;
}
