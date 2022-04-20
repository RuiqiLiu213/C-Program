#include "Function02.h"
#include "math.h"
namespace fre {
	double F1::Value(double x) { return x * x - 2; }
	double F1::Deriv(double x) { return 2 * x; }

	double F2::Value(double x) { return x * x - a; }
	double F2::Deriv(double x) { return 2 * x; }

	double BondPrice::Value(double x) {
		double P = 0.0;
		for (int i = 1; i <= T; i += 1) {
			P += C * exp(-x * i);
		}
		P += F * exp(-x * T);
		return P;
	};
	double BondPrice::Deriv(double x) {
		double dP = 0.0;
		for (int i = 1; i <= T; i += 1) {
			dP += -i * C * exp(-x * i);

		}
		dP += -T * F * exp(-x * T);
		return dP;
	};
}