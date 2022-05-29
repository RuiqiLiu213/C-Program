#pragma once
#include "Stock.h"
#include "Time.h"
#include <stdio.h>
#include <map>
#include <locale>
#include "curl/curl.h"

struct MemoryStruct;

void* myrealloc(void* ptr, size_t size);

int write_data(void* ptr, size_t size, size_t nmemb, void* data);

//derieve data of a single stock
int detrievesinglestock(CURL* handle, const string symbol, Stock& stock, const string url_common, const string api_token, const map<string, Stock> Benchmark);

//retrieve market data using libcurl
//void GetMarketData(map<string, Stock>& stks, const double* thre, map<string, Stock>& beat, map<string, Stock>& miss, map<string, Stock>& meet, const string url_common, const string api_token, const map<string, Stock> Benchmark);

//retrieve benchmark data suing libcurl
int GetBenchmark(map<string, Stock>& benchmark, const string bench, string url_common, string api_token);

//split data into Beat, Meet, Miss three groups
void Split_Group(map<string, Stock>& stks, const double* thre, map<string, Stock>& beat, map<string, Stock>& miss, map<string, Stock>& meet);

//retrieve market data using libcurl
void Get_Data(const string url_common, const string api_token, map<string, Stock>* group, const map<string, Stock> Benchmark);