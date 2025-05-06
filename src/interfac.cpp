//	Interfac.cpp

#include "interfac.h"

CInterface::~CInterface() {
    // no-op: unique_ptr in Cameras will handle destruction
}

void CInterface::SnapAll() {
    for (auto& Cam : Cameras) {
        Cam->Snap();
    }
}
