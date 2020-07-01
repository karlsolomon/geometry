namespace Geometry
{
	class Line {
	public:
	    Line(double m, double b);
	    Line(Point p1, Point p2);
	    double getSlope(void);
	    double getIntercept(void);
	    bool contains(Point p);
	    bool isPerpendicular(Line l);
	    @Override bool equals(Line l);
	    @Override int hashCode();

	    // does not include equal lines
	    bool parallel(Line l);
	    bool operator<(const Line &l);
	    static bool comp(Line l1, Line l2);
	};
}