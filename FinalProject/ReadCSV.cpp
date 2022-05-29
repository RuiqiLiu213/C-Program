#include "ReadCSV.h"
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>

void insertSort(vector<double>& vec, const int length) {
	int i, j;
	for (i = 1; i < length; i++) {
		double tmp = vec[i];
		for (j = i; j > 0 && vec[j - 1] > tmp; j--) {
			vec[j] = vec[j - 1];
		}
		vec[j] = tmp;
	}
}

void ReadCSV(map<string, Stock>& stks, double* thre, void(*sort)(vector<double>& vector, const int length), const char* cIWV3000AnnouncemnetsFile, const int N) //read announcement information into a map<string, Stock>, and generate a vector of surprise%s, sort it, and get the threshold
{
	ifstream fin; //Input stream class to operate on file. File streams are associated with files either on construction, or by calling member open
	fin.open(cIWV3000AnnouncemnetsFile, ios::in);
	vector<double> surpers; //a vector of surprise%s

	string line, symbol, anndate, enddate, est, rep, sur, surper;

	getline(fin, line); //skip first line
	while (getline(fin, line))//getline in fin and store in line
	{
		stringstream sin(line); //single line
		getline(sin, symbol, ',');
		stks[symbol] = Stock(); //initialize a stock class

		getline(sin, anndate, ',');
		ANNDateStandard(anndate); //adjust announcement date as a standard format
		stks[symbol].SetANNDate(anndate); //set anndate
		getline(sin, enddate, ',');
		stks[symbol].SetEndDate(enddate); //set enddate
		getline(sin, est, ',');
		stks[symbol].SetEst(stod(est)); //set est: stod(string) = double
		getline(sin, rep, ',');
		stks[symbol].SetRep(stod(rep)); //set rep
		getline(sin, sur, ',');
		stks[symbol].SetSur(stod(sur)); //set sur
		getline(sin, surper, ',');
		stks[symbol].SetSurper(stod(surper)); //set surper
		surpers.push_back(stod(surper));
		stks[symbol].SetN(N); //set N
	}
	sort(surpers, surpers.size()); //sort surprises, ascending = True
	fin.close();

	thre[0] = surpers[int(surpers.size() / 3.0) - 1]; //33% percentile
	thre[1] = surpers[2.0 * int(surpers.size() / 3.0) - 1]; //66% percentile
}