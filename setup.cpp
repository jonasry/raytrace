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

	double 	Fr=0.55, 
			Fg=0.75, 
			Fb=0.70;

	FHelper(Fr, Fg, Fb);

	
	double	Gr=0.80, 
			Gg=0.40, 
			Gb=0.20;

	FHelper(Gr, Gg, Gb);

	
	//	Make definitions for textures needed
	//	------------------------------------

	WHITE = new CTexture(White);			Studio.Textures.AddTail(WHITE);
	BLUE  = new CTexture(Blue);				Studio.Textures.AddTail(BLUE);
	GREEN = new CTexture(Green);			Studio.Textures.AddTail(GREEN);
	RED   = new CTexture(Red);				Studio.Textures.AddTail(RED);
	BLACK = new CTexture(Black);			Studio.Textures.AddTail(BLACK);

	Tex5  = new CTexture(Black,CColor(Gr,Gg,Gb),.1,.6,Gg);	Studio.Textures.AddTail(Tex5);
	Tex6  = new CTexture(Black,CColor(Fr,Fg,Fb),.1,.6,Fg);	Studio.Textures.AddTail(Tex6);
	Tex7  = new CTexture(Black,CColor(Fr,Fg,Fb),.05,.6,Fg);	Studio.Textures.AddTail(Tex7);           

	Tex8  = new CChecker(WHITE,BLUE,2,2,2); Studio.Textures.AddTail(Tex8);

	Tex9  = new CNoise(WHITE,BLACK,.3);		Studio.Textures.AddTail(Tex9);
	Tex10 = new CTurbulence(WHITE,RED,.2);	Studio.Textures.AddTail(Tex10);
	Tex11 = new CMarble(Tex10,Tex7,0.25);	Studio.Textures.AddTail(Tex11);
	Tex12 = new CTurbulence(BLUE,BLACK,.5);	Studio.Textures.AddTail(Tex12);
	Tex13 = new CTurbulence(WHITE,BLACK,.5);Studio.Textures.AddTail(Tex13);
	Tex14 = new CTurbulence(WHITE,Tex5,.5); Studio.Textures.AddTail(Tex14);

}

void SetupLights(CStudio& Studio) {

	//	Define lights
	//	-------------

	Studio.Lights.AddTail(new CPointLight(CVector(5,10,20),White*0.15));
	Studio.Lights.AddTail(new CPointLight(CVector(-10,-5,20),White*0.12));
	Studio.Lights.AddTail(new CPointLight(CVector(7,-5,20),White*0.15));
	Studio.Lights.AddTail(new CAmbientLight(CColor(2,2*0.96,2*0.87)*0.15));

}

