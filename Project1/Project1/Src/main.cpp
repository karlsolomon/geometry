#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <set>
#include <cstring>
#include <ctime>

#include "DoubleCompare.h"
#include "Line.h"
#include "LineSegment.h"
#include "Point.h"

using namespace std;

typedef enum
{
	SOURCE_HARD_CODE,
	SOURCE_HARD_CODE_UNIT_CIRCLE,
	SOURCE_FILE,
	SOURCE_RANDOM,
	SOURCE_COUNT
} PointSource_t;

#define POINT_SOURCE				SOURCE_HARD_CODE_UNIT_CIRCLE

typedef enum
{
	DEBUG_ERROR,
	DEBUG_INFO,
	DEBUG_FULL,
	DEBUG_COUNT
} DebugVerbosity_t;

// Logging Enable/Disable and printing
#define DEBUG_LVL					DEBUG_INFO
#define PRINT(lvl, ...)				do { \
										if((lvl) <= DEBUG_LVL) { \
											printf(__VA_ARGS__); \
											printf("\n\r"); \
										} \
									} while(0)
#define PRINT_ERROR(...)			PRINT(DEBUG_ERROR, __VA_ARGS__)
#define PRINT_INFO(...)				PRINT(DEBUG_INFO, __VA_ARGS__)
#define PRINT_DEBUG(...)			PRINT(DEBUG_FULL, __VA_ARGS__)

// Random Point Generator Parameters
#define NUM_RANDOM_POINTS			((uint32_t)9)
#define ALLOW_NEGATIVE_RANDOM_POINT	true
#define RANDOM_POINT_BOUND			((int) 2)

// Hard-code unit circle configs - feel free to change these to whatever you'd like to get more tests.
// If you make radius small enough (or num points large enough) you'll eventually hit the rounding error built into the double math.
#define UNIT_CIRCLE_NUM_POINTS			((uint16_t)201)	// number of points to evently distribute about a unit circle. Creates equilateral polygons w/ this number of vertices
#define UNIT_CIRCLE_ROTATION_RADIANS	(1.1)	// number of radians to rotate/advance each point in the unit circle. This way equilateral polygon isn't always "facing up".
#define UNIT_CIRCLE_X_SHIFT				(3.1)	// units to dc-shift the unit circle along x-axis
#define UNIT_CIRCLE_Y_SHIFT				(2)		// units to dc-shift the unit circle along x-axis
#define UNIT_CIRCLE_RADIUS				(1.01)

// Function Prototypes
static void getPotentialLinesOfSymmetry(set<Point> points, set<Line>* symmetryLines);
static void getLinesOfSymmetryOnWholePointSet(set<Point> points, set<Line>* symmetryLines);
static void initHardCodedPoints(set<Point>* points);
static void initUnitCirclePoints(set<Point>* points);
static void initFilePoints(set<Point>* points);
static void initRandomPoints(set<Point>* points);
static double formatIntToDoubleWithBounds(int val);
bool isPointValid(char* pointStr);

int main(void) {
	set<Point> points;
	switch (POINT_SOURCE)
	{
	case SOURCE_HARD_CODE:
		initHardCodedPoints(&points);
		break;
	case SOURCE_HARD_CODE_UNIT_CIRCLE:
		initUnitCirclePoints(&points);
		break;
	case SOURCE_FILE:
		initFilePoints(&points);
		break;
	case SOURCE_RANDOM:
		initRandomPoints(&points);
		break;
	default:
		PRINT_ERROR("ERROR! Must use a valid source for points. See PointSource_t for possible options.");
		return -1;
	}

	set<Line> symmetryLines;
	getPotentialLinesOfSymmetry(points, &symmetryLines);
	getLinesOfSymmetryOnWholePointSet(points, &symmetryLines);

	 PRINT_INFO("");
	 PRINT_INFO("Lines of Symmetry Across All Points (nPoints = %i, nLines = %i):", points.size(), symmetryLines.size());
	 for (std::set<Line>::iterator i = symmetryLines.begin(); i != symmetryLines.end(); i++)
	 {
		 if (isinf(i->GetSlope()))
		 {
			 PRINT_INFO("\tx = %g", i->GetIntercept());
		 }
		 else if (D_EQUALS(i->GetSlope(), 0.0))
		 {
			 PRINT_INFO("\ty = %g", i->GetIntercept());
		 }
		 else
		 {
			 PRINT_INFO("\ty = %gx + %g", i->GetSlope(), i->GetIntercept());
		 }
	 }
	return 0;
}

