#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Stock {
private:
	string ANNDate; //earning announcemnet date
	string EndDate; //period ending
	double Est; //estimate
	double Rep; //reported
	double Sur; //surprise
	double Surper; //surprise%
	int N; //total 2N+1 of days

	string Group; //group the stock belongs to
	vector<string> Dailyd; // trading days
	vector<double> Dailyp; //daily adjusted close price
public:
	Stock() :ANNDate("None"), EndDate("None"), Est(0.0), Rep(0.0), Sur(0.0), Surper(0.0),Group("None"),N(60) {}

	//for reading csv file
	void SetANNDate(const string anndate) { ANNDate = anndate; }
	void SetEndDate(const string enddate) { EndDate = enddate; }
	void SetEst(const double est) { Est = est; }
	void SetRep(const double rep) { Rep = rep; }
	void SetSur(const double sur) { Sur = sur; }
	void SetSurper(const double surper) { Surper = surper; }
	void SetN(const int n) { N = n; }
	string GetANNDate() const { return ANNDate; }
	string GetEndDate() const { return EndDate; }
	double GetEst()const { return Est; }
	double GetRep()const { return Rep; }
	double GetSur()const { return Sur; }
	double GetSurper()const { return Surper; }
	int GetN() const { return N; }

	//for grouping and downloading market data
	void SetGroup(const string group) { Group = group; }
	void addDailyd(const string d) { Dailyd.push_back(d); }
	void addDailyp(const double p) { Dailyp.push_back(p); }
	string GetGroup() const { return Group; }
	vector<string> GetDailyd() const { return Dailyd; }
	vector<double> GetDailyp() const { return Dailyp; }

	int LengthofDailyd() const { return Dailyd.size(); }
	void displayDailyd() const;
	void displayDailyp() const;
};