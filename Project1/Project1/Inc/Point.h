#pragma once

class Point
{
private:
	double x;
	double y;

public:
	Point(void);
	Point(double x, double y);
	~Point();

	double GetX(void) const;
	double GetY(void) const;

	// XPosition is major sort factor, YPosition is minor sort factor
	bool operator<(const Point& l) const;
	bool operator==(const Point& l) const;
};