// Given a set of points get a set of all the lines of symmetry
static void getPotentialLinesOfSymmetry(set<Point> points, set<Line>* symmetryLines)
{
	// create a line segment for each unique pair of points
	vector<LineSegment> segments;
	set<Point>::iterator it1;
	set<Point>::iterator it2;
	for (it1 = points.begin(); it1 != points.end(); it1++) {
		for (it2 = it1; it2 != points.end(); it2++) {
			if (it2 != it1) {
				segments.push_back(LineSegment((*it1), (*it2)));
			}
		}
	}

	// find lines of symmetry
	bool lineOfSymmetryFound;
	for (uint32_t i = 0; i < segments.size(); i++) {
		lineOfSymmetryFound = false;

		// Determine if a symmetric triangle exists
		for (set<Point>::iterator p = points.begin(); p != points.end(); p++) {
			// it is a line of symmetry if there is a point on this line which
			// is not on the originating line segment (because that would be another point on
			// the original line, not a polygon)
			if (segments[i].GetPerpendicular().Contains((*p)) && !segments[i].Contains((*p))) {
				PRINT_DEBUG("symmetric triangle: segment %i + point (%lf,%lf)", i, p->GetX(), p->GetY());
				lineOfSymmetryFound = true;
				break;
			}
		}

		// If there was no symmetric triangle search for a symmetric quadralateral (either rhombus or rectangle)
		if (!lineOfSymmetryFound) {
			for (uint32_t j = i + 1; j < segments.size(); j++) {
				bool Parallel = segments[i].Parallel(segments[j]);
				bool sameBisect = segments[i].GetPerpendicular().Contains(segments[j].GetMidPoint());
				bool coLinear = segments[i].Contains(segments[j].GetMidPoint());
				if (Parallel && sameBisect && !coLinear) {
					PRINT_DEBUG("symmetric quadralateral: segment %i + segment %i", i, j);
					lineOfSymmetryFound = true;
					break;
				}
			}
		}
		if (lineOfSymmetryFound) {
			symmetryLines->insert(segments[i].GetPerpendicular());
		}
	}
}

// Given a set of points and potential lines of symmetry, remove any lines which do not apply to all points
// in the given set.
static void getLinesOfSymmetryOnWholePointSet(set<Point> points, set<Line>* symmetryLines)
{
	// create a line segment for each unique pair of points
	set<Line>::iterator it1 = symmetryLines->begin();
	set<Point>::iterator it2;
	Point reflected;
	bool symmetricForWholeSetOfPoints = true;
	while(it1 != symmetryLines->end()) {
		symmetricForWholeSetOfPoints = true;
		for (it2 = points.begin(); it2 != points.end(); it2++) {
			if (!it1->Contains((Point) *it2)) {
				if (isinf(it1->GetSlope()))
				{
					// reflect x component only
					reflected = Point(2*it1->GetIntercept() - it2->GetX(), it2->GetY());
				}
				else if (D_EQUALS(it1->GetSlope(), 0.0))
				{
					// reflect y component only
					reflected = Point(it2->GetX(), 2*it1->GetIntercept() - it2->GetY());
				}
				else
				{
					// reflect x and y components

					// Get line perpendicular to the potential line of symmetry which intersects point it2
					Line perpLineWithPoint = Line(-1.0 / it1->GetSlope(), it2->GetY() + (it2->GetX() / it1->GetSlope()));

					// Find point where perpLineWithPoint intersects the potential line of symmetry
					double xIntersect = (perpLineWithPoint.GetIntercept() - it1->GetIntercept()) / (it1->GetSlope() - perpLineWithPoint.GetSlope());
					
					// Get it2's reflection across the potential line of symmetry
					double reflectX = (2 * xIntersect) - it2->GetX();
					reflected = Point(reflectX, perpLineWithPoint.GetSlope()*reflectX + perpLineWithPoint.GetIntercept());
				}

				if (points.find((const Point)reflected) == points.end())
				{
					// If it2's reflected point across potential line of symmetry is not found in the set of points
					// then we know that this line of symmetry is not valid for this point.
					PRINT_DEBUG("reflected point = (%lf, %lf)", reflected.GetX(), reflected.GetY());
					symmetricForWholeSetOfPoints = false;
					break;
				}
			}
		}
		if (symmetricForWholeSetOfPoints)
		{
			PRINT_DEBUG("line (%lf, %lf) remains", it1->GetSlope(), it1->GetIntercept());
			it1++;
		}
		else
		{
			PRINT_DEBUG("line (%lf, %lf) removed from sym lines due to point (%lf,%lf)", it1->GetSlope(), it1->GetIntercept(), it2->GetX(), it2->GetY());
			it1 = symmetryLines->erase(it1);
		}
	}
}

