#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <vector>
using namespace std;
// using namespace Geometry;
// #include "Point.h"
// #include "Line.h"
// #include "LineSegment.h"

static bool m_logEnable = false;

#define PRINT(...) if(m_logEnable) printf(__VA_ARGS__)
#define LOG_ENABLE() m_logEnable = true
#define LOG_DISABLE() m_logEnable = false

static bool fequals(double a, double b)
{
    bool floatEqual = false;
    if(isinf(a) && isinf(b))
    {
        floatEqual = true;
    }
    else
    {
        floatEqual = abs(a-b) < 0.0000001;
    }
	return floatEqual;
}

class Point {
public:
	Point(double x = 0.0, double y = 0.0) {
		this->x = x;
		this->y = y;
	}

	double getX(void) {
		return this->x;
	}
	double getY(void) {
		return this->y;
	}
	bool equals(Point p) {
		return (fequals(p.getY(),this->getY())) && (fequals(p.getX(),this->getX()));
	}

private:
	double x;
	double y;
};

static const uint32_t crcTable[256] =
{
    0x00000000, 0x13A29877, 0x274530EE, 0x34E7A899, 0x4E8A61DC, 0x5D28F9AB, 0x69CF5132, 0x7A6DC945,
    0x9D14C3B8, 0x8EB65BCF, 0xBA51F356, 0xA9F36B21, 0xD39EA264, 0xC03C3A13, 0xF4DB928A, 0xE7790AFD,
    0x3FC5F181, 0x2C6769F6, 0x1880C16F, 0x0B225918, 0x714F905D, 0x62ED082A, 0x560AA0B3, 0x45A838C4,
    0xA2D13239, 0xB173AA4E, 0x859402D7, 0x96369AA0, 0xEC5B53E5, 0xFFF9CB92, 0xCB1E630B, 0xD8BCFB7C,
    0x7F8BE302, 0x6C297B75, 0x58CED3EC, 0x4B6C4B9B, 0x310182DE, 0x22A31AA9, 0x1644B230, 0x05E62A47,
    0xE29F20BA, 0xF13DB8CD, 0xC5DA1054, 0xD6788823, 0xAC154166, 0xBFB7D911, 0x8B507188, 0x98F2E9FF,
    0x404E1283, 0x53EC8AF4, 0x670B226D, 0x74A9BA1A, 0x0EC4735F, 0x1D66EB28, 0x298143B1, 0x3A23DBC6,
    0xDD5AD13B, 0xCEF8494C, 0xFA1FE1D5, 0xE9BD79A2, 0x93D0B0E7, 0x80722890, 0xB4958009, 0xA737187E,
    0xFF17C604, 0xECB55E73, 0xD852F6EA, 0xCBF06E9D, 0xB19DA7D8, 0xA23F3FAF, 0x96D89736, 0x857A0F41,
    0x620305BC, 0x71A19DCB, 0x45463552, 0x56E4AD25, 0x2C896460, 0x3F2BFC17, 0x0BCC548E, 0x186ECCF9,
    0xC0D23785, 0xD370AFF2, 0xE797076B, 0xF4359F1C, 0x8E585659, 0x9DFACE2E, 0xA91D66B7, 0xBABFFEC0,
    0x5DC6F43D, 0x4E646C4A, 0x7A83C4D3, 0x69215CA4, 0x134C95E1, 0x00EE0D96, 0x3409A50F, 0x27AB3D78,
    0x809C2506, 0x933EBD71, 0xA7D915E8, 0xB47B8D9F, 0xCE1644DA, 0xDDB4DCAD, 0xE9537434, 0xFAF1EC43,
    0x1D88E6BE, 0x0E2A7EC9, 0x3ACDD650, 0x296F4E27, 0x53028762, 0x40A01F15, 0x7447B78C, 0x67E52FFB,
    0xBF59D487, 0xACFB4CF0, 0x981CE469, 0x8BBE7C1E, 0xF1D3B55B, 0xE2712D2C, 0xD69685B5, 0xC5341DC2,
    0x224D173F, 0x31EF8F48, 0x050827D1, 0x16AABFA6, 0x6CC776E3, 0x7F65EE94, 0x4B82460D, 0x5820DE7A,
    0xFBC3FAF9, 0xE861628E, 0xDC86CA17, 0xCF245260, 0xB5499B25, 0xA6EB0352, 0x920CABCB, 0x81AE33BC,
    0x66D73941, 0x7575A136, 0x419209AF, 0x523091D8, 0x285D589D, 0x3BFFC0EA, 0x0F186873, 0x1CBAF004,
    0xC4060B78, 0xD7A4930F, 0xE3433B96, 0xF0E1A3E1, 0x8A8C6AA4, 0x992EF2D3, 0xADC95A4A, 0xBE6BC23D,
    0x5912C8C0, 0x4AB050B7, 0x7E57F82E, 0x6DF56059, 0x1798A91C, 0x043A316B, 0x30DD99F2, 0x237F0185,
    0x844819FB, 0x97EA818C, 0xA30D2915, 0xB0AFB162, 0xCAC27827, 0xD960E050, 0xED8748C9, 0xFE25D0BE,
    0x195CDA43, 0x0AFE4234, 0x3E19EAAD, 0x2DBB72DA, 0x57D6BB9F, 0x447423E8, 0x70938B71, 0x63311306,
    0xBB8DE87A, 0xA82F700D, 0x9CC8D894, 0x8F6A40E3, 0xF50789A6, 0xE6A511D1, 0xD242B948, 0xC1E0213F,
    0x26992BC2, 0x353BB3B5, 0x01DC1B2C, 0x127E835B, 0x68134A1E, 0x7BB1D269, 0x4F567AF0, 0x5CF4E287,
    0x04D43CFD, 0x1776A48A, 0x23910C13, 0x30339464, 0x4A5E5D21, 0x59FCC556, 0x6D1B6DCF, 0x7EB9F5B8,
    0x99C0FF45, 0x8A626732, 0xBE85CFAB, 0xAD2757DC, 0xD74A9E99, 0xC4E806EE, 0xF00FAE77, 0xE3AD3600,
    0x3B11CD7C, 0x28B3550B, 0x1C54FD92, 0x0FF665E5, 0x759BACA0, 0x663934D7, 0x52DE9C4E, 0x417C0439,
    0xA6050EC4, 0xB5A796B3, 0x81403E2A, 0x92E2A65D, 0xE88F6F18, 0xFB2DF76F, 0xCFCA5FF6, 0xDC68C781,
    0x7B5FDFFF, 0x68FD4788, 0x5C1AEF11, 0x4FB87766, 0x35D5BE23, 0x26772654, 0x12908ECD, 0x013216BA,
    0xE64B1C47, 0xF5E98430, 0xC10E2CA9, 0xD2ACB4DE, 0xA8C17D9B, 0xBB63E5EC, 0x8F844D75, 0x9C26D502,
    0x449A2E7E, 0x5738B609, 0x63DF1E90, 0x707D86E7, 0x0A104FA2, 0x19B2D7D5, 0x2D557F4C, 0x3EF7E73B,
    0xD98EEDC6, 0xCA2C75B1, 0xFECBDD28, 0xED69455F, 0x97048C1A, 0x84A6146D, 0xB041BCF4, 0xA3E32483
};
class Line {
	private:
    double m;
    double b;
    int hash;
    static void doubleToBytes(double d, uint8_t* bytes) {
        uint8_t* doubleBytes = (uint8_t*) &d;
        for(uint8_t i = 0; i < sizeof(double); i++)
        {
            bytes[i] = doubleBytes[i];
        }
    }

