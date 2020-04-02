#ifndef CELLULARNOISE_HPP
#define CELLULARNOISE_HPP

class CellularNoise
{
  public:
    // Generate a n-dimensional cellular noise into a n-dimentional array with a given number of cells
    static void generateCellularGrid(float* cellularGrid,
                                     const unsigned int* tabSizes,
                                     const unsigned int nbOfCells,
                                     const unsigned int dimension);

  private:
    // Disallow construction
    CellularNoise();
    ~CellularNoise();

    // Return an uniform random value between given high and low limits
    static float getUniformRandom(const float highLimit, const float lowLimite);

    // Generate random coordonates of a given number of cells.
    static void generateRandomCells(unsigned int* cellsGrid,
                                    const unsigned int dimension,
                                    const unsigned int* tabSizes,
                                    const unsigned int nbOfCells);

    // Return the closest distance from the given point to cells
    static float findClosestCellPoint(const unsigned int* globalCoord,
                                      const unsigned int* cellsGrid,
                                      const unsigned int nbOfCells,
                                      const unsigned int dimension,
                                      const float maxDistance);

    // Fill the next coordinate array, to avoid doing n for()
    static void incrementCoord(const unsigned int* tabSizes, unsigned int* coord);
};

#endif
