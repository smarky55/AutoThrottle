// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <functional>
#include <memory>
#include <string>

using XPLMMenuID = void*;

class Menu;

class MenuItem {
public:
	MenuItem(std::string name, const Menu* parentMenu, size_t index = -1);
	MenuItem(std::string name, XPLMMenuID parentMenu);
	~MenuItem();

	size_t menuItem() const;

	Menu* menu();

	void setOnClickHandler(std::function<void(void*)> onClickHandler);

	void onClick(void* itemref);

	const Menu* parentMenu() const;
	XPLMMenuID xpParentMenu() const;

private:
	std::string m_name;
	const Menu* m_parentMenu;
	XPLMMenuID m_xpParentMenu;

	size_t m_index;

	std::unique_ptr<Menu> m_menu;

	std::function<void(void*)> m_onClickhandler;
};

