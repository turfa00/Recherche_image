#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

typedef struct Result
{
    int x, y, width, height;
    double D;
    /*
    x = x coordinate in input image where search image start
    y = y coordinate in input image where search image start
    width = width of search image
    height = height of search image
    D = D value
    */

} Result;

unsigned char *getGrayImg(unsigned char *inputImg, int inputImgWidth, int inputImgHeight, int inputNbChannels)
{
    unsigned char *outputImg = (unsigned char *)malloc(inputImgHeight * inputImgWidth / inputNbChannels * sizeof(unsigned char *));
    int k = 0;
    for (int i = 0; i < inputImgHeight * inputImgWidth * inputNbChannels; i = i + inputNbChannels)
    {
        unsigned char gray = fmin(0.299 * (float)inputImg[i] + 0.587 * (float)inputImg[i + 1] + 0.114 * (float)inputImg[i + 2], 255.0);
        outputImg[k] = gray;
        k++;
    }
    return outputImg;
}

// SSD: SDD: Sum of square differences
double SSD(unsigned char *searchImg, unsigned char *inputImg, int index, int searchImgWidth, int searchImgHeight, int inputImgWidth, int inputImgHeight)
{
    // Get actual x and y of input images
    int xIn = index % inputImgWidth;
    int yIn = index / inputImgWidth;

    // Search image size exceeds input Image at a given point
    if (xIn + searchImgWidth > inputImgWidth || yIn + searchImgHeight > inputImgHeight)
    {
        return 1e20;
    }
    double D_xy = 0.0;
    double d = 0.0;
    // Do this shit in parallel idk lol
    for (int i = 0; i < searchImgHeight; i++)
    {
        for (int j = 0; j < searchImgWidth; j++)
        {
            d = inputImg[index + j + (i * inputImgWidth)] - searchImg[(i * searchImgWidth) + j];
            D_xy += pow(d, 2);
        }
    }
    return D_xy;
}

Result searchInImage(unsigned char *searchImg, int searchImgWidth, int searchImgHeight, unsigned char *inputImg, int inputImgWidth, int inputImgHeight)
{
    // Gotta do this shit in parallel
    Result result;
    // Set a big D value to update
    double globalD = 1e20;
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < inputImgHeight * inputImgWidth; i++)
    {
        double D_xy = SSD(searchImg, inputImg, i, searchImgWidth, searchImgHeight, inputImgWidth, inputImgHeight);
        //  #pragma omp critical
        if (D_xy >= 0.0 && D_xy < globalD)
        {
            result.x = i % inputImgWidth;
            result.y = i / inputImgWidth;
            result.width = searchImgWidth;
            result.height = searchImgHeight;
            result.D = D_xy;
            globalD = D_xy;
        }
    }
#pragma omp barrier
    return result;
}

unsigned char *drawRedBox(unsigned char *inputImg, int inputImgWidth, int inputImgHeight, Result result)
{
    // Input Image coordinates are multiplied by 3 because it has 3 color channels
    //   Top Border
    for (int i = result.x; i < (result.x + result.width); i++)
    {
        inputImg[(result.y * inputImgWidth + i) * 3] = 255;
        inputImg[(result.y * inputImgWidth + i) * 3 + 1] = 0;
        inputImg[(result.y * inputImgWidth + i) * 3 + 2] = 0;
    }
    // Left Border
    for (int i = result.y; i < (result.y + result.height); i++)
    {
        inputImg[(i * inputImgWidth + result.x) * 3] = 255;
        inputImg[(i * inputImgWidth + result.x) * 3 + 1] = 0;
        inputImg[(i * inputImgWidth + result.x) * 3 + 2] = 0;
    }
    // Right Border
    for (int i = result.y; i < (result.y + result.height); i++)
    {
        inputImg[(i * inputImgWidth + (result.x + result.width)) * 3] = 255;
        inputImg[(i * inputImgWidth + (result.x + result.width)) * 3 + 1] = 0;
        inputImg[(i * inputImgWidth + (result.x + result.width)) * 3 + 2] = 0;
    }
    // Bottom Border
    for (int i = result.x; i < (result.x + result.width); i++)
    {
        inputImg[((result.y + result.height) * inputImgWidth + i) * 3] = 255;
        inputImg[((result.y + result.height) * inputImgWidth + i) * 3 + 1] = 0;
        inputImg[((result.y + result.height) * inputImgWidth + i) * 3 + 2] = 0;
    }

    return inputImg;
}

int mainSearchImageOpenMp(unsigned char *searchImg, int searchImgWidth, int searchImgHeight, unsigned char *inputImg, int inputImgWidth, int inputImgHeight)
{
    unsigned char *grayInputImg = getGrayImg(inputImg, inputImgWidth, inputImgHeight, 3);
    unsigned char *graySearchImg = getGrayImg(searchImg, searchImgWidth, searchImgHeight, 3);
    Result result = searchInImage(graySearchImg, searchImgWidth, searchImgHeight, grayInputImg, inputImgWidth, inputImgHeight);
    if (result.D >= 1e20)
    {
        printf("Search Image not found in Input Image");
        return 0;
    }
    printf("Search Image found at: (%d, %d) with a D value of %.2f\n", result.x, result.y, result.D);
    unsigned char *resultImage = drawRedBox(inputImg, inputImgWidth, inputImgHeight, result);
    stbi_write_png("img/result_image.png", inputImgWidth, inputImgHeight, 3, resultImage, inputImgWidth * 3);
    printf("Result Image with red box saved as img/result_image.png\n");
    free(grayInputImg);
    free(graySearchImg);
    return 0;
}
