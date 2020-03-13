#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

class PerlinNoise
{
  public:
    // Generate a n-dimensional perlin noise into a n-dimentional array with a given frequency
    static void generatePerlinGrid(float* perlinGrid,
                                   const unsigned int* tabSizes,
                                   const unsigned int frequency,
                                   const unsigned int dimension);

  private:
    // Disallow construction
    PerlinNoise();
    ~PerlinNoise();

    // Return an uniform random value between given high and low limits
    static float getUniformRandom(const float highLimit, const float lowLimite);

    // Fill a random normal vector of a given dimension
    static void getNormalRandomVector(const float highLimit,
                                      const float lowLimite,
                                      const unsigned int dimension,
                                      float* vector);

    // Return a new coordinate through a SmoothStep function to avoid image artefacts
    static float smootherStep(const float x);

    // Return a normalized noise value between 0 and 1
    static float normalizeNoiseValue(const float noiseValue);

    // Dot product between two given vectors in a given dimension
    static float dotProduct(const float* gradientVector, const float* coordinates, const unsigned int dimension);

    // Multidimensional linear interpolation
    static float nLinearInterpolation(const float* dotTable,
                                      const float* coordinates,
                                      const unsigned int dimension,
                                      const unsigned int nbOfVectors);

    // Convert an int into its corresponding int in the gray table
    static unsigned int binaryToGray(const unsigned int num);

    // Fill a given array with an inverted gray binary code of a given int
    // The least significant bit is at the highest address
    static void fillInvertedBinary(const unsigned int num, unsigned int* bin);

    // Fill the array of the coordinates of the point's surrounding gradient cells
    static void fillSigma(const unsigned int dimension, const unsigned int nbOfVectors, unsigned int* sigma);

    // Fill the array of the direction vectors between surrounding gradient cells and the point
    static void fillListVectors(float* listVectors,
                                const float* localCoord,
                                const float norm,
                                const unsigned int dimension,
                                const unsigned int nbOfVectors,
                                const unsigned int* sigma);

    // Return the rank of the gradient grid in the right order
    static unsigned int getRankGrad(const unsigned int dimension,
                                    const unsigned int* tabSizes,
                                    const unsigned int numVect,
                                    const unsigned int* gradCoord,
                                    const unsigned int* sigma);

    // Fill the next coordinate array, to avoid doing n for()
    static void incrementCoord(const unsigned int* tabSizes, unsigned int* coord);

    // Generate the gradient grid
    static void generateGradientGrid(float* gradientGrid, const unsigned int dimension, const unsigned int totalSize);

    // Fill an array of the results of dot product between distance vectors and gradient vectors
    static void fillDotTable(float* dotTable,
                             const unsigned int dimension,
                             const unsigned int nbOfVectors,
                             float* listVectors,
                             const float* gradientGrid,
                             const unsigned int* tabSizes,
                             unsigned int* gradCoord,
                             const unsigned int* sigma);

    // For each cell of the n-dimensional array retrun the coresponding perlin noise value
    static float perlinNoise(const unsigned int* globalCoordinates,
                             const unsigned int frequency,
                             const float* gradientGrid,
                             const unsigned int* tabSizes,
                             const unsigned int dimension,
                             const unsigned int nbOfVectors,
                             const unsigned int* sigma,
                             const float norm);
};

#endif
