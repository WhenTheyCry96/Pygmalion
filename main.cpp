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

	string filename("testcube.stl");
	geo::Obj* obj;
	unsigned int trigleNum;
	double extension = 1;
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
	img = Mat(WIDTH, HEIGHT, CV_8UC1, Scalar(0));
	namedWindow("testImage");
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
	while (true) {
		i_hand = e_hand;
		d_hand = i_hand;
		double matHand[3][3] = { { cos(angle * Deg2Rad), 0, sin(angle * Deg2Rad) },
								{ 0, 1, 0 },
								{ -sin(angle * Deg2Rad), 0, cos(angle * Deg2Rad) } };
		d_hand.rotate(matHand);

		objlist.rotateObjList(maty);
		objlist.drawObjListMat(img, 255);
		geo::drawHandMat(img, d_hand, 255);

		//imshow("testImage", img);
		//waitKey(1000 / 1000);

		objlist.drawObjListMat(img, 0);
		geo::drawHandMat(img, d_hand, 0);
		
		if (controller.frame().hands()[0].isValid() == true) {
			if (geo::isFingerTouched(i_hand)) {

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

		angle = angle + 3;
		if (angle == 360) {
			printf("%d\n", clock());
			cout << geo::distPoint(i_hand.fingertip[0], i_hand.fingertip[1]) << endl;
			angle = 0;
		}
	}

	getch();

	return 0;
}