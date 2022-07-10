#include <math.h>
#include <stdint.h>

#include <stdlib.h>

#include "filters.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            uint8_t average = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE (*copy)[width] = malloc(sizeof(int[height][width]));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][width - j - 1];
        }
    }
    free(copy);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE (*copy)[width] = malloc(sizeof(int[height][width]));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int neighbours_count = 0;
            int neighbours_score_red = 0;
            int neighbours_score_blue = 0;
            int neighbours_score_green = 0;
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if ((i + k != -1) && (i + k != height) && (j + l != -1) && (j + l != width))
                    {
                        neighbours_count++;
                        neighbours_score_red += copy[i + k][j + l].rgbtRed;
                        neighbours_score_blue += copy[i + k][j + l].rgbtBlue;
                        neighbours_score_green += copy[i + k][j + l].rgbtGreen;
                    }
                }
            }
            image[i][j].rgbtRed = round((double)neighbours_score_red / (double)neighbours_count);
            image[i][j].rgbtBlue = round((double)neighbours_score_blue / (double)neighbours_count);
            image[i][j].rgbtGreen = round((double)neighbours_score_green / (double)neighbours_count);
        }
    }
    free(copy);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE (*copy)[width] = malloc(sizeof(int[height][width]));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int GxRed = 0, GxBlue = 0, GxGreen = 0;
            int GyRed = 0, GyBlue = 0, GyGreen = 0;
            int GzRed, GzBlue, GzGreen;
            int area_x[3][3] = {{-1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 }};
            int area_y[3][3] = {{ -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 }};
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if ((i + k != -1) && (i + k != height) && (j + l != -1) && (j + l != width))
                    {
                        GxRed += area_x[k + 1][l + 1] * copy[i + k][j + l].rgbtRed;
                        GxBlue += area_x[k + 1][l + 1] * copy[i + k][j + l].rgbtBlue;
                        GxGreen += area_x[k + 1][l + 1] * copy[i + k][j + l].rgbtGreen;
                        GyRed += area_y[k + 1][l + 1] * copy[i + k][j + l].rgbtRed;
                        GyBlue += area_y[k + 1][l + 1] * copy[i + k][j + l].rgbtBlue;
                        GyGreen += area_y[k + 1][l + 1] * copy[i + k][j + l].rgbtGreen;
                    }
                }
            }
            GzRed = round(sqrt(GxRed * GxRed + GyRed * GyRed));
            GzBlue = round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));
            GzGreen = round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            if (GzRed < 255)
            {
                image[i][j].rgbtRed = GzRed;
            }
            else
            {
                image[i][j].rgbtRed = 255;
            }
            if (GzBlue < 255)
            {
                image[i][j].rgbtBlue = GzBlue;
            }
            else
            {
                image[i][j].rgbtBlue = 255;
            }
            if (GzGreen < 255)
            {
                image[i][j].rgbtGreen = GzGreen;
            }
            else
            {
                image[i][j].rgbtGreen =  255;
            }
        }
    }
    free(copy);
    return;
}

void nofilter(void)
{
    return;
}