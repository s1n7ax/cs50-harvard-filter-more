#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int row = 0; row < height; row++) {
        for(int pixel = 0; pixel < width; pixel++) {
            int average = getAverageRGBValue(image[row][pixel]);
            image[row][pixel] = getRGBFromGrayscaleValue(average);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int middle = (int)(width / 2);

    for(int row = 0; row < height; row++) {
        for(int pixel = 0; pixel < middle; pixel++) {
            RGBTRIPLE tmp = image[row][pixel];
            image[row][pixel] = image[row][width - 1 - pixel];
            image[row][width - 1 - pixel] = tmp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    for(int row = 0; row < height; row++) {
        for(int column = 0; column < width; column++) {
            image[row][column] = getBoxBlurPixel(height, width, image, row, column);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    for(int row = 0; row < height; row++) {
        for(int column = 0; column < width; column++) {
            image[row][column] = getEdgeDetectedPixel(height, width, image, row, column);
        }
    }

    return;
}


int getAverageRGBValue(RGBTRIPLE pixel)
{
    double total = (pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0;
    return (int) ((total - floor(total) > 0.5) ? ceil(total) : floor(total));
}

RGBTRIPLE getRGBFromGrayscaleValue(int value)
{
    RGBTRIPLE pixel;

    pixel.rgbtBlue = value;
    pixel.rgbtGreen = value;
    pixel.rgbtRed = value;

    return pixel;
}

RGBTRIPLE getBoxBlurPixel(int height, int width, RGBTRIPLE image[height][width], int row, int column) {
    int pixelCount = 0;
    int r_total = 0;
    int g_total = 0;
    int b_total = 0;

    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            int boxRow = row + i;
            int boxColumn = column + j;

            if(boxRow < 0 || boxRow >= height) {
                continue;
            }

            if(boxColumn < 0 || boxColumn >= width) {
                continue;
            }

            pixelCount++;

            r_total += image[boxRow][boxColumn].rgbtRed;
            g_total += image[boxRow][boxColumn].rgbtGreen;
            b_total += image[boxRow][boxColumn].rgbtBlue;
        }
    }

    RGBTRIPLE pixel;

    pixel.rgbtRed = r_total/pixelCount;
    pixel.rgbtGreen = g_total/pixelCount;
    pixel.rgbtBlue = b_total/pixelCount;

    return pixel;
}

RGBTRIPLE getEdgeDetectedPixel(int height, int width, RGBTRIPLE image[height][width], int row, int column) {
    int r_gx = 0;
    int g_gx = 0;
    int b_gx = 0;

    int r_gy = 0;
    int g_gy = 0;
    int b_gy = 0;
    
    /*
    ┌───────────┬──────────┬──────────┐
    │           │          │          │
    │           │          │          │
    │   -1,-1   │   -1,0   │   -1,+1  │
    │           │          │          │
    │           │          │          │
    ├───────────┼──────────┼──────────┤
    │           │          │          │
    │           │          │          │
    │   0 ,-1   │   0, 0   │   0,+1   │
    │           │          │          │
    │           │          │          │
    ├───────────┼──────────┼──────────┤
    │           │          │          │
    │           │          │          │
    │   +1,-1   │   +1,0   │   +1,+1  │
    │           │          │          │
    │           │          │          │
    └───────────┴──────────┴──────────┘
    */
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {

            if(i < 0 || (row + i) >= height)
                continue;

            if(j < 0 || (column + i) >= width)
                continue;

            int gxMultiplier = j;
            int gyMultiplier = i;

            if(i == 0) {
                gxMultiplier = (j * 2);
            }

            if(j == 0) {
                gyMultiplier = (i * 2);
            }

            // calculating Gx
            r_gx += gxMultiplier * image[row + i][column + j].rgbtRed;
            g_gx += gxMultiplier * image[row + i][column + j].rgbtGreen;
            b_gx += gxMultiplier * image[row + i][column + j].rgbtBlue;


            // calculating Gy
            r_gy += gyMultiplier * image[row + i][column + j].rgbtRed;
            g_gy += gyMultiplier * image[row + i][column + j].rgbtGreen;
            b_gy += gyMultiplier * image[row + i][column + j].rgbtBlue;
        }
    }

    int r_edge = sqrt((r_gx * r_gx) + (r_gy + r_gy));
    int g_edge = sqrt((g_gx * g_gx) + (g_gy + g_gy));
    int b_edge = sqrt((b_gx * b_gx) + (b_gy + b_gy));

    r_edge = r_edge > 255 ? 255: r_edge;
    g_edge = g_edge > 255 ? 255: g_edge;
    b_edge = b_edge > 255 ? 255: b_edge;

    RGBTRIPLE edge_pixel;

    edge_pixel.rgbtRed = r_edge;
    edge_pixel.rgbtGreen = g_edge;
    edge_pixel.rgbtBlue = b_edge;

    return edge_pixel;
}