    static void crc32(uint32_t* crc, uint8_t const* bytes, uint32_t size)
    {
        for(uint32_t i = 0; i < size; i++)
        {
            *crc ^= crcTable[bytes[i]]; 
        }
    }


public:
    Line(double m = 0.0, double b = 0.0) {
        this->m = m;
        this->b = b;
        if(isinf(this->m) || (fequals(this->m, -0.0)))
        {
        	this->m = abs(this->m);
        }
        uint8_t mBytes[8] = {0};
        uint8_t bBytes[8] = {0};
        doubleToBytes(this->m, mBytes);
        doubleToBytes(this->b, bBytes);

        this->hash = 0;
        crc32((uint32_t*) &(this->hash), mBytes, sizeof(mBytes));
        crc32((uint32_t*) &(this->hash), bBytes, sizeof(bBytes));
    }

    Line(Point p1, Point p2) {
        this->m = (p2.getY() - p1.getY()) / (p2.getX() - p1.getX());
        if(isinf(this->m) || (fequals(this->m, -0.0)))
        {
        	this->m = abs(this->m);
        }
        this->b = p1.getY() - this->m*p1.getX();

        if(isinf(this->m))
        {
        	// if the slope is infinite store the root, not the y-intercept
        	this->b = p1.getX();
        }
    }

    double getSlope(void) {
        return this->m;
    }
    double getIntercept(void) {
        return this->b;
    }
    bool contains(Point p) {
    	bool contains = fequals(p.getY() - (this->m)*p.getX(), this->getIntercept());
    	if(isinf(this->m) && fequals(p.getX(), this->b))
    	{
			contains = true;
    	}
    	return contains;
    }
    bool isPerpendicular(Line l) {
    	bool perpendicular = false;
    	if((isinf(l.getSlope()) && fequals(this->getSlope(), 0.0)) || 
    	  (isinf(this->getSlope()) && fequals(l.getSlope(),0.0)))
    	{
    		perpendicular = true;
    	}
    	else
    	{
    		perpendicular = fequals(this->getSlope(),((-1.0) / l.getSlope()));
    	}
        return perpendicular;
    }

    bool equals(Line l) {
        return (fequals(l.getSlope(), this->getSlope())) && (fequals(l.getIntercept(), this->getIntercept()));
    }

    int hashCode(){
        return this->hash;
    }

    // does not include equal lines
    bool parallel(Line l) {
        bool parallel = false;
        parallel = fequals(l.getSlope(),this->getSlope());
        return parallel;
    }

