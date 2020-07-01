namespace Geometry
{
    class Point
    {
    public:
		Point(double x = 0.0, double y = 0.0);

		double getX(void);
		double getY(void);
		bool equals(Point p);
	};
}