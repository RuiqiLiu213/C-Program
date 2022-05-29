#include "Stock.h"
#include "ReadMarketData.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>

struct MemoryStruct {
	char* memory;
	size_t size;
	size_t total_size;
};

void* myrealloc(void* ptr, size_t size)
{
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

int write_data(void* ptr, size_t size, size_t nmemb, void* data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)data;
	if ((mem->size + realsize) >= mem->total_size)
		mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
		if (mem->size > mem->total_size)
			mem->total_size = mem->size;
	}
	return realsize;
}

int detrievesinglestock(CURL* handle, const string symbol, Stock& stock, const string url_common, const string api_token, const map<string, Stock> Benchmark) {

	//determine start date and end date
	vector<string> days = Benchmark.begin()->second.GetDailyd();
	string day = stock.GetANNDate();

	//move announcemnet date to the most previous trading day
	auto it = find(days.begin(), days.end(), day);
	while (it == days.end()) {
		ANNDateMinusOne(day);
		it = find(days.begin(), days.end(), day);
	}
	stock.SetANNDate(*it);
	string start_date = *(it - stock.GetN());
	string end_date = *(it + stock.GetN()); // +/- N: out of range?

	cout << "Stock: " << symbol << " Time range: " << *(it - stock.GetN()) << " - " << *(it + stock.GetN()) << endl;
	CURLcode result;

	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;
	data.total_size = 0;

	data.size = 0;
	memset(data.memory, '\0', data.total_size);

	string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
	curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

	//adding a user agent
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

	// perform, then store the expected code in result
	result = curl_easy_perform(handle);

	// check for errors
	if (result != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
		return -1;
	}

	stringstream sData;
	sData.str(data.memory);
	string sValue, sDate;
	double dValue = 0;
	string line;
	while (getline(sData, line)) {
		size_t found = line.find('-');
		if (found != std::string::npos)
		{
			// cout << line << endl;
			sDate = line.substr(0, line.find_first_of(','));
			stock.addDailyd(sDate);
			line.erase(line.find_last_of(','));
			sValue = line.substr(line.find_last_of(',') + 1);
			dValue = strtod(sValue.c_str(), NULL);
			stock.addDailyp(dValue);
		}
	}
	free(data.memory);
	data.size = 0;
	return 0;
}

/*
void GetMarketData(map<string, Stock>& stks, const double* thre, map<string, Stock>& beat, map<string, Stock>& miss, map<string, Stock>& meet, const string url_common, const string api_token, const map<string, Stock> Benchmark) {
	auto begin = chrono::high_resolution_clock::now();

	// declaration of an object CURL
	CURL* handle;

	// set up the program environment that libcurl needs
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle
	handle = curl_easy_init();

	// if everything's all right with the easy handle...
	if (handle)
	{
		for (map<string, Stock>::iterator itr = stks.begin(); itr != stks.end(); itr++) {

			if (itr->second.GetSurper() <= thre[0]) { //miss the market
				miss[itr->first] = Stock(itr->second);
				miss[itr->first].SetGroup("Miss");
				detrievesinglestock(handle, itr->first, miss[itr->first], url_common, api_token, Benchmark);
			}
			else if (itr->second.GetSurper() > thre[1]) { // beat the market
				beat[itr->first] = Stock(itr->second);
				beat[itr->first].SetGroup("Beat");
				detrievesinglestock(handle, itr->first, beat[itr->first], url_common, api_token, Benchmark);
			}
			else {
				meet[itr->first] = Stock(itr->second); // meet the market
				meet[itr->first].SetGroup("Meet");
				detrievesinglestock(handle, itr->first, meet[itr->first], url_common, api_token, Benchmark);
			}
		}
	}

	else
	{
		fprintf(stderr, "Curl init failed!\n");
	}

	// cleanup since you've used curl_easy_init
	curl_easy_cleanup(handle);

	// release resources acquired by curl_global_init()
	curl_global_cleanup();

	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	cout << endl;
	cout << "Total Run time for downloading market data : " << fixed << setprecision(2) << elapsed.count() / 1000.0 << " seconds" << endl;
}
*/

int GetBenchmark(map<string, Stock>& benchmark, const string bench, string url_common, string api_token) {

	// declaration of an object CURL
	CURL* handle;

	// set up the program environment that libcurl needs
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle
	handle = curl_easy_init();

	// if everything's all right with the easy handle...
	if (handle)
	{
		string start_date = "2021-01-01";
		string end_date = "2022-03-31";

		benchmark[bench] = Stock(); // meet the market
		benchmark[bench].SetGroup(bench);
		CURLcode result;

		struct MemoryStruct data;
		data.memory = NULL;
		data.size = 0;
		data.total_size = 0;

		/*
		data.size = 0;
		memset(data.memory, '\0', data.total_size);
		*/

		string url_request = url_common + bench + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
		curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

		//adding a user agent
		curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

		// perform, then store the expected code in result
		result = curl_easy_perform(handle);

		if (result != CURLE_OK)
		{
			// if errors have occured, tell us what is wrong with result
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}

		stringstream sData;
		sData.str(data.memory);
		string sValue, sDate;
		double dValue = 0;
		string line;
		while (getline(sData, line)) {
			size_t found = line.find('-');
			if (found != std::string::npos)
			{
				// cout << line << endl;
				sDate = line.substr(0, line.find_first_of(','));
				benchmark[bench].addDailyd(sDate);
				line.erase(line.find_last_of(','));
				sValue = line.substr(line.find_last_of(',') + 1);
				dValue = strtod(sValue.c_str(), NULL);
				benchmark[bench].addDailyp(dValue);
			}
		}

		free(data.memory);
		data.size = 0;
		cout << "Successfully get Benchmark market data from " << start_date << " to " << end_date << endl;
	}

	else
	{
		fprintf(stderr, "Curl init failed!\n");
	}

	// cleanup since you've used curl_easy_init
	curl_easy_cleanup(handle);

	// release resources acquired by curl_global_init()
	curl_global_cleanup();
	return 0;
}

///---------------------------------------------------
void Split_Group(map<string, Stock>& stks, const double* thre, map<string, Stock>& beat, map<string, Stock>& miss, map<string, Stock>& meet) {
	for (map<string, Stock>::iterator itr = stks.begin(); itr != stks.end(); itr++) {
		if (itr->second.GetSurper() <= thre[0]) { //miss the market
			miss[itr->first] = Stock(itr->second);
			miss[itr->first].SetGroup("Miss");
		}
		else if (itr->second.GetSurper() > thre[1]) { // beat the market
			beat[itr->first] = Stock(itr->second);
			beat[itr->first].SetGroup("Beat");
		}
		else {
			meet[itr->first] = Stock(itr->second); // meet the market
			meet[itr->first].SetGroup("Meet");
		}
	}
}

void Get_Data(const string url_common, const string api_token, map<string, Stock>* group, const map<string, Stock> Benchmark) {
	// declaration of an object CURL
	CURL* handle;

	// set up the program environment that libcurl needs
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle
	handle = curl_easy_init();

	if (handle)
	{
		for (map<string, Stock>::iterator itr = group->begin(); itr != group->end(); itr++) {
			detrievesinglestock(handle, itr->first, (*group)[itr->first], url_common, api_token, Benchmark);
		}

	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
	}

	// cleanup since you've used curl_easy_init
	curl_easy_cleanup(handle);

	// release resources acquired by curl_global_init()
	curl_global_cleanup();

}