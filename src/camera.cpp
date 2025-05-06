// 	camera.cpp
//
//	created: 	1993-09-22 J Rydin
//	revision:   A

#include <stdlib.h>

#include "camera.h"
#include "rtdefs.h"
#include "ray.h"

#if defined VERBOSE

	#include <iostream.h>
	#include <time.h>

#endif

CCamera::CCamera(const COptic& O, const CStudio* theStudio, char* FileName,
				 CStorage::ImgClass ImgType, scoord X_Res, scoord Y_Res) :
	COptic(O),
	m_Frame(FileName,ImgType,X_Res,Y_Res),
	m_Studio(theStudio) {                                     
	
	srand(4711u);

#if defined VERBOSE

	cerr << "Resolution " << X_Res << " x " << Y_Res << '\n' ;

#endif

}


//	Service functions ===>

int CCamera::Snap() {

#if defined VERBOSE

	time_t StartTime = time(NULL);

	cerr << "Rendering...";


	char Sign[4]={'|','/','-','\\'};
	int a=0;

	cerr << '\n';

#endif

	//	Scan through all lines of the screen and save
	//	each line.

	for ( m_Line = 0; m_Line < m_Frame.m_ScreenSize.Y; m_Line++ ) {

#if defined VERBOSE
		cerr << '\r' << Sign[a++%4] << ' ' << (m_Line*100)/m_Frame.m_ScreenSize.Y << '%';
#endif
		ScanLine(m_Line);
		m_Frame.StoreScanline(m_Line);

	}

	m_Frame.Close();

#if defined VERBOSE
	cerr << '\n' << "Time: " << difftime(time(NULL),StartTime);
	cerr <<	" s\n" ;
#endif

	return 1;

}

int CCamera::ScanLine(scoord Line) {

	//	Get a sample for each pixel and store in a
	//	vector.

	for ( m_Pixel = 0; m_Pixel < m_Frame.m_ScreenSize.X; m_Pixel++ )
		m_Frame.m_ScanBuffer[m_Pixel]=Sample(m_Pixel, Line);

	return m_Pixel;

}

CColor CCamera::Sample(scoord X_Coord, scoord Y_Coord) {

	//  Get a sample for the specified pixel by
	//	tracing rays in object/texture-space.
                                                                  
	CColor sample(0,0,0);
            
	unsigned int num=1;  // rand()%8+1;
	
	for (unsigned int i=0; i<num; i++) {

		double q = double(i/2)-0.5; // double(rand())/RAND_MAX-.5;
		double p = double(i%2)-0.5; // double(rand())/RAND_MAX-.5;
                                                                  
		vector d = ( 2 * ( (double(X_Coord)+q) / (m_Frame.m_ScreenSize.X-1) ) - 1 ) * U
			     + ( 2 * ( (double(Y_Coord)-p) / (m_Frame.m_ScreenSize.Y-1) ) - 1 ) * V
		    	 + Y;

		CRay Ray(d,LOC);
		
		sample+=Ray.Sample(m_Studio,1);
		
	}

	return sample/num;
	
//	return Ray.Sample(m_Studio,1);

}
