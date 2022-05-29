#ifndef Matrix_hpp
#define Matrix_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

typedef vector<double> Vector;
typedef vector<Vector> Matrix;

// define the type of the required matrix as Result_Mat
typedef vector<vector<vector<double>>> Result_Mat;

// overloaded operators for calculation
Vector operator+(const Vector& V, const Vector& W);
Vector operator-(const Vector& V, const Vector& W);
Vector operator/(const Vector& V, const double& a);

// overloaded operators for calculating the standard deviation

// to take the square root in the last step
Vector pow(const Vector& V, const double& n);

// to take the square of each (x-ave)
Matrix pow(const Matrix& M, const double& n);

// to calculate the elementwise sum of each row of the matrix
Vector rowSum(Matrix& M);

// to subtract each row of the matrix by a single vector elementwisely
Matrix operator-(const Matrix& M, const Vector& V);

// to output Vector and Matrix if necessary
ostream& operator<<(ostream& out, Vector& V);
ostream& operator<<(ostream& out, Matrix& M);
#endif /* Matrix_hpp */
