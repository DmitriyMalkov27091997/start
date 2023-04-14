/**
 * @file menu_helper.c
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Вспомогательные методы реализации меню пользователя.
 * @version 1.0
 * @date 2023-03-15
 * 
 * @copyright (C) Ярославль, 2023
 * 
 */

#include "menu_helper.h"
#include "type.h"

Menu_Item_t NULL_MENU = {0};
static Menu_Item_t* CurrentMenuItem = &NULL_MENU;

Menu_Item_t* Menu_CurrentMenuItem(void)
{
	return CurrentMenuItem;
}

BYTE Menu_IsNullOrEmpty(Menu_Item_t* const menu)
{
	return (menu == &NULL_MENU) || (menu == NULL);
}

void Menu_GoTo(Menu_Item_t* const menu_name)
{
	if (Menu_IsNullOrEmpty(menu_name))
		return;

	CurrentMenuItem = menu_name;
}

void Menu_NavigateCurrent(void)
{
	if (Menu_IsNullOrEmpty(CurrentMenuItem))
		return;

	void (*NavigateDelegate)(void) = CurrentMenuItem->NavigateDelegate;

	if (NavigateDelegate)
		NavigateDelegate();
}

void Menu_RenderCurrent(void)
{
	if (Menu_IsNullOrEmpty(CurrentMenuItem))
		return;

	void (*RenderDelegate)(void) = CurrentMenuItem->RenderDelegate;

	if (RenderDelegate)
		RenderDelegate();
}