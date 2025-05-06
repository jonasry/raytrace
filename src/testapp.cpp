//	Test application for ray lib
#include "testapp.h"
#include <csignal>
#include <execinfo.h>
#include <unistd.h>    // for STDERR_FILENO
#include <cstdlib>
#include <iostream>
using namespace std;

// Signal handler to print stack trace on crash
static void crashHandler(int sig) {
    void* bt[20];
    int n = backtrace(bt, 20);
    backtrace_symbols_fd(bt, n, STDERR_FILENO);
    exit(128 + sig);
}

void Catcher(int *reglist) {

//	cerr << '\n' << Cam->Line() << ' ' << Cam->Pixel();

	*(reglist + 8) = 3;
}

// Main entry point
int main(int argc, char* argv[]) {
    signal(SIGSEGV, crashHandler);
    signal(SIGABRT, crashHandler);
    signal(SIGFPE,  crashHandler);
    signal(SIGTRAP, crashHandler);
    return RTmain(argc, argv);
}
/*
int matherr (struct exception* a) {
//	cerr << '\n' << Cam->Line() << ' ' << Cam->Pixel() << ':';
	cerr << '\n' << a->name << ": " << a->arg1 << ' ' << a->arg2;
	exit(1);
	return 0;
}
*/
int RTmain(int argc, char* argv[]) {

	cout << "Raytrace ";

#ifdef _DEBUG
	cout << "DEBUG version " << VER << endl;
#else
	cout << "version " << VER << endl;
#endif

	int xres, yres;

	if (argc != 2) {
		xres=200;
		yres=160;
	}
	else
		xres=yres=atoi(argv[1]);
 

//	signal(SIGFPE, (fptr)Catcher);
	

//	-------------------------------------------------------------------------
/*
	ifstream InFile("c:\\dump");
	
	//	read the number of points:
	
	double P; InFile >> P;
	int NumPoints = (int) P;
	    
	char X[20], Y[20], Z[20];
	double x, y, z;
	
	cout << NumPoints << " points" << endl;
	
	vector* Loc = new vector[NumPoints];
	    
	for (int i=0; i<NumPoints; i++) {
	
		InFile >> X >> Y >> Z;
		
		if(strcmp(X,"-nan")==0) strcpy(X,"-1.0e50");
		if(strcmp(X,"nan")==0) strcpy(X,"1.0e50");
		
		if(strcmp(Y,"-nan")==0) strcpy(Y,"-1.0e50");
		if(strcmp(Y,"nan")==0) strcpy(Y,"1.0e50");
		
		if(strcmp(Z,"-nan")==0) strcpy(Z,"-1.0e50");
		if(strcmp(Z,"nan")==0) strcpy(Z,"1.0e50");
		
		x=atof(X); y=atof(Y); z=atof(Z);
		
		Loc[i]=CVector(x,y,z);
		
		//cout << Loc[i].GetX() << endl;
		
	}
	
	vector* Vel = new vector[NumPoints];
	    
	for (i=0; i<NumPoints; i++) {
	
		InFile >> X >> Y >> Z;
		
		if(strcmp(X,"-nan")==0) strcpy(X,"-1.0e50");
		if(strcmp(X,"nan")==0) strcpy(X,"1.0e50");
		
		if(strcmp(Y,"-nan")==0) strcpy(Y,"-1.0e50");
		if(strcmp(Y,"nan")==0) strcpy(Y,"1.0e50");
		
		if(strcmp(Z,"-nan")==0) strcpy(Z,"-1.0e50");
		if(strcmp(Z,"nan")==0) strcpy(Z,"1.0e50");
		
		x=atof(X); y=atof(Y); z=atof(Z);
		
		vector v=CVector(x,y,z);

		if(v.NormOf()!=0)
			Vel[i]=Normalize(v);
		else
			Vel[i]=CVector(1,0,0);
		
//		cout << Vel[i].GetX() << ' ' << Vel[i].GetY() << ' ' << Vel[i].GetZ() << ' ' << endl; 
	}
	
	vector* Up = new vector[NumPoints];
	    
	for (i=0; i<NumPoints; i++) {
	
		InFile >> X >> Y >> Z;
		
		if(strcmp(X,"-nan")==0) strcpy(X,"-1.0e50");
		if(strcmp(X,"nan")==0) strcpy(X,"1.0e50");
		
		if(strcmp(Y,"-nan")==0) strcpy(Y,"-1.0e50");
		if(strcmp(Y,"nan")==0) strcpy(Y,"1.0e50");
		
		if(strcmp(Z,"-nan")==0) strcpy(Z,"-1.0e50");
		if(strcmp(Z,"nan")==0) strcpy(Z,"1.0e50");
		
		x=atof(X); y=atof(Y); z=atof(Z);
		
		Up[i]=Normalize(CVector(x,y,z));
		
	}
	
	InFile.close();
*/		

	CInterface RT_Interface;

	SetupStudio(RT_Interface.Studio);
	SetupCameras(RT_Interface.Cameras,RT_Interface.Studio, xres, yres);

	RT_Interface.SnapAll();
		

	return 0;

}
