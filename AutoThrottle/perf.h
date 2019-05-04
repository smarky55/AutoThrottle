// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0.If a copy of the MPL was not distributed with this
// file, You can obtain one at http ://mozilla.org/MPL/2.0/.

#pragma once

#include <string>
#include <vector>

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

enum DrefFlag {
	DrefFlag_IsArray = 1,
	DrefFlag_ISACorrect = 2
};

class PerfTable {
public:
	std::string x_dref;
	std::string y_dref;
	std::string z_dref;
	int dref_indices[3] = { -1, -1, -1 };
	int dref_flags[3] = { 0, 0, 0 };

	PerfTable();
	~PerfTable();

#ifdef _DEBUG
	void testPerf();
#endif // _DEBUG


	int getValue(int x, int y);

	template<class Archive>
	void serialize(Archive & ar) {
		ar( CEREAL_NVP(x_dref),
			CEREAL_NVP(y_dref),
			CEREAL_NVP(z_dref),
			CEREAL_NVP(dref_indices),
			CEREAL_NVP(dref_flags),
			CEREAL_NVP(keys_x),
			CEREAL_NVP(keys_y),
			CEREAL_NVP(data));
	}
private:
	std::vector<int> keys_x;
	std::vector<int> keys_y;
	std::vector<std::vector<int>> data;
};
