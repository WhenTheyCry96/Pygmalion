#include "geo.h"

void geo::Point::extenPoint(double extension) {
	this->x = this->x * extension;
	this->y = this->y * extension;
	this->z = this->z * extension;

	return;
}

void geo::ObjList::add(geo::Obj* obj) {
	if (head == NULL) {
		head = obj;
		tail = obj;
	}
	else {
		tail->next = obj;
		tail = obj;
	}
	return;
}

void geo::ObjList::drawObjListMat(cv::Mat& img, int value) {
	geo::Obj* tempobj = head;
	while (tempobj != NULL) {
		geo::drawObjMat(img, *tempobj, value);
		tempobj = tempobj->next;
	}
	return;
}

void geo::ObjList::rotateObjList(double Mat[][3]) {
	geo::Obj* tempobj = head;
	while (tempobj != NULL) {
		geo::rotateObj(*tempobj, Mat);
		tempobj = tempobj->next;
	}
	return;
}

void geo::Hand::rotate(double Mat[][3]) {
	for (int i = 0; i < 5; i++) {
		geo::rotate(this->fingertip[i], Mat);
		geo::rotate(this->proximal[i], Mat);

	}
	geo::rotate(this->wrist, Mat);
	return;
}

inline void geo::dot(GLbyte* data, int x, int y, int value) {
	int _x = x + WIDTH / 2;
	int _y = y + HEIGHT / 2;

	if (_x >= 0 && _x < WIDTH && _y >= 0 && _y < HEIGHT) {
		data[_y * WIDTH + _x] = value;
	}
	return;
}

void geo::dotMat(cv::Mat& img, int x, int y, int value) {
	int _x = x + WIDTH / 2;
	int _y = y + HEIGHT / 2;

	if (_x >= 0 && _x < WIDTH && _y >= 0 && _y < HEIGHT) {
		img.at<unsigned char>(cv::Point(_x, HEIGHT - _y -1)) = value;
	}
	return;
}

void geo::dot24bitMat(cv::Mat& img, int x, int y, int value, int base) {
	int _x = x + WIDTH / 2;
	int _y = y + HEIGHT / 2;
	int index = _x + _y * WIDTH;
	int indextr = index / 24 + base * WIDTH * HEIGHT / 24;
	int __y = indextr / WIDTH;
	int __x = indextr % WIDTH;
	int subIndex = index % 24;
	int RGB = subIndex / 8;
	int RGBIndex = subIndex % 8;
	if (_x >= 0 && _x < WIDTH && _y >= 0 && _y < HEIGHT) {
		img.at<cv::Vec3b>(cv::Point(__x, HEIGHT - __y - 1))[RGB] = value << RGBIndex;
	}
	return;
}

inline void geo::swap(double &x, double &y) {
	double tmp = x;
	x = y;
	y = tmp;
	return;
}

void geo::rotate(Point& _Point, double Mat[][3]) {
	double _x = _Point.x;
	double _y = _Point.y;
	double _z = _Point.z;

	_Point.x = Mat[0][0] * _x + Mat[0][1] * _y + Mat[0][2] * _z;
	_Point.y = Mat[1][0] * _x + Mat[1][1] * _y + Mat[1][2] * _z;
	_Point.z = Mat[2][0] * _x + Mat[2][1] * _y + Mat[2][2] * _z;

	return;
}

void geo::rotateObj(geo::Obj& obj, double Mat[][3]) {
	Triangle* T = obj.T;
	int num = obj.num;

	for (int i = 0; i < num; i++) {
		geo::rotate(T[i].P1, Mat);
		geo::rotate(T[i].P2, Mat);
		geo::rotate(T[i].P3, Mat);
	}

	return;
}

void geo::transformPoint(Point& _Point, double Mat[]) {
	_Point.x = _Point.x + Mat[0];
	_Point.y = _Point.y + Mat[1];
	_Point.z = _Point.z + Mat[2];
	return;
}

void geo::transformTriangle(geo::Triangle& _T, double Mat[]) {
	geo::transformPoint(_T.P1, Mat);
	geo::transformPoint(_T.P2, Mat);
	geo::transformPoint(_T.P3, Mat);
	return;
}

void geo::transformObj(geo::Obj& obj, double Mat[]) {
	Triangle* T = obj.T;
	int num = obj.num;

	for (int i = 0; i < num; i++) {
		geo::transformPoint(T[i].P1, Mat);
		geo::transformPoint(T[i].P2, Mat);
		geo::transformPoint(T[i].P3, Mat);
	}
	return;
}


