/**
 * @file menu_helper.h
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @brief ������������ ���� ��������������� ������� ���������� ���� ������������.
 * @version 1.0
 * @date 2023-03-15
 *
 * @copyright (C) ���������, 2023
 */

#ifndef _MENU_HELPER_H_
#define _MENU_HELPER_H_

#include "type.h"

/**
 * @brief ����� ����.
 * @remark ������ ����� ���� ������ ���� ��������������� � ������� ���������������� ������� \ref MENU_ITEM(),
 * � �� ����������� � ���� ��������������� �� ����� ����.
 */
typedef const struct Menu_Item
{
	const struct Menu_Item *Next;	  /**< ��������� �� ��������� ����� ���� */
	const struct Menu_Item *Previous; /**< ��������� �� ���������� ����� ���� */
	const struct Menu_Item *Parent;	  /**< ��������� �� ������������ ����� ���� */
	const struct Menu_Item *Child;	  /**< ��������� �� �������� ����� ���� */
	void (*NavigateDelegate)(void);	  /**< �����, ���������� ��� ��������� �� ����. */
	void (*RenderDelegate)(void);	  /**< �����, ���������� ��� ������������ ������ ���� �� �������. */
} Menu_Item_t;

/** 
 * @brief ������� ����� ����� ����.
 *  @param[in] Name      �������� ������ ���� (<b> ������ ���� ���������� </b>).
 *  @param[in] Next      ��� ���������� ���������� ������ ���� ��� \ref NULL_MENU, ���� ��� ������ �� ����
 *  @param[in] Previous  ��� ����������� ���������� ������ ���� ��� \ref NULL_MENU, ���� ��� ������ �� ����
 *  @param[in] Parent    ��� ������������� ���������� ������ ���� ��� \ref NULL_MENU, ���� ��� ������ �� ����
 *  @param[in] Child     ��� ��������� ���������� ������ ���� ��� \ref NULL_MENU, ���� ��� ������ �� ����
 *  @param[in] NavigateDelegate  �����, ���������� ��� ��������� �� ����.
 *  @param[in] RenderDelegate   �����, ���������� ��� ������������ ������ ���� �� �������.
 */
#define MENU_ITEM(Name, Next, Previous, Parent, Child, NavigateDelegate, RenderDelegate) \
	extern Menu_Item_t const Next;                                                     \
	extern Menu_Item_t const Previous;                                                 \
	extern Menu_Item_t const Parent;                                                   \
	extern Menu_Item_t const Child;                                                    \
	Menu_Item_t const Name = {&Next, &Previous, &Parent, &Child, NavigateDelegate, RenderDelegate}

/**
 * @brief �������� ��������� �� ������������ ����� ����.
 */
#define MENU_PARENT (Menu_CurrentMenuItem()->Parent)

/**
 * @brief �������� ��������� �� �������� ����� ����.
 */
#define MENU_CHILD (Menu_CurrentMenuItem()->Child)

/**
 * @brief �������� ��������� �� ��������� ����� ����.
 */
#define MENU_NEXT (Menu_CurrentMenuItem()->Next)

/**
 * @brief �������� ��������� �� ���������� ����� ����.
 */
#define MENU_PREVIOUS (Menu_CurrentMenuItem()->Previous)

/**
 * @brief ������ ������ ����.
 * @remark ������������ � ������������ \ref MENU_ITEM(), ��� �� ������ ���� ������ �� ������ ����� ����.
 */
extern Menu_Item_t NULL_MENU;

/**
 * @brief �������� ������� ��������� ����� ����.
 * @return ��������� �� ������� ��������� ����� ����.
 */
Menu_Item_t *Menu_CurrentMenuItem(void);

/**
 * @brief ���������, �� �������� �� ��������� ����� ���� ������.
 * 
 * @param menu ����������� ����� ����.
 * @return \b 1 - ��������� �� ���������� ��� ����� ���� �������� ������. 
 * 		\n \b 0 -  ��������� ���������� � ����� ���� �� �������� ������. 
 */
extern BYTE Menu_IsNullOrEmpty(Menu_Item_t* const menu);

/**
 * @brief ������� � ���������� ������ ����.
 * @param menu_name ��������� �� ����� ����, � �������� �������������� �������.
 */
extern void Menu_GoTo(Menu_Item_t *const menu_name);

/// @brief ��������� �� �������� ������ ����. 
extern void Menu_NavigateCurrent(void);

/// @brief ������������ �������� ������ ���� �� �������.
extern void Menu_RenderCurrent(void);

#endif // _MENU_HELPER_H_
