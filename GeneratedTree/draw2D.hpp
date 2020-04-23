#ifndef DRAW2D_HPP
#define DRAW2D_HPP

#include "Branch.hpp"
#include "Point.hpp"
#include <fstream>
#include <vector>

class Draw2D
{
  public:
    // Fill a grid with pixels corresponding to the tree
    static void drawBMP(float* grid, const std::vector<Branch>& branches_vector, unsigned int height)
    {
        for(std::vector<Branch>::const_iterator branch_iter = branches_vector.begin(), end = branches_vector.end();
            branch_iter != end;
            ++branch_iter)
        {
            int dx(1);
            int dy(1);
            Point current_point((*branch_iter).start_);
            while(dx != 0 || dy != 0)
            {
                grid[(height * (int) current_point.coordinates_[0]) + height - (int) current_point.coordinates_[1]]
                    = 0.5;
                dx = (int) ((*branch_iter).end_.coordinates_[0] - current_point.coordinates_[0]);
                dy = (int) ((*branch_iter).end_.coordinates_[1] - current_point.coordinates_[1]);
                goToNextCell(current_point, dx, dy);
            }
        }
    };
    // Create an SVG image corresponding to the tree
    static void drawSVG(const std::vector<Branch>& branches_vector,
                        const double size,
                        const double size_trunk,
                        std::string name)
    {
        double factor(20.);
        double origin_thickness(size / factor);

        std::ofstream myfile;
        myfile.open(name + ".svg");
        myfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"";
        myfile << size + size_trunk;
        myfile << "\" width=\"";
        myfile << size;
        myfile << "\">\n<g transform=\"rotate(180 ";
        myfile << size / 2.;
        myfile << " ";
        myfile << (size + size_trunk) / 2.;
        myfile << ") scale(1)\">\n";

        for(std::vector<Branch>::const_iterator branch_iter = branches_vector.begin();
            branch_iter != branches_vector.end();
            ++branch_iter)
        {
            double thickness = origin_thickness
                / (double) ((*branch_iter).generation_ * ((*branch_iter).age_ + (2. * origin_thickness))
                            / (2 * origin_thickness));

            myfile << "<line x1=\"";
            myfile << (*branch_iter).start_.coordinates_[0];
            myfile << "\" y1=\"";
            myfile << (*branch_iter).start_.coordinates_[1];
            myfile << "\" x2=\"";
            myfile << (*branch_iter).end_.coordinates_[0];
            myfile << "\" y2=\"";
            myfile << (*branch_iter).end_.coordinates_[1];
            myfile << "\" fill=\"none\" stroke=\"gray\" stroke-width=\"";
            myfile << thickness;
            myfile << "\"/>\n";
        }

        myfile << "</g>\n</svg>";
        myfile.close();
    };

  private:
    Draw2D();
    ~Draw2D();
    // Return the sign of a number
    static int sign(double a)
    {
        if(a >= 0.0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    // While drawing a line on a grid go from a cell to the next one
    static void goToNextCell(Point& point, const int dx, const int dy)
    {
        int X(0);
        int Y(0);
        if(dx != 0)
        {
            double a = std::abs((double) dy / (double) dx);
            if(a < 3.)
            {
                X = sign(dx);
            }
            if(a > 1. / 3.)
            {
                Y = sign(dy);
            }
        }
        else if(dy != 0)
        {
            Y = sign(dy);
        }
        point.coordinates_[0] += X;
        point.coordinates_[1] += Y;
    };
};

#endif
