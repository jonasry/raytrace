// 	camera.h
//	Camera class for raytracing applications.
//
//	created: 	1993-09-21 J Rydin
//	revision:   A

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "color.h"
#include "optic.h"
#include "storage.h"
#include "studio.h"
#include <string>

// using namespace std; // removed to avoid std::vector conflict

class CCamera : public COptic {

public: // Contructor

    CCamera(const COptic& O, const CStudio* theStudio,
            const std::string& FileName,
            CStorage::ImgClass ImgType, scoord X_Res, scoord Y_Res);


public:	// High-level interface ===>

	int 	Snap();


public:	// Low-level interface	===>

	int		ScanLine(scoord Line);		//	Get a sample for a specified line
	CColor	Sample(scoord X, scoord Y);	//	Get a sample for a specified pixel

public: // General services

	scoord Line() const { return m_Line; }
	scoord Pixel() const { return m_Pixel; }

	CStorage	m_Frame;

//	virtual void Serialize(CArchive A);

private: // Data

	const CStudio* m_Studio;

	scoord m_Line;
	scoord m_Pixel;
	
};


#endif	// __CAMERA_H__