void geo::drawLine(GLbyte* data, const Line& _Line, int value) {
	double x1 = _Line.Point1.x;
	double y1 = _Line.Point1.y;
	double x2 = _Line.Point2.x;
	double y2 = _Line.Point2.y;
	// y = ax + b
	if (abs(x1 - x2) > abs(y1 - y2)) {
		double a = (y2 - y1) / (x2 - x1);
		double b = -a*x1 + y1;
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
		double a = (y2 - y1) / (x2 - x1);
		double b = -a*x1 + y1;
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

void geo::drawLineMat(cv::Mat& img, const geo::Line& _Line, int value) {
	double x1 = _Line.Point1.x;
	double y1 = _Line.Point1.y;
	double x2 = _Line.Point2.x;
	double y2 = _Line.Point2.y;
	// y = ax + b
	if (abs(x1 - x2) > abs(y1 - y2)) {
		double a = (y2 - y1) / (x2 - x1);
		double b = -a*x1 + y1;
		if (x1 < x2) {
			for (; x1 <= x2; x1++) {
				geo::dotMat(img, x1, a*x1 + b, value);
			}
		}
		else {
			for (; x2 <= x1; x2++) {
				geo::dotMat(img, x2, a*x2 + b, value);
			}
		}
	}
	else {
		geo::swap(x1, y1);
		geo::swap(x2, y2);
		double a = (y2 - y1) / (x2 - x1);
		double b = -a*x1 + y1;
		if (x1 < x2) {
			for (; x1 <= x2; x1++) {
				geo::dotMat(img, a*x1 + b, x1, value);
			}
		}
		else {
			for (; x2 <= x1; x2++) {
				geo::dotMat(img, a*x2 + b, x2, value);
			}
		}
	}
	return;
}

void geo::drawLine24bitMat(cv::Mat& img, const geo::Line& _Line, int value, int base) {
	double x1 = _Line.Point1.x;
	double y1 = _Line.Point1.y;
	double x2 = _Line.Point2.x;
	double y2 = _Line.Point2.y;
	// y = ax + b
	if (abs(x1 - x2) > abs(y1 - y2)) {
		double a = (y2 - y1) / (x2 - x1);
		double b = -a*x1 + y1;
		if (x1 < x2) {
			for (; x1 <= x2; x1++) {
				geo::dot24bitMat(img, x1, a*x1 + b, value, base);
			}
		}
		else {
			for (; x2 <= x1; x2++) {
				geo::dot24bitMat(img, x2, a*x2 + b, value, base);
			}
		}
	}
	else {
		geo::swap(x1, y1);
		geo::swap(x2, y2);
		double a = (y2 - y1) / (x2 - x1);
		double b = -a*x1 + y1;
		if (x1 < x2) {
			for (; x1 <= x2; x1++) {
				geo::dot24bitMat(img, a*x1 + b, x1, value, base);
			}
		}
		else {
			for (; x2 <= x1; x2++) {
				geo::dot24bitMat(img, a*x2 + b, x2, value, base);
			}
		}
	}
	return;
}

void geo::drawTriangleMat(cv::Mat& img, const geo::Triangle& T, int value) {
	geo::drawLineMat(img, geo::Line(T.P1, T.P2), value);
	geo::drawLineMat(img, geo::Line(T.P2, T.P3), value);
	geo::drawLineMat(img, geo::Line(T.P3, T.P1), value);

	return;
}

void geo::drawTriangle24bitMat(cv::Mat& img, const geo::Triangle& T, int value, int base) {
	geo::drawLine24bitMat(img, geo::Line(T.P1, T.P2), value, base);
	geo::drawLine24bitMat(img, geo::Line(T.P2, T.P3), value, base);
	geo::drawLine24bitMat(img, geo::Line(T.P3, T.P1), value, base);

	return;
}

void geo::drawObjMat(cv::Mat& img, const geo::Obj &obj, int value) {
	int num = obj.num;

#pragma omp parallel for
	for (int i = 0; i < num; i++) {
		geo::drawTriangleMat(img, obj.T[i], value);
	}

	return;
}

void geo::drawObj24bitMat(cv::Mat& img, const geo::Obj &obj, int value, int base) {
	int num = obj.num;

#pragma omp parallel for
	for (int i = 0; i < num; i++) {
		geo::drawTriangle24bitMat(img, obj.T[i], value, base);
	}

	return;
}

void geo::drawHandMat(cv::Mat& img, const geo::Hand &hand, int value) {
	for (int i = 0; i < 5; i++) {
		geo::Line templine(hand.fingertip[i], hand.proximal[i]);
		geo::drawLineMat(img, templine, value);

		templine = geo::Line(hand.wrist, hand.proximal[i]);
		geo::drawLineMat(img, templine, value);
	}
	return;
}

inline double geo::areaofTriangle(geo::Point P1, geo::Point P2, geo::Point P3) {
	return 0.5 * abs(P1.y * P2.z + P2.y * P3.z + P3.y * P1.z - P1.z * P2.y - P2.z * P3.y - P3.z * P1.y);
}

bool geo::isPointCrossTriangle(const geo::Point& P, const geo::Triangle& T) {

	double A = geo::areaofTriangle(geo::Point(P.x, T.P1.y, T.P1.z), geo::Point(P.x, T.P2.y, T.P2.z), geo::Point(P.x, T.P3.y, T.P3.z));
	double A1 = geo::areaofTriangle(geo::Point(P.x, P.y, P.z), geo::Point(P.x, T.P2.y, T.P2.z), geo::Point(P.x, T.P3.y, T.P3.z));
	double A2 = geo::areaofTriangle(geo::Point(P.x, T.P1.y, T.P1.z), geo::Point(P.x, P.y, P.z), geo::Point(P.x, T.P3.y, T.P3.z));
	double A3 = geo::areaofTriangle(geo::Point(P.x, T.P1.y, T.P1.z), geo::Point(P.x, T.P2.y, T.P2.z), geo::Point(P.x, P.y, P.z));
	//printf("A: %f\tA1: %f\tA2: %f\tA3: %f\tA1+A2+A3: %f\tA==S(A_n): %d\n", A, A1, A2, A3, A1+A2+A3, abs(A - (A1 + A2 + A3)) < 0.1f);
	if (abs(A - (A1 + A2 + A3)) < 0.001) {
		if ((T.P3.z - T.P1.z)*(T.P2.y - T.P1.y) - (T.P2.z - T.P1.z)*(T.P3.y - T.P1.y) == 0) return false;
		double x = T.P1.x - ((P.y - T.P1.y)*((T.P2.z - T.P1.z)*(T.P3.x - T.P1.x) - (T.P3.z - T.P1.z)*(T.P2.x - T.P1.x)) + (P.z - T.P1.z)*((T.P2.x - T.P1.x)*(T.P3.y - T.P1.y) - (T.P2.y - T.P1.y)*(T.P3.x - T.P1.x))) / ((T.P3.z - T.P1.z)*(T.P2.y - T.P1.y) - (T.P2.z - T.P1.z)*(T.P3.y - T.P1.y));
		if (isnan(x) == true) return false;
		if (x > P.x) {
			//printf("v1(%f, %f, %f)\t", T.P1.x, T.P1.y, T.P1.z);
			//printf("v2(%f, %f, %f)\t", T.P2.x, T.P2.y, T.P2.z);
			//printf("v3(%f, %f, %f)\t\n", T.P3.x, T.P3.y, T.P3.z);

			return true;
		}
	}
	return false;
}

bool geo::isPointInObj(const geo::Point& P, const geo::Obj& obj) {

	int chk = 0;
	int num = obj.num;

	for (int i = 0; i < num; i++) {
		if (geo::isPointCrossTriangle(P, obj.T[i]) == true) chk = chk + 1;
	}

	if (chk % 2 == 1) return true;
	
	return false;
}

geo::Obj* geo::SearchObjRelativePoint(const geo::ObjList& objlist, geo::Point& P) {
	if (objlist.head != NULL) {
		geo::Obj* obj = objlist.head;
		while (true) {
			if (geo::isPointInObj(P, *obj) == true) {
				return obj;
			}
			else if (obj->next == NULL) {
				return NULL;
			}
			else {
				obj = obj->next;
			}
		}
	}
	return NULL;
}

void geo::array8bit2Mat(cv::Mat& img, char* data, int width, int height) {
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			img.at<unsigned char>(cv::Point(w, h)) = data[(height - h - 1)*width + w] * 255;
		}
	}

	return;
}
bool geo::isFingerTouched(geo::Hand& hand) {
	if (geo::distPoint(hand.fingertip[0], hand.fingertip[1]) < 50) return true;
	else return false;
}

geo::Point geo::getFingerCoord(geo::Hand& hand) {
	return hand.fingertip[0];
}

double geo::distPoint(geo::Point& P1, geo::Point& P2) {
	return sqrt(pow(P1.x - P2.x, 2) + pow(P1.y - P2.y, 2) + pow(P1.z - P2.z, 2));
}