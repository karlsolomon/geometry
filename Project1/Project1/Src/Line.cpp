#include <cmath>
#include <cstdlib>

#include "DoubleCompare.h"
#include "Line.h"

double m;
double b;

Line::Line(double m, double b) {
    this->m = m;
    this->b = b;
    if(isinf(this->m) || (D_EQUALS(this->m, -0.0)))
    {
        this->m = abs(this->m);
    }

    // round nearly zero slope/intercepts to 0.0
    if(D_EQUALS(this->m, 0.0))
    {
        this->m = 0.0;
    }
    if(D_EQUALS(this->b, 0.0))
    {
        this->b = 0.0;
    }
}

Line::Line(Point p1, Point p2) {
	double rise = (p2.GetY() - p1.GetY());
    double run = (p2.GetX() - p1.GetX());

	if (D_EQUALS(rise, 0.0) && !D_EQUALS(run, 0.0))
	{
		this->m = 0.0;
	}
	else if (D_EQUALS(run, 0.0) && !D_EQUALS(rise, 0.0))
	{
		this->m = HUGE_VAL;
	}
	else
	{
		this->m = rise / run;
	}

	double mul = 0.0;
	D_MUL(this->m, p1.GetX(), &mul);
	this->b = p1.GetY() - mul;

    if(isinf(this->m))
    {
        // if the slope is infinite store the root, not the y-intercept
        this->b = p1.GetX();
    }
}

Line::Line(void)
{
	this->m = 0.0;
	this->b = 0.0;
}

Line::~Line()
{
	// do nothing
}

double Line::GetSlope(void) const {
    return this->m;
}

double Line::GetIntercept(void) const {
    return this->b;
}

// True if the value p lies on the line; false otherwise
bool Line::Contains(const Point p) const {
    bool Contains = D_EQUALS(p.GetY() - (this->m)*p.GetX(), this->GetIntercept());
    if(isinf(this->m) && D_EQUALS(p.GetX(), this->b))
    {
		Contains = true;
    }
    return Contains;
}
    
// True if the given line has the same slope, but different y-intercept; false otherwise
bool Line::Parallel(Line l) const {
    bool Parallel = false;
    Parallel = D_EQUALS(l.GetSlope(),this->GetSlope());
    return Parallel;
}

// Slope is major sort factor, intercept is minor sort factor
bool Line::operator<(const Line& l) const {
	bool equal = D_EQUALS(this->m, l.m) && D_EQUALS(this->b, l.b);
	bool lessThanL = false;
	if (!equal)
	{
		if (D_EQUALS(this->m,l.m))
		{
			lessThanL = this->b < l.b;
		}
		else
		{
			lessThanL = this->m < l.m;
		}
	}
	return lessThanL;
}

bool Line::operator==(const Line& l) const {
	bool equal = D_EQUALS(this->m, l.m) && D_EQUALS(this->b, l.b);
	return equal;
}

