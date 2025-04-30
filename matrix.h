#ifndef MATRIX_H
#define MATRIX_H

class CVector;

class Matrix {
public:
  // uninitialized matrix
  Matrix();

  // matrix with main diagonal set to same number, 
  // otherwise zero
  Matrix(double);

  // matrix with main diagonal set to a vector,
  // otherwize zero
  Matrix(const CVector &);

  // matrix set by three vectors
  Matrix(const CVector &, const CVector &, const CVector &, const CVector &);

  // matrix set by the elements of another matrix
  Matrix(const Matrix &);

  ~Matrix();

public:
  Matrix operator -(const Matrix &) const;
  Matrix operator +(const Matrix &) const;
  Matrix operator *(const Matrix &) const;
  
  const Matrix& operator -=(const Matrix &);
  const Matrix& operator +=(const Matrix &);
  const Matrix& operator *=(const Matrix &);

  const Matrix& operator  =(const Matrix &);

  double& operator ()(int, int) const;


public:
  const Matrix& setZero();
  const Matrix& setIdentity();
  const Matrix& setElement(int,int,double);
  const Matrix& set(const Matrix &);

  int isZero() const;
  int isIdentity() const;
  int isIdenticalTo(const Matrix &) const;

  const Matrix& transpose();

public:
  static Matrix ZeroMatrix;
  static Matrix IdentityMatrix;

  friend int Multiply(Matrix& Operand1, Matrix& Operand2,
  			 Matrix& Answer);

  friend int Multiply(Matrix& Operand1, Matrix& Operand2, 
		     Matrix& Operand3, 
		     Matrix& Answer);

  friend int Multiply(Matrix& Operand1, Matrix& Operand2, 
		     Matrix& Operand3, Matrix& Operand4, 
		     Matrix& Answer);

  friend int Multiply(Matrix& Operand1, Matrix& Operand2, 
		     Matrix& Operand3, Matrix& Operand4,
		     Matrix& Operand5, 
		     Matrix& Answer);

  friend int identical(const Matrix &, const Matrix &); 

public: // DEBUG UTILS

  void print(char* text=0) const;

private:
  double** m_Body;
  
};

#endif
