using namespace Geometry;

class Point {
public:
	Point(double x = 0.0, double y = 0.0) {
		this.x = x;
		this.y = y;
	}

	double getX(void) {
		return this.x;
	}
	double getY(void) {
		return this.y;
	}
	bool equals(Point p) {
		return (p.getY() == this.getY()) && (p.getX() == this.getX());
	}

private:
	double x;
	double y;
};