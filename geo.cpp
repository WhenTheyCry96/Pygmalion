#include "geo.h"

void Point::extenPoint(float extension) {
	this->x = this->x * extension;
	this->y = this->y * extension;
	this->z = this->z * extension;

	return;
}

inline void dot(GLbyte* data, int x, int y, int value) {
	int _x = x + WIDTH / 2;
	int _y = y + HEIGHT / 2;

	if (_x >= 0 && _x < WIDTH && _y >= 0 && _y < HEIGHT) {
		data[_y * WIDTH + _x] = value;
	}
	return;
}

inline void swap(float &x, float &y) {
	float tmp = x;
	x = y;
	y = tmp;
	return;
}

void rotate(Point& _Point, float Mat[][3]) {
	float _x = _Point.x;
	float _y = _Point.y;
	float _z = _Point.z;

	_Point.x = Mat[0][0] * _x + Mat[0][1] * _y + Mat[0][2] * _z;
	_Point.y = Mat[1][0] * _x + Mat[1][1] * _y + Mat[1][2] * _z;
	_Point.z = Mat[2][0] * _x + Mat[2][1] * _y + Mat[2][2] * _z;

	return;
}

void drawLine(GLbyte* data, const Line& _Line, int value) {
	float x1 = _Line.Point1.x;
	float y1 = _Line.Point1.y;
	float x2 = _Line.Point2.x;
	float y2 = _Line.Point2.y;
	// y = ax + b
	if (abs(x1 - x2) > abs(y1 - y2)) {
		float a = (y2 - y1) / (x2 - x1);
		float b = -a*x1 + y1;
		if (x1 < x2) {
			for (; x1 <= x2; x1++) {
				dot(data, x1, a*x1 + b, value);
			}
		}
		else {
			for (; x2 <= x1; x2++) {
				dot(data, x2, a*x2 + b, value);
			}
		}
	}
	else {
		swap(x1, y1);
		swap(x2, y2);
		float a = (y2 - y1) / (x2 - x1);
		float b = -a*x1 + y1;
		if (x1 < x2) {
			for (; x1 <= x2; x1++) {
				dot(data, a*x1 + b, x1, value);
			}
		}
		else {
			for (; x2 <= x1; x2++) {
				dot(data, a*x2 + b, x2, value);
			}
		}
	}
	return;
}