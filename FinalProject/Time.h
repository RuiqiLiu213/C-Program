#pragma once
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <iostream>
#include <ctime>

using namespace std;

// Adjust date by a number of days +/-
void DatePlusDays(struct tm* date, int days);

//Adjust announcement date as the format "2022-03-01"
void ANNDateStandard(string& day);

//Adjust announcement date to previous date
void ANNDateMinusOne(string& day);

