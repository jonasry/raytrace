#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "studio.h"
#include "camera.h"
#include <vector>
#include <memory>
#include "object.h"

class CInterface : public CObject {

public:
	CInterface() {}
	~CInterface();

public:
	void SnapAll();

//	virtual void Serialize(CArchive A);

public:

    CStudio Studio;
    std::vector<std::unique_ptr<CCamera>> Cameras;


};

#endif
