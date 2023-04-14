/**
 * @file menu_helper.h
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Заголовочный файл вспомогательных методов реализации меню пользователя.
 * @version 1.0
 * @date 2023-03-15
 *
 * @copyright (C) Ярославль, 2023
 */

#ifndef _MENU_HELPER_H_
#define _MENU_HELPER_H_

#include "type.h"

/**
 * @brief Пункт меню.
 * @remark Каждый пункт меню должен быть инициализирован с помощью вспомогательного макроса \ref MENU_ITEM(),
 * а не создаваться в коде непосредственно из этого типа.
 */
typedef const struct Menu_Item
{
	const struct Menu_Item *Next;	  /**< Указатель на следующий пункт меню */
	const struct Menu_Item *Previous; /**< Указатель на предыдущий пункт меню */
	const struct Menu_Item *Parent;	  /**< Указатель на родительский пункт меню */
	const struct Menu_Item *Child;	  /**< Указатель на дочерний пункт меню */
	void (*NavigateDelegate)(void);	  /**< Метод, вызываемый для навигации по меню. */
	void (*RenderDelegate)(void);	  /**< Метод, вызываемый для визуализации пункта меню на дисплее. */
} Menu_Item_t;

/** 
 * @brief Создать новый пункт меню.
 *  @param[in] Name      Название пункта меню (<b> должно быть уникальным </b>).
 *  @param[in] Next      Имя следующего связанного пункта меню или \ref NULL_MENU, если нет ссылки на меню
 *  @param[in] Previous  Имя предыдущего связанного пункта меню или \ref NULL_MENU, если нет ссылки на меню
 *  @param[in] Parent    Имя родительского связанного пункта меню или \ref NULL_MENU, если нет ссылки на меню
 *  @param[in] Child     Имя дочернего связанного пункта меню или \ref NULL_MENU, если нет ссылки на меню
 *  @param[in] NavigateDelegate  Метод, вызываемый для навигации по меню.
 *  @param[in] RenderDelegate   Метод, вызываемый для визуализации пункта меню на дисплее.
 */
#define MENU_ITEM(Name, Next, Previous, Parent, Child, NavigateDelegate, RenderDelegate) \
	extern Menu_Item_t const Next;                                                     \
	extern Menu_Item_t const Previous;                                                 \
	extern Menu_Item_t const Parent;                                                   \
	extern Menu_Item_t const Child;                                                    \
	Menu_Item_t const Name = {&Next, &Previous, &Parent, &Child, NavigateDelegate, RenderDelegate}

/**
 * @brief Получить указатель на родительский пункт меню.
 */
#define MENU_PARENT (Menu_CurrentMenuItem()->Parent)

/**
 * @brief Получить указатель на дочерний пункт меню.
 */
#define MENU_CHILD (Menu_CurrentMenuItem()->Child)

/**
 * @brief Получить указатель на следующий пункт меню.
 */
#define MENU_NEXT (Menu_CurrentMenuItem()->Next)

/**
 * @brief Получить указатель на предыдущий пункт меню.
 */
#define MENU_PREVIOUS (Menu_CurrentMenuItem()->Previous)

/**
 * @brief Пустая запись меню.
 * @remark Используется в определениях \ref MENU_ITEM(), где не должно быть ссылки на другой пункт меню.
 */
extern Menu_Item_t NULL_MENU;

/**
 * @brief Получить текущий выбранный пункт меню.
 * @return Указатель на текущий выбранный пункт меню.
 */
Menu_Item_t *Menu_CurrentMenuItem(void);

/**
 * @brief Проверить, не является ли указанный пункт меню пустым.
 * 
 * @param menu Проверяемый пункт меню.
 * @return \b 1 - указателя не существует или пункт меню является пустым. 
 * 		\n \b 0 -  указатель существует и пункт меню не является пустым. 
 */
extern BYTE Menu_IsNullOrEmpty(Menu_Item_t* const menu);

/**
 * @brief Переход к указанному пункту меню.
 * @param menu_name Указатель на пункт меню, к которому осуществляется переход.
 */
extern void Menu_GoTo(Menu_Item_t *const menu_name);

/// @brief Навигация по текущему пункту меню. 
extern void Menu_NavigateCurrent(void);

/// @brief Визуализации текущего пункта меню на дисплее.
extern void Menu_RenderCurrent(void);

#endif // _MENU_HELPER_H_