// Use hard-coded points as shown in the example to find lines of symmetry between
static void initHardCodedPoints(set<Point>* points)
{
	points->insert(Point(0.0, 0.0));
	points->insert(Point(0.0, 1.0));
	points->insert(Point(1.0, 0.0));
	points->insert(Point(-1.0, 0.0));
	points->insert(Point(0.0, -1.0));
	points->insert(Point(-1.0, -1.0));
	points->insert(Point(1.0, 1.0));
}

// Use hard-coded points from unit circle
static void initUnitCirclePoints(set<Point>* points)
{
	double pi = 2.0*acos(0.0);
	double x;
	double y;
	// add all points of unit circle with UNIT_CIRCLE_NUM_POINTS separation
	for (int i = 0; i < UNIT_CIRCLE_NUM_POINTS; i++)
	{
		x = UNIT_CIRCLE_X_SHIFT + (UNIT_CIRCLE_RADIUS * cos(UNIT_CIRCLE_ROTATION_RADIANS + (i * pi) / (UNIT_CIRCLE_NUM_POINTS / 2.0)));
		y = UNIT_CIRCLE_Y_SHIFT + (UNIT_CIRCLE_RADIUS * sin(UNIT_CIRCLE_ROTATION_RADIANS + (i * pi) / (UNIT_CIRCLE_NUM_POINTS / 2.0)));
		points->insert(Point(x, y));
	}
}

// Use a csv file to extract points from. 
static void initFilePoints(set<Point>* points)
{
#define MAX_NUM_CHAR_IN_UINT32_T (10)
	char point[2*MAX_NUM_CHAR_IN_UINT32_T + 4] = { 0 }; // add 3 char for '\n' '\r' ',' and NULL
	char* xPos;
	char* yPos;
	ifstream pointFile;
	pointFile.open("Inc/AllPoints.csv", ios::in);
	int x = 0;
	int y = 0;
	int lineCnt = -1;
	bool validPoint;

	while (pointFile.getline(point, sizeof(point) - 1)) {
		lineCnt++;
		validPoint = true;
		xPos = strtok(point, ", ");
		if (isPointValid(xPos))
		{
			x = atoi(xPos);
		}
		else
		{
			PRINT_DEBUG("On line %d, invalid x-point %s", lineCnt, xPos);
			validPoint = false;
		}
		yPos = strtok(NULL, ", ");
		if (isPointValid(yPos))
		{
			y = atoi(yPos);
		}
		else
		{
			PRINT_DEBUG("On line %d, invalid y-point %s", lineCnt, yPos);
			validPoint = false;
		}
		if (validPoint)
		{
			points->insert(Point((double)x, (double)y));
		}
	}
	pointFile.close();
}

bool isPointValid(char* pointStr)
{
	#define MIN_NUMERIC_CHAR_VALUE '0'
	#define MAX_NUMERIC_CHAR_VALUE '9'
	#define NEGATIVE_CHAR		   '-'
	bool valid = true;
	if (pointStr != NULL)
	{
		for (uint32_t i = 0; i < strlen(pointStr); i++)
		{
			if ((pointStr[i] < MIN_NUMERIC_CHAR_VALUE) ||
				(pointStr[i] > MAX_NUMERIC_CHAR_VALUE))
			{
				if ((pointStr[i] == NEGATIVE_CHAR) && (i == 0))
				{
					// leading negative char is OK
					continue;
				}
				else if ((pointStr[i] == '\n' ) || (pointStr[i] == '\r') || (pointStr[i] == '\0'))
				{
					// received end of line/end of file character
					break;
				}
				else
				{
					// invalid character; skip this line
					PRINT_DEBUG("Invalid point %s", pointStr);
					valid = false;
					break;
				}
			}
		}
	}
	else
	{
		PRINT_DEBUG("Invalid point was NULL");
		valid = false;
	}
	return valid;
}


// Use a random number generator (with some parameters) to create points
// See top of file for 
static void initRandomPoints(set<Point>* points)
{
	int p1;
	int p2;
	srand(time(NULL));
	while(points->size() < NUM_RANDOM_POINTS)
	{
		p1 = rand();
		p2 = rand();
		points->insert(Point(formatIntToDoubleWithBounds(p1), formatIntToDoubleWithBounds(p2)));
	}
}

// Given a random integer keep it within the configured "bounds" and randomize positive/negative
// since rand() function just generates non-negative numbers
static double formatIntToDoubleWithBounds(int val)
{
	if (val > RANDOM_POINT_BOUND)
	{
		val %= RANDOM_POINT_BOUND;
	}
	if (ALLOW_NEGATIVE_RANDOM_POINT) {
		// make half of the random numbers negative
		if ((rand() % 2) == 1)
		{
			val *= -1;
		}
	}
	return (double)val;
}