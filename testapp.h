#include "rtlib.h"

#define VER 0.9

#if defined VERBOSE
	#include <iostream.h>
#endif

typedef void (*fptr)(int);

void SetupTextures(CStudio& Studio);
void SetupLights(CStudio& Studio);
void SetupObjects(CStudio& Studio);
void SetupStudio(CStudio& Studio);
void SetupCameras(CObList& Cameras, CStudio& Studio, int xres, int yres);
