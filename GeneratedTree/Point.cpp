#include "Point.hpp"
#include <math.h>

Point::Point() : coordinates_({ 0 })
{
}

Point::~Point()
{
}

Point::Point(std::vector<double> coordinates) : coordinates_(coordinates)
{
}

Point Point::operator+(const Point& lpoint) const
{
    unsigned int end = coordinates_.size();
    std::vector<double> new_coordinates(end);
    for(unsigned int i = 0; i < end; i++)
    {
        new_coordinates[i] = coordinates_[i] + lpoint.coordinates_[i];
    }
    return Point(new_coordinates);
}

Point Point::operator-(const Point& lpoint) const
{
    unsigned int end = coordinates_.size();
    std::vector<double> new_coordinates(end);
    for(unsigned int i = 0; i < end; i++)
    {
        new_coordinates[i] = coordinates_[i] - lpoint.coordinates_[i];
    }
    return Point(new_coordinates);
}

Point Point::operator*(double lval) const
{
    unsigned int end = coordinates_.size();
    std::vector<double> new_coordinates(end);
    for(unsigned int i = 0; i < end; i++)
    {
        new_coordinates[i] = coordinates_[i] * lval;
    }
    return Point(new_coordinates);
}

Point Point::operator/(double lval) const
{
    unsigned int end = coordinates_.size();
    std::vector<double> new_coordinates(end);
    for(unsigned int i = 0; i < end; i++)
    {
        new_coordinates[i] = coordinates_[i] / lval;
    }
    return Point(new_coordinates);
}

void Point::operator+=(const Point& lpoint)
{
    for(unsigned int i = 0, end = coordinates_.size(); i < end; i++)
    {
        coordinates_[i] += lpoint.coordinates_[i];
    }
}

void Point::operator/=(double lval)
{
    for(unsigned int i = 0, end = coordinates_.size(); i < end; i++)
    {
        coordinates_[i] /= lval;
    }
}

// Return the sign of a number
double sign(double a)
{
    if(a >= 0.0)
    {
        return 1.;
    }
    else
    {
        return -1.;
    }
}

std::vector<double> Point::get_angles() const
{
    unsigned int end = coordinates_.size();
    std::vector<double> angles(end, 0.0);
    if(coordinates_ != angles)
    {
        for(unsigned int i = 0; i < end - 1; i++)
        {
            angles[i] = sign(coordinates_[i])
                * std::acos(coordinates_[i + 1]
                            / std::sqrt(coordinates_[i] * coordinates_[i] + coordinates_[i + 1] * coordinates_[i + 1]));
        }
    }
    return angles;
}

double Point::getSquaredDistance() const
{
    double squared_distance(0);
    for(unsigned int i = 0, end = coordinates_.size(); i < end; i++)
    {
        squared_distance += coordinates_[i] * coordinates_[i];
    }
    return squared_distance;
}

void Point::normalize()
{
    double squared_norm = getSquaredDistance();
    if(squared_norm > 0.0)
    {
        (*this) /= std::sqrt(squared_norm);
    }
}