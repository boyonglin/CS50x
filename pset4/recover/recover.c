#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open memory card
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    typedef uint8_t BYTE;
    BYTE buffer[BLOCK_SIZE];
    int JPEG_counter = -1;
    char filename[8];
    FILE *img = NULL;

    // read every block from raw file, either return 512 or 0
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // check the start signature of a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            JPEG_counter++;
            sprintf(filename, "%03i.jpg", JPEG_counter);

            // the very first JPEG
            if (JPEG_counter == 0)
            {
                img = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
            else
            {
                fclose(img);
                img = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
        }
        else
        {
            // if already found JPEG
            if (JPEG_counter >= 0)
            {
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
        }
    }

    // Close files
    fclose(raw_file);
    fclose(img);
    return 0;
}