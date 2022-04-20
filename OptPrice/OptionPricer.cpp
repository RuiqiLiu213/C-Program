#include "BinomialTreeModel02.h"
#include "Option08.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include<cmath>

using namespace std;
using namespace fre;
int main()
{
	double S0 = 106.0;
	double r = 0.058;
	double sigma = 0.46;
	double T = (double)9 / 12;
	double K = 100.0;
	int N = 8;
	double h = T / N;
	double U = exp(sigma * sqrt(h));
	double D = 1 / U;
	double R = exp(r * h);

	BinomialTreeModel Model(S0, U, D, R);
	Call call(N, K);
	OptionCalculation callCalculation(&call);
	BinLattice<double> CallPriceTree(N);
	BinLattice<bool> CallStoppingTree(N);

	cout << setiosflags(ios::fixed) << setprecision(5);
	cout<< "S0=" << S0 << endl <<"r=" << r << endl << 
		"sigma=" << sigma << endl << "T=" << T << endl << 
		"K=" << K << endl << "N=" << N << endl;
	cout << " " << endl;
	cout << "U=" << U << endl << "D=" << D << endl << "R=" << R << endl;
	cout << " " << endl;
	cout << "American call option price =" << 
		callCalculation.PriceBySnell(Model, CallPriceTree, CallStoppingTree) << endl;

	/*Output message:
	S0=106.00000
	r=0.05800
	sigma=0.46000
	T=0.75000
	K=100.00000
	N=8

	U=1.15125
	D=0.86862
	R=1.00545

	American call option price =21.68171
	*/

	return 0;
}
