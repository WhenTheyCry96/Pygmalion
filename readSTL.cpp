#include "readSTL.h"

using namespace std;

float parseFloat(std::ifstream& file) {
	char f_buf[sizeof(float)];
	file.read(f_buf, 4);
	float* fptr = (float*)f_buf;
	return *fptr;
}

geo::Point parsePoint(std::ifstream& file) {
	float x = parseFloat(file);
	float y = parseFloat(file);
	float z = parseFloat(file);
	return geo::Point((double)x, (double)y, (double)z);
}

geo::Line* parseSTL2Line(const string filename, unsigned int& _num, double extension) {
	ifstream fileSTL(filename.c_str(), ios::in | ios::binary);
	if (!fileSTL) {
		cerr << "open stl file error" << endl;
		assert(false);
	}

	char header[81] = "";
	char numTriangles[4];
	char dummy[2];

	fileSTL.read(header, 80);
	fileSTL.read(numTriangles, 4);
	unsigned int* r = (unsigned int*)numTriangles;
	unsigned int num = *r;
	_num = num;

	cout << "Num Polygon : " << num << endl << endl;

	geo::Line *line = new geo::Line[num * 3];

	for (unsigned int i = 0; i < num; i++) {
		geo::Point normalVector = parsePoint(fileSTL);
		geo::Point v1 = parsePoint(fileSTL);
		geo::Point v2 = parsePoint(fileSTL);
		geo::Point v3 = parsePoint(fileSTL);

		v1.extenPoint(extension);
		v2.extenPoint(extension);
		v3.extenPoint(extension);

		//cout << v1.x << " " << v1.y << " " << v1.z << " " << endl;
		//cout << v2.x << " " << v2.y << " " << v2.z << " " << endl;
		//cout << v3.x << " " << v3.y << " " << v3.z << " " << endl;
		//cout << endl;
		fileSTL.read(dummy, 2);

		line[i * 3] = geo::Line(v1, v2);
		line[i * 3 + 1] = geo::Line(v1, v3);
		line[i * 3 + 2] = geo::Line(v2, v3);
	}

	return line;
}

geo::Obj* parseSTL2Obj(const std::string filename, unsigned int& _num, double extension) {
	geo::Obj *obj = new geo::Obj;
	ifstream fileSTL(filename.c_str(), ios::in | ios::binary);
	if (!fileSTL) {
		cerr << "open stl file error" << endl;
		assert(false);
	}

	char header[81] = "";
	char numTriangles[4];
	char dummy[2];

	fileSTL.read(header, 80);
	fileSTL.read(numTriangles, 4);
	unsigned int* r = (unsigned int*)numTriangles;
	unsigned int num = *r;
	_num = num;

	cout << "Num Polygon : " << num << endl << endl;
	obj->T = new geo::Triangle[num];
	obj->num = num;

	for (unsigned int i = 0; i < num; i++) {
		geo::Point normalVector = parsePoint(fileSTL);
		geo::Point v1 = parsePoint(fileSTL);
		geo::Point v2 = parsePoint(fileSTL);
		geo::Point v3 = parsePoint(fileSTL);

		v1.extenPoint(extension);
		v2.extenPoint(extension);
		v3.extenPoint(extension);

		//cout << v1.x << " " << v1.y << " " << v1.z << " " << endl;
		//cout << v2.x << " " << v2.y << " " << v2.z << " " << endl;
		//cout << v3.x << " " << v3.y << " " << v3.z << " " << endl;
		//cout << endl;
		fileSTL.read(dummy, 2);

		obj->T[i] = geo::Triangle(v1, v2, v3);
	}

	return obj;
}