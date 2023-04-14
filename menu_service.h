/**
 * @file menu_service.h
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Заголовочный файл, содержащий, методы навигации и отрисовки инженерного меню.
 * @version 1.0
 * @date 2023-03-21
 *
 * @copyright (C) Ярославль, 2023
 */

#ifndef _MENU_SERVICE_H_
#define _MENU_SERVICE_H_

#include "menu_helper.h"

//-----------------------------------------------------------------------------
//	enum eSERVICE_ITEM_TYPE
//-----------------------------------------------------------------------------
typedef enum eSERVICE_ITEM_TYPE
{
	ESERVICE_TYPE_NONE = 0, // Конец списка
	ESERVICE_TYPE_COMMAND,	// Команда
	ESERVICE_TYPE_INT_U,	// ПЗ. Целое 0000
	ESERVICE_TYPE_INT_P,	// ЗК. Целое 0000
	ESERVICE_TYPE_FLOAT_U,	// ПЗ. С точкой 000.000
	ESERVICE_TYPE_FLOAT_P,	// ЗК. С точкой 000.000
	ESERVICE_TYPE_FLOAT_PB, // ЗК. С точкой s000000.0
	ESERVICE_TYPE_FLAGS,	// Флаги
} eSERVICE_ITEM_TYPE;

/// @brief Структура сервисного элемента
typedef struct sSERVICE_ITEM
{
	BYTE *name;
	DWORD type;
	union
	{
		volatile void *addr;
		void (*command)(void);
	};
	WORD pos;
	WORD size;
} sSERVICE_ITEM;

/// @brief Выполнить вход в инженерное меню.
/// @param serviceMenu Указать на экран инженерного меню.
extern void Service_Entry(Menu_Item_t *serviceMenu);

/// @brief Реализует навигацию по инженерному меню.
extern void Service_Navigate(void);

/// @brief Реализует отрисовку инженерного меню.
extern void Service_Render(void);

/// @brief Записать значение датчика положения соответствующее положению 0%.
extern void Service_Command_SetPosition_0(void);

/// @brief Записать значение датчика положения соответствующее положению 100%.
extern void Service_Command_SetPosition_100(void);

/// @brief Записать значение входного токового сигнала соответствующее 4мА.
extern void Service_Command_SetAIn_4(void);

/// @brief Записать значение входного токового сигнала соответствующее 20мА.
extern void Service_Command_SetAIn_20(void);

/// @brief Выполнить перезагрузку.
extern void Service_Command_Reboot(void);

/// @brief Выполнить полный сброс настройкам.
extern void Service_Command_FactoryReset(void);

/// @brief Выполнить сброс настроек пользователя.
extern void Service_Command_FullReset(void);

/// @brief Изменить температурное исполнение позиционера. 
/// @param newVersion Новое значение исполнения.
extern void Service_Command_ChangeTempVersion(int newVersion);

#endif // _MENU_SERVICE_H_