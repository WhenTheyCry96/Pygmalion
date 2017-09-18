#pragma once
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <string.h>
#include <cstring>
#include <cassert>
#include <WinUser.h>
#include <chrono>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#define geo_PI 3.14159265
#define Deg2Rad geo_PI/180
#define GLbyte char
#define WIDTH 768
#define HEIGHT 768

namespace geo {
	class Point {
	public:
		double x;
		double y;
		double z;
		Point() { }
		Point(const Point& _Point) :
			x(_Point.x), y(_Point.y), z(_Point.z)
		{ }
		Point(double _x, double _y, double _z) :
			x(_x), y(_y), z(_z)
		{ }
		void extenPoint(double extension);
	};

	class Line {
	public:
		Point Point1;
		Point Point2;
		Line() {}
		Line(Line& _Line) :
			Point1(_Line.Point1), Point2(_Line.Point2)
		{ }
		Line(const Point& P1, const Point& P2) :
			Point1(P1), Point2(P2)
		{ }
	};

	class Triangle {
	public:
		Point P1;
		Point P2;
		Point P3;
		Triangle(){ }
		Triangle(Point& _P1, Point& _P2, Point& _P3):
			P1(_P1), P2(_P2), P3(_P3)
		{ }
		Triangle(Triangle& T):
			P1(T.P1), P2(T.P2), P3(T.P3)
		{ }
	};

	class Obj {
	public:
		geo::Triangle* T;
		geo::Obj* next;
		geo::Point center;
		int num;

		Obj() :
			T(NULL), num(0), next(NULL)
		{ }

		void setCenter();
	};

	class ObjList {
	public:
		geo::Obj* head;
		geo::Obj* tail;
		ObjList() :
			head(NULL), tail(NULL)
		{ }
		void add(geo::Obj* obj);
		void drawObjListMat(cv::Mat& img, int value);
		void drawObjList24bitMat(cv::Mat& img, int value, int base);
		void rotateObjList(double Mat[][3]);
		void rotateinPlaceObjList(double Mat[][3]);
	};

	class Hand {
	public:
		geo::Point fingertip[5];
		geo::Point proximal[5];
		geo::Point wrist;
		Hand() {}
		Hand(geo::Hand& _H) {
			for (int i = 0; i < 5; i++) {
				this->fingertip[i] = _H.fingertip[i];
				this->proximal[i] = _H.proximal[i];
			}
			this->wrist = _H.wrist;
		}
		void rotate(double Mat[][3]);
	};

	inline void dot(GLbyte* data, int x, int y, int value);
	void dotMat(cv::Mat& img, int x, int y, int value);
	void dot24bitMat(cv::Mat& img, int x, int y, int value, int base);

	inline void swap(double &x, double &y);

	void rotate(Point& _Point, double Mat[][3]);
	void rotateObj(geo::Obj& obj, double Mat[][3]);

	void transformPoint(Point& _Point, double Mat[]);
	void transformTriangle(geo::Triangle& _T, double Mat[]);
	void transformObj(geo::Obj& obj, double Mat[]);


	void drawLine(GLbyte* data, const Line& _Line, int value);
	void drawLineMat(cv::Mat& img, const geo::Line& _Line, int value);
	void drawLine24bitMat(cv::Mat& img, const geo::Line& _Line, int value, int base);
	void drawTriangleMat(cv::Mat& img, const geo::Triangle& T, int value);
	void drawTriangle24bitMat(cv::Mat& img, const geo::Triangle& T, int value, int vase);
	void drawObjMat(cv::Mat& img, const geo::Obj &obj, int value);
	void drawObj24bitMat(cv::Mat& img, const geo::Obj &obj, int value, int base);
	void drawHandMat(cv::Mat& img, const geo::Hand &hand, int value);

	inline double areaofTriangle(geo::Point P1, geo::Point P2, geo::Point P3);

	bool isPointCrossTriangle(const geo::Point& P, const geo::Triangle& T);
	bool isPointInObj(const geo::Point& P, const geo::Obj& obj);

	geo::Obj* SearchObjRelativePoint(const geo::ObjList& objlist, geo::Point& P);

	void array8bit2Mat(cv::Mat& mat, char* data, int width, int height);

	bool isFingerTouched(geo::Hand& hand);
	geo::Point getFingerCoord(geo::Hand& hand);

	double distPoint(geo::Point& P1, geo::Point& P2);
}