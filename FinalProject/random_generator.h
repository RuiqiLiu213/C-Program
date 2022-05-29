#pragma once
#include <vector>
#include <random>

using namespace std;

class rand_num_generator {
protected:
	int N;
	vector <int> numbers;
public:
	rand_num_generator(int N_) {
		N = N_;
	}
	void set_Nums() {
		for (int i = 0; i < N; i++)       // add 0-900 to the vector
			numbers.push_back(i);
	}

	vector <int> get_Nums() {
		// obtain a time-based seed:
		unsigned seed = (unsigned)time(NULL);
		shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));

		vector <int> output;
		for (int i = 0; i < 80; i++)   //set the number of random here
			output.push_back(numbers[i]);
		return output;
	}
};
