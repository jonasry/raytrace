#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "storage.h"
#include "color.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


CStorage::CStorage(const std::string& fileName, ImgClass FileType,
		 scoord X_Resolution, scoord Y_Resolution)
		 :	m_ScreenSize(CSize(X_Resolution, Y_Resolution)),
		 	FileName(fileName),
		 	IC(FileType)
{
	const char* outputDir = std::getenv("RT_OUTPUT");
	std::string directory = outputDir ? outputDir : "output";
	fs::create_directories(directory);
	
	std::string fullPath = directory + "/" + FileName;
	FileName = fullPath;
	
	// allocate scan buffers using STL vectors
	m_ScanBuffer.resize(X_Resolution);
	m_ScanLineBuffer.resize(X_Resolution * Y_Resolution * 3);

	std::cerr << "Writing to " << FileName << '\n' ;
}

CStorage::~CStorage() {

	// buffers are managed by std::vector, no manual delete needed
	
	#if defined VERBOSE
		cerr << "Closing file " << endl;
	#endif

}

scoord CStorage::StoreScanline(scoord lineNo, CColor* line) {
	scoord i;
	int offset = lineNo * m_ScreenSize.X * 3;

	for (i = 0; i < m_ScreenSize.X; i++) {
		short rr = short(floor(double(line[i].Red()) * 255.0 + 0.5));
		short gg = short(floor(double(line[i].Green()) * 255.0 + 0.5));
		short bb = short(floor(double(line[i].Blue()) * 255.0 + 0.5));
	
		m_ScanLineBuffer[offset + i * 3 + 0] = (rr < 255) ? (BYTE)rr : 255;
		m_ScanLineBuffer[offset + i * 3 + 1] = (gg < 255) ? (BYTE)gg : 255;
		m_ScanLineBuffer[offset + i * 3 + 2] = (bb < 255) ? (BYTE)bb : 255;
	}

	return i;

}

void CStorage::Close() {
	#if defined VERBOSE
		cerr << "Storing to file " << FileName << '\n' ;
	#endif
	switch(IC) {
		case PNG: StorePNG(); break;
		case TGA: StoreTGA(); break;
		case HDR: StoreHDR(); break;
		case JPG: StoreJPG(); break;
		default: break;
	}
}

int CStorage::StorePNG() {
    return stbi_write_png(FileName.c_str(), m_ScreenSize.X, m_ScreenSize.Y, 3,
                         m_ScanLineBuffer.data(), m_ScreenSize.X * 3);
}

int CStorage::StoreTGA() {
	return 0; //stbi_write_tga(FileName, m_ScreenSize.X, m_ScreenSize.Y, 3, m_ScanLineBuffer, m_ScreenSize.X * 3);
}

int CStorage::StoreHDR() {
	return 0; //stbi_write_hdr(FileName, m_ScreenSize.X, m_ScreenSize.Y, 3, m_ScanLineBuffer, m_ScreenSize.X * 3);
}

int CStorage::StoreJPG() {
    return stbi_write_jpg(FileName.c_str(), m_ScreenSize.X, m_ScreenSize.Y, 3,
                         m_ScanLineBuffer.data(), m_ScreenSize.X * 3);
}
