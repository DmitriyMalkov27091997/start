/**
 * @file watchdog.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Сторожевой таймер.
 * @version 0.1
 * @date 20.05.2016
 * 
 * @copyright (C) Ярославль, 2016
 * 
 */
#include "watchdog.h"

#ifdef PRESENT_IWDG
volatile int		mainloop_counter = 0;
#endif // PRESENT_IWDG

#ifdef FAIL_DEBUG
volatile int		fail_systick = 0;
volatile int		fail_mainloop = 0;
#endif // FAIL_DEBUG

#ifdef PRESENT_IWDG
//-----------------------------------------------------------------------------
//	void iwdg_start(WORD prescaler)
//-----------------------------------------------------------------------------
void iwdg_start(WORD prescaler)
{
	IWDG->KR = IWDG_KEY_START;
	IWDG->KR = IWDG_KEY_UPDATE;
	IWDG->PR = prescaler;
	IWDG->KR = IWDG_KEY_CLEAR;
}

//-----------------------------------------------------------------------------
//	void iwdg_update(WORD value)
//-----------------------------------------------------------------------------
void iwdg_update(WORD value)
{
	IWDG->KR = IWDG_KEY_UPDATE;
	IWDG->RLR = value;
	IWDG->KR = IWDG_KEY_CLEAR;
}

//-----------------------------------------------------------------------------
//	void iwdg_clear(void)
//-----------------------------------------------------------------------------
void iwdg_clear(void)
{
	IWDG->KR = IWDG_KEY_CLEAR;
}
#endif // PRESENT_IWDG

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
