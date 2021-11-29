#include "PerlinNoise.hpp"
#include <cstdlib>   // srand, rand
#include <ctime>   // time
#include <iostream>
#include <math.h>

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::~PerlinNoise()
{
}

float PerlinNoise::getUniformRandom(const float highLimit, const float lowLimit)
{
    return lowLimit + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (highLimit - lowLimit)));
}

void PerlinNoise::getNormalRandomVector(const float highLimit,
                                        const float lowLimit,
                                        const unsigned int dimension,
                                        float* vector)
{
    float normSquared(0);
    for(unsigned int i = 0; i < dimension; i++)
    {
        vector[i] = getUniformRandom(highLimit, lowLimit);
        normSquared += vector[i] * vector[i];
    }

    float norm = sqrt(normSquared);
    for(unsigned int i = 0; i < dimension; i++)
    {
        vector[i] /= norm;
    }
}

float PerlinNoise::smootherStep(const float x)
{
    return x * x * x * (x * (6 * x - 15) + 10);
}

float PerlinNoise::normalizeNoiseValue(const float noiseValue)
{
    // Sigmoid normalization
    // return 1 / (1 + exp(-5 * noiseValue));
    // Linear normalization
    return (1 + noiseValue) / 2;
    // Rigged noise
    // return std::abs(noiseValue);
    /*if(std::abs(noiseValue) > (float) 0.1)
    {
        return 1;
    }
    else
    {
        return 0;
    }*/
    // No treatment
    // return noiseValue;
}

float PerlinNoise::dotProduct(const float* gradientVector, const float* coordinates, const unsigned int dimension)
{
    float dotProduct(0);
    for(unsigned int i = 0; i < dimension; i++)
    {
        dotProduct += gradientVector[i] * coordinates[i];
    }
    return dotProduct;
}

unsigned int PerlinNoise::binaryToGray(const unsigned int num)
{
    return num ^ (num >> 1);
}

void PerlinNoise::fillInvertedBinary(const unsigned int num, unsigned int* bin)
{
    bin[0] = num % 2;
    if(num / 2 > 0)
    {
        fillInvertedBinary(num / 2, &bin[1]);
    }
}

void PerlinNoise::fillSigma(const unsigned int dimension, const unsigned int nbOfVectors, unsigned int* sigma)
{
    for(unsigned int i = 0; i < nbOfVectors; i++)
    {
        fillInvertedBinary(binaryToGray(i), &sigma[i * dimension]);
    }
}

void PerlinNoise::fillListVectors(float* listVectors,
                                  const float* localCoord,
                                  const float norm,
                                  const unsigned int dimension,
                                  const unsigned int nbOfVectors,
                                  const unsigned int* sigma)
{
    for(unsigned int i = 0; i < nbOfVectors; i++)
    {
        for(unsigned int j = 0; j < dimension; j++)
        {
            listVectors[(i * dimension) + j] = (localCoord[j] - (float) sigma[(i * dimension) + j]) / norm;
        }
    }
}

float PerlinNoise::nLinearInterpolation(const float* dotTable,
                                        const float* coordinates,
                                        const unsigned int dimension,
                                        const unsigned int nbOfVectors)
{
    if(dimension > 1)
    {
        float A = nLinearInterpolation(&dotTable[0], coordinates, dimension - 1, nbOfVectors / 2);
        float B = nLinearInterpolation(&dotTable[nbOfVectors / 2], coordinates, dimension - 1, nbOfVectors / 2);
        return A + (coordinates[dimension - 1] * (B - A));
    }
    else
    {
        return dotTable[0] + (coordinates[0] * (dotTable[1] - dotTable[0]));
    }
}

void PerlinNoise::incrementCoord(const unsigned int* tabSizes, unsigned int* coord)
{
    if(coord[0] < tabSizes[0] - 1)
    {
        coord[0] += 1;
    }
    else
    {
        coord[0] = 0;
        incrementCoord(&tabSizes[1], &coord[1]);
    }
}

unsigned int PerlinNoise::getRankGrad(const unsigned int dimension,
                                      const unsigned int* tabSizes,
                                      const unsigned int numVect,
                                      const unsigned int* gradCoord,
                                      const unsigned int* sigma)
{
    unsigned int rankGrad = gradCoord[0] + sigma[dimension * numVect];
    for(unsigned int i = 1; i < dimension; i++)
    {
        rankGrad = (tabSizes[i] * rankGrad) + gradCoord[i] + sigma[(dimension * numVect) + i];
    }
    return dimension * rankGrad;
}

