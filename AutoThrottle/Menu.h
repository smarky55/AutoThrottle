// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>
#include <string>
#include <vector>

using XPLMMenuID = void*;

class MenuItem;

class Menu {
public:
	Menu(std::string name, const MenuItem* parentItem);
	~Menu();

	XPLMMenuID menu() const;

	static void menuCallback(void* menuRef, void* itemRef);

	virtual void onMenuCallback(void* menuRef, void* itemRef);

	MenuItem* appendMenuItem(std::string name);

	void appendSeparator();
private:
	XPLMMenuID m_menuId;

	std::vector<std::unique_ptr<MenuItem>> m_items;
};

