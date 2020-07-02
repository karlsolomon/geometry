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
	SOURCE_FILE,
	SOURCE_RANDOM,
	SOURCE_COUNT
} PointSource_t;

#define POINT_SOURCE				SOURCE_FILE

typedef enum
{
	DEBUG_ERROR,
	DEBUG_INFO,
	DEBUG_FULL,
	DEBUG_COUNT
} DebugVerbosity_t;

// Logging Enable/Disable and printing
#define DEBUG_LVL					DEBUG_INFO
#define PRINT_DEBUG(...)			if((DEBUG_LVL) >= DEBUG_FULL) printf(__VA_ARGS__)
#define PRINT_INFO(...)				if((DEBUG_LVL) >= DEBUG_INFO) printf(__VA_ARGS__)
#define PRINT_ERROR(...)			if((DEBUG_LVL) >= DEBUG_ERROR) printf(__VA_ARGS__)

// Random Point Generator Parameters
#define NUM_RANDOM_POINTS			((uint32_t)16)
#define ALLOW_NEGATIVE_RANDOM_POINT	false
#define RANDOM_POINT_BOUND			((int) 4)

// Function Prototypes
static void getLinesOfSymmetry(set<Point> points, set<Line>* symmetryLines);
static void initHardCodedPoints(set<Point>* points);
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
	getLinesOfSymmetry(points, &symmetryLines);

	 PRINT_INFO("");
	 PRINT_INFO("Lines of Symmetry:");
	 PRINT_INFO("\tLines are printed in terms of y = mx+b.\n");
	 PRINT_INFO("\tAny line which is not a function (undefined infinite slope) will print the slope as \"inf\" and the root (x-intercept) as 'b'.\n\n");
	 for (std::set<Line>::iterator i = symmetryLines.begin(); i != symmetryLines.end(); i++)
	 {
		 PRINT_INFO("\ty = %lfx + %lf\n", i->GetSlope(), i->GetIntercept());
	 }

	return 0;
}

// Given a set of points get a set of all the lines of symmetry
static void getLinesOfSymmetry(set<Point> points, set<Line>* symmetryLines)
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
				PRINT_DEBUG("symmetric triangle: segment %i + point (%lf,%lf)\n", i, p->GetX(), p->GetY());
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
					PRINT_DEBUG("symmetric quadralateral: segment %i + segment %i\n", i, j);
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
			PRINT_ERROR("On line %d, invalid x-point %s", lineCnt, xPos);
			validPoint = false;
		}
		yPos = strtok(NULL, ", ");
		if (isPointValid(yPos))
		{
			y = atoi(yPos);
		}
		else
		{
			PRINT_ERROR("On line %d, invalid y-point %s", lineCnt, yPos);
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
				else
				{
					// invalid character; skip this line
					PRINT_ERROR("Invalid point %s", pointStr);
					valid = false;
					break;
				}
			}
		}
	}
	else
	{
		PRINT_ERROR("Invalid point was NULL");
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