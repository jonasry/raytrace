#ifndef TRANS_H
#define TRANS_H

#include "matrix.h"
#include "ray.h"

class CTrans {
public:
  CTrans();

public:
  void translate (const CVector& t);
  void scale (const CVector& s);
  void rotate (const CVector& r);

  CVector transPoint(CVector& v) const;
  CVector invTransPoint(CVector& v) const;
  CVector transDirection(CVector& v) const;
  CVector invTransDirection(CVector& v) const;
  CVector transNormal(CVector& v) const;
  CVector invTransNormal(CVector& v) const;
  CLine   transLine(CLine& r) const;
  CLine   invTransLine(CLine& r) const;
  CRay    transRay(CRay& r) const;
  CRay    invTransRay(CRay& r) const;

private:
  Matrix m_Trans;
  Matrix m_InvTrans;

//friend int main();

};

#endif
