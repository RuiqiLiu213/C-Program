#include "ReadCSV.h"
#include "ReadMarketData.h"
#include "Matrix.hpp"
#include "StockCalculation.hpp"
#include "GroupCalculation.hpp"
#include "random_generator.h"
#include <thread>
#include <stdio.h>
using namespace std;
using namespace groupCal;

// set EPS announcements filename and benchmark index string
const char* cIWV3000AnnouncemnetsFile = "Russell3000EarningsAnnouncements.csv";
const string benchmarkindex = "IWV";

int main(void) {
	// set url and api
	string url_common = "https://eodhistoricaldata.com/api/eod/";
	string api_token = "62699b17aa68e6.28551607";

	// set global variables
	int N = 60; //total 2N+1 of days
	map<string, Stock> Stks;
	double thre[2]; //two thresholds to seperate three groups

	map<string, Stock> Benchmark;
	map<string, Stock> Beat;
	map<string, Stock> Miss;
	map<string, Stock> Meet;

	// set required matrix, three rows for 3 groups
	// 1st row: Beat
	// 2nd row: Miss
	// 3rd row: Meet
	vector<vector<vector<double>>> mat(3);

	// menu
	char selection;
	string menu = string("Menu \n")
		+ "======================================================================== \n"
		+ "A: Enter N to retrieve 2N+1 days of historical price data for all stocks \n"
		+ "B: Pull information for one stock from one group \n"
		+ "C: Show AAR, AAR-STD, CAAR and CAAR-STD for one group \n"
		+ "D: Show the gnuplot graph with CAAR for all 3 groups \n"
		+ "X: Exit \n"
		+ "======================================================================== \n"
		+ "Enter Selection: ";
	int flag = 1;
	while (flag)
	{
		cout << menu;
		cin >> selection;
		switch (selection)
		{
			// A: Retrieve data, do all the calculation
		case 'A':
		case 'a': {
			cout << "Please enter N (60<=N<=112, default 60): ";
			cin >> N;

			if ((N >= 60) && (N <= 112))
			{
				cout << "Begin retrieving data......" << endl;
			}
			else
			{
				cout << "Please enter a valid number." << endl;

				cout << endl;
				break;
			}

			// record start time
			auto begin = chrono::high_resolution_clock::now();

			// read CSV
			cout << "Begin reading CSV......" << endl;
			ReadCSV(Stks, thre, insertSort, cIWV3000AnnouncemnetsFile, N);
			cout << "Reading CSV finished." << endl;

			// get benchmark
			cout << "Begin retrieving benchmark......" << endl;
			GetBenchmark(Benchmark, benchmarkindex, url_common, api_token);

			//------------------------------------
			// retrieve stocks data
			cout << "Begin retrieving stock data......" << endl;

			Split_Group(Stks, thre, Beat, Miss, Meet);

			thread first(Get_Data, url_common, api_token, &Beat, Benchmark); // thread 1
			thread second(Get_Data, url_common, api_token, &Meet, Benchmark); //thread 2
			thread third(Get_Data, url_common, api_token, &Miss, Benchmark); //thread 3

			first.join();
			second.join();
			third.join();

			cout << "Retrieval of all data finished." << endl;
			//----------------------------------------------
			cout << "Begin calculating......" << endl;
			// calculate benchmark return
			Stock* benchmark = &Benchmark[benchmarkindex];
			map<string, double> BM_rtn = get_bench_rtn(benchmark);

			// calculate the expected AAR, AAR_STD, CAAR, CAAR_STD for three groups
			Vector ave_AAR_Beat(2 * N);
			Vector AAR_std_Beat(2 * N);
			Vector ave_CAAR_Beat(2 * N);
			Vector CAAR_std_Beat(2 * N);
			finalCalculation(ave_AAR_Beat, AAR_std_Beat, ave_CAAR_Beat, CAAR_std_Beat, Beat, N, BM_rtn);


			Vector ave_AAR_Miss(2 * N);
			Vector AAR_std_Miss(2 * N);
			Vector ave_CAAR_Miss(2 * N);
			Vector CAAR_std_Miss(2 * N);
			finalCalculation(ave_AAR_Miss, AAR_std_Miss, ave_CAAR_Miss, CAAR_std_Miss, Miss, N, BM_rtn);


			Vector ave_AAR_Meet(2 * N);
			Vector AAR_std_Meet(2 * N);
			Vector ave_CAAR_Meet(2 * N);
			Vector CAAR_std_Meet(2 * N);
			finalCalculation(ave_AAR_Meet, AAR_std_Meet, ave_CAAR_Meet, CAAR_std_Meet, Meet, N, BM_rtn);

			// resize each element of 'mat' to 4 elements of type vector<double> to store the expected AAR, AAR_STD, CAAR, CAAR_STD for each group
			for (int i = 0; i < 3; i++)
				mat[i].resize(4, vector<double>(2 * N));

			// for every row of 'mat', the 4 columns are 'expected AAR', 'AAR_STD', 'expected CAAR' and 'CAAR_STD', respectively
			// populate the first row for 'Beat' group
			mat[0][0] = ave_AAR_Beat; mat[0][1] = AAR_std_Beat; mat[0][2] = ave_CAAR_Beat; mat[0][3] = CAAR_std_Beat;

			// populate the 2nd row for 'Miss' group
			mat[1][0] = ave_AAR_Miss; mat[1][1] = AAR_std_Miss; mat[1][2] = ave_CAAR_Miss; mat[1][3] = CAAR_std_Miss;

			// populate the 3rd row for 'Meet' group
			mat[2][0] = ave_AAR_Meet; mat[2][1] = AAR_std_Meet; mat[2][2] = ave_CAAR_Meet; mat[2][3] = CAAR_std_Meet;

			cout << "Calculation finished." << endl;
			// output processing time
			auto end = chrono::high_resolution_clock::now();
			auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
			cout << "Total Run time : " << fixed << setprecision(2) << elapsed.count() / 1000.0 << " seconds" << endl;
			cout << endl;
			break;
		}
				// B: Pull information for one stock from one group
		case 'B':
		case 'b': {
			string stock_symbol;
			cout << "Please input a stock abbreviation: ";
			cin >> stock_symbol;
			if (Stks.find(stock_symbol) == Stks.end())
			{
				cout << "The stock does not exist, please enter a valid stock abbreviation." << endl;

				cout << endl;
				break;
			}

			if (Beat.find(stock_symbol) != Beat.end())
			{
				cout << "Daily prices of " << stock_symbol << ": " << endl;
				for (int i = 0; i < (2 * N + 1); i++)
				{
					cout << Beat[stock_symbol].GetDailyd()[i] << " " << Beat[stock_symbol].GetDailyp()[i] << endl;
				}
				cout << "Cumulative daily returns of " << stock_symbol << ": " << endl;
				for (int i = 1; i < (2 * N + 1); i++)
				{
					cout << Beat[stock_symbol].GetDailyd()[i] << " " << fixed << setprecision(2)
						<< ((Beat[stock_symbol].GetDailyp()[i] - Beat[stock_symbol].GetDailyp()[0]) / Beat[stock_symbol].GetDailyp()[0]) << endl;
				}
				cout << stock_symbol << " belongs to Beat group." << endl;
				cout << "Earning announcement date of " << stock_symbol << ": " << Beat[stock_symbol].GetANNDate() << endl;
				cout << "Period ending of " << stock_symbol << ": " << Beat[stock_symbol].GetEndDate() << endl;
				cout << "Estimated EPS of " << stock_symbol << ": " << Beat[stock_symbol].GetEst() << endl;
				cout << "Reported EPS of " << stock_symbol << ": " << Beat[stock_symbol].GetRep() << endl;
				cout << "Surprise of " << stock_symbol << ": " << Beat[stock_symbol].GetSur() << endl;
				cout << "Surprise% of " << stock_symbol << ": " << Beat[stock_symbol].GetSurper() << endl;
			}

			if (Meet.find(stock_symbol) != Meet.end())
			{
				cout << "Daily prices of " << stock_symbol << ": " << endl;
				for (int i = 0; i < (2 * N + 1); i++)
				{
					cout << Meet[stock_symbol].GetDailyd()[i] << " " << Meet[stock_symbol].GetDailyp()[i] << endl;
				}
				cout << "Cumulative daily returns of " << stock_symbol << ": " << endl;
				for (int i = 1; i < (2 * N + 1); i++)
				{
					cout << Meet[stock_symbol].GetDailyd()[i] << " " << fixed << setprecision(2)
						<< ((Meet[stock_symbol].GetDailyp()[i] - Meet[stock_symbol].GetDailyp()[0]) / Meet[stock_symbol].GetDailyp()[0]) << endl;
				}
				cout << stock_symbol << " belongs to Meet group." << endl;
				cout << "Earning announcement date of " << stock_symbol << ": " << Meet[stock_symbol].GetANNDate() << endl;
				cout << "Period ending of " << stock_symbol << ": " << Meet[stock_symbol].GetEndDate() << endl;
				cout << "Estimated EPS of " << stock_symbol << ": " << Meet[stock_symbol].GetEst() << endl;
				cout << "Reported EPS of " << stock_symbol << ": " << Meet[stock_symbol].GetRep() << endl;
				cout << "Surprise of " << stock_symbol << ": " << Meet[stock_symbol].GetSur() << endl;
				cout << "Surprise% of " << stock_symbol << ": " << Meet[stock_symbol].GetSurper() << endl;
			}

			if (Miss.find(stock_symbol) != Miss.end())
			{
				cout << "Daily prices of " << stock_symbol << ": " << endl;
				for (int i = 0; i < (2 * N + 1); i++)
				{
					cout << Miss[stock_symbol].GetDailyd()[i] << " " << Miss[stock_symbol].GetDailyp()[i] << endl;
				}
				cout << "Cumulative daily returns of " << stock_symbol << ": " << endl;
				for (int i = 1; i < (2 * N + 1); i++)
				{
					cout << Miss[stock_symbol].GetDailyd()[i] << " " << fixed << setprecision(2)
						<< ((Miss[stock_symbol].GetDailyp()[i] - Miss[stock_symbol].GetDailyp()[0]) / Miss[stock_symbol].GetDailyp()[0]) << endl;
				}
				cout << stock_symbol << " belongs to Miss group." << endl;
				cout << "Earning announcement date of " << stock_symbol << ": " << Miss[stock_symbol].GetANNDate() << endl;
				cout << "Period ending of " << stock_symbol << ": " << Miss[stock_symbol].GetEndDate() << endl;
				cout << "Estimated EPS of " << stock_symbol << ": " << Miss[stock_symbol].GetEst() << endl;
				cout << "Reported EPS of " << stock_symbol << ": " << Miss[stock_symbol].GetRep() << endl;
				cout << "Surprise of " << stock_symbol << ": " << Miss[stock_symbol].GetSur() << endl;
				cout << "Surprise% of " << stock_symbol << ": " << Miss[stock_symbol].GetSurper() << endl;
			}

			cout << endl;
			break;
		}
				// C: Show AAR, AAR-STD, CAAR and CAAR-STD for one group
		case 'C':
		case 'c': {
			string group_symbol;
			cout << "Please choose a group from Beat, Meet and Miss: ";
			cin >> group_symbol;

			if (group_symbol == "Beat")
			{
				cout << "ARR: " << endl;
				cout << mat[0][0] << endl;
				cout << "ARR-STD: " << endl;
				cout << mat[0][1] << endl;
				cout << "CAAR: " << endl;
				cout << mat[0][2] << endl;
				cout << "CAAR-STD: " << endl;
				cout << mat[0][3] << endl;
			}
			else if (group_symbol == "Miss")
			{
				cout << "ARR: " << endl;
				cout << mat[1][0] << endl;
				cout << "ARR-STD: " << endl;
				cout << mat[1][1] << endl;
				cout << "CAAR: " << endl;
				cout << mat[1][2] << endl;
				cout << "CAAR-STD: " << endl;
				cout << mat[1][3] << endl;
			}
			else if (group_symbol == "Meet")
			{
				cout << "ARR: " << endl;
				cout << mat[2][0] << endl;
				cout << "ARR-STD: " << endl;
				cout << mat[2][1] << endl;
				cout << "CAAR: " << endl;
				cout << mat[2][2] << endl;
				cout << "CAAR-STD: " << endl;
				cout << mat[2][3] << endl;
			}
			else
			{
				cout << "The group does not exist, please enter a valid group." << endl;

				cout << endl;
				break;
			}

			cout << endl;
			break;
		}
				// D: Show the gnuplot graph with CAAR for all 3 groups
		case 'D':
		case 'd': {
			FILE* pipe = _popen("gnuplot", "w");
			FILE* temp;
			if (pipe == NULL) 
			{
				cout << "Gnuplot not found." << endl;

				cout << endl;
				break;
			}

			fprintf(pipe, "set title 'CAAR for All 3 Groups'\n");
			fprintf(pipe, "set xlabel 'Number of days to announcement'\n");
			fprintf(pipe, "set ylabel 'CAAR'\n");
			fprintf(pipe, "set grid\n");
			fprintf(pipe, "plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", "Beat", "Miss", "Meet");

			vector<int> x_axis;
			double x, y, x2, y2, x3, y3;
			int i;
			for (int i = (-N + 1); i <= N; i++)
			{
				x_axis.push_back(i);
			}

			temp = fopen("Beat", "w");
			for (size_t i = 0; i < mat[0][2].size(); i++) {
				x = x_axis[i];
				y = mat[0][2][i];
				fprintf(temp, "%lf %lf\n", x, y);
			}
			fclose(temp);

			temp = fopen("Miss", "w");
			for (size_t i = 0; i < mat[1][2].size(); i++) {
				x2 = x_axis[i];
				y2 = mat[1][2][i];
				fprintf(temp, "%lf %lf\n", x2, y2);
			}
			fclose(temp);

			temp = fopen("Meet", "w");
			for (size_t i = 0; i < mat[2][2].size(); i++) {
				x3 = x_axis[i];
				y3 = mat[2][2][i];
				fprintf(temp, "%lf %lf\n", x3, y3);
			}
			fclose(temp);

			fprintf(pipe, "pause mouse\n");
			_pclose(pipe);

			cout << endl;
			break;
		}
				// X: Exit
		case 'X':
		case 'x': {
			cout << "Program shut down." << endl;
			flag = 0;
			break;
		}

		default: {
			cerr << "Invalid selection. Choose again." << endl;
			break;
		}
		}
	}
	return 0;
}