#ifndef POINT_HPP
#define POINT_HPP

#include <vector>

class Point
{
  public:
    Point();
    ~Point();
    Point(std::vector<double> coordinates);
    // Of the size of the chosen dimension
    std::vector<double> coordinates_;
    // Basic operations on points coordinates vectors
    Point operator+(const Point& lpoint) const;
    Point operator-(const Point& lpoint) const;
    Point operator/(const double lval) const;
    Point operator*(const double lval) const;
    void operator+=(const Point& lpoint);
    void operator/=(const double lval);
    // Return a vector of angles of the vector between the origin and the point
    std::vector<double> get_angles() const;
    // Return the norm to the square of the vector between the origin and the point
    double getSquaredDistance() const;
    // Normalize the vector between the origin and the point
    void normalize();
};

#endif
