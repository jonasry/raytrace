//	Test application for ray lib
#include "testapp.h"
#include <csignal>
#include <execinfo.h>
#include <unistd.h>    // for STDERR_FILENO
#include <cstdlib>
#include <iostream>
#include <string>
#include "SceneLoader.hpp"
using namespace std;

// Signal handler to print stack trace on crash
static void crashHandler(int sig) {
    void* bt[20];
    int n = backtrace(bt, 20);
    backtrace_symbols_fd(bt, n, STDERR_FILENO);
    exit(128 + sig);
}

// Main entry point
int main(int argc, char* argv[]) {
    signal(SIGSEGV, crashHandler);
    signal(SIGABRT, crashHandler);
    signal(SIGFPE,  crashHandler);
    signal(SIGTRAP, crashHandler);
    return RTmain(argc, argv);
}

int RTmain(int argc, char* argv[]) {

	cout << "Raytrace ";

#ifdef _DEBUG
	cout << "DEBUG version " << VER << endl;
#else
	cout << "version " << VER << endl;
#endif

	int xres, yres;

    CInterface RT_Interface;
    // If a single YAML file is provided, load scene from file
    if (argc == 2) {
        std::string arg(argv[1]);
        auto has_ext = [&](const std::string& s, const std::string& ext) {
            return s.size() > ext.size() && s.substr(s.size() - ext.size()) == ext;
        };
        if (has_ext(arg, ".yaml") || has_ext(arg, ".yml")) {
            if (!SceneLoader::load(arg, RT_Interface.Studio, RT_Interface.Cameras)) {
                std::cerr << "Failed to load scene file: " << arg << std::endl;
                return 1;
            }
            RT_Interface.SnapAll();
            return 0;
        }
        // otherwise treat as resolution
        xres = yres = atoi(argv[1]);
    } else {
        xres = 200;
        yres = 160;
    }

	SetupStudio(RT_Interface.Studio);
	SetupCameras(RT_Interface.Cameras,RT_Interface.Studio, xres, yres);

	RT_Interface.SnapAll();

	return 0;
}
