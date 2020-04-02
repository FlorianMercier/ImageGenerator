#include "CellularNoise.hpp"
#include <cstdlib>   // srand, rand
#include <ctime>   // time
#include <math.h>

CellularNoise::CellularNoise()
{
}

CellularNoise::~CellularNoise()
{
}

float CellularNoise::getUniformRandom(const float highLimit, const float lowLimite)
{
    return lowLimite + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (highLimit - lowLimite)));
}

void CellularNoise::generateRandomCells(unsigned int* cellsGrid,
                                        const unsigned int dimension,
                                        const unsigned int* tabSizes,
                                        const unsigned int nbOfCells)
{
    // Initialize the pseudo random fonction rand for different result every iteration
    srand(time(NULL));
    for(unsigned int i = 0; i < nbOfCells; i++)
    {
        for(unsigned int j = 0; j < dimension; j++)
        {
            cellsGrid[(i * dimension) + j] = (int) getUniformRandom(0., (float) tabSizes[j]);
        }
    }
}

void CellularNoise::incrementCoord(const unsigned int* tabSizes, unsigned int* coord)
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

float CellularNoise::findClosestCellPoint(const unsigned int* globalCoord,
                                          const unsigned int* cellsGrid,
                                          const unsigned int nbOfCells,
                                          const unsigned int dimension,
                                          const float maxDistance)
{
    float distance(0);
    float closestDistance(maxDistance);
    for(unsigned int i = 0; i < nbOfCells; i++)
    {
        for(unsigned int j = 0; j < dimension; j++)
        {
            distance += (float) ((globalCoord[j] - cellsGrid[i * dimension + j])
                                 * (globalCoord[j] - cellsGrid[i * dimension + j]));
        }
        if(distance < closestDistance)
        {
            closestDistance = distance;
        }
        distance = 0;
    }
    return std::sqrt(closestDistance / maxDistance);
}

void CellularNoise::generateCellularGrid(float* cellularGrid,
                                         const unsigned int* tabSizes,
                                         const unsigned int nbOfCells,
                                         const unsigned int dimension)
{
    unsigned int* globalCoord = new unsigned int[dimension + 1]();
    // Compute the grid's number of cell and the maximum distance between cells
    unsigned int totalCellularGridSize(1);
    float maxDistance(0.);
    for(unsigned int i = 0; i < dimension; i++)
    {
        totalCellularGridSize *= tabSizes[i];
        maxDistance += (float) (tabSizes[i] * tabSizes[i]);
    }

    unsigned int* cellsGrid = new unsigned int[nbOfCells * dimension]();
    generateRandomCells(cellsGrid, dimension, tabSizes, nbOfCells);

    // Cellular noise computation
    for(unsigned int i = 0u; i < totalCellularGridSize; i++)
    {
        cellularGrid[i] = findClosestCellPoint(globalCoord, cellsGrid, nbOfCells, dimension, maxDistance);
        incrementCoord(tabSizes, globalCoord);
    }

    // Free memory
    delete globalCoord;
    delete cellsGrid;
}