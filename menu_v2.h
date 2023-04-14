/**
 * @file menu_v2.h
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Заголовочный файл меню.
 * @version 2.1
 * @date 10.03.2023
 *
 * @copyright (C) Ярославль, 2023
 *
 */
#ifndef __MENU_V2_H__
#define __MENU_V2_H__

#include "type.h"
#include "define.h"

#define TO_MENU_V2_START (3000UL) // Таймаут показа начального экрана, мс

/// @brief Структура отображающая состояние кнопок.
typedef struct sMENU_BTN
{
	union
	{
		BYTE btn; // Общее значение
		struct
		{
			BYTE btn_up : 1;	 // Нажатие "Вверх"
			BYTE btn_down : 1;	 // Нажатие "Вниз"
			BYTE btn_ok : 1;	 // Нажатие "Ок"
			BYTE btn_cancel : 1; // Нажатие "Выход"
			BYTE clr_up : 1;	 // Очистка "Вверх"
			BYTE clr_down : 1;	 // Очистка "Вниз"
			BYTE clr_ok : 1;	 // Очистка "Ок"
			BYTE clr_cancel : 1; // Очистка "Выход"
		};
	};
} sMENU_BTN;

/// @brief Варианты ручного управления электромагнитом.
typedef enum eMENU_CALIB_PWM
{
	/// @brief Не управляется
	MENU_CALIB_PWM_NONE = 0,

	/// @brief Сброс давления
	MENU_CALIB_PWM_0,

	/// @brief Середина
	MENU_CALIB_PWM_50,

	/// @brief Набор давления
	MENU_CALIB_PWM_100,
} eMENU_CALIB_PWM;

/// @brief Редактируемые данные
typedef struct sMENU_DATA
{
	union
	{
		/// @brief Представление "Флаги"
		sEEPROM_FLAGS f;

		/// @brief Представление 32-битное знаковое целое число
		int i;

		/// @brief Представление предыдущего значения переменной "i"
		int str;
	};
} sMENU_DATA;

/// @brief Определть нажатые кнопки и обновить состояние меню
extern void menu_v2_update(void);

/// @brief Выполнить отрисовку меню
extern void menu_v2_redraw(void);

/// @brief Текущее состояние кнопок.
extern sMENU_BTN menu_btn;

/// @brief Ручное управление электромагнитом
extern eMENU_CALIB_PWM menu_calib_pwm;

extern volatile sMENU_DATA menu_data;

/// @brief Целевое положение в ручном режиме
extern volatile int menu_hand_pos_t;

/// @brief Временная переменая для отслеживания последнего состояния menu_data.i
extern int i_old; 

/// @brief Временная переменная, используемая в качестве переключателя в пунктах меню.
extern volatile int menu_switch;

/// @brief Временная переменная, используемая в качестве счетчика в "Журнале событий".
extern volatile int counterItem;

extern tTIMER to_menu; // исключить по окончании миграции.

#endif // __MENU_V2_H__