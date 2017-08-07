#pragma once
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <windows.h>

#define PI 3.14159265
#define Deg2Rad PI/180
#define GLbyte char
#define WIDTH 768
#define HEIGHT 768

class Point {
public:
	float x;
	float y;
	float z;
	Point() { }
	Point(Point& _Point) :
		x(_Point.x), y(_Point.y), z(_Point.z)
	{ }
	Point(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z)
	{ }
};

class Line {
public:
	Point Point1;
	Point Point2;
	Line() {}
	Line(Line& _Line) :
		Point1(_Line.Point1), Point2(_Line.Point2)
	{ }
	Line(Point P1, Point P2) :
		Point1(P1), Point2(P2)
	{ }
};

inline void dot(GLbyte* data, int x, int y, int value);

inline void swap(float &x, float &y);

void rotate(Point& _Point, float Mat[][3]);

void drawLine(GLbyte* data, const Line& _Line, int value);