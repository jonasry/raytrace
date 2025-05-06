// 	storage.h
//	File storage class for raytracing applications.
//
//	created: 	1993-09-22 J Rydin
//	revision:   A

#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <fstream>
#include "defs.h"
#include "rtdefs.h"

// Import only necessary names from std
using std::ofstream;
using std::ios;

typedef unsigned char BYTE;

class CStorage {

public:	//	Constants

	enum ImgClass { PNG = 1, TGA = 2, HDR = 3, JPG = 4 };

public:	// 	Constructors

	CStorage(char* FileName, ImgClass FileType,
			 scoord X_Resolution, scoord Y_Resolution);
	~CStorage();

public:	// 	Services

	scoord StoreScanline(scoord lineNo, CColor* Scanline);
	scoord StoreScanline(scoord lineNo) { return StoreScanline(lineNo, m_ScanBuffer); }

	void Close();

public:
	//	Picture dimensions:

	struct CSize {
		scoord X, Y;
		CSize(){}
		CSize(scoord x, scoord y) : X(x), Y(y) {}
	} m_ScreenSize;

	CColor* m_ScanBuffer;

private:// Data

	const char* FileName;
	const ImgClass IC;
	BYTE* m_ScanLineBuffer;

private:// Methods for storing

	int StoreTGA();
	int	StorePNG();
	int StoreHDR();
	int	StoreJPG();
};

#endif