    bool operator<(Line &l) {
        bool thisSmaller = false;
        if(fequals(l.getSlope(), this->getSlope()))
        {
            thisSmaller = this->getIntercept() < l.getIntercept();
        }
        else
        {
            thisSmaller = this->getSlope() < l.getSlope();
        }
        return thisSmaller;
    }

    static bool comp(Line l1, Line l2) {
        return l1 < l2;
    }
}; 

class LineSegment : public Line {
public:
    LineSegment(Point p1, Point p2) : Line(p1, p2) {
        double midY = ((p2.getY() - p1.getY()) / 2) + p1.getY();
        double midX = ((p2.getX() - p1.getX()) / 2) + p1.getX();
        this->midpt = Point(midX, midY);

        // prepare perpendicular line
        double perpSlope = (-1.0)/(this->getSlope());
        double perpIntercept = 0.0;
        if(isinf(perpSlope))
        {
            perpSlope = abs(perpSlope);
        }

        if(isinf(this->getSlope()))
        {
            perpIntercept = this->getMidPoint().getY();
        }
        else if(fequals(this->getSlope(), 0.0))
        {
            perpIntercept = this->getMidPoint().getX();
        }
        else
        {
            perpIntercept = this->getMidPoint().getY() - perpSlope*this->getMidPoint().getX();
        }

        this->perpendicular = Line(perpSlope,perpIntercept);
    }

    Point getMidPoint(void) {
    	return this->midpt;
    }
    Line getPerpendicular(void) {
        return this->perpendicular;
    }
private:
	Point midpt;
	Line perpendicular;
    bool perpInit;
};

int main(void) {
	// parse point list from a file
	vector<Point> points;
	points.push_back(Point(0.0, 0.0));
	points.push_back(Point(0.0, 1.0));
	points.push_back(Point(1.0, 0.0));
	points.push_back(Point(-1.0, 0.0));
	points.push_back(Point(0.0, -1.0));
	points.push_back(Point(-1.0, -1.0));

	// create line segment for all point pairs
	vector<LineSegment> segments;
	for(long i = 0; i < points.size(); i++) {
		for(long j = i+1; j < points.size(); j++) {
			segments.push_back(LineSegment(points[i], points[j]));
		}
	}

	// // for each line segment
	vector<Line> perpendicularLines;
	for(int i = 0; i < segments.size(); i++) {
		perpendicularLines.push_back(segments[i].getPerpendicular());

        PRINT("seg (%lf,%lf)\n", segments[i].getSlope(), segments[i].getIntercept());
        PRINT("\tmidpt: (%lf, %lf)\n", segments[i].getMidPoint().getX(), segments[i].getMidPoint().getY());
        PRINT("\tperp = (%lf, %lf)\n", perpendicularLines[i].getSlope(), perpendicularLines[i].getIntercept());
	}

	bool los;
	vector<Line> symmetryLines;
	for(int i = 0; i < segments.size(); i++) {
		los = false;
		// PRINT("\n i = %i\n", i);
		// PRINT("perp = (%lf, %lf); midpt = (%lf, %lf)\n", perpendicularLines[i].getSlope(), perpendicularLines[i].getIntercept(), segments[i].getMidPoint().getX(), segments[i].getMidPoint().getY());
		for(int j = 0; j < points.size(); j++) {
			// It is a line of symmetry if there is a point on this line which
			// is not on the originating line segment (because that would be another point on
			// the original line, not a polygon)
			// PRINT("\n\tp = (%lf,%lf)\n", points[j].getX(), points[j].getY());
			if(perpendicularLines[i].contains(points[j]) && !segments[i].contains(points[j])) {
				LOG_ENABLE();
                PRINT("\n\t\tnew sym1 m = %lf, b = %lf\n", perpendicularLines[i].getSlope(), perpendicularLines[i].getIntercept());
				los = true;
                LOG_DISABLE();
				break;
			}
		}
		// If I sort the segments this'll be faster to find.
		// This part requires that segment is a Set, not a Vector
		if(!los) {
			for(int j = i+1; j < segments.size(); j++) {
				// PRINT("\n\ts = (%lf,%lf); midpt = (%lf, %lf)\n", segments[j].getSlope(), segments[j].getIntercept(), segments[j].getMidPoint().getX(), segments[j].getMidPoint().getY());
				bool a = segments[i].parallel(segments[j]);
                bool b = perpendicularLines[i].contains(segments[j].getMidPoint());
                bool c = !segments[i].contains(segments[j].getMidPoint());
                // PRINT("\t\tres: %d%d%d\n", a, b, c);
                if(a && b && c) {
                    LOG_ENABLE();
					PRINT("\n\t\tnew sym2 m = %lf, b = %lf\n", perpendicularLines[i].getSlope(), perpendicularLines[i].getIntercept());
					los = true;
                    LOG_DISABLE();
					break;
				}
			}
		}
		if(los) {
			symmetryLines.push_back(perpendicularLines[i]);
			// PRINT("\nnew sym m = %lf, b = %lf\n", perpendicularLines[i].getSlope(), perpendicularLines[i].getIntercept());
		}
	}
	return 0;
}


