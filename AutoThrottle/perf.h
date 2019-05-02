// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0.If a copy of the MPL was not distributed with this
// file, You can obtain one at http ://mozilla.org/MPL/2.0/.

#pragma once

#include <vector>

#include <cereal/types/vector.hpp>

class PerfTable {
public:
	PerfTable();
	~PerfTable();

#ifdef _DEBUG
	void testPerf();
#endif // _DEBUG


	int getValue(int x, int y);

	template<class Archive>
	void serialize(Archive & ar) {
		ar( CEREAL_NVP(keys_x),
			CEREAL_NVP(keys_y),
			CEREAL_NVP(data));
	}
private:
	std::vector<int> keys_x;
	std::vector<int> keys_y;
	std::vector<std::vector<int>> data;
};
