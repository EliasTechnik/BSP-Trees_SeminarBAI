#pragma once
#include "dList.h"

template <class Location, int dimension>
class cPlane {
private:
	Location origin;
	dList<Location> saddlepoints;

};