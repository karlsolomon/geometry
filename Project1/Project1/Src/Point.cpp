#include <cmath>
#include "Point.h"
#include "DoubleCompare.h"

double x;
double y;

Point::Point(void) {
	this->x = 0.0;
	this->y = 0.0;
}

Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}

Point::~Point()
{
	// do nothing
}

double Point::GetX(void) const {
	return this->x;
}

double Point::GetY(void) const {
	return this->y;
}

// XPosition is major sort factor, YPosition is minor sort factor
bool Point::operator<(const Point& l) const {
	bool lessThanP = false;
	if (D_EQUALS(this->GetX(), l.GetX()))
	{
		if (!D_EQUALS(this->GetY(), l.GetY()))
		{
			lessThanP = this->GetY() < l.GetY();
		}		
	}
	else
	{
		lessThanP = this->GetX() < l.GetX();
	}
	return lessThanP;
}

bool Point::operator==(const Point& l) const {
	bool left = ((*this) < l);
	bool right = (l < (*this));
	return !(left ^ right);
}