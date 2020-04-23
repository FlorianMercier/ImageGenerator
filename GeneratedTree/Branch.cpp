#include "Branch.hpp"
#include <math.h>

Branch::Branch() : start_(Point()), end_(Point()), age_(1), generation_(1), son_(Point())
{
}

Branch::~Branch()
{
}

Branch::Branch(const Point& start, const Point& end, unsigned int age, unsigned int generation)
    : start_(start)
    , end_(end)
    , age_(age)
    , generation_(generation)
    , son_(Point())
{
}

Branch Branch::grow(const Point& grow_direction, const unsigned int new_age) const
{
    Point branch_direction = end_ - start_;
    unsigned int new_generation(generation_);
    std::vector<double> grow_angles = grow_direction.get_angles();
    std::vector<double> branch_angles = branch_direction.get_angles();

    // Check if the branch is spliting or continuing straight
    unsigned int i(0), end = grow_angles.size();
    while(new_generation == generation_ && i < end)
    {
        double angle_diff = std::abs(grow_angles[i] - branch_angles[i]);
        if(angle_diff > 1.1780972451 && angle_diff < 5.10508806208)   // 3Pi/8 and 13Pi/8
        {
            new_generation++;
        }
        i++;
    }
    return Branch(end_, end_ + grow_direction, new_age, new_generation);
}

void Branch::setSon(const Point& son)
{
    son_ = son;
}

// Check if two doubles are equals within a given precision
bool isAlmostEqualbis(const double lval, const double rval, int precision)
{
    return (int) (precision * (lval - rval)) == 0;
}

bool Branch::haveAlreadyTried(const Point& next_son)
{
    for(unsigned int i = 0, end = son_.coordinates_.size(); i < end; i++)
    {
        if(!isAlmostEqualbis(son_.coordinates_[i], next_son.coordinates_[i], 1000))
        {
            return false;
        }
    }
    return true;
}