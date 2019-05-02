// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0.If a copy of the MPL was not distributed with this
// file, You can obtain one at http ://mozilla.org/MPL/2.0/.

#include "perf.h"

PerfTable::PerfTable() {}

PerfTable::~PerfTable() {}

#ifdef _DEBUG
void PerfTable::testPerf() {
	int numx = 5;
	int numy = 10;
	int initial_value = 1;

	for (int i = 0; i < numx; i++) {
		keys_x.push_back(i);
	}
	for (int i = 0; i < numy; i++) {
		keys_y.push_back(i * 10);
	}
	data.resize(numx, std::vector<int>(numy, initial_value));
}
#endif // _DEBUG


int PerfTable::getValue(int x, int y) {
	int x_lower_index = -1, x_lower_value, x_upper_index = -1, x_upper_value;
	for (int i = 0; i < keys_x.size(); i++) {
		if (keys_x[i] < x) {
			x_lower_index = i;
			x_lower_value = keys_x[i];
		}
		if (keys_x[i] > x) {
			x_upper_index = i;
			x_upper_value = keys_x[i];
			break;
		}
	}

	int y_lower_index = -1, y_lower_value, y_upper_index = -1, y_upper_value;
	for (int i = 0; i < keys_y.size(); i++) {
		if (keys_y[i] < y) {
			y_lower_index = i;
			y_lower_value = keys_y[i];
		}
		if (keys_y[i] > y) {
			y_upper_index = i;
			y_upper_value = keys_y[i];
			break;
		}
	}

	int q11, q12, q21, q22, r1, r2;

	// x < keys_x[0]
	if (x_lower_index < 0) {
		x_lower_index = 0;
		x_lower_value = keys_x[x_lower_index];
	}
	// x > keys_x[-1]
	if (x_upper_index < 0) {
		x_upper_index = keys_x.size() - 1;
		x_upper_value = keys_x[x_upper_index];
	}
	// y < keys_y[0]
	if (y_lower_index < 0) {
		y_lower_index = 0;
		y_lower_value = keys_y[y_lower_index];
	}
	// y > keys_y[-1]
	if (y_upper_index < 0) {
		y_upper_index = keys_y.size() - 1;
		y_upper_value = keys_y[y_upper_index];
	}

	q11 = data[x_lower_index][y_lower_index];
	q12 = data[x_lower_index][y_upper_index];
	q21 = data[x_upper_index][y_lower_index];
	q22 = data[x_upper_index][y_upper_index];

	r1 = q11 + (x - x_lower_value) * ((q21 - q11) / (x_upper_value - x_lower_value));
	r2 = q12 + (x - x_lower_value) * ((q22 - q12) / (x_upper_value - x_lower_value));

	return r1 + (y - y_lower_value) * ((r2 - r1) / (y_upper_value - y_lower_value));

}