void PerlinNoise::generateGradientGrid(float* gradientGrid, const unsigned int dimension, const unsigned int totalSize)
{
    // Initialize the pseudo random function rand for different result every iteration
    srand(time(NULL));

    float* vector = new float[dimension]();
    for(unsigned int i = 0; i < totalSize; i++)
    {
        // Get an uniform random normal vector
        getNormalRandomVector(1, -1, dimension, vector);
        // Fill the gradient grid with uniform random normal vectors
        for(unsigned int j = 0; j < dimension; j++)
        {
            gradientGrid[(dimension * i) + j] = vector[j];
        }
    }
    delete vector;
}

void PerlinNoise::fillDotTable(float* dotTable,
                               const unsigned int dimension,
                               const unsigned int nbOfVectors,
                               float* listVectors,
                               const float* gradientGrid,
                               const unsigned int* tabSizes,
                               unsigned int* gradCoord,
                               const unsigned int* sigma)
{
    unsigned int rankGrad(0);
    for(unsigned int i = 0; i < nbOfVectors; i++)
    {
        rankGrad = getRankGrad(dimension, tabSizes, i, gradCoord, sigma);
        dotTable[binaryToGray(i)] = dotProduct(&gradientGrid[rankGrad], &listVectors[i * dimension], dimension);
    }
}

float PerlinNoise::perlinNoise(const unsigned int* globalCoord,
                               const unsigned int frequency,
                               const float* gradientGrid,
                               const unsigned int* tabSizes,
                               const unsigned int dimension,
                               const unsigned int nbOfVectors,
                               const unsigned int* sigma,
                               const float norm)
{
    // Create all the needed arrays
    float* localCoord = new float[dimension]();
    unsigned int* gradCoord = new unsigned int[dimension]();
    float* smoothCoord = new float[dimension]();
    float* dotTable = new float[nbOfVectors]();
    float* listVectors = new float[nbOfVectors * dimension]();

    // Convert global coordinates into local
    for(unsigned int i = 0; i < dimension; i++)
    {
        localCoord[i] = ((float) (globalCoord[i] % frequency) + (float) 0.5) / (float) frequency;
        gradCoord[i] = (globalCoord[i] - (globalCoord[i] % frequency)) / frequency;
        smoothCoord[i] = smootherStep(localCoord[i]);
    }

    // Fill with normalized distance vectors
    fillListVectors(listVectors, localCoord, norm, dimension, nbOfVectors, sigma);
    // Compute and fill the dot product of every edges
    fillDotTable(dotTable, dimension, nbOfVectors, listVectors, gradientGrid, tabSizes, gradCoord, sigma);
    // Interpolate all the dot products with a non linear smoothed point
    const float interpolatedValue = nLinearInterpolation(dotTable, smoothCoord, dimension, nbOfVectors);

    // Free memory
    delete dotTable;
    delete localCoord;
    delete gradCoord;
    delete smoothCoord;
    delete listVectors;

    // Return the normalized value between 0 and 1
    return normalizeNoiseValue(interpolatedValue);
}

void PerlinNoise::generatePerlinGrid(float* perlinGrid,
                                     const unsigned int* tabSizes,
                                     const unsigned int frequency,
                                     const unsigned int dimension)
{
    // Sizes of the gradient grid
    unsigned int* gradGridSizes = new unsigned int[dimension]();
    for(unsigned int i = 0; i < dimension; i++)
    {
        gradGridSizes[i] = (tabSizes[i] / frequency) + 1;
    }
    // Compute the grid's number of cell
    unsigned int totalGradGridSize(1);
    for(unsigned int i = 0; i < dimension; i++)
    {
        totalGradGridSize *= gradGridSizes[i];
    }

    float* gradientGrid = new float[totalGradGridSize * dimension]();
    generateGradientGrid(gradientGrid, dimension, totalGradGridSize);

    // Number of distance vectors between the edges of the grid and the point
    const unsigned int nbOfVectors = (unsigned int) std::pow(2u, dimension);

    // Max length used to normalize distance vectors
    const float norm = std::sqrt(((float) frequency - (float) 1) / ((float) frequency));

    // Create distance vectors sigma function
    unsigned int* sigma = new unsigned int[dimension * nbOfVectors]();
    fillSigma(dimension, nbOfVectors, sigma);

    unsigned int* globalCoord = new unsigned int[dimension + 1]();
    // Compute the grid's number of cell
    unsigned int totalperlinGridSize(1);
    for(unsigned int i = 0; i < dimension; i++)
    {
        totalperlinGridSize *= tabSizes[i];
    }

    // Perlin noise computation
    for(unsigned int i = 0u; i < totalperlinGridSize; i++)
    {
        perlinGrid[i]
            = perlinNoise(globalCoord, frequency, gradientGrid, gradGridSizes, dimension, nbOfVectors, sigma, norm);
        incrementCoord(tabSizes, globalCoord);
    }
    // Free memory
    delete sigma;
    delete gradientGrid;
    delete gradGridSizes;
    delete globalCoord;
}