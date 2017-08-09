#include "makeBMP.h"
#include "readSTL.h"
#include "geo.h"

using namespace cv;
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
	geo::Line* line;
	unsigned int trigleNum;
	float extension = 1;
	line = parseSTL2Line(filename, trigleNum, extension);

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

	Mat img;
	img = Mat(WIDTH, HEIGHT, CV_8UC1, Scalar(0));
	namedWindow("testImage");
	if (!img.data) {
		cerr << "imread error" << endl;
		assert(false);
	}

	int chk = 0;
	while (true) {
		for (int i = 0; i < linenum; i++) {
			rotate(line[i].Point1, maty);
			rotate(line[i].Point2, maty);
			drawLineMat(img, line[i], 255);//faster than cv::line()
			//cv::line(img, cv::Point(line[i].Point1.x + WIDTH/2, line[i].Point1.y + HEIGHT / 2), cv::Point(line[i].Point2.x + WIDTH / 2, line[i].Point2.y + HEIGHT / 2), 255);
		}

		imshow("testImage", img);
		waitKey(1000 / 1000);

		for (int i = 0; i < linenum; i++) {
			drawLineMat(img, line[i], 0);//faster than cv::line()
			//cv::line(img, cv::Point(line[i].Point1.x + WIDTH / 2, line[i].Point1.y + HEIGHT / 2), cv::Point(line[i].Point2.x + WIDTH / 2, line[i].Point2.y + HEIGHT / 2), 0);
		}

		if(chk++ % 120 == 0) printf("%d %d\n", clock(), (int)(1000.0 / 60 * chk));
	}

	return 0;
}