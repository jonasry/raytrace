#ifndef __TESTAPP_H__
#define __TESTAPP_H__
#include "rtlib.h"
#include <vector>
#include <memory>

#define VER 0.9

#if defined VERBOSE
	#include <iostream.h>
#endif

typedef void (*fptr)(int);

void SetupTextures(CStudio& Studio);
void SetupLights(CStudio& Studio);
void SetupObjects(CStudio& Studio);
void SetupStudio(CStudio& Studio);
void SetupCameras(std::vector<std::unique_ptr<CCamera>>& Cameras, CStudio& Studio, int xres, int yres);
// Main raytracing entry point
int RTmain(int argc, char* argv[]);

#endif // __TESTAPP_H__
