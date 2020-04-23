#ifndef GENERATEDTREE_HPP
#define GENERATEDTREE_HPP

#include "Branch.hpp"
#include "Point.hpp"
#include <vector>

class GeneratedTree
{
  public:
    static void generate(float* grid,
                         const unsigned int DIMENSION,
                         const unsigned int NB_LEAVES,
                         const double SIZE,
                         const double range_max,
                         const double range_min,
                         const double grow_step);

  private:
    GeneratedTree();
    ~GeneratedTree();
    // Return an uniform random value between given high and low limits
    static double getUniformRandom(const double highLimit, const double lowLimite);
    // Return a point with coordinates within a chosen n-dimensional shape
    static Point getShapedRandomPoint(const unsigned int dimension, const double size);
    // Fill a vector of randomly generated points within a n-dimensional shape
    static void generateLeaves(std::vector<Point>& leaves_vector,
                               const unsigned int nb_leaves,
                               const unsigned int dimension,
                               const double size,
                               const double size_trunk);
    // Fill a vector with a branch
    static void plantTree(std::vector<Branch>& growing_branches_vector,
                          const unsigned int dimension,
                          const double size,
                          const double size_trunk);
    // Get the distance to the square between a point and the ending point of a branch
    static double getSquaredDistance(const Point& point, const Branch& branch);
    // Check if a ending branch point is the closest to a point from any other branch
    static bool isClosest(const Point& leaf, const Branch& branch, const std::vector<Branch>& growing_branches_vector);
    // Check if the next branch is not growing back
    static bool isGrowingBackward(const Branch& branch, const Point& direction);
};

#endif
