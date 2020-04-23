#include "GeneratedTree.hpp"
#include "draw2D.hpp"
#include <cstdlib>   // srand, rand
#include <ctime>   // time
#include <iostream>
#include <math.h>

GeneratedTree::GeneratedTree()
{
}

GeneratedTree::~GeneratedTree()
{
}

double GeneratedTree::getUniformRandom(const double highLimit, const double lowLimite)
{
    return lowLimite + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (highLimit - lowLimite)));
}

bool isInside(const Point& point, const unsigned int dimension, const double size)
{
    double radius = size / 2.;
    Point center(std::vector<double>(dimension, radius));
    // The shape can be modified here
    return (point - center).getSquaredDistance() < radius * radius
        && (point - center).getSquaredDistance() > radius * radius / 9.;
}

Point GeneratedTree::getShapedRandomPoint(const unsigned int dimension, const double size)
{
    std::vector<double> new_coordinates(dimension);
    for(unsigned int i = 0; i < dimension; i++)
    {
        new_coordinates[i] = getUniformRandom(0, size);
    }
    Point new_point(new_coordinates);

    if(isInside(new_point, dimension, size))
    {
        return new_point;
    }
    else
    {
        return getShapedRandomPoint(dimension, size);
    }
}

void GeneratedTree::generateLeaves(std::vector<Point>& leaves_vector,
                                   const unsigned int nb_leaves,
                                   const unsigned int dimension,
                                   const double size,
                                   const double size_trunk)
{
    Point shift(std::vector<double>(dimension, 0.0));
    shift.coordinates_[1] = size_trunk;
    srand(time(NULL));
    for(unsigned int k = 0; k < nb_leaves; k++)
    {
        leaves_vector.push_back(getShapedRandomPoint(dimension, size) + shift);
    }
}

void GeneratedTree::plantTree(std::vector<Branch>& growing_branches_vector,
                              const unsigned int dimension,
                              const double size,
                              const double size_trunk)
{
    Point seed(std::vector<double>(dimension, size));
    seed.coordinates_[1] = 0.0;
    Point trunk(seed);
    trunk.coordinates_[1] += size_trunk;

    growing_branches_vector.push_back(Branch(seed, trunk, 1, 1));
}

double GeneratedTree::getSquaredDistance(const Point& point, const Branch& branch)
{
    return (branch.end_ - point).getSquaredDistance();
}

// Check if two doubles are equals with a given precision
bool isAlmostEqual(const double lval, const double rval, int precision)
{
    return (int) (precision * (lval - rval)) == 0;
}

bool GeneratedTree::isClosest(const Point& leaf,
                              const Branch& branch,
                              const std::vector<Branch>& growing_branches_vector)
{
    double min_distance(getSquaredDistance(leaf, (*growing_branches_vector.begin())));

    for(std::vector<Branch>::const_iterator branch_iter = growing_branches_vector.begin() + 1,
                                            end = growing_branches_vector.end();
        branch_iter != end;
        ++branch_iter)
    {
        double distance = getSquaredDistance(leaf, (*branch_iter));
        if(distance < min_distance)
        {
            min_distance = distance;
        }
    }
    return isAlmostEqual(getSquaredDistance(leaf, branch), min_distance, 1000);
}

bool GeneratedTree::isGrowingBackward(const Branch& branch, const Point& grow_direction)
{
    std::vector<double> grow_angles = grow_direction.get_angles();
    std::vector<double> branch_angles = (branch.end_ - branch.start_).get_angles();
    bool growing_backward(false);
    unsigned int i(0), end = grow_angles.size();
    while(growing_backward == false && i < end)
    {
        double angle_diff = std::abs(grow_angles[i] - branch_angles[i]);
        if(angle_diff > 2.94524311274 && angle_diff < 3.33794219444)   // 15Pi/16 and 17Pi/16
        {
            growing_backward = true;
        }
        i++;
    }
    return growing_backward;
}

void GeneratedTree::generate(float* grid,
                             const unsigned int DIMENSION,
                             const unsigned int NB_LEAVES,
                             const double SIZE,
                             const double range_max,
                             const double range_min,
                             const double grow_step)
{
    const double RANGE_MAX = range_max * range_max;
    const double RANGE_MIN = range_min * range_min;
    // Fixed variables
    const double SIZE_TRUNK = 50;
    Point WEIGHT({ 0.0, -0.0 });
    const std::string NAME = "tree";
    // Tree vectors
    std::vector<Point> leaves_vector;
    std::vector<Branch> branches_vector;
    std::vector<Branch> growing_branches_vector;

    generateLeaves(leaves_vector, NB_LEAVES, DIMENSION, SIZE, SIZE_TRUNK);
    // More plantTree() can be add to start with several seeds
    plantTree(growing_branches_vector, DIMENSION, SIZE / 2., 2.);

    unsigned int age(1);
    while(!growing_branches_vector.empty())
    {
        std::vector<Branch> branch_add;
        age++;
        std::cout << growing_branches_vector.size() << " - " << leaves_vector.size() << std::endl;

        std::vector<Branch>::iterator branch_iter = growing_branches_vector.begin();
        while(branch_iter != growing_branches_vector.end())
        {
            Point direction(std::vector<double>(DIMENSION, 0.0));
            unsigned int nb_atractive_leaves(0);

            std::vector<Point>::iterator leaf_iter = leaves_vector.begin();
            while(leaf_iter != leaves_vector.end())
            {
                double squared_distance_leaf_branch = getSquaredDistance((*leaf_iter), (*branch_iter));
                if(squared_distance_leaf_branch < RANGE_MAX
                   && isClosest(*leaf_iter, *branch_iter, growing_branches_vector))
                {
                    // Computed the average of the normalized directions toward attractive points
                    Point growing_vector = ((*leaf_iter) - (*branch_iter).end_);
                    growing_vector.normalize();
                    direction += growing_vector;
                    nb_atractive_leaves++;
                }
                if(squared_distance_leaf_branch < RANGE_MIN)
                {
                    // This attractive point has been reached
                    leaf_iter = leaves_vector.erase(leaf_iter);
                }
                else
                {
                    ++leaf_iter;
                }
            }
            if(nb_atractive_leaves > 0 && !isGrowingBackward((*branch_iter), direction))
            {
                direction.normalize();
                direction += WEIGHT;
                direction.normalize();
                Branch new_branch = (*branch_iter).grow(direction * grow_step, age);
                if(!((*branch_iter).haveAlreadyTried(new_branch.end_)))
                {
                    // Grow a new branch from this branch
                    (*branch_iter).setSon(new_branch.end_);
                    branch_add.push_back(new_branch);
                    ++branch_iter;
                }
                else
                {
                    // Deactivate this branch because it is on a loop
                    branches_vector.push_back((*branch_iter));
                    branch_iter = growing_branches_vector.erase(branch_iter);
                }
            }
            else
            {
                // Deactivate this branch because it is too far from attractive points
                branches_vector.push_back((*branch_iter));
                branch_iter = growing_branches_vector.erase(branch_iter);
            }
        }
        for(std::vector<Branch>::iterator branch_iter = branch_add.begin(); branch_iter != branch_add.end();
            ++branch_iter)
        {
            // Activate the new created branch
            growing_branches_vector.push_back((*branch_iter));
        }
    }
    Draw2D::drawSVG(branches_vector, SIZE, SIZE_TRUNK, NAME);
    Draw2D::drawBMP(grid, branches_vector, (unsigned int) (SIZE + SIZE_TRUNK));
}