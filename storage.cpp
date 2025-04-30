#include "mstorage.h"
#include "color.h"

#if defined VERBOSE
	#include <iostream.h>
#endif

CStorage::CStorage(char* FileName, ImgClass FileType,
		 scoord X_Resolution, scoord Y_Resolution)
		 :	IC(FileType),
		 	m_ScreenSize(CSize(X_Resolution,Y_Resolution)),
		 	Store(FileName,ios::binary|ios::out) 
{

		m_ScanLine=new BYTE[X_Resolution*3];
		m_ScanBuffer=new CColor[X_Resolution];

		switch(IC) {
			case IMG: SetupIMG(); break;
			case TGA: SetupTGA(); break;
			default: break;
		}

		#if defined VERBOSE
			cerr << "Storing on file " << FileName << '\n' ;
		#endif


}

CStorage::~CStorage() {

	delete[] m_ScanBuffer;
	delete[] m_ScanLine;
	
	#if defined VERBOSE
		cerr << "Closing file " << endl;
	#endif
	
	Store.close();

}

scoord CStorage::StoreScanline(CColor* line) {

	//	Temporary implementation of CStorage::StoreScanline

	scoord i;

	for (i=0; i<m_ScreenSize.X; i++) {

		short rr = short(floor(double(line[i].Red()) * double(255) + 0.5 ) );
		short gg = short(floor(double(line[i].Green()) * double(255) + 0.5 ) );
		short bb = short(floor(double(line[i].Blue()) * double(255) + 0.5 ) );

		m_ScanLine[i*3  ]=(rr < 255) ? (BYTE) rr : (BYTE) 255;
		m_ScanLine[i*3+1]=(gg < 255) ? (BYTE) gg : (BYTE) 255;
		m_ScanLine[i*3+2]=(bb < 255) ? (BYTE) bb : (BYTE) 255;

	}

	switch(IC) {
		case IMG: StoreIMG(); break;
		case TGA: StoreTGA(); break;
	default: break;
	}

	return i;

}

int	CStorage::SetupIMG() {

	//	Header:

	Store.put((BYTE)((m_ScreenSize.X&0xff00)>>8));
	Store.put((BYTE)(m_ScreenSize.X&0x00ff));
	Store.put((BYTE)((m_ScreenSize.Y&0xff00)>>8));
	Store.put((BYTE)(m_ScreenSize.Y&0x00ff));
	Store.put((BYTE) 0);
	Store.put((BYTE) 0);
	Store.put((BYTE)((m_ScreenSize.Y&0xff00)>>8));
	Store.put((BYTE)(m_ScreenSize.Y&0x00ff));
	Store.put((BYTE) 0);
	Store.put((BYTE) 24);

	Store.flush();

	return 1;

}

int	CStorage::SetupTGA() {

	//	Header:

	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(2));
	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(0));
	Store.put((BYTE)(m_ScreenSize.X&0x00ff));
	Store.put((BYTE)((m_ScreenSize.X&0xff00)>>8));
	Store.put((BYTE)((m_ScreenSize.Y)&0x00ff));
	Store.put((BYTE)((m_ScreenSize.Y&0xff00)>>8));
	Store.put((BYTE)(24));
	Store.put((BYTE)(32));

	Store.flush();

	return 1;

}

int CStorage::StoreIMG() { //temporary

	BYTE qw=1;
	BYTE qx, qy, qz;

	for (unsigned short kk=0; kk<3*(m_ScreenSize.X-1); kk+=3) {

		qx=m_ScanLine[kk  ];
		qy=m_ScanLine[kk+1];
		qz=m_ScanLine[kk+2];

		if( ( qx == m_ScanLine[kk+3] ) &&
			( qy == m_ScanLine[kk+4] ) &&
			( qz == m_ScanLine[kk+5] ) && (qw < 255) )

			qw++;	//	same color repeated, inc count

		else {
			Store.put(qw);
			Store.put(qz);
			Store.put(qy);
			Store.put(qx);	//	save with count
			qw=1;

		}

	}

	qx = m_ScanLine[ 3*m_ScreenSize.X-3 ];	//	last entry
	qy = m_ScanLine[ 3*m_ScreenSize.X-2 ];	//	last entry
	qz = m_ScanLine[ 3*m_ScreenSize.X-1 ];	//	last entry

	Store.put(qw);
	Store.put(qz);
	Store.put(qy);
	Store.put(qx);	//	save with count

	return 0;

}

int CStorage::StoreTGA() {

	BYTE qx, qy, qz;

	for (unsigned short kk=0; kk<3*m_ScreenSize.X; kk+=3) {

		qx=m_ScanLine[kk  ];
		qy=m_ScanLine[kk+1];
		qz=m_ScanLine[kk+2];

		Store.put(qz);
		Store.put(qy);
		Store.put(qx);	//	save with count

	}

	return 0;
}
