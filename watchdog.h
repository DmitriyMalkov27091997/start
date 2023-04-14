/**
 * @file watchdog.h
 * @author ����� �.�. (dnega@mail.ru)
 * @brief ������������ ���� ����������� �������.
 * @version 0.1
 * @date 20.05.2016
 *
 * @copyright (C) ���������, 2016
 *
 */
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

/******************************************************************************
 * README
 ******************************************************************************
����������� ��� ������� �������:
	- PRESENT_IWDG
	- FAIL_DEBUG

���������� � ������ �� ������� �������:
	- IWDG_INIT - � main() ��� ������������� �� mainloop
	- IWDG_MAINLOOP - � mainloop
	- IWDG_TIMER - � ����� SysTick

����������:
	- ������ �������������, �������� � watchdog.h � ����������� TO_IWDG

******************************************************************************/

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include "define.h"

/******************************************************************************
 * DEFINE
 *****************************************************************************/
#ifdef PRESENT_IWDG
#define IWDG_PRESCALER (256) //!< ������������ ����������� �������
#define TO_IWDG (10000UL)	 //!< ������� ����������� �������, ���
#define VAL_IWDG (FLSI * TO_IWDG / IWDG_PRESCALER / 1000UL)
//!< ������� ����������� �������, ������
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
//! \brief ������ IWDG_MAINLOOP ����� ������� � ������� �����

//! \def IWDG_TIMER
//! \brief ������ IWDG_TIMER ����� ������� � ��������� �������

//! \def IWDG_INIT
//! \brief ������ IWDG_INIT �������������� ���������� ������

//! \def IWDG_TIMER_FAIL
//! \brief ������ IWDG_TIMER_FAIL �������� ���� � ��������� �������

//! \def IWDG_MAINLOOP_FAIL
//! \brief ������ IWDG_MAINLOOP_FAIL �������� ���� � ������� �����

//! \def IWDG_MAINLOOP_INC
//! \brief ������ IWDG_MAINLOOP_INC ��������� ���������� #mainloop_counter

//! \def IWDG_TIMER_CLEAR
//! \brief ������ IWDG_TIMER_CLEAR ���������� ���������� ������ � ��������� �������

/******************************************************************************
 * FUNCTION
 *****************************************************************************/
#ifdef PRESENT_IWDG
//-----------------------------------------------------------------------------
//! \fn extern void iwdg_start(WORD prescaler)
//! \brief ��������� ���������� ������
//! \param prescaler ������������ \ref IWDG_PRESCALER
//-----------------------------------------------------------------------------
extern void iwdg_start(WORD prescaler);

//-----------------------------------------------------------------------------
//! \fn extern void iwdg_update(WORD value)
//! \brief �������� ������� ����������� �������
//! \param value ������� ����������� ������� \ref TO_IWDG, \ref VAL_IWDG
//-----------------------------------------------------------------------------
extern void iwdg_update(WORD value);

//-----------------------------------------------------------------------------
//! \fn extern void iwdg_clear(void)
//! \brief �������� ���������� ������
//-----------------------------------------------------------------------------
extern void iwdg_clear(void);
#endif // PRESENT_IWDG

/******************************************************************************
 * VARIABLE
 *****************************************************************************/
#ifdef PRESENT_IWDG
extern volatile int mainloop_counter; //!< ������� � ������� �����
#endif								  // PRESENT_IWDG

#ifdef FAIL_DEBUG
extern volatile int fail_systick;  //!< ���� ���������� �������
extern volatile int fail_mainloop; //!< ���� �������� �����
#endif							   // FAIL_DEBUG

#endif // __WATCHDOG_H__
/******************************************************************************
 * ����� �����
 *****************************************************************************/
