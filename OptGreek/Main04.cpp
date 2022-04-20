#include <iostream>
#include "PathDepOption.h"
#include "GmtrAsianCall.h"

using namespace std;
using namespace fre;

int main()
{
    double S0=100.0, r=0.03, sigma=0.2;
    MCModel Model(S0,r,sigma); //

    double T =1.0/12.0, K=100.0;
    int m=30;

    ArthmAsianCall Option(T,K,m); // main object
    GmtrAsianCall  CVOption(T,K,m); //

    long N=30000;
    double epsilon =0.001;

    /*
    Option.PriceByVarRedMC(Model,N,CVOption,epsilon);
    cout << "Arithmetic call price = " << Option.GetPrice() << endl
        << "Error = " << Option.GetPricingError() << endl << endl;
    */
    Option.PriceByMC(Model,N,epsilon);
    cout << "Asian Call Price = " << Option.GetPrice() << endl
        << " Pricing Error = " << Option.GetPricingError() << endl 
        << " delta = " << Option.GetDelta() << endl
        << " gamma = " << Option.GetGamma() << endl << endl;
    return 0;
}

/*
Output message:
Asian Call Price = 1.43103
 Pricing Error = 0.0120318
 delta = 0.529087
 gamma = 0.116708
*/

