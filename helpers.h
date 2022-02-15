#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

int getAverageRGBValue(RGBTRIPLE pixel);

RGBTRIPLE getRGBFromGrayscaleValue(int value);

RGBTRIPLE getBoxBlurPixel(int height, int width, RGBTRIPLE image[height][width], int row, int column);

RGBTRIPLE getEdgeDetectedPixel(int height, int width, RGBTRIPLE image[height][width], int row, int column);
