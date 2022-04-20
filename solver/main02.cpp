#include <iostream>
#include <iomanip>
#include "Function02.h"
#include "NonlinearSolver02.h"
using namespace std;
using namespace fre;
int main()
{
	double Acc = 0.0001; 
	double LEnd = 0.0, REnd = 1.0;
	double P = 98.56, Guess = 0.2;
	double C = 1.2, T = 3.0, F = 100.0;

	NonlinearSolver solver(P, LEnd, REnd, Acc, Guess);
	BondPrice bondprice(C,T,F);
	cout << "Yield by bisection method:  " << fixed << setprecision(4)
		<< solver.SolveByBisect(&bondprice) << endl;
	cout << "Yield by Newton-Raphson method: " << fixed << setprecision(4)
		<< solver.SolveByNR(&bondprice) << endl;

	/*
		Output message:
		Yield by bisection method:  0.0168
		Yield by Newton-Raphson method: 0.0168
	*/

	return 0;
}

