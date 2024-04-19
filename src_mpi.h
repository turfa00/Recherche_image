#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
// #include <mpi.h>

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