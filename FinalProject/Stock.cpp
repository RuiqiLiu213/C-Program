#include "Stock.h"

void Stock::displayDailyd() const {
	for (int i = 0;i<Dailyd.size();i++) {
		cout << Dailyd[i] << ",";
	}
	cout << endl;
}
void Stock::displayDailyp() const {
	for (int i = 0; i < Dailyp.size(); i++) {
		cout << Dailyp[i] << ",";
	}
	cout << endl;
}
