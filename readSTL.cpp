#include "readSTL.h"

using namespace std;

float parseFloat(std::ifstream& file) {
	char f_buf[sizeof(float)];
	file.read(f_buf, 4);
	float* fptr = (float*)f_buf;
	return *fptr;
}

Point parsePoint(std::ifstream& file) {
	float x = parseFloat(file);
	float y = parseFloat(file);
	float z = parseFloat(file);
	return Point(x, y, z);
}

Line* parseSTL(const string filename, unsigned int& _num) {
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

	cout << num << endl << endl;

	Line *line = new Line[num * 3];

	for (unsigned int i = 0; i < num; i++) {
		Point normalVector = parsePoint(fileSTL);
		Point v1 = parsePoint(fileSTL);
		Point v2 = parsePoint(fileSTL);
		Point v3 = parsePoint(fileSTL);
		//cout << v1.x << " " << v1.y << " " << v1.z << " " << endl;
		//cout << v2.x << " " << v2.y << " " << v2.z << " " << endl;
		//cout << v3.x << " " << v3.y << " " << v3.z << " " << endl;
		//cout << endl;
		fileSTL.read(dummy, 2);

		line[i * 3] = Line(v1, v2);
		line[i * 3 + 1] = Line(v1, v3);
		line[i * 3 + 2] = Line(v2, v3);
	}

	return line;
}