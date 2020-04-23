#ifndef BRANCH_HPP
#define BRANCH_HPP

#include "Point.hpp"

class Branch
{
  public:
    Branch();
    ~Branch();
    Branch(const Point& start, const Point& end, unsigned int age, unsigned int generation);
    // Point of the origin of the branch
    Point start_;
    // Growing point of the branch
    Point end_;
    // Age used to compute the thickness of the branch
    unsigned int age_;
    // Generation also used to compute the thickness of the branch
    unsigned int generation_;
    Point son_;
    // Return the next branch, grown toward the given direction and with an increased generation if it is spliting from
    // its parent.
    Branch grow(const Point& direction, unsigned int age) const;
    // Set the value of the new branch grown from this one, to avoid growing twice at the same position
    void setSon(const Point& son);
    // Check if the branch has already grown to this point to avoid redundancy. It is using son_
    bool haveAlreadyTried(const Point& next_son);
};

#endif
