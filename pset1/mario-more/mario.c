#include <cs50.h>
#include <stdio.h>

int get_height(void);
void print_grid(int height);

int main(void)
{
    // user keyin height
    int height = get_height();

    // print the pyramids
    print_grid(height);
}

int get_height(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (1 > height || height > 8);
    return height;
}

void print_grid(int height)
{
    for (int i = 1; i <= height; i++)
    {
        // left grid
        for (int j = height - i; j > 0; j--)
        {
            printf(" ");
        }
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }
        printf("  ");

        // right grid
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}