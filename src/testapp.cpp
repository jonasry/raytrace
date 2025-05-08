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
	} else {
		xres=yres=atoi(argv[1]);
	}

	CInterface RT_Interface;

	SetupStudio(RT_Interface.Studio);
	SetupCameras(RT_Interface.Cameras,RT_Interface.Studio, xres, yres);

	RT_Interface.SnapAll();

	return 0;
}