void SetupObjects(CStudio& Studio) {

		const int Flip = CPrimitive::Invert;
		const int NoFlip = CPrimitive::NonInvert;

	//	Define Objects
	//	--------------
/*
	Studio.Objects.Objects.AddTail(new CPlane(CVector(0,0,0.01),CVector(0,0,1),Tex8,0,0));

	Studio.Objects.Objects.AddTail(new CSphere(0.50,CVector( 1,-3.0, 2),RED,   0,0));
	Studio.Objects.Objects.AddTail(new CSphere(0.50,CVector( 1, 2.5, 3),GREEN, 0,0));
	Studio.Objects.Objects.AddTail(new CSphere(0.50,CVector(-2,-2.5, 2),WHITE, 0,0));
	Studio.Objects.Objects.AddTail(new CSphere(1.25,CVector( 0, 4.0, 3),BLUE,  0,0));
	
	Studio.Objects.Objects.AddTail(new CBox(CVector(-5,0,0), CVector(-4,20,5), WHITE,0,0));
	
	CCone* C = new CCone(CVector(1,3,2), CVector( 0, 4.0, 3),.5,BLUE,  0,0);
	                                
	C->Translate(CVector(1,1,1));
	                                
	Studio.Objects.Objects.AddTail(C);
*/	
/*                                   
	CSphere* S1 = new CSphere(1,CVector(0,0,0),RED ,0,0);
	CSphere* S2 = new CSphere(1,CVector(0,0,0),BLUE,0,0);
			             
	S2->Translate(CVector(0,0,0.5));
	S2->SetInside(Flip);
		             
	CIntersection* I = new CIntersection(0,0);
	I->Objects.Add(S1);
	I->Objects.Add(S2);          
	
	I->Rotate(CVector(.2,.1,.3));
	
	I->Translate(CVector(0,0,3));

	Studio.Objects.Objects.AddTail(I);
*/	
//	Studio.Objects.Objects.AddTail(S1);	
//	Studio.Objects.Objects.AddTail(S2);
	

	CBox* B = new CBox(CVector(1,1,1),CVector(0,0,0),RED,0,0);                                   
	B->Translate(CVector(-.5,-.5,0));
	B->Rotate(CVector(0,0,.1745));
	B->Scale(CVector(1,1,2));            
	B->Translate(CVector(0,5,0));
	Studio.Objects.Objects.AddTail(B);

	CBox* B1 = new CBox(CVector(1,1,1),CVector(0,0,0),Tex14,0,0);
	B1->Translate(CVector(-.5,-.5,0));
	B1->Rotate(CVector(0,0,.22));
	B1->Scale(CVector(1,1,2));
	B1->Translate(CVector(-3,5,0));
	Studio.Objects.Objects.AddTail(B1);

	CBox* B2 = new CBox(CVector(1,1,1),CVector(0,0,0),Tex13,0,0);
	B2->Translate(CVector(-.5,-.5,0));
	B2->Rotate(CVector(0,.15,.10));
	B2->Scale(CVector(1,1,2));
	B2->Translate(CVector(-3,7,1));
	Studio.Objects.Objects.AddTail(B2);

	CBox* B3 = new CBox(CVector(1,1,1),CVector(0,0,0),Tex10,0,0);
	B3->Translate(CVector(-.5,-.5,0));
	B3->Rotate(CVector(.20,0,.33));
	B3->Scale(CVector(1,1,2));
	B3->Translate(CVector(-3,3,2));
	Studio.Objects.Objects.AddTail(B3);
                                   
	CCone* C = new CCone(CLine(CVector(0,1,0)),1.0,Tex11,0,1);
//	C->Rotate(CVector(0,0,.20));
//	C->Translate(CVector(-3,3,1.2));
//	Studio.Objects.Objects.AddTail(C);
                                   
	CPlane* P = new CPlane(CVector(0,0,0.01),CVector(0,0,1),Tex8,0,0);
	P->Translate(CVector(0,0,0));
	P->Rotate(CVector(0,0,0.3491)); 
	Studio.Objects.Objects.AddTail(P);


	CIntersection* I = new CIntersection(0,0);
	I->Objects.AddTail(new CSphere(2.0,CVector( 0, 1, 0),Tex6, 0,0));
	I->Objects.AddTail(C);

	Studio.Objects.Objects.AddTail(new CSphere(2.0,CVector( -5, 4, 3),Tex5, 0,0));

	CSphere* S1=new CSphere(1.0,CVector( 0, 0, 0),Tex5, 0,0);
	S1->Translate(CVector(0,4,4));
	Studio.Objects.Objects.AddTail(S1);

	CSphere* S2=new CSphere(1.0,CVector( 0, 0, 0),Tex7, 0,0);
	S2->Translate(CVector(0,4,2));
	S2->Rotate(CVector(0,0,0.3));
	Studio.Objects.Objects.AddTail(S2);

	CSphere* S3=new CSphere(1.0,CVector( 0, 0, 0),Tex6, 0,0);
	S3->Translate(CVector(0,4,2));
	S3->Rotate(CVector(0.2,0.3,0));
	Studio.Objects.Objects.AddTail(S3);

	CGroup*	g = new CGroup(0); Studio.Objects.Objects.AddTail(g);

	for (int x=0; x<24; x++) {
		double u = 2.0 * 3.141542 * x / 24.0;
		CSphere* S4=new CSphere(0.5,CVector( 0, 0, 0),Tex6, 0,0);
		S4->Translate(CVector(0,2,(sin(4*u)+1)/4.0 +.5));
		S4->Rotate(CVector(0,0,u));
		g->Objects.AddTail(S4);
	}

	g->Translate(CVector(0,0,0));

//	CSphere* S4=new CSphere(0.5,CVector( 0, 0, 0),Tex6, 0,0);
//	S4->Translate(CVector(0,4,2));
//	S4->Rotate(CVector(0.2,0.3,0));
//	Studio.Objects.Objects.AddTail(S4);


/*
	Studio.Objects.Objects.AddTail(new CSphere(2.0,CVector( 0, 0, 2),Tex6, 0,0));

	Studio.Objects.Objects.AddTail(new CSphere(1.0,CVector(-3, 0, 2),GREEN,0,0));
	Studio.Objects.Objects.AddTail(new CSphere(1.0,CVector( 3, 0, 2),WHITE,0,0));
	Studio.Objects.Objects.AddTail(new CSphere(1.0,CVector( 0,-3, 2),WHITE,0,0));
	Studio.Objects.Objects.AddTail(new CSphere(1.0,CVector( 0, 3, 2),WHITE,0,0));

	Studio.Objects.Objects.AddTail(new CSphere(0.6,CVector(-3, 3, 2),RED,  0,0));
	Studio.Objects.Objects.AddTail(new CSphere(0.6,CVector( 3, 3, 2),BLUE, 0,0));
	Studio.Objects.Objects.AddTail(new CSphere(0.6,CVector( 3,-3, 2),GREEN,0,0));
	Studio.Objects.Objects.AddTail(new CSphere(0.6,CVector(-3,-3, 2),WHITE,0,0));
*/	
}

void SetupCameras(CObList& Cameras, CStudio& Studio, int xres, int yres) {

	CVector CamPos(3,-10,6);
	CVector FocusAt(0,-2,4);
	CVector Up(0,0,1);
	
	CVector LeftPos (CamPos-!CrossProduct(Up,CamPos-FocusAt)*.3);
	CVector RightPos(CamPos+!CrossProduct(Up,CamPos-FocusAt)*.3);

//	CVector LeftPos(CamPos);

	Cameras.AddTail(new CCamera(COptic(CLine(FocusAt-LeftPos,LeftPos),Up,40.0/2.0,40.0/2.0),&Studio,LEFT,CStorage::PNG,xres,yres));
/*
	Cameras.AddTail(new CCamera(
							COptic(
								CLine(FocusAt-RightPos,RightPos),
								Up,
								40.0/2.0,
								40.0/2.0),
							&Studio,
							RIGHT, 
							CStorage::IMG, 
							xres, 
							yres));
*/
}

void SetupStudio(CStudio& Studio) {

	SetupTextures(Studio);
	SetupLights(Studio);
	SetupObjects(Studio);

}
