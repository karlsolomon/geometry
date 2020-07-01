class LineSegment : public Line {
public:
    LineSegment(Point p1, Point p2) {
        line = Line(p1, p2);
        double midY = ((p2.getY() - p1.getY()) / 2) + p1.getY();
        double midX = ((p2.getX() - p1.getX()) / 2) + p1.getX();
        this.midpt = Point(midX, midY);
    }

    Point getMidPoint(void) {
    	return this.midpt;
    }
    Line getPerpendicular(void) {
        double m = (-1.0)/(this.getSlope());
        double b = this.getMidPoint.getY() - m*this.getMidPoint().getX();
        return new Line(m, b);
    }
private:
	Point midpt;
};