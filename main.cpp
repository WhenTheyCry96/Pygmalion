#include "makeBMP.h"
#include "readSTL.h"
#include "geo.h"
#include "controller.h"
#include "omp.h"

using namespace cv;
using namespace std;

GLbyte data[HEIGHT * WIDTH];

geo::Hand e_hand;
int flag = 0;

int main() {
	omp_set_num_threads(4);
	Leap::Controller controller;
	Leap_Listener listener;
	controller.addListener(listener);

	geo::ObjList objlist;

	string filename("cube.stl");
	geo::Obj* obj;
	unsigned int trigleNum;
	double extension = 1;
	obj = parseSTL2Obj(filename, trigleNum, extension);

	objlist.add(obj);

	double degx = 3;
	double degy = 0.5;
	double degz = 0.5;
	double matx[3][3] = { { 1, 0, 0 },
						{ 0, cos(degx * Deg2Rad), -sin(degx * Deg2Rad) },
						{ 0, sin(degx * Deg2Rad), cos(degx * Deg2Rad) } };

	double matz[3][3] = { { cos(degz * Deg2Rad), -sin(degz * Deg2Rad), 0 },
						{ sin(degz * Deg2Rad),  cos(degz * Deg2Rad), 0 },
						{ 0, 0, 1 } };

	double maty[3][3] = { { cos(degy * Deg2Rad), 0, sin(degy * Deg2Rad) },
						{ 0, 1, 0 },
						{ -sin(degy * Deg2Rad), 0, cos(degy * Deg2Rad) } };

	Mat img;
	Mat test;
	img = Mat(WIDTH, HEIGHT, CV_8UC3, Scalar(0));
	test = Mat(WIDTH, HEIGHT, CV_8UC1, Scalar(0));

	cvNamedWindow("testImage", CV_WINDOW_NORMAL);
	cvSetWindowProperty("testImage", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	if (!img.data) {
		cerr << "imread error" << endl;
		assert(false);
	}

	double angle = 0;

	geo::Point prevLeapFinger;
	geo::Point prevLeapPalm;
	//obj = NULL;
	
	geo::Hand i_hand;
	geo::Hand d_hand;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	clock_t bclock = clock();

	double angletest = 0;
	geo::Point prevp;
	while (true) {
		double matz[3][3] = { { cos(angle * Deg2Rad), -sin(angle * Deg2Rad), 0 },
							{ sin(angle * Deg2Rad),  cos(angle * Deg2Rad), 0 },
							{ 0, 0, 1 } };

		double matzrev[3][3] = { { cos(-angle * Deg2Rad), -sin(-angle * Deg2Rad), 0 },
							{ sin(-angle * Deg2Rad),  cos(-angle * Deg2Rad), 0 },
							{ 0, 0, 1 } };

		double matztest[3][3] = { { cos(angletest * Deg2Rad), -sin(angletest * Deg2Rad), 0 },
								{ sin(angletest * Deg2Rad),  cos(angletest * Deg2Rad), 0 },
								{ 0, 0, 1 } };
		double matytest[3][3] = { { cos(angletest * Deg2Rad), 0, sin(angletest * Deg2Rad) },
							{ 0, 1, 0 },
							{ -sin(angletest * Deg2Rad), 0, cos(angletest * Deg2Rad) } };

		i_hand = e_hand;
		d_hand = i_hand;

		double matHand[3][3] = { { cos(angle * Deg2Rad), 0, sin(angle * Deg2Rad) },
								{ 0, 1, 0 },
								{ -sin(angle * Deg2Rad), 0, cos(angle * Deg2Rad) } };

		clock_t __clock = clock();

		//d_hand.rotate(matHand);
		//if(flag == 0) objlist.rotateObjList(matztest);
		//if(flag == 0) objlist.rotateObjList(matytest);
		objlist.rotateinPlaceObjList(matztest);
		objlist.rotateinPlaceObjList(matytest);

		clock_t ___clock = clock(); 
		if (controller.frame().hands()[0].isValid() == true) {
			if ((GetKeyState('B') == 0 || GetKeyState('B') == 1)) {
				if (flag == 0) {
					prevp = geo::getFingerCoord(i_hand);
					flag = 1;
				}
				else {
					geo::Point p = geo::getFingerCoord(i_hand);
					double mat[3] = {p.x - prevp.x, p.y - prevp.y, p.z - prevp.z};
					geo::transformObj(*obj, mat);
					prevp = p;
				}
			}
		}
		else {
			flag = 0;
		}
		//std::cout << clock() - ___clock << " ";
				
		__clock = clock();
		if ((int)(angle / 3) % 24 == 0) {
			////objlist.drawObjList24bitMat(img, 1, ((int)angle / 3) % 24);
			for(int i = 0; i < 24; i++)
				objlist.drawObjList24bitMat(img, 1, i);

		}
		//geo::drawHandMat(img, d_hand, 255);

		//cout << "draw time : " << clock() - __clock << endl;
		double matztestrev[3][3] = { { cos(-angletest * Deg2Rad), -sin(-angletest * Deg2Rad), 0 },
									{ sin(-angletest * Deg2Rad),  cos(-angletest * Deg2Rad), 0 },
									{ 0, 0, 1 } };
		double matytestrev[3][3] = { { cos(-angletest * Deg2Rad), 0, sin(-angletest * Deg2Rad) },
								{ 0, 1, 0 },
								{ -sin(-angletest * Deg2Rad), 0, cos(-angletest * Deg2Rad) } };

		//if(flag == 0) objlist.rotateObjList(matytestrev);
		//if(flag == 0) objlist.rotateObjList(matztestrev);
		objlist.rotateinPlaceObjList(matztestrev);

		if (((int)angle / 3) % 24 == 23) {
			int time = 170;
			if (time - (clock() - bclock) < 0) {}
			else {
				Sleep(time - (clock() - bclock));
			}
			bclock = clock();
			printf("%d\n", std::chrono::system_clock::now() - start);
			start = std::chrono::system_clock::now();

			imshow("testImage", img);
			waitKey(1);

#pragma omp parallel for
			for (int i = 0; i < WIDTH; i++) {
				for (int j = 0; j < HEIGHT; j++) {
					img.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
				}
			}
		}
		angle = angle + 3;
		if (angle == 360) {
			angle = 0;
		}
		angletest += 0.9;
	}

	getch();

	return 0;
}