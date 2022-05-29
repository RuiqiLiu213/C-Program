#include "GroupCalculation.hpp"
#include "Matrix.hpp"
#include "random_generator.h"
#include "StockCalculation.hpp"
#include "Stock.h"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>

//using namespace std;

namespace groupCal {

    // calculate the CAAR of each group
    Vector cumAbRet(Vector& AAR) {
        int size = (int)AAR.size();
        Vector cum_AAR(size);
        cum_AAR[0] = AAR[0];
        for (int i = 1; i < size; i++) {
            cum_AAR[i] = cum_AAR[i - 1] + AAR[i];
        }
        return cum_AAR;
    }

    // calculate the time series standard deviation
    Vector stdev(Matrix& M, Vector& ave) {
        int size = (int)ave.size();
        Vector std(size);
        Matrix tmp = pow(M - ave, 2.0);
        Vector var = rowSum(tmp) / 39.0;
        std = pow(var, 0.5);
        return std;
    }

    // calculate benchmark return
    map<string, double> get_bench_rtn(Stock* bench) {
        map<string, double> bench_rtn;
        vector<string> dailyD = bench->GetDailyd();
        Vector dailyP = bench->GetDailyp();
        int size = (int)dailyP.size();
        for (int i = 1; i < size; i++) {
            bench_rtn.insert({ dailyD[i], (dailyP[i] - dailyP[i - 1]) / dailyP[i - 1] });
        }
        return bench_rtn;
    }

    // get corresponding benchmark return for each stock
    Vector get_bench_indiv(vector<string> dailyD, map<string, double> bench_rtn) {
        int size = (int)dailyD.size();
        Vector returns(size - 1);
        for (int i = 1; i < size; i++) {
            returns[i - 1] = bench_rtn[dailyD[i]];
        }
        return returns;
    }

    // remove stocks whose LengthofDailyd != 2N+1
    map<string, Stock> remove_stocks(map<string, Stock> stockmp, int N) {
        for (auto it = stockmp.cbegin(); it != stockmp.cend() /* not hoisted */; /* no increment */)
        {
            if ((*it).second.LengthofDailyd() != 2 * N + 1)
            {
                stockmp.erase(it++);

            }
            else { ++it; }
        }
        return stockmp;
    }

    void finalCalculation(Vector& ave_AAR, Vector& AAR_std, Vector& ave_CAAR, Vector& CAAR_std, map<string, Stock>& groupMap, int N, map<string, double>& BM_rtn)
    {
        // remove stocks whose trade_dates < 2N+1 for the group
        groupMap = remove_stocks(groupMap, N);

        // initialize random_generator for this group
        rand_num_generator gen((int)groupMap.size());
        gen.set_Nums();

        // create the matrix to store the AAR/CAAR for the 40 samplings, respectively to calculate STD
        Matrix mat_AAR(40);
        Matrix mat_CAAR(40);

        for (int i = 0; i < 40; i++) {
            // get 80 random stocks from the group
            vector<int> rand_gen = gen.get_Nums();
            Vector AAR(2 * N);
            Vector CAAR(2 * N);
            for (int c = 0; c < rand_gen.size(); c++) {
                auto it_fwd = groupMap.begin();
                advance(it_fwd, rand_gen[c]);
                Vector bm_rtn_ind = get_bench_indiv(it_fwd->second.GetDailyd(), BM_rtn);
                Stock* s = &(it_fwd->second);
                StockCalculation cal(s);
                Vector daily_rtn = cal.dailyReturn();
                Vector ab_rtn = cal.abnormalReturn(bm_rtn_ind);
                AAR = AAR + ab_rtn;
            }
            AAR = AAR / 80.0;
            CAAR = cumAbRet(AAR);

            ave_AAR = ave_AAR + AAR;
            ave_CAAR = ave_CAAR + CAAR;
            mat_AAR[i] = AAR;
            mat_CAAR[i] = CAAR;
        }
        ave_AAR = ave_AAR / 40.0;
        ave_CAAR = ave_CAAR / 40.0;
        AAR_std = stdev(mat_AAR, ave_AAR);
        CAAR_std = stdev(mat_CAAR, ave_CAAR);
    }

}