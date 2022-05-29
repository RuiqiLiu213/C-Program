#pragma once
#include <map>
#include "Stock.h"
#include "Time.h"

//Sorting function
void insertSort(vector<double>& vec, const int length);

//read announcement information into a map<string, Stock>, and generate a vector of surprise%s, sort it, and get the threshold
void ReadCSV(map<string, Stock>& stks, double* thre, void(*sort)(vector<double>& vector, const int length), const char* cIWV3000AnnouncemnetsFile, const int N);

