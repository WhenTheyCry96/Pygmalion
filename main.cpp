#include "makeBMP.h"
#include "readSTL.h"
#include "geo.h"

using namespace std;

GLbyte data[HEIGHT * WIDTH];

int main() {

	/***************
	*
	* drawLine Check
	*
	***************/
	//for (int i = 0; i < 360; i += 120) {
	//	cout << i << " x: " << (10 + i) * cos(i * Deg2Rad) << " y: " << (10 + i) * sin(i * Deg2Rad) << endl;
	//	Point P1(0, 0, 0);
	//	Point P2(0 + (10 + i) * cos(i * Deg2Rad), 0 + (10 + i) * sin(i * Deg2Rad), 0);

	//	Line L1(P1, P2);
	//	drawLine(::data, L1, 1);
	//}
	//StoreIMG(::data, WIDTH, HEIGHT);


	/****************
	*
	* rotate Check
	*
	****************/
	//Line line[120];
	//for (int i = 0; i < 360; i += 3) {
	//	Point P1(0, 0, 0);
	//	Point P2((10 + i) * cos(i * Deg2Rad), (10 + i) * sin(i * Deg2Rad), 0);

	//	line[i / 3] = Line(P1, P2);
	//}

	//float degx = 3;
	//float degy = 0;
	//float degz = 0;
	//float matx[3][3] = { { 1, 0, 0 },
	//					{ 0, cos(degx * Deg2Rad), -sin(degx * Deg2Rad) },
	//					{ 0, sin(degx * Deg2Rad), cos(degx * Deg2Rad) } };
	//float matz[3][3] = { {cos(degz * Deg2Rad), -sin(degz * Deg2Rad), 0 },
	//					{ sin(degz * Deg2Rad),  cos(degz * Deg2Rad), 0 },
	//					{0, 0, 1} };
	//float maty[3][3] = { { cos(degy * Deg2Rad), 0, sin(degy * Deg2Rad) },
	//					{ 0, 1, 0 },
	//					{ -sin(degy * Deg2Rad), 0, cos(degy * Deg2Rad) } };
	//int chk = 0;
	//while (true) {
	//	for (int i = 0; i < 120; i++) {
	//		rotate(line[i].Point1, matz);
	//		rotate(line[i].Point2, matz);
	//		drawLine(::data, line[i], 1);

	//	}

	//	//StoreIMG(::data, WIDTH, HEIGHT);

	//	for (int i = 0; i < 120; i++) {
	//		drawLine(::data, line[i], 0);
	//	}
	//	if(chk++ % 1800 ==0) printf("%d ", clock());

	//	//Sleep(1000);
	//}

	string filename("testcube.stl");
	Line* line;
	unsigned int trigleNum;
	line = parseSTL(filename, trigleNum);

	int linenum = trigleNum * 3;

	float degx = 3;
	float degy = 3;
	float degz = 3;
	float matx[3][3] = { { 1, 0, 0 },
						{ 0, cos(degx * Deg2Rad), -sin(degx * Deg2Rad) },
						{ 0, sin(degx * Deg2Rad), cos(degx * Deg2Rad) } };
	float matz[3][3] = { { cos(degz * Deg2Rad), -sin(degz * Deg2Rad), 0 },
						{ sin(degz * Deg2Rad),  cos(degz * Deg2Rad), 0 },
						{ 0, 0, 1 } };
	float maty[3][3] = { { cos(degy * Deg2Rad), 0, sin(degy * Deg2Rad) },
						{ 0, 1, 0 },
						{ -sin(degy * Deg2Rad), 0, cos(degy * Deg2Rad) } };
	int chk = 0;
	while (true) {
		for (int i = 0; i < linenum; i++) {
			rotate(line[i].Point1, maty);
			rotate(line[i].Point2, maty);
			drawLine(::data, line[i], 1);
		}

		StoreIMG(::data, WIDTH, HEIGHT);

		for (int i = 0; i < linenum; i++) {
			drawLine(::data, line[i], 0);
		}

		//for (int i = 0; i < WIDTH * HEIGHT; i++) {
		//	::data[i] = 0;
		//}

		if(chk++ % 120 == 0) printf("%d ", clock());

		//Sleep(200);
	}

	return 0;
}