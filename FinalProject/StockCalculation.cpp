#include "StockCalculation.hpp"
#include "Matrix.hpp"
using namespace std;

// calculate the daily return of a single stock
Vector StockCalculation::dailyReturn() {
   Vector dailyP = stock -> GetDailyp();
   int size = (int)dailyP.size();
   Vector returns(size-1);
   for (int i=1; i<size; i++)
   {
      returns[i-1] = (dailyP[i] - dailyP[i-1])/dailyP[i-1];
   }
   return returns;
}

// calculate the cumulative daily return of a single stock
Vector StockCalculation::cum_dailyRet(Vector& daily_ret) {
   int size = (int)daily_ret.size();
   Vector cum_daily(size);
   cum_daily[0] = daily_ret[0];
   for (int i=1; i<size; i++) {
      cum_daily[i] = cum_daily[i-1] + daily_ret[i];
   }
   return cum_daily;
}

// calculate the abnormal return for a single stock
Vector StockCalculation::abnormalReturn(Vector& bench_ret) {
   int size = (int)bench_ret.size();
   Vector ab_ret(size);
   ab_ret = this->dailyReturn() - bench_ret;
   return ab_ret;
}








