//	color.h
//	Color class for raytracing applications.
//
//	created: 	1993-09-22 J Rydin
//	revision:   A

#ifndef __COLOR_H__
#define __COLOR_H__

#include <math.h>
#define MAX(a,b) (((a)>(b))?(a):(b))

class CColor {

public: // Constructors:

	CColor() {}
	CColor(double r,double g,double b);
	CColor(const CColor& C);
	~CColor();


public: // Services:

	double Get(int i) { return i==0?m_Red:(i==1?m_Green:m_Blue); }
	double Red() { return m_Red; }
	double Green() { return m_Green; }
	double Blue() { return m_Blue; }
	double Gray() { return (m_Red*0.333 + m_Blue*0.333 + m_Green*0.333); }

//	friend double max(CColor C) { return MAX(C.m_Red,MAX(C.m_Green,C.m_Blue)); }



public: // Operators

	CColor operator*(const CColor& C) const;
	CColor operator*(double c) const;
	friend CColor operator*(double q, const CColor& C);

	CColor operator/(const CColor& C) const;
	CColor operator/(double c) const;
	friend CColor operator/(double q, const CColor& C);

	CColor operator+(const CColor& C) const;
	CColor operator+(double c) const;
	friend CColor operator+(double q, const CColor& C);

	CColor operator-(const CColor& C) const;
	CColor operator-(double c) const;
	friend CColor operator-(double q, const CColor& C);

	const CColor& operator=(const CColor& C);

	const CColor& operator+=(const CColor& C);


public: // Math functions

	friend CColor sqrt(const CColor& C);


private:

	double m_Red;
	double m_Green;
	double m_Blue;

};


//	CONSTRUCTORS ===>

inline
CColor::CColor(double r, double g, double b) {

	m_Red	= r;
	m_Green	= g;
	m_Blue	= b;

}

inline
CColor::CColor(const CColor& C) {

	m_Red=C.m_Red;
	m_Green=C.m_Green;
	m_Blue=C.m_Blue;

}

inline
CColor::~CColor() {}


//	OPERATORS ===>

inline
CColor CColor::operator*(const CColor& C) const {

	return CColor(m_Red*C.m_Red,m_Green*C.m_Green,m_Blue*C.m_Blue);

}

inline
CColor CColor::operator*(double c) const {

	return CColor(m_Red*c,m_Green*c,m_Blue*c);

}

inline
CColor operator*(double q, const CColor& C) {

	return C*q;

}

inline
CColor CColor::operator/(const CColor& C) const {

	return CColor(m_Red/C.m_Red,m_Green/C.m_Green,m_Blue/C.m_Blue);

}

inline
CColor CColor::operator/(double c) const {

	return CColor(m_Red/c,m_Green/c,m_Blue/c);

}

inline
CColor operator/(double q, const CColor& C) {

	return C/q;

}

inline
CColor CColor::operator+(const CColor& C) const {

	return CColor(m_Red+C.m_Red,m_Green+C.m_Green,m_Blue+C.m_Blue);

}

inline
CColor CColor::operator+(double c) const {

	return CColor(m_Red+c,m_Green+c,m_Blue+c);

}

inline
CColor operator+(double q, const CColor& C) {

	return C+q;

}

inline
CColor CColor::operator-(const CColor& C) const {

	return CColor(m_Red-C.m_Red,m_Green-C.m_Green,m_Blue-C.m_Blue);

}

inline
CColor CColor::operator-(double c) const {

	return CColor(m_Red-c,m_Green-c,m_Blue-c);

}

inline
CColor operator-(double q, const CColor& C) {

	return C-q;

}

inline
const CColor& CColor::operator+=(const CColor& C) {

	m_Red+=C.m_Red;
	m_Green+=C.m_Green;
	m_Blue+=C.m_Blue;

	return *this;

}

inline
const CColor& CColor::operator=(const CColor& C) {

	if (this !=&C) {

		m_Red=C.m_Red;
		m_Green=C.m_Green;
		m_Blue=C.m_Blue;

	}

	return *this;

}

double sqrt(double);

inline
CColor sqrt(const CColor& C) {
	return CColor(sqrt(C.m_Red),sqrt(C.m_Green),sqrt(C.m_Blue));
}


#endif
