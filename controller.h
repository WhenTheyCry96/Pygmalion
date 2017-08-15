#pragma once
#include "geo.h"
#include "Leap.h"
#include "LeapMath.h"

class Leap_Listener : public Leap::Listener {
public:
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
};

extern geo::Hand e_hand;