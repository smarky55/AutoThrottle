// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Menu.h>

#include <XPLM/XPLMMenus.h>

#include <AutoThrottle/MenuItem.h>

Menu::Menu(std::string name, const MenuItem* parentItem)
{
	m_menuId = XPLMCreateMenu(name.c_str(), parentItem->xpParentMenu(), parentItem->menuItem(), menuCallback, this);
}

Menu::~Menu()
{
	m_items.clear();
	XPLMDestroyMenu(m_menuId);
}

XPLMMenuID Menu::menu() const
{
	return m_menuId;
}

void Menu::menuCallback(void* menuRef, void* itemRef)
{
	static_cast<Menu*>(menuRef)->onMenuCallback(menuRef, itemRef);
}

void Menu::onMenuCallback(void* menuRef, void* itemRef)
{
	size_t index = reinterpret_cast<intptr_t>(itemRef);
	m_items[index]->onClick(itemRef);
}

MenuItem* Menu::appendMenuItem(std::string name)
{
	m_items.push_back(std::make_unique<MenuItem>(name, this, m_items.size()));
	return m_items.back().get();
}

void Menu::appendSeparator()
{
	XPLMAppendMenuSeparator(m_menuId);
}
