#include "controller.h"

void Leap_Listener::onConnect(const Leap::Controller& controller) {
	std::cout << "Connected" << std::endl;
	// Enable gestures, set Config values:
	controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
	controller.config().setFloat("Gesture.Swipe.MinLength", 200.0);
	controller.config().save();
}
//Not dispatched when running in a debugger
void Leap_Listener::onDisconnect(const Leap::Controller& controller) {
	std::cout << "Disconnected" << std::endl;
}

void Leap_Listener::onFrame(const Leap::Controller& controller) {
	 Leap::Frame frame = controller.frame();
	Leap::Hand hand = frame.hands()[0];
	if (hand.isValid() == false) ;

	Leap::FingerList phalanges1 = hand.fingers();
	int count = 0;
	for (Leap::FingerList::const_iterator fl = phalanges1.begin(); fl != phalanges1.end(); fl++) {
		Leap::Vector curPos = (*fl).tipPosition();
		e_hand.fingertip[count] = geo::Point(curPos[0], curPos[1], curPos[2]);

		Leap::Bone bone;
		Leap::Bone::Type boneType = Leap::Bone::Type::TYPE_PROXIMAL;
		bone = (*fl).bone(boneType);
		curPos = bone.center();
		e_hand.proximal[count] = geo::Point(curPos[0], curPos[1], curPos[2]);

		count++;
	}

	Leap::Vector curPos;
	curPos = hand.wristPosition();
	e_hand.wrist = geo::Point(curPos[0], curPos[1], curPos[2]);

	return;
}
