#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

class CVector {

public:	// Constructors

	CVector	() : x(d[0]), y(d[1]), z(d[2]) {}
	CVector (double a, double b, double c) : x(d[0]), y(d[1]), z(d[2]) { x=a; y=b; z=c; }
	CVector	(const CVector& v) : x(d[0]), y(d[1]), z(d[2]) { x=v.x; y=v.y; z=v.z; }
	CVector (double D[3]) : x(d[0]), y(d[1]), z(d[2]) {x=D[0]; y=D[1]; z=D[2]; }


public: // Services

	void	Set(const CVector &);
	void	Set(const double, const double, const double);
	void	Set(const double *);
	void	Set(int i, double u);

	double	GetX() const { return x; }
	double	GetY() const { return y; }
	double	GetZ() const { return z; }
	double	Get(int i) const;

	friend
	CVector	Normalize(const CVector &);

	CVector& Normalize();

	double	NormOf() const { return sqrt(x*x+y*y+z*z); }

	friend
	CVector	CrossProduct( const CVector &, const CVector &);



public: // Overloaded operators

	CVector	operator!() const { return ::Normalize(*this); }

    	double& operator[](int i) const { return (double &)d[i]; }

	friend
	double	operator* ( const CVector &, const CVector &);	// dotproduct    a * b

	friend
	CVector	operator* ( const double, const CVector &);		// scaling

	friend
	CVector	operator* ( const CVector &, const double );	// scaling

	friend
	CVector	operator/ ( const CVector &, const double );	// scaling by division

	friend
	CVector	operator+ ( const CVector &, const CVector &);	// vectorsum a + b

	friend
	CVector	operator- ( const CVector &, const CVector &);	// vectordifferance a - b

	CVector	operator- () const;

	CVector& operator= ( const CVector & );					// assignments
	CVector& operator= ( const double * );

private:
	double  &x,&y,&z;	// components of vector 
	double  d[3];

	double dotprod (const CVector &, const CVector &) const;


};

typedef CVector vector;

inline
void
CVector::Set(const CVector& rT) {
	x=rT.x;
	y=rT.y;
	z=rT.z;
}

inline
void
CVector::Set( const double x1, const double x2, const double x3 ) {
	x=x1;
	y=x2;
	z=x3;
}

inline
void
CVector::Set( const double * d ) {

	x = d[0];
	y = d[1];
	z = d[2];
}                                                     

inline
void
CVector::Set(int i,double u) { 
    
	if (i==0) x=u;
	if (i==1) y=u;
	if (i==2) z=u;	

}

inline
double
CVector::Get(int i) const { 
    
	if (i==0) return x;
	if (i==1) return y;
	
	return z;

}

inline
CVector&
CVector::Normalize() {

	*this=*this/NormOf();
	return *this;
}

inline
CVector
Normalize(const CVector & V) {

	return CVector(V/V.NormOf());
}

inline
double
CVector::dotprod(const CVector& v1, const CVector& v2) const {
	return ( v1.GetX() * v2.GetX() +
			 v1.GetY() * v2.GetY() +
			 v1.GetZ() * v2.GetZ() );
}

inline
double
operator* (const CVector& A, const CVector& B) {

	return A.dotprod(A, B);

}

inline
CVector
operator* (const CVector& B, const double d) {
	return d*B;
}

inline
CVector
operator* (const double d, const CVector & B) {

	return CVector (B.x*d,B.y*d,B.z*d);
}

inline
CVector
CrossProduct( const CVector& A, const CVector& B ) {

	return CVector(A.y*B.z - B.y*A.z, A.z*B.x - B.z*A.x, A.x*B.y - B.x*A.y);

}

inline
CVector
operator/ (const CVector& A, const double d) {

	return CVector(A.x/d, A.y/d, A.z/d);

}

inline
CVector
operator+ (const CVector& A, const CVector& B) {

	return CVector(A.x + B.x, A.y + B.y, A.z + B.z);

}

inline
CVector
operator- (const CVector& A, const CVector& B) {

	return CVector(A.x - B.x, A.y - B.y, A.z - B.z);

}

inline
CVector&
CVector::operator=(const double* d) {	// expr of type A = d

	x = d[0];
	y = d[1];
	z = d[2];

	return *this;
}

inline
CVector&
CVector::operator=(const CVector& T) {	// expr of type A = T

	x = T.x;
	y = T.y;
	z = T.z;

	return *this;
}

inline
CVector
CVector::operator-() const {

	return CVector (-x, -y, -z);

}

#endif
