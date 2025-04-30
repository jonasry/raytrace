#include "trans.h"
#include "vector.h"

extern "C" {
  double cos(double);
  double sin(double);
}

CTrans::CTrans() : m_Trans(1), m_InvTrans(1) {}

void CTrans::translate(const CVector& t) {

  Matrix T(1), I(1);

  for (int i=0; i<3; i++) {
    T(i,3)=0.0+t[i];
    I(i,3)=0.0-t[i];
  }

  m_Trans = m_Trans*T;
  m_InvTrans=I*m_InvTrans;

  //  m_Trans.print("Translate");
  //  m_InvTrans.print("Translate, inverse");  
  
}

void CTrans::scale(const CVector& s) {

  Matrix S(1), I(1);

  for (int i=0; i<3; i++) {
    S(i,i)=s[i];
    I(i,i)=1.0/s[i];
  }

  m_Trans = m_Trans*S;
  m_InvTrans=I*m_InvTrans;

  //  m_Trans.print("Scale");
  //  m_InvTrans.print("Scale, inverse");

}

void CTrans::rotate(const CVector& r) {

  Matrix Rx(1), Ix(1);
  Matrix Ry(1), Iy(1);
  Matrix Rz(1), Iz(1);

  double cos_x = cos(r[0]);
  double sin_x = sin(r[0]);
  double cos_y = cos(r[1]);
  double sin_y = sin(r[1]);
  double cos_z = cos(r[2]);
  double sin_z = sin(r[2]);


  // X rotation

  Rx(1,1)=cos_x;
  Rx(2,2)=cos_x;
  Rx(1,2)=sin_x;
  Rx(2,1)=0.0-sin_x;

  m_Trans=m_Trans*Rx;
  m_InvTrans=Rx.transpose()*m_InvTrans;


  // Y rotation

  Ry(0,0)=cos_y;
  Ry(2,2)=cos_y;
  Ry(0,2)=sin_y;
  Ry(2,0)=0.0-sin_y;

  m_Trans=m_Trans*Ry;
  m_InvTrans=Ry.transpose()*m_InvTrans;


  // Z rotation

  Rz(0,0)=cos_z;
  Rz(1,1)=cos_z;
  Rz(0,1)=sin_z;
  Rz(1,0)=0.0-sin_z;

  m_Trans=m_Trans*Rz;
  m_InvTrans=Rz.transpose()*m_InvTrans;

  //  m_Trans.print("Rotate");
  //  m_InvTrans.print("Rotate, inverse");

}

// void CTrans::rotate(double* r, double v) {}

CVector CTrans::transPoint(CVector& v) const {

  double tmp[3];

  for (int i=0; i<3; i++)
    tmp[i]=v[i];

  for (i=0; i<3; i++) {
    v[i]=0.0;
    for (int j=0; j<3; j++) 
      v[i] += m_Trans(i,j)*tmp[j];
    v[i]+=m_Trans(i,3);
  }

  return v;

}

CVector CTrans::invTransPoint(CVector& v) const {

  double tmp[3];

  for (int i=0; i<3; i++)
    tmp[i]=v[i];

  for (i=0; i<3; i++) {
    v[i]=0.0;
    for (int j=0; j<3; j++) 
      v[i] += m_InvTrans(i,j)*tmp[j];
    v[i]+=m_InvTrans(i,3);
  }

  return v;

}
 
CVector CTrans::transDirection(CVector& v) const {

  double tmp[3];

  for (int i=0; i<3; i++)
    tmp[i]=v[i];

  for (i=0; i<3; i++) {
    v[i]=0.0;
    for (int j=0; j<3; j++) 
      v[i] += m_Trans(i,j)*tmp[j];
    //v[i]+=m_Trans(i,3);
  }

  return v;

}

CVector CTrans::invTransDirection(CVector& v) const {

  double tmp[3];

  for (int i=0; i<3; i++)
    tmp[i]=v[i];

  for (i=0; i<3; i++) {
    v[i]=0.0;
    for (int j=0; j<3; j++) 
      v[i] += m_InvTrans(i,j)*tmp[j];
    //v[i]+=m_InvTrans(i,3);
  }

  return v;

}

CLine CTrans::transLine(CLine& l) const {

  CVector dir(l.Dir()), loc(l.Loc());

  transDirection(dir);
  transPoint(loc);
  
  l.Set(dir,loc);

  return l;
  
}

CLine CTrans::invTransLine(CLine& l) const {

  CVector dir(l.Dir()), 
  		  loc(l.Loc());

  invTransDirection(dir);
  invTransPoint(loc);
  
  l.Set(dir,loc);

  return l;

}

CRay CTrans::transRay(CRay& r) const {

  CVector dir(r.Dir()), loc(r.Loc());

  transDirection(dir);
  transPoint(loc);
  
  r.Set(dir,loc);

  return r;

}

CRay CTrans::invTransRay(CRay& r) const {

  CVector dir(r.Dir()), 
  		  loc(r.Loc());

  invTransDirection(dir);
  invTransPoint(loc);
  
  r.Set(dir,loc);

  return r;

}

