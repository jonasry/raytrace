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

	enum ImgClass { IMG = 1, TGA = 2, RAW = 3, RLE = 4 };


public:	// 	Constructors

	CStorage(char* FileName, ImgClass FileType,
			 scoord X_Resolution, scoord Y_Resolution);

	~CStorage();



public:	// 	Services

	scoord StoreScanline(CColor* Scanline);
	scoord StoreScanline() { return StoreScanline(m_ScanBuffer); }

	int operator<<(CColor* Scanline);
	
	void close() { Store.close(); }



public:
	//	Picture dimensions:

	struct CSize {
		scoord X, Y;
		CSize(){}
		CSize(scoord x, scoord y) : X(x), Y(y) {}
	} m_ScreenSize;

	CColor* m_ScanBuffer;
	BYTE*	m_ScanLine;



private:// Data

	const ImgClass IC;
	ofstream Store;


private:// Methods for storing

	int StoreTGA();
	int	StoreIMG();
	int StoreRAW();
	int	StoreRLE();

	int SetupTGA();
	int	SetupIMG();
	int SetupRAW();
	int	SetupRLE();


};

#endif
