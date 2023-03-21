#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float rgbGray_f = (image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0;
            int rgbGray_i = round(rgbGray_f);
            image[h][w].rgbtRed = rgbGray_i;
            image[h][w].rgbtGreen = rgbGray_i;
            image[h][w].rgbtBlue = rgbGray_i;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*image_temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_temp[h][w] = image[h][w];
        }
        for (int w = 0; w < width; w++)
        {
            image[h][w] = image_temp[h][width - w - 1];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*image_temp)[width + 2] = calloc((height + 2), (width + 2) * sizeof(RGBTRIPLE));
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_temp[h + 1][w + 1] = image[h][w];
        }
    }
    for (int h = 1; h <= height; h++)
    {
        for (int w = 1; w <= width; w++)
        {
            float box = 0;
            float temp_rgbtRed = 0;
            float temp_rgbtGreen = 0;
            float temp_rgbtBlue = 0;
            for (int box_h = 0; box_h < 3; box_h++)
            {
                for (int box_w = 0; box_w < 3; box_w++)
                {
                    temp_rgbtRed += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtRed;
                    temp_rgbtGreen += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtGreen;
                    temp_rgbtBlue += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtBlue;
                    if ((h - 1 + box_h != 0) && (w - 1 + box_w != 0) && (h - 1 + box_h != height + 1) && (w - 1 + box_w != width + 1))
                    {
                        box++;
                    }
                }
            }
            image[h - 1][w - 1].rgbtRed = round(temp_rgbtRed / box);
            image[h - 1][w - 1].rgbtGreen = round(temp_rgbtGreen / box);
            image[h - 1][w - 1].rgbtBlue = round(temp_rgbtBlue / box);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // copy original image to image_temp array with a 1 pixel solid black border
    RGBTRIPLE(*image_temp)[width + 2] = calloc((height + 2), (width + 2) * sizeof(RGBTRIPLE));
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_temp[h + 1][w + 1] = image[h][w];
        }
    }

    // create GX and GY arrays
    int gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int h = 1; h <= height; h++)
    {
        for (int w = 1; w <= width; w++)
        {
            int position = 0;
            float gx_red = 0, gx_green = 0, gx_blue = 0;
            float gy_red = 0, gy_green = 0, gy_blue = 0;
            float temp_rgbtRed = 0, temp_rgbtGreen = 0, temp_rgbtBlue = 0;

            // loop throught GX and GY array
            for (int box_h = 0; box_h < 3; box_h++)
            {
                for (int box_w = 0; box_w < 3; box_w++)
                {
                    gx_red += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtRed * gx[position];
                    gx_green += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtGreen * gx[position];
                    gx_blue += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtBlue * gx[position];
                    gy_red += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtRed * gy[position];
                    gy_green += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtGreen * gy[position];
                    gy_blue += image_temp[h - 1 + box_h][w - 1 + box_w].rgbtBlue * gy[position];
                    position++;
                }
            }

            // calculate square root for GX and GY
            temp_rgbtRed = round(sqrt(pow(gx_red, 2) + pow(gy_red, 2)));
            temp_rgbtGreen = round(sqrt(pow(gx_green, 2) + pow(gy_green, 2)));
            temp_rgbtBlue = round(sqrt(pow(gx_blue, 2) + pow(gy_blue, 2)));

            // for color values out of borders
            if (temp_rgbtRed > 255)
            {
                temp_rgbtRed = 255;
            }
            if (temp_rgbtGreen > 255)
            {
                temp_rgbtGreen = 255;
            }
            if (temp_rgbtBlue > 255)
            {
                temp_rgbtBlue = 255;
            }

            // apply new colors pixels
            image[h - 1][w - 1].rgbtRed = temp_rgbtRed;
            image[h - 1][w - 1].rgbtGreen = temp_rgbtGreen;
            image[h - 1][w - 1].rgbtBlue = temp_rgbtBlue;
        }
    }
    return;
}
