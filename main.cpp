#include "makeBMP.h"
#include "readSTL.h"
#include "geo.h"
#include "controller.h"
#include "omp.h"

using namespace cv;
using namespace std;

GLbyte data[HEIGHT * WIDTH];

geo::Hand e_hand;

int main() {
	omp_set_num_threads(4);
	Leap::Controller controller;
	Leap_Listener listener;
	controller.addListener(listener);

	geo::ObjList objlist;

	string filename("cube.stl");
	geo::Obj* obj;
	unsigned int trigleNum;
	double extension = 2;
	obj = parseSTL2Obj(filename, trigleNum, extension);

	objlist.add(obj);

	double degx = 3;
	double degy = 3;
	double degz = 3;
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
	//cvSetWindowProperty("testImage", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	if (!img.data) {
		cerr << "imread error" << endl;
		assert(false);
	}

	int angle = 0;

	geo::Point prevLeapFinger;
	geo::Point prevLeapPalm;
	//obj = NULL;

	geo::Hand i_hand;
	geo::Hand d_hand;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	clock_t bclock = clock();
	while (true) {
		i_hand = e_hand;
		d_hand = i_hand;

		double matHand[3][3] = { { cos(angle * Deg2Rad), 0, sin(angle * Deg2Rad) },
								{ 0, 1, 0 },
								{ -sin(angle * Deg2Rad), 0, cos(angle * Deg2Rad) } };

		clock_t __clock = clock();

		//d_hand.rotate(matHand);
		objlist.rotateObjList(maty);

		//cout << "rotate time : " << clock() - __clock << endl;

		clock_t ___clock = clock(); 
		//std::cout << !(GetKeyState('B') == 0 || GetKeyState('B') == 1) << " ";
		if (controller.frame().hands()[0].isValid() == true) {
			if (!(GetKeyState('B') == 0 || GetKeyState('B') == 1)) {

				geo::Point temp = geo::getFingerCoord(i_hand);
				geo::Obj* tempobj = geo::SearchObjRelativePoint(objlist, temp);
				if (tempobj != NULL && obj != NULL) {
					double mat[3] = { (temp.x - prevLeapFinger.x) * cos(Deg2Rad * angle), temp.y - prevLeapFinger.y, (temp.z - prevLeapFinger.z) * sin(Deg2Rad * angle) };
					geo::transformObj(*obj, mat);
				}
				else {
					obj = tempobj;
				}
				prevLeapFinger = temp;
			}
			else {
				obj = NULL;
			}
		}
		else {
			obj = NULL;
		}
		//std::cout << clock() - ___clock << " ";
				
		__clock = clock();
		//if ((angle / 3) % 24 == 1) {
			objlist.drawObjList24bitMat(img, 1, (angle / 3) % 24);
		//}
		//geo::drawHandMat(img, d_hand, 255);

		//cout << "draw time : " << clock() - __clock << endl;

		if ((angle / 3) % 24 == 23) {
			//while((std::chrono::system_clock::now() - start) < 330000){}
			Sleep(1000 - (clock() - bclock));
			bclock = clock();
			imshow("testImage", img);
			waitKey(1);
			printf("%d\n", std::chrono::system_clock::now() - start);
			start = std::chrono::system_clock::now();


//#pragma omp parallel for
//			for (int i = 0; i < WIDTH; i++) {
//				for (int j = 0; j < HEIGHT; j++) {
//					test.at<unsigned char>(i, j) = (((int)(img.at<cv::Vec3b>(i, j)[0]) >> 1) & 1) * 255;
//					//std::cout << (((int)(img.at<cv::Vec3b>(i, j)[0]) >> 2) & 1);
//				}
//			}
#pragma omp parallel for
			for (int i = 0; i < WIDTH; i++) {
				for (int j = 0; j < HEIGHT; j++) {
					img.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
				}
			}
			/*imshow("testImage", test);
			waitKey(1);*/
		}

		//objlist.drawObjListMat(img, 0);
		//geo::drawHandMat(img, d_hand, 0);
		
		angle = angle + 3;
		if (angle == 360) {
			//cout << geo::distPoint(i_hand.fingertip[0], i_hand.fingertip[1]) << endl;
			angle = 0;
		}
	}

	getch();

	return 0;
}