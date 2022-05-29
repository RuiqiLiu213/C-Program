#include "Matrix.hpp"
#include <cmath>
#include <iomanip>
using namespace std;

Vector operator+(const Vector& V, const Vector& W)
{
   int d = (int)V.size();
   Vector U(d);
   for (int j=0; j<d; j++)
      U[j] = V[j] + W[j];
   return U;
}

Vector operator-(const Vector& V, const Vector& W)
{
   int d = (int)V.size();
   Vector U(d);
   for (int j=0; j<d; j++)
      U[j] = V[j] - W[j];
   return U;
}

//Vector operator-(const double& a, const Vector& V)
//{
//   int d = (int)V.size();
//   Vector U(d);
//   for (int j=0; j<d; j++)
//      U[j] = V[j] - a;
//   return U;
//}

Vector operator/(const Vector& V, const double& a)
{
   int d = (int)V.size();
   Vector U(d);
   for (int j=0; j<d; j++)
      U[j] = V[j]/a;
   return U;
}

Vector pow(const Vector& V, const double& n)
{
   int d = (int)V.size();
   Vector U(d);
   for (int j=0; j<d; j++)
      U[j] = std::pow(V[j],n);
   return U;
}

Matrix pow(const Matrix& M, const double& n)
{
   int size = (int)M.size();
   Matrix Mat(size);
   for (int j=0; j<size; j++)
      Mat[j] = pow(M[j],n);
   return Mat;
}

Vector rowSum(Matrix& M)
{
   int d = (int)M[0].size();
   Vector U(d);
   for (Matrix::iterator itr = M.begin(); itr!=M.end(); itr++)
   {
         U = U + *itr;
   }
   return U;
}

Matrix operator-(const Matrix& M, const Vector& V)
{
   int size = (int)M.size();
   Matrix Mat(size);
   for (int j=0; j<size; j++)
      Mat[j] = M[j] - V;
   return Mat;
}

ostream& operator<<(ostream& out, Vector& V)
{
   for (Vector::iterator itr=V.begin(); itr != V.end(); itr++)
      out << fixed << setprecision(4) << *itr << " ";
   out << endl;
   return out;
}

ostream& operator<<(ostream& out, Matrix& M)
{
   for (Matrix::iterator itr = M.begin(); itr!=M.end(); itr++)
      out << *itr;
   out << endl;
   return out;
}


//vector<double> operator- (vector<double> const& v1, vector<double> const& v2)
//{
//    assert(v1.size() == v2.size());
//    std::vector<double> result;
//    result.reserve(v1.size());
//
//    for (unsigned i = 0; i < v1.size(); i++)
//    {
//        result.push_back(v1[i] - v2[i]);
//    }
//
//    return result;
//}
//
//map<string, vector<double>> Return(map<string, vector<double>>& p)
//{
//    map<string, vector<double>> result;
//
//    map<string, vector<double>>::iterator it;
//
//    for (it = p.begin(); it != p.end(); it++) {
//        vector<double> Return;
//        int size = (it->second).size();
//
//        for (int i = 1; i < size; i++) {
//
//            Return.push_back((it->second)[i] / (it->second)[i - 1] - 1);
//
//        }
//
//        result[(it->first)] = Return;
//
//    }
//    return result;
//
//}
