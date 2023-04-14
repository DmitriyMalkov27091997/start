/**
 * @file watchdog.h
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Заголовочный файл сторожевого таймера.
 * @version 0.1
 * @date 20.05.2016
 *
 * @copyright (C) Ярославль, 2016
 *
 */
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/******************************************************************************
 * README
 ******************************************************************************
Определения для внешних модулей:
	- PRESENT_IWDG
	- FAIL_DEBUG

Требования к вызову из внешних модулей:
	- IWDG_INIT - в main() при инициализации до mainloop
	- IWDG_MAINLOOP - в mainloop
	- IWDG_TIMER - в конце SysTick

Примечания:
	- период фиксированный, задается в watchdog.h в определении TO_IWDG

******************************************************************************/

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include "define.h"

/******************************************************************************
 * DEFINE
 *****************************************************************************/
#ifdef PRESENT_IWDG
#define IWDG_PRESCALER (256) //!< Предделитель сторожевого таймера
#define TO_IWDG (10000UL)	 //!< Таймаут сторожевого таймера, сек
#define VAL_IWDG (FLSI * TO_IWDG / IWDG_PRESCALER / 1000UL)
//!< Таймаут сторожевого таймера, циклов
#endif // PRESENT_IWDG

#ifdef PRESENT_IWDG
#define IWDG_INIT                  \
	do                             \
	{                              \
		iwdg_start(IWDG_PRES_256); \
		iwdg_update(VAL_IWDG);     \
	} while (0)
#define IWDG_MAINLOOP_INC   \
	do                      \
	{                       \
		mainloop_counter++; \
	} while (0)
#define IWDG_TIMER_CLEAR          \
	do                            \
	{                             \
		if (mainloop_counter)     \
		{                         \
			mainloop_counter = 0; \
			iwdg_clear();         \
		}                         \
	} while (0)
#else
#define IWDG_INIT \
	do            \
	{             \
	} while (0)
#define IWDG_MAINLOOP_INC \
	do                    \
	{                     \
	} while (0)
#define IWDG_TIMER_CLEAR \
	do                   \
	{                    \
	} while (0)
#endif // PRESENT_IWDG

#ifdef FAIL_DEBUG
#define IWDG_MAINLOOP_FAIL \
	do                     \
	{                      \
	} while (fail_mainloop)
#define IWDG_TIMER_FAIL \
	do                  \
	{                   \
	} while (fail_systick)
#else
#define IWDG_MAINLOOP_FAIL \
	do                     \
	{                      \
	} while (0)
#define IWDG_TIMER_FAIL \
	do                  \
	{                   \
	} while (0)
#endif // FAIL_DEBUG

#define IWDG_MAINLOOP       \
	do                      \
	{                       \
		IWDG_MAINLOOP_INC;  \
		IWDG_MAINLOOP_FAIL; \
	} while (0)
#define IWDG_TIMER        \
	do                    \
	{                     \
		IWDG_TIMER_CLEAR; \
		IWDG_TIMER_FAIL;  \
	} while (0)

//! \def IWDG_MAINLOOP
//! \brief Макрос IWDG_MAINLOOP нужно вызвать в главном цикле

//! \def IWDG_TIMER
//! \brief Макрос IWDG_TIMER нужно вызвать в системном таймере

//! \def IWDG_INIT
//! \brief Макрос IWDG_INIT инициализирует сторожевой таймер

//! \def IWDG_TIMER_FAIL
//! \brief Макрос IWDG_TIMER_FAIL вызывает сбой в системном таймере

//! \def IWDG_MAINLOOP_FAIL
//! \brief Макрос IWDG_MAINLOOP_FAIL вызывает сбой в главном цикле

//! \def IWDG_MAINLOOP_INC
//! \brief Макрос IWDG_MAINLOOP_INC управляет переменной #mainloop_counter

//! \def IWDG_TIMER_CLEAR
//! \brief Макрос IWDG_TIMER_CLEAR сбрасывает сторожевой таймер в системном таймере

/******************************************************************************
 * FUNCTION
 *****************************************************************************/
#ifdef PRESENT_IWDG
//-----------------------------------------------------------------------------
//! \fn extern void iwdg_start(WORD prescaler)
//! \brief Запустить сторожевой таймер
//! \param prescaler Предделитель \ref IWDG_PRESCALER
//-----------------------------------------------------------------------------
extern void iwdg_start(WORD prescaler);

//-----------------------------------------------------------------------------
//! \fn extern void iwdg_update(WORD value)
//! \brief Обновить таймаут сторожевого таймера
//! \param value Таймаут сторожевого таймера \ref TO_IWDG, \ref VAL_IWDG
//-----------------------------------------------------------------------------
extern void iwdg_update(WORD value);

//-----------------------------------------------------------------------------
//! \fn extern void iwdg_clear(void)
//! \brief Сбросить сторожевой таймер
//-----------------------------------------------------------------------------
extern void iwdg_clear(void);
#endif // PRESENT_IWDG

/******************************************************************************
 * VARIABLE
 *****************************************************************************/
#ifdef PRESENT_IWDG
extern volatile int mainloop_counter; //!< Счетчик в главном цикле
#endif								  // PRESENT_IWDG

#ifdef FAIL_DEBUG
extern volatile int fail_systick;  //!< Сбой системного таймера
extern volatile int fail_mainloop; //!< Сбой главного цикла
#endif							   // FAIL_DEBUG

#endif // __WATCHDOG_H__
/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
