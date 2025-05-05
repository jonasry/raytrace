//	Interfac.cpp

#include "interfac.h"

CInterface::~CInterface() {

    // Delete all cameras
    for (auto* cam : Cameras) {
        delete cam;
    }
    Cameras.clear();

}

void CInterface::SnapAll() {

    for (auto* Cam : Cameras) {
        Cam->Snap();
    }

}
