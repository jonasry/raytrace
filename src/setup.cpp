#include "testapp.h"

#define FILENAME "untitled.img"
#define IMAGE    "image.img"
#define RIGHT	 "right.img"
#define LEFT	 "left.png"

#define CENTER(x,y,z) 	CVector(x,y,z)
#define NORMAL(x,y,z) 	CVector(x,y,z)
#define DIR(x,y,z) 		CVector(x,y,z)
#define BASE(x,y,z) 	CVector(x,y,z)

CTexture	*Tex5,*Tex6,*Tex7,*Tex8,*Tex9,*Tex10,*Tex11,*Tex12,*Tex13,*Tex14,
			*WHITE,*BLUE,*GREEN,*RED,*BLACK, *Checker1;

CColor 	Blue(0,0,10*.2);
CColor 	Green(0,10*.2,0);
CColor 	Red(10*.2,0,0);
CColor 	Black(0,0,0);
CColor 	White(10*.2,10*.2,10*.2);

void FHelper(double &r, double &g, double &b) {
	r = (1.+sqrt(r))/(1.-sqrt(r));
	g = (1.+sqrt(g))/(1.-sqrt(g));
	b = (1.+sqrt(b))/(1.-sqrt(b));
}

void SetupTextures(CStudio& Studio) {
	
	//	Define textures
	//	---------------

	Studio.Textures.emplace_back(new CTexture(White));
	WHITE = Studio.Textures.back().get();
	Studio.Textures.emplace_back(new CTexture(Blue));
	BLUE = Studio.Textures.back().get();
	Studio.Textures.emplace_back(new CTexture(Green));
	GREEN = Studio.Textures.back().get();
	Studio.Textures.emplace_back(new CTexture(Red));
	RED = Studio.Textures.back().get();
	Studio.Textures.emplace_back(new CTexture(Black));
	BLACK = Studio.Textures.back().get();
}

void SetupLights(CStudio& Studio) {

	//	Define lights
	//	-------------

	Studio.Lights.emplace_back(new CPointLight(CVector(5,10,20),White*0.15));
	Studio.Lights.emplace_back(new CPointLight(CVector(-10,-5,20),White*0.12));
	Studio.Lights.emplace_back(new CPointLight(CVector(7,-5,20),White*0.15));
	Studio.Lights.emplace_back(new CAmbientLight(CColor(2,2*0.96,2*0.87)*0.15));
}

void SetupObjects(CStudio& Studio) {

	//	Define Objects
	//	--------------	

    CBox* B = new CBox(CVector(1,1,1), CVector(0,0,0), RED, nullptr, 0);
    B->Translate(CVector(-.5,-.5,0));
    B->Rotate(CVector(0,0,.1745));
    B->Scale(CVector(1,1,2));
    B->Translate(CVector(0,5,0));
    Studio.Objects.Objects.push_back(B);
                                   
    CPlane* P = new CPlane(CVector(0,0,0.01), CVector(0,0,1), GREEN, nullptr, 0);
    P->Translate(CVector(0,0,0));
    P->Rotate(CVector(0,0,0.3491));
    Studio.Objects.Objects.push_back(P);
}

void SetupCameras(std::vector<std::unique_ptr<CCamera>>& Cameras, CStudio& Studio, int xres, int yres) {
	CVector CamPos(3,-10,6);
	CVector FocusAt(0,-2,4);
	CVector Up(0,0,1);
	
	CVector LeftPos (CamPos-!CrossProduct(Up,CamPos-FocusAt)*.3);
	CVector RightPos(CamPos+!CrossProduct(Up,CamPos-FocusAt)*.3);

	Cameras.emplace_back(new CCamera(COptic(CLine(FocusAt-LeftPos,LeftPos),Up,40.0/2.0,40.0/2.0),&Studio,LEFT,CStorage::PNG,xres,yres));
}

void SetupStudio(CStudio& Studio) {
	SetupTextures(Studio);
	SetupLights(Studio);
	SetupObjects(Studio);
}
