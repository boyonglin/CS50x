#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int startSize;
    do
    {
        startSize = get_int("Start size: ");
    }
    while (startSize < 9);

    // TODO: Prompt for end size
    int endSize;
    do
    {
        endSize = get_int("End size: ");
    }
    while (endSize < startSize);

    // TODO: Calculate number of years until we reach threshold
    int year = 0;
    int llama = startSize;
    if (startSize == endSize)
    {
        year = 0;
    }
    else
    {
        do
        {
            llama = llama + floor(llama / 3) - floor(llama / 4);
            year++;
        }
        while (llama < endSize);
    // }

    // TODO: Print number of years
    printf("Years: %i\n", year);
}
