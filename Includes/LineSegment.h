namespace Geometry
{
    class LineSegment : public Line
    {
    public:
    	LineSegment(Point p1, Point p2);

	    Point getMidPoint(void);
	    Line getPerpendicular(void);
}