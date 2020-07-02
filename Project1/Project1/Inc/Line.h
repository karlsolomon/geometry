#pragma once
#include "Point.h"
class Line
{
private:
	double m;
	double b;

public:
	Line();
	Line(double m, double b);
	Line(Point p1, Point p2);
	~Line();
		
	double GetSlope(void) const;
	double GetIntercept(void) const;

	// True if the value p lies on the line; false otherwise
	bool Contains(Point p);

	// True if the given line has the same slope, but different y-intercept; false otherwise
	bool Parallel(Line l);

	// Slope is major sort factor, intercept is minor sort factor
	bool operator<(const Line& l) const;
	bool operator==(const Line& l) const;
};

