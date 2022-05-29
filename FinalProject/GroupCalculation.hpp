#ifndef GroupCalculation_hpp
#define GroupCalculation_hpp

#include <stdio.h>
#include <map>
#include "Stock.h"
#include "Matrix.hpp"
using namespace std;

namespace groupCal {

// calculate the CAAR of each group
Vector cumAbRet(Vector& AAR);

// calulate the standard deviation
Vector stdev(Matrix& M, Vector& ave);

// calculate benchmark return
map<string,double> get_bench_rtn(Stock* bench);

// get corresponding benchmark return for each stock
Vector get_bench_indiv(vector<string> dailyD, map<string,double> bench_rtn);

// remove stocks whose LengthofDailyd != 2N+1
map<string, Stock> remove_stocks(map<string, Stock> stockmp, int N);

// calculate the expected AAR, AAR_STD, ave_CAAR and CAAR_STD for the group
void finalCalculation(Vector& ave_AAR, Vector& AAR_std, Vector& ave_CAAR, Vector& CAAR_std, map<string, Stock>& groupMap, int N, map<string, double>& BM_rtn);
}


#endif /* GroupCalculation_hpp */
