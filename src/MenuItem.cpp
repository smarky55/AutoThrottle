// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/MenuItem.h>

#include <XPLMMenus.h>

#include <AutoThrottle/Menu.h>

MenuItem::MenuItem(std::string name, const Menu* parentMenu, size_t index)
	: m_parentMenu(parentMenu),
	m_xpParentMenu(parentMenu->menu())
{
	void* itemref = (index < 0) ? NULL : reinterpret_cast<void*>(index);
	m_index = XPLMAppendMenuItem(parentMenu->menu(), name.c_str(), itemref, 0);
}

MenuItem::MenuItem(std::string name, XPLMMenuID parentMenu)
	: m_parentMenu(nullptr),
	m_xpParentMenu(parentMenu)
{
	m_index = XPLMAppendMenuItem(parentMenu, name.c_str(), NULL, 0);
}

MenuItem::~MenuItem()
{
	//TODO remove menu items - need to track shift in indices
}

size_t MenuItem::menuItem() const
{
	return m_index;
}

Menu* MenuItem::menu()
{
	if (!m_menu) {
		m_menu = std::make_unique<Menu>(m_name, this);
	}
	return m_menu.get();
}

void MenuItem::setOnClickHandler(std::function<void(void*)> onClickHandler)
{
	m_onClickhandler = onClickHandler;
}

void MenuItem::onClick(void* itemRef)
{
	if (m_onClickhandler) {
		m_onClickhandler(itemRef);
	}
}

const Menu* MenuItem::parentMenu() const
{
	return m_parentMenu;
}

XPLMMenuID MenuItem::xpParentMenu() const
{
	return m_xpParentMenu;
}
