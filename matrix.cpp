#include "matrix.h"
#include <iostream.h>

Matrix Matrix::ZeroMatrix;
Matrix Matrix::IdentityMatrix(1);

typedef double* pDouble;

Matrix::~Matrix() {

  for (int i=0; i<4; i++)
    delete[] m_Body[i];

  delete[] m_Body;

}

Matrix::Matrix() {

  m_Body=new pDouble[4];

  for (int i=0; i<4; i++)
    m_Body[i]=new double[4];

}

Matrix::Matrix(double d) {

  m_Body=new pDouble[4];

  for (int i=0; i<4; i++) {
    m_Body[i]=new double[4];
    for (int j=0; j<4; j++) {
      if (i==j) {
	m_Body[i][j]=d;
      }
      else {
	m_Body[i][j]=0.0;
      }
    }
  }
}

Matrix::Matrix(const Matrix& M) {

  m_Body=new pDouble[4];
  for (int i=0; i<4; i++) {
    m_Body[i]=new double[4];
    for (int j=0; j<4; j++)
	m_Body[i][j]=M.m_Body[i][j];
  }

}

Matrix Matrix::operator -(const Matrix& M) const {

  Matrix Result;

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      Result.m_Body[i][j]=this->m_Body[i][j] - M.m_Body[i][j];

  return Result;

}

Matrix Matrix::operator +(const Matrix& M) const {

  Matrix Result;

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      Result.m_Body[i][j]=this->m_Body[i][j] + M.m_Body[i][j];

  return Result;

}

Matrix Matrix::operator *(const Matrix& M) const {

  Matrix Result;

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++) {
      Result.m_Body[i][j]=0.0;
      for (int k=0; k<4; k++)
	Result.m_Body[i][j]+=this->m_Body[i][k] * M.m_Body[k][j];
    }

  return Result;

}

const Matrix& Matrix::operator -=(const Matrix& M) {

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      this->m_Body[i][j] -= M.m_Body[i][j];

  return *this;

}

const Matrix& Matrix::operator +=(const Matrix& M) {

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      this->m_Body[i][j] += M.m_Body[i][j];

  return *this;

}

const Matrix& Matrix::operator *=(const Matrix& M) {

  Matrix Result;

  //  this->print("this");

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++) {
      Result.m_Body[i][j]=0.0;
      for (int k=0; k<4; k++)
	Result.m_Body[i][j]=this->m_Body[i][k] * M.m_Body[k][j];
    }

  for (i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      this->m_Body[i][j] = Result.m_Body[i][j];
  
  return *this;

}

const Matrix& Matrix::operator =(const Matrix& M) {

 return this->set(M);

}

double& Matrix::operator ()(int i, int j) const {

  return this->m_Body[i][j];

}

const Matrix& Matrix::setZero() {

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      this->m_Body[i][j] = 0.0;
  
  return *this;  

}

const Matrix& Matrix::setIdentity() {

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      if (i==j) {
	this->m_Body[i][j] = 1.0;
      }
      else {
	this->m_Body[i][j] = 0.0;
      }
  
  return *this;  

}

const Matrix& Matrix::setElement(int i, int j, double d) {

  this->m_Body[i][j]=d;

  return *this;

}

const Matrix& Matrix::set(const Matrix& M) {

  if (&M==this)
    return *this;

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      this->m_Body[i][j] = M.m_Body[i][j];
  
  return *this;  

}

int Matrix::isZero() const {

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      if (this->m_Body[i][j] != 0.0)
	return 0;
  
  return 1;  

}

int Matrix::isIdentity() const {

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      if (i==j) {
	if (this->m_Body[i][j] != 1.0)
	  return 0;
      }
      else {
  	if (this->m_Body[i][j] != 0.0)
	  return 0;
      }

  return 1;  

}

int Matrix::isIdenticalTo(const Matrix& M) const {

  for (int i=0; i<4; i++) 
    for (int j=0; j<4; j++)
      if (this->m_Body[i][j] != M.m_Body[i][j]) {
	return 0;
      }
      
  return 1;  

}

const Matrix& Matrix::transpose() {

  for (int i=0; i<3; i++) {
    for (int j=i+1; j<4; j++) {
      double tmp=this->m_Body[i][j];
      this->m_Body[i][j]=this->m_Body[j][i];
      this->m_Body[j][i]=tmp;
    }
  }

  return *this;

}

int Multiply(Matrix& Operand1, Matrix& Operand2, Matrix& Operand3, Matrix& Answer) {

  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      Answer.m_Body[i][j]=0.0;
      for (int k=0; k<4; k++) {
	double tmp=0.0;
	for(int l=0; l<4; l++) {
	  tmp+=Operand1.m_Body[i][l] * Operand2.m_Body[l][k];
	}
	Answer.m_Body[i][j]+=tmp*Operand3.m_Body[k][j];
      }
    }
  }
  
  return 1;

}


