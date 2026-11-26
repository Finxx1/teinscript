#ifndef TEIN_MATH_H_
#define TEIN_MATH_H_

namespace glaiel {

struct Vec2D {
	double x;
	double y;
};

struct CollisionBoundingBox {
	int _padding_;
	glaiel::Vec2D offset;
	glaiel::Vec2D dimensions;
	glaiel::Vec2D* center;
};

}

#endif