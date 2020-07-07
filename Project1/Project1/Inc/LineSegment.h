#pragma once
#include "Line.h"
class LineSegment : public Line
{
private:
	Point midpt;
	Point p1;
	Point p2;
	Line perpendicular;

public:
	LineSegment(Point p1, Point p2);
	~LineSegment();
	Point GetMidPoint(void) const;
	Line GetPerpendicular(void) const;
};

