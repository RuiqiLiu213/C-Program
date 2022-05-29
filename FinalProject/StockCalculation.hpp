#pragma once
#ifndef StockCalculation_hpp
#define StockCalculation_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "Stock.h"
#include "Matrix.hpp"
using namespace std;

class StockCalculation {
   protected:
      Stock* stock;
      
   public:
      StockCalculation(Stock* stock_) {
         stock = stock_;
      }
      
      Vector dailyReturn();
      Vector cum_dailyRet(Vector& daily_ret);
      Vector abnormalReturn(Vector& bench_ret);
      //Vector cum_aveAbRet(Vector& ab_ret);
   };


#endif /* StockCalculation_hpp */
