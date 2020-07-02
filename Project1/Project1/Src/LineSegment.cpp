#include <cmath>

#include "DoubleCompare.h"
#include "LineSegment.h"

Point midpt;
Point p1;
Point p2;
Line perpendicular;

LineSegment::LineSegment()
{
	this->p1 = Point(0.0, 0.0);
	this->p2 = Point(0.0, 0.0);
	this->midpt = Point(0.0, 0.0);
	this->perpendicular = Line(0.0, 0.0);
}

LineSegment::~LineSegment()
{
}

LineSegment::LineSegment(Point p1, Point p2) : Line(p1, p2) {
    this->p1 = p1;
    this->p2 = p2;
    double midY = ((p2.GetY() - p1.GetY()) / 2) + p1.GetY();
    double midX = ((p2.GetX() - p1.GetX()) / 2) + p1.GetX();
    this->midpt = Point(midX, midY);

    // prepare perpendicular line
    double perpSlope = (-1.0)/(this->GetSlope());
    double perpIntercept = 0.0;
    if(isinf(perpSlope))
    {
        perpSlope = abs(perpSlope);
    }

    if(isinf(this->GetSlope()))
    {
        perpIntercept = this->GetMidPoint().GetY();
    }
    else if(D_EQUALS(this->GetSlope(), 0.0))
    {
        perpIntercept = this->GetMidPoint().GetX();
    }
    else
    {
        perpIntercept = this->GetMidPoint().GetY() - perpSlope*this->GetMidPoint().GetX();
    }

    this->perpendicular = Line(perpSlope,perpIntercept);
}

Point LineSegment::GetMidPoint(void) const {
    return this->midpt;
}

Line LineSegment::GetPerpendicular(void) const {
    return this->perpendicular;
}