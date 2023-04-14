/**
 * @file isr.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Файл обработчиков прерываний.
 * @version 0.1
 * @date 20.05.2016
 *
 * @copyright (C) Ярославль, 2016
 *
 */

#include <math.h>
#include "isr.h"
#include "hart.h"
#include "menu.h"
#include "menu_v2.h"
#include "watchdog.h"
#include "spi_sensor.h"

int __errno;

const btn_value[] =
	{
		186, 527, 814, 1047, 1248, 1433, 1596, 2000
		// 184, 523, 808, 1039, 1239, 1424, 1587, 1740
};

volatile int ht_cnt = 0;
volatile int tc_cnt = 0;
volatile double full_angle = 0;
volatile double curr_angle = 0;
volatile double linearize = 0;
volatile int hmc_adc_lim_0 = 0;
volatile int hmc_adc_lim_100 = 0;
volatile WORD hmc_adc_cur = 0; // Последнее значение АЦП по положению.

//-----------------------------------------------------------------------------
//	void get_adc_for_percent(int percent)
// Получить значение АЦП по указанному значению процента диапазона.
//-----------------------------------------------------------------------------
int get_adc_for_percent(int percent)
{
	int point = abs(data_eeprom.hmc_adc_100 - data_eeprom.hmc_adc_0) * percent / 1000;

	if (0)
	{
	}
	else if (data_eeprom.hmc_adc_0 < data_eeprom.hmc_adc_100) // прямая шкала
	{
		point = data_eeprom.hmc_adc_0 + point;
	}
	else // обратная шкала
	{
		point = data_eeprom.hmc_adc_100 - point;
	}

	return point;
}

int minimum(int x, int y)
{
	if (x < y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

//-----------------------------------------------------------------------------
//  Возвращает посчитанное абсолютное положение (в 0,1%) с выключенной линеаризацией.
//	int get_hmc_pos(void)
//-----------------------------------------------------------------------------
int get_hmc_pos(void)
{
	update_hmc_pos(hmc_adc_cur, 0);
	return ihmc_pos_a;
}

//-----------------------------------------------------------------------------
//	void update_hmc_pos(const int tihmc_adc_s, const int linear_en)
//-----------------------------------------------------------------------------
void update_hmc_pos(const int tihmc_adc_s, const int linear_en)
{
	pos_value = tihmc_adc_s;
	ihmc_adc_s = tihmc_adc_s;

	if (linear_en > 0)
	{
		//---------------------------------------
		//  Линеаризация положения
		//---------------------------------------

		hmc_adc_lim_0 = get_adc_for_percent(data_eeprom.hmc_lim_0);
		hmc_adc_lim_100 = get_adc_for_percent(data_eeprom.hmc_lim_100);

		full_angle = (data_eeprom.hmc_g_100 - data_eeprom.hmc_g_0) * M_PI / 1800;
		curr_angle = full_angle * ((double)(ihmc_adc_s - minimum(hmc_adc_lim_0, hmc_adc_lim_100)) /
								   abs(hmc_adc_lim_100 - hmc_adc_lim_0));

		linearize = (sin(M_PI / 2 - (full_angle / 2)) * sin(curr_angle)) /
					(-1.0 * sin(full_angle) * sin((curr_angle * -1) - (M_PI / 2) + (full_angle / 2)));
		double dfhmc_adc_s = abs(hmc_adc_lim_100 - hmc_adc_lim_0) * linearize + minimum(hmc_adc_lim_0, hmc_adc_lim_100);
		ihmc_adc_s = dfhmc_adc_s;
	}

	//---------------------------------------
	//	Фильтрация hmc_adc(hmc_adc_s)
	//---------------------------------------
	if (((ihmc_adc_s - 100) < fhmc_adc) && (fhmc_adc < (ihmc_adc_s + 100)))
		fhmc_adc = fhmc_adc + (ihmc_adc_s - fhmc_adc) * data_eeprom.lpf_pos;
	else
		fhmc_adc = ihmc_adc_s;

	ihmc_adc = fhmc_adc;

	//---------------------------------------
	//	Расчет hmc_pos_s(hmc_adc)
	//---------------------------------------
	if (0)
	{
	}
	else if (data_eeprom.hmc_adc_0 < data_eeprom.hmc_adc_100)
	{
		// прямая шкала
		if (0)
		{
		}
		else if (ihmc_adc < data_eeprom.hmc_adc_0)
			fhmc_pos_a = 0.0;
		else if (ihmc_adc > data_eeprom.hmc_adc_100)
			fhmc_pos_a = 100.0;
		else
		{
			fhmc_pos_a = 0.0 + (ihmc_adc - data_eeprom.hmc_adc_0) * 100.0 / (data_eeprom.hmc_adc_100 - data_eeprom.hmc_adc_0);
		}
	}
	else if (data_eeprom.hmc_adc_0 > data_eeprom.hmc_adc_100)
	{
		// обратная шкала
		if (0)
		{
		}
		else if (ihmc_adc < data_eeprom.hmc_adc_100)
			fhmc_pos_a = 100.0;
		else if (ihmc_adc > data_eeprom.hmc_adc_0)
			fhmc_pos_a = 0.0;
		else
		{
			fhmc_pos_a = 0.0 + (data_eeprom.hmc_adc_0 - ihmc_adc) * 100.0 / (data_eeprom.hmc_adc_0 - data_eeprom.hmc_adc_100);
		}
	}
	else
	{
		fhmc_pos_a = 0.0;
	}
	ihmc_pos_a = fhmc_pos_a * 10.0;

	//---------------------------------------
	//	Расчет hmc_pos(hmc_pos_s)
	//---------------------------------------
	if (0)
	{
	}
	else if (data_eeprom.hmc_lim_0 < data_eeprom.hmc_lim_100)
	{
		// прямая шкала
		if (0)
		{
		}
		else if (ihmc_pos_a < data_eeprom.hmc_lim_0)
			fhmc_pos = 0.0;
		else if (ihmc_pos_a > data_eeprom.hmc_lim_100)
			fhmc_pos = 100.0;
		else
		{
			fhmc_pos = 0.0 + ((fhmc_pos_a * 10.0) - data_eeprom.hmc_lim_0) * 100.0 / (data_eeprom.hmc_lim_100 - data_eeprom.hmc_lim_0);
		}
	}
	else if (data_eeprom.hmc_lim_0 > data_eeprom.hmc_lim_100)
	{
		// обратная шкала
		if (0)
		{
		}
		else if (ihmc_pos_a < data_eeprom.hmc_lim_100)
			fhmc_pos = 100.0;
		else if (ihmc_pos_a > data_eeprom.hmc_lim_0)
			fhmc_pos = 0.0;
		else
		{
			fhmc_pos = 0.0 + (data_eeprom.hmc_lim_0 - (fhmc_pos_a * 10.0)) * 100.0 / (data_eeprom.hmc_lim_0 - data_eeprom.hmc_lim_100);
		}
	}
	else
	{
		fhmc_pos = fhmc_pos_a;
	}
	ihmc_pos = fhmc_pos * 10.0;
}

//-----------------------------------------------------------------------------
//	void update_hmc_disp(void)
//-----------------------------------------------------------------------------
void update_hmc_disp(void)
{
	static float fhmc_adc_d = 0.0;

	//---------------------------------------
	//	Фильтрация hmc_adc_d(hmc_adc_s)
	//---------------------------------------
	switch (data_eeprom.flags.display_filter)
	{
	case 0:
		fhmc_adc_d = fhmc_adc;
		break;
	case 1:
		fhmc_adc_d = ihmc_adc_s;
		break;
	default:
		if (((ihmc_adc_s - 100) < fhmc_adc_d) && (fhmc_adc_d < (ihmc_adc_s + 100)))
			fhmc_adc_d = fhmc_adc_d + ((float)ihmc_adc_s - fhmc_adc_d) * hmc_disp_fs[data_eeprom.flags.display_filter - 2];
		else
			fhmc_adc_d = ihmc_adc_s;
		break;
	}
	ihmc_adc_d = fhmc_adc_d;

	//---------------------------------------
	//	Расчет hmc_pos_da(hmc_adc_d) и hmc_g(hmc_adc_d)
	//---------------------------------------
	if (0)
	{
	}
	else if (data_eeprom.hmc_adc_0 < data_eeprom.hmc_adc_100)
	{
		// прямая шкала
		if (0)
		{
		}
		else if (ihmc_adc_d < data_eeprom.hmc_adc_0)
		{
			ihmc_pos_da = 0;
			ihmc_g = data_eeprom.hmc_g_0;
		}
		else if (ihmc_adc_d > data_eeprom.hmc_adc_100)
		{
			ihmc_pos_da = 1000;
			ihmc_g = data_eeprom.hmc_g_100;
		}
		else
		{
			ihmc_pos_da = 0 + (ihmc_adc_d - data_eeprom.hmc_adc_0) * 1000 / (data_eeprom.hmc_adc_100 - data_eeprom.hmc_adc_0);

			ihmc_g = data_eeprom.hmc_g_0 + (ihmc_adc_d - data_eeprom.hmc_adc_0) * (data_eeprom.hmc_g_100 - data_eeprom.hmc_g_0) / (data_eeprom.hmc_adc_100 - data_eeprom.hmc_adc_0);
		}
	}
	else if (data_eeprom.hmc_adc_0 > data_eeprom.hmc_adc_100)
	{
		// обратная шкала
		if (0)
		{
		}
		else if (ihmc_adc_d < data_eeprom.hmc_adc_100)
		{
			ihmc_pos_da = 1000;
			ihmc_g = data_eeprom.hmc_g_100;
		}
		else if (ihmc_adc_d > data_eeprom.hmc_adc_0)
		{
			ihmc_pos_da = 0;
			ihmc_g = data_eeprom.hmc_g_0;
		}
		else
		{
			ihmc_pos_da = 0 + (data_eeprom.hmc_adc_0 - ihmc_adc_d) * 1000 / (data_eeprom.hmc_adc_0 - data_eeprom.hmc_adc_100);

			ihmc_g = data_eeprom.hmc_g_0 + (data_eeprom.hmc_adc_0 - ihmc_adc_d) * (data_eeprom.hmc_g_100 - data_eeprom.hmc_g_0) / (data_eeprom.hmc_adc_0 - data_eeprom.hmc_adc_100);
		}
	}
	else
	{
		ihmc_pos_da = 0;
		ihmc_g = 0;
	}

	//---------------------------------------
	//	Расчет hmc_pos_d(hmc_pos_da)
	//---------------------------------------
	if (0)
	{
	}
	else if (data_eeprom.hmc_lim_0 < data_eeprom.hmc_lim_100)
	{
		// прямая шкала
		if (0)
		{
		}
		else if (ihmc_pos_da < data_eeprom.hmc_lim_0)
			ihmc_pos_d = 0;
		else if (ihmc_pos_da > data_eeprom.hmc_lim_100)
			ihmc_pos_d = 1000;
		else
		{
			ihmc_pos_d = 0 + (ihmc_pos_da - data_eeprom.hmc_lim_0) * 1000 / (data_eeprom.hmc_lim_100 - data_eeprom.hmc_lim_0);
		}
	}
	else if (data_eeprom.hmc_lim_0 > data_eeprom.hmc_lim_100)
	{
		// обратная шкала
		if (0)
		{
		}
		else if (ihmc_pos_da < data_eeprom.hmc_lim_100)
			ihmc_pos_d = 1000;
		else if (ihmc_pos_da > data_eeprom.hmc_lim_0)
			ihmc_pos_d = 0;
		else
		{
			ihmc_pos_d = 0 + (data_eeprom.hmc_lim_0 - ihmc_pos_da) * 1000 / (data_eeprom.hmc_lim_0 - data_eeprom.hmc_lim_100);
		}
	}
	else
	{
		ihmc_pos_d = ihmc_pos_da;
	}
}

//-----------------------------------------------------------------------------
//	void set_coil_pwm(const int t_coil_pwm)
//-----------------------------------------------------------------------------
void set_coil_pwm(const int t_coil_pwm)
{
	int i;
	coil_pwm = t_coil_pwm;

	i = data_eeprom.coil_pwm_limit;
	if (i < 0)
		i = 0;
	if (i > 90)
		i = 90;
	if (coil_pwm < (MAX_PWM_COIL * i / 200))
		coil_pwm = MAX_PWM_COIL * i / 200;
	if (coil_pwm > (MAX_PWM_COIL - MAX_PWM_COIL * i / 200))
		coil_pwm = MAX_PWM_COIL - MAX_PWM_COIL * i / 200;

	if (to_power_fail < TO_POWER_FAIL)
		PWM_COIL = 0;
	else
		PWM_COIL = coil_pwm;
}

//-----------------------------------------------------------------------------
//	int an_stat_pos(void)
//-----------------------------------------------------------------------------
int an_stat_pos(void)
{
	int i, min, max, avg;
	min = stat_pos_arr[0];
	max = stat_pos_arr[0];
	avg = stat_pos_arr[0];
	for (i = 1; i < STAT_POS_SIZE; i++)
	{
		if (min > stat_pos_arr[i])
			min = stat_pos_arr[i];
		if (max < stat_pos_arr[i])
			max = stat_pos_arr[i];
		avg += stat_pos_arr[i];
	}
	stat_pos_min = min;
	stat_pos_max = max;
	stat_pos_delta = max - min;
	stat_pos_avg = avg / STAT_POS_SIZE;
	return (max - min);
}

//-----------------------------------------------------------------------------
//	void auto_update(const WORD adc_hmc, const WORD adc_btn)
//-----------------------------------------------------------------------------
void auto_update(const WORD adc_hmc, const WORD adc_btn)
{
	int i, d;
	float hmc_d;
	static float hmc_d_prev;
	static float hmc_d_int = 0.0f;
	float hmc_d_dif;
	float coil_pwm_f;
	static float hmc_pos_th = 0.0f;
	float lp_up, lp_down, kp;
	sEEPROM_FLAGS flags;

	btn_update(adc_btn);
	update_hmc_pos(adc_hmc, 0); // 0 - без линеаризации

	if (st_auto == ST_AUTO_IDLE)
	{
	}
	else if (st_auto == ST_AUTO_SELECT)
	{
	}
	else if (st_auto == ST_AUTO_START)
	{
		stat_pos_arr_valid = 0;
		for (i = 0; i < STAT_POS_SIZE; i++)
			stat_pos_arr[i] = STAT_POS_DEF;
		stat_pos = 0;

		eeprom_unlock();
		data_eeprom.hmc_lim_0 = 0;
		eeprom_lock();

		eeprom_unlock();
		data_eeprom.hmc_lim_100 = 1000;
		eeprom_lock();

		to_auto = 0;
		st_auto = ST_AUTO_DOWN_LIMIT;
		set_coil_pwm(0);

		con_str("auto_move_down... ");
		con_start();
	}
	else if (st_auto == ST_AUTO_DOWN_LIMIT)
	{
		stat_pos_arr[stat_pos] = ihmc_pos_a;
		stat_pos++;
		if (stat_pos >= STAT_POS_SIZE)
		{
			stat_pos = 0;
			stat_pos_arr_valid = 1;
		}
		tick(&to_auto);
		if (stat_pos_arr_valid)
		{
			d = an_stat_pos();
			if (d < 10)
			{
				stat_pos_adc_min = stat_pos_avg;

				con_str("done (");
				con_dec(stat_pos_adc_min);
				con_str(")\n\r");
				con_start();

				stat_pos_arr_valid = 0;
				for (i = 0; i < STAT_POS_SIZE; i++)
					stat_pos_arr[i] = STAT_POS_DEF;
				stat_pos = 0;
				to_auto = 0;
				st_auto = ST_AUTO_UP_LIMIT;
				set_coil_pwm(MAX_PWM_COIL);

				con_str("auto_move_up... ");
				con_start();
			}
		}
	}
	else if (st_auto == ST_AUTO_UP_LIMIT)
	{
		stat_pos_arr[stat_pos] = ihmc_pos_a;
		stat_pos++;
		if (stat_pos >= STAT_POS_SIZE)
		{
			stat_pos = 0;
			stat_pos_arr_valid = 1;
		}
		tick(&to_auto);
		if (stat_pos_arr_valid)
		{
			d = an_stat_pos();
			if (d < 10)
			{
				stat_pos_adc_max = stat_pos_avg;

				con_str("done (");
				con_dec(stat_pos_adc_max);
				con_str(")\n\r");
				con_start();

				eeprom_unlock();
				data_eeprom.hmc_lim_0 = stat_pos_adc_min;
				data_eeprom.hmc_lim_100 = stat_pos_adc_max;

				flags.dw = data_eeprom.flags.dw;
				flags.coil_inverse = COIL_INVERSE_NONE;
				data_eeprom.flags.dw = flags.dw;
				eeprom_lock();

				stat_pos_adc_min = 0;
				stat_pos_adc_max = 1000;
				stat_pos_delta = (stat_pos_adc_max - stat_pos_adc_min) / 100;

				to_auto = 0;
				stat_time_down = 0;
				st_auto = ST_AUTO_DOWN_TIME;
				set_coil_pwm(0);

				con_str("time_down...\n\r");
				con_start();

				/*
				stat_pos_arr_valid = 0;
				for(i = 0; i < STAT_POS_SIZE; i++) stat_pos_arr[i] = STAT_POS_DEF;
				stat_pos = 0;
				to_auto = 0;
				st_auto = ST_AUTO_UP_LIMIT;
				set_coil_pwm(MAX_PWM_COIL);
				*/
			}
		}
	}
	else if (st_auto == ST_AUTO_DOWN_TIME)
	{
		// stat_pos_cur = (ihmc_pos - stat_pos_adc_min) * 1000 / (stat_pos_adc_max - stat_pos_adc_min);
		if (ihmc_pos > (stat_pos_adc_max - stat_pos_delta))
		{
			// выше верхнего предела - 1%
		}
		else if (ihmc_pos < (stat_pos_adc_min + stat_pos_delta))
		{
			// ниже нижнего предела + 1%
			to_auto = 0;
			stat_time_up = 0;
			st_auto = ST_AUTO_UP_TIME;
			set_coil_pwm(MAX_PWM_COIL);

			con_str("time_up...\n\r");
			con_start();
		}
		else
			stat_time_down++;
	}
	else if (st_auto == ST_AUTO_UP_TIME)
	{
		// stat_pos_cur = (ihmc_pos - stat_pos_adc_min) * 1000 / (stat_pos_adc_max - stat_pos_adc_min);
		if (ihmc_pos < (stat_pos_adc_min + stat_pos_delta))
		{
			// ниже нижнего предела + 1%
		}
		else if (ihmc_pos > (stat_pos_adc_max - stat_pos_delta))
		{
			// выше верхнего предела - 1%
			stat_pos_arr_valid = 0;
			for (i = 0; i < STAT_POS_SIZE; i++)
				stat_pos_arr[i] = STAT_POS_DEF;
			stat_pos = 0;
			to_auto = 0;
			st_auto = ST_AUTO_TIME_DONE;
			hmc_d_int = 0.0f;
			stat_coil_pwm = MAX_PWM_COIL * 0.5f;
			set_coil_pwm(MAX_PWM_COIL / 2);

			if (0)
			{
			}
			else if (stat_time_up < TIME_MIN)
				lp_up = T_LP_M / TIME_MIN;
			//			else if(stat_time_up > TIME_MAX) lp_up = T_LP_M / TIME_MAX;
			else
			{
				lp_up = T_LP_M / (stat_time_up + LP_TIME_ADD);
			}

			if (0)
			{
			}
			else if (stat_time_down < TIME_MIN)
				lp_down = T_LP_M / TIME_MIN;
			//			else if(stat_time_down > TIME_MAX) lp_down = T_LP_M / TIME_MAX;
			else
			{
				lp_down = T_LP_M / (stat_time_down + LP_TIME_ADD);
			}

			kp = (stat_time_up + stat_time_down) * 0.5;

			if (0)
			{
			}
			else if (kp < TIME_MIN)
				kp = KP_MIN;
			else if (kp > TIME_MAX)
				kp = KP_MAX;
			else
			{
				kp = (kp - TIME_MIN) / (TIME_MAX - TIME_MIN) * (KP_MAX - KP_MIN) + KP_MIN;
			}
			// kp += 20.0;

			con_str("AUTORES: lp_up=");
			con_float(lp_up);
			con_str(", lp_down=");
			con_float(lp_down);
			con_str(", kp=");
			con_float(kp);
			con_str("\n\r");
			con_start();

			eeprom_unlock();

			if (auto_type == TYPE_REVERSE)
			{
				stat_pos_adc_min = data_eeprom.hmc_lim_100;
				stat_pos_adc_max = data_eeprom.hmc_lim_0;

				data_eeprom.hmc_lim_0 = stat_pos_adc_min;
				data_eeprom.hmc_lim_100 = stat_pos_adc_max;

				flags.dw = data_eeprom.flags.dw;
				flags.coil_inverse = COIL_INVERSE_INV;
				data_eeprom.flags.dw = flags.dw;

				stat_pos_adc_min = 0;
				stat_pos_adc_max = 1000;
			}

			data_eeprom.kp = kp;
			data_eeprom.ki = 0.2;
			data_eeprom.pid_int_min = -500.0 / data_eeprom.ki;
			data_eeprom.pid_int_max = 500.0 / data_eeprom.ki;
			data_eeprom.kd = 0.0;
			data_eeprom.t_lp_up = lp_up;
			data_eeprom.t_lp_down = lp_down;

			to_auto = 0;
			if (linear_temp_disable == 1)
			{
				linear_temp_disable = 0;
				data_eeprom.linear_en = 1;
			}

			eeprom_lock();
		}
		else
			stat_time_up++;
	}
	else if (st_auto == ST_AUTO_TIME_DONE)
	{
		stat_pos_arr[stat_pos] = ihmc_pos;
		stat_pos++;
		if (stat_pos >= STAT_POS_SIZE)
		{
			stat_pos = 0;
			stat_pos_arr_valid = 1;
		}
		if (stat_pos_arr_valid)
		{
			d = an_stat_pos();
		}
		// stat_pos_cur = (ihmc_pos_a - stat_pos_adc_min) * 1000 / (stat_pos_adc_max - stat_pos_adc_min);
		// stat_pos_cur = ihmc_pos_a;
		hmc_d = (float)((stat_pos_adc_max + stat_pos_adc_min) / 2 - ihmc_pos);
		hmc_d_int += hmc_d;

		if (auto_type == TYPE_DIRECT)
			coil_pwm_f = MAX_PWM_COIL * 0.5f + (5.0f) * hmc_d + (0.01f) * hmc_d_int;
		else
			coil_pwm_f = MAX_PWM_COIL * 0.5f - (5.0f) * hmc_d - (0.01f) * hmc_d_int;

		set_coil_pwm(coil_pwm_f);
		stat_coil_pwm = stat_coil_pwm + (coil_pwm_f - stat_coil_pwm) * 0.01f;
		if (stat_pos_delta < 10)
			coil_pwm_def = stat_coil_pwm;
		if (stat_pos == 0)
		{
			con_str("[coil_pwm=");
			con_dec(stat_coil_pwm);
			con_str("]");
			con_start();
		}
		tick(&to_auto);
	}
	else
	{
	}
}

//-----------------------------------------------------------------------------
//	void USART3_IRQHandler(void)
//-----------------------------------------------------------------------------
void USART3_IRQHandler(void)
{
	BYTE x;
	while (USART3->SR & (1 << USART_SR_RXNE))
	{
		// новый символ в приемнике
		con.rx_tm = 0;
		hart_on_rx(USART3->DR);
		to_hart_act = 0;
	}

	if (USART3->SR & (1 << USART_SR_TXE))
	{
		// передатчик пуст
		if (hart_on_tx(&x))
			USART3->DR = x;
		else
			USART3->CR1 &= ~(1 << USART_CR1_TXEIE);
		to_hart_act = 0;
	}

	if (USART3->SR & (1 << USART_SR_TC))
	{
		USART3->SR = ~(1 << USART_SR_TC);
		set_pin(HART_RTS_PIN, 1); // HART_RTS = 1
		to_hart_act = 0;
	}

	if (USART3->SR & (1 << USART_SR_ORE))
		USART3->DR;
}

#ifdef ADC_OVER
//-----------------------------------------------------------------------------
//	void DMA1_Channel1_IRQHandler(void)
//-----------------------------------------------------------------------------
void DMA1_Channel1_IRQHandler(void)
{
	int i, j, k, min_i, min_v, max_i, max_v, hmc_data, avg;

	if (DMA1->ISR & (1 << DMA_ISR_TCIF1))
	{
		DMA1->IFCR = (1 << DMA_IFCR_CTCIF1);
		tc_cnt++;

		for (k = 0; k < (ADC_HMC_X / 2); k++)
		{
			min_i = k;
			min_v = adc_buf[k * 2];
			max_i = ADC_HMC_N - 1 - k;
			max_v = adc_buf[(ADC_HMC_N - 1 - k) * 2];

			for (j = (k + 1); j < ADC_HMC_N; j++)
			{
				if (adc_buf[j * 2] < min_v)
				{
					min_i = j;
					min_v = adc_buf[min_i * 2];
				}

				if (adc_buf[(ADC_HMC_N - 1 - j) * 2] > max_v)
				{
					max_i = ADC_HMC_N - 1 - j;
					max_v = adc_buf[max_i * 2];
				}
			}

			adc_buf[min_i * 2] = adc_buf[k * 2];
			adc_buf[k * 2] = min_v;
			adc_buf[max_i * 2] = adc_buf[(ADC_HMC_N - 1 - k) * 2];
			adc_buf[(ADC_HMC_N - 1 - k) * 2] = max_v;
		}

		hmc_data = 0;

		for (j = (ADC_HMC_X / 2); j < (ADC_HMC_N - (ADC_HMC_X / 2)); j++)
			hmc_data += adc_buf[j * 2];

		hmc_data /= (ADC_HMC_N - ADC_HMC_X);

		// hmc_data = get_data_via_spi(to_power_fail < TO_POWER_FAIL); // если используется spi датчик

		if (st_auto == ST_AUTO_IDLE)
		{

			new_samples(
				(adc_buf[0 * 4 + 1] + adc_buf[1 * 4 + 1] + adc_buf[2 * 4 + 1] + adc_buf[3 * 4 + 1]) / 4, // ain
				hmc_data,																				 // pos
				(adc_buf[0 * 8 + 3] + adc_buf[1 * 8 + 3]) / 2,											 // btn
				(adc_buf[0 * 8 + 7] + adc_buf[1 * 8 + 7]) / 2);											 // temp
		}
		else
		{
			pid_int_clear = 1;
			auto_update(hmc_data, (adc_buf[0 * 8 + 3] + adc_buf[1 * 8 + 3]) / 2);
		}
	}
}
#endif // ADC_OVER

//-----------------------------------------------------------------------------
//	void btn_update(const WORD adc_btn)
//-----------------------------------------------------------------------------
void btn_update(const WORD adc_btn)
{
	int i;
	static int last_btn = 8;
	static int to_btn = 0;
	static int cur_btn = 8;

	btn_adc = adc_btn;

	i = 0;
	while ((i < 8) && (btn_adc > btn_value[i]))
		i++;
	if (i == last_btn)
	{
		if (to_btn < 3)
		{
			to_btn++;
			if (to_btn == 3)
			{
				cur_btn = last_btn;
				// con_str("BTN = ");
				if (cur_btn == 8)
				{
					// con_push('F');
					button_pressed = 0;
					to_button_pressed = 0;

					tamper = 1;
				}
				else
				{
					tamper = 0;

					if (data_eeprom.flags.display_rotate == DISPLAY_ROTATE_ON)
						button_pressed = (rbit8(cur_btn) >> 5) ^ 7;
					else
						button_pressed = cur_btn ^ 7;

					// con_push((cur_btn & 4)? '0' : '1');
					// con_push((cur_btn & 2)? '0' : '1');
					// con_push((cur_btn & 1)? '0' : '1');

					switch (button_pressed)
					{
					case 4:
						menu_btn.btn_down = 1;
						break;
					case 1:
						menu_btn.btn_up = 1;
						break;
					case 2:
						menu_btn.btn_ok = 1;
						break;
					case 5:
						menu_btn.btn_cancel = 1;
						break;
					default:
						to_button_pressed = 0;
						break;
					}
				}
				// con_str("\n\r");
				// con_start();
			}
		}
	}
	else
	{
		to_btn = 0;
		to_button_pressed = 0;
		last_btn = i;
	}
	if (cur_btn == 8)
		to_disp_init = 0;
}

//-----------------------------------------------------------------------------
//	void new_samples(const WORD adc_ain, const WORD adc_pos, const WORD adc_btn, const WORD adc_tmp)
//-----------------------------------------------------------------------------
void new_samples(const WORD adc_ain, const WORD adc_pos, const WORD adc_btn, const WORD adc_tmp)
{
	int i;
	int val;

	float hmc_d;
	float hmc_dp;
	static float hmc_d_prev;
	static float hmc_d_int = 0.0f;
	float hmc_d_dif;
	float coil_pwm_f;
	static float fhmc_pos_th = 0.0f;

	float ph_value;
	float ph_k;

	static int ihmc_pos_last = -1;

	static int pid_debug_n = 0;
	static int im_ofs = 0;

	static int int_delta = 0;

	// hmc_adc_cur = adc_pos;
	ain_adc_s = adc_ain;
	tempeature_adc = adc_tmp;

	update_hmc_pos(adc_pos, data_eeprom.linear_en);
	update_hmc_disp();

	i = ihmc_pos * GIST_POS_SIZE / 1000;
	if (0)
	{
	}
	else if (i < 0)
		gist_pos[0]++;
	else if (i >= GIST_POS_SIZE)
		gist_pos[GIST_POS_SIZE - 1]++;
	else
		gist_pos[i]++;

	if (hart_act_count)
	{
		switch (hart_act_data_type)
		{
		case HART_ACT_DATA_POS:
			hart_act_data[hart_act_data_pos] = ihmc_pos;
			break;
		case HART_ACT_DATA_TGT:
			hart_act_data[hart_act_data_pos] = ihmc_pos_t;
			break;
		case HART_ACT_DATA_POS_TGT:
			if ((hart_act_data_pos & 1) == 0)
			{
				hart_act_data[hart_act_data_pos + 0] = ihmc_pos;
				hart_act_data[hart_act_data_pos + 1] = ihmc_pos_t;
			}
			break;
		}
		hart_act_data_pos++;
		if (hart_act_data_pos >= 64)
			hart_act_data_pos = 0;

		if (hart_act_data_pos == 32)
			hart_act_data_page_send = 0;
		if (hart_act_data_pos == 0)
			hart_act_data_page_send = 1;
	}

	if (to_hart_act < TO_HART_ACT)
		to_hart_act++;
	else
	{
		if ((hart_act_data_page_send != -1) && (hart_act_count > 0))
		{
			hart_act_count--;
			hart_send_act();
			hart_act_data_page_send = -1;
		}
	}

#ifdef NO_AMP
	set_pin(TEST_PIN, (to_hart_act < TO_HART_ACT) ? 0 : 1);
#endif // NO_AMP

	//---------------------------------------
	//	Расчет температуры
	//---------------------------------------
	tempeature = (110L - 30L) * (tempeature_adc * 3300L / 3000L - TS_CAL1) / (TS_CAL2 - TS_CAL1) + 30L;

	if (tempeature != TEMPERATURE_BAD)
	{
		i = (tempeature + 50) * GIST_TMP_SIZE / (80 + 50);
		if (0)
		{
		}
		else if (i < 0)
			gist_tmp[0]++;
		else if (i >= GIST_TMP_SIZE)
			gist_tmp[GIST_TMP_SIZE - 1]++;
		else
			gist_tmp[i]++;
	}

	//---------------------------------------
	//	Обработка кнопок
	//---------------------------------------
	btn_update(adc_btn);

	//---------------------------------------
	//	Показать данные с АЦП
	//---------------------------------------
	if (get_adc_flag)
	{
		get_adc_flag = 0;

		con_str("ADC: hmc = ");
		con_dec(ihmc_adc_s);
		con_str("[");
		con_dec(ihmc_adc);
		con_str("], ain = ");
		con_dec(ain_adc_s);
		con_str(", btn = ");
		con_dec(btn_adc);
		con_str(", tem = ");
		con_dec(tempeature);
		con_str("\n\r");
		con_start();
	}

	//---------------------------------------
	//	Фильтрация ain_adc(ain_adc_s)
	//---------------------------------------

	if (((ain_adc_s - 100) < fain_adc) && (fain_adc < (ain_adc_s + 100)))
	{
		float lpf = Get_LpfValue(data_eeprom.lpf_a_in);// data_eeprom.lpf_max_samples / (FADC * data_eeprom.lpf_a_in);
		fain_adc = fain_adc + (ain_adc_s - fain_adc) * lpf;
	}
	else
		fain_adc = ain_adc_s;

	//---------------------------------------
	//	Расчет ain_ma_s(ain_adc)
	//---------------------------------------
	ain_ma_s = data_eeprom.ain_ma_min + (fain_adc - data_eeprom.ain_adc_min) * (data_eeprom.ain_ma_max - data_eeprom.ain_ma_min) / (data_eeprom.ain_adc_max - data_eeprom.ain_adc_min);

	//---------------------------------------
	//	Получение задания по HART
	//---------------------------------------
	if ((data_eeprom.flags.hart_prio == HART_PRIO_HART) && (hart_ext_current != 0.0f))
		hart_ma = hart_ext_current * 100.0f;
	else
		hart_ma = -1;

#ifdef AIN_AVG

	if (ain_avg_tmin < 0)
		ain_avg_tmin = ain_adc;
	if (ain_avg_tmax < 0)
		ain_avg_tmax = ain_adc;

	if (ain_adc < ain_avg_tmin)
		ain_avg_tmin = ain_adc;
	if (ain_adc > ain_avg_tmax)
		ain_avg_tmax = ain_adc;

	if (ain_avg_sum < 0)
		ain_avg_sum = ain_adc;
	else
		ain_avg_sum += ain_adc;
	ain_avg_cnt++;

	if (to_ain_avg > 10000UL)
	{
		to_ain_avg = 0;

		ain_avg_min = ain_avg_tmin;
		ain_avg_max = ain_avg_tmax;
		ain_avg_avg = ain_avg_sum / ain_avg_cnt;

		ain_avg_tmin = -1;
		ain_avg_tmax = -1;
		ain_avg_sum = -1;
		ain_avg_cnt = 0;

		ain_avg_done = 1;
	}
#endif // AIN_AVG

	//---------------------------------------
	// Учет хода клапана
	//---------------------------------------
	if (ihmc_path != -1)
	{
		if (ihmc_pos_last == -1)
			ihmc_pos_last = ihmc_pos;

		if (ihmc_pos > (ihmc_pos_last + HMC_PATH_DELTA))
		{
			ihmc_path += ihmc_pos - ihmc_pos_last;
			ihmc_pos_last = ihmc_pos;
		}
		else if ((ihmc_pos + HMC_PATH_DELTA) < ihmc_pos_last)
		{
			ihmc_path += ihmc_pos_last - ihmc_pos;
			ihmc_pos_last = ihmc_pos;
		}
	}

	i = ain_ma_s;
	if (i < 400)
		i = 400;
	if (i > 2000)
		i = 2000;
	if ((data_eeprom.flags.im_ph != 5) && (data_eeprom.flags.im_ph < 11))
	{
		ph_value = (i - 400) / 1600.0f;
		ph_k = im_ph_k[data_eeprom.flags.im_ph];
		ph_value = (1.0f - 1.0f / powf(ph_k, ph_value)) / (1.0f - 1.0f / ph_k);
		ain_ma = (int)(ph_value * 1600.0f + 400.0f);
	}
	else
		ain_ma = i;

	switch (pid_source)
	{
	case PID_SOURCE_SIGNAL:
		//---------------------------------------
		//	Рассчет hmc_pos_t(ain_ma)
		//---------------------------------------
		if (hart_ma == -1)
		{
			if (data_eeprom.ain_ma_t_min < data_eeprom.ain_ma_t_max)
			{
				// AIN
				if (0)
				{
				}
				else if (ain_ma < data_eeprom.ain_ma_t_min)
					ihmc_pos_t = data_eeprom.hmc_pos_t_min;
				else if (ain_ma > data_eeprom.ain_ma_t_max)
					ihmc_pos_t = data_eeprom.hmc_pos_t_max;
				else
				{
					ihmc_pos_t = data_eeprom.hmc_pos_t_min + (ain_ma - data_eeprom.ain_ma_t_min) * (data_eeprom.hmc_pos_t_max - data_eeprom.hmc_pos_t_min) / (data_eeprom.ain_ma_t_max - data_eeprom.ain_ma_t_min);
				}
			}
			else
			{
				// AIN
				if (0)
				{
				}
				else if (ain_ma > data_eeprom.ain_ma_t_min)
					ihmc_pos_t = data_eeprom.hmc_pos_t_min;
				else if (ain_ma < data_eeprom.ain_ma_t_max)
					ihmc_pos_t = data_eeprom.hmc_pos_t_max;
				else
				{
					ihmc_pos_t = data_eeprom.hmc_pos_t_min + (data_eeprom.ain_ma_t_min - ain_ma) * (data_eeprom.hmc_pos_t_max - data_eeprom.hmc_pos_t_min) / (data_eeprom.ain_ma_t_min - data_eeprom.ain_ma_t_max);
				}
			}
		}
		else
		{
			// HART
			if (0)
			{
			}
			else if (hart_ma < 400)
				ihmc_pos_t = data_eeprom.hmc_pos_t_min;
			else if (hart_ma > 2000)
				ihmc_pos_t = data_eeprom.hmc_pos_t_max;
			else
			{
				ihmc_pos_t = data_eeprom.hmc_pos_t_min + (hart_ma - 400) * (data_eeprom.hmc_pos_t_max - data_eeprom.hmc_pos_t_min) / (2000 - 400);
			}
		}
		break;

	case PID_SOURCE_HAND:
		ihmc_pos_t = menu_hand_pos_t;
		break;

		// case HMC_T_SOURCE_HART:
		// hmc_pos_t = 500;
		// break;
	}

	i = ihmc_pos_t * GIST_TGT_SIZE / 1000;
	if (0)
	{
	}
	else if (i < 0)
		gist_tgt[0]++;
	else if (i >= GIST_TGT_SIZE)
		gist_tgt[GIST_TGT_SIZE - 1]++;
	else
		gist_tgt[i]++;

	//-----------------------------------------------------------------------------
	//	Step Test
	//-----------------------------------------------------------------------------
	if (step_test_n)
	{
		ihmc_pos_t = step_test_min + (step_test_max - step_test_min) * step_test_c / step_test_n;
		step_test_to++;
		if (step_test_to >= step_test_t)
		{
			step_test_to = 0;
			step_test_c++;
			if (step_test_c > step_test_n)
			{
				step_test_n = 0;
				step_test_c = 0;
				step_test_t = 0;
				step_test_to = 0;
			}
		}
	}

	//-----------------------------------------------------------------------------
	//	PST
	//-----------------------------------------------------------------------------
	if (pst_t)
	{
		if (0)
		{
		}
		else if (pst_to < pst_t)
		{
			// шаг
			ihmc_pos_t = pst_cur;
			if ((abs(pst_cur - ihmc_pos) < 10) && (pst_to < pst_te))
				pst_result |= (1 << 0);
			pst_to++;
		}
		else if (pst_to < pst_t * 2)
		{
			// обратно
			ihmc_pos_t = pst_last;
			if ((abs(pst_last - ihmc_pos) < 10) && ((pst_to - pst_t) < pst_te))
				pst_result |= (1 << 1);
			pst_to++;
		}
		else
		{
			con_str("PST result=");
			con_dec(pst_result);
			con_str("\n\r");
			con_start();
			if (pst_result != 3)
				pst_event_fail = TO_PST_EVENT;
			pst_t = 0;
		}
	}
	else
	{
		if (data_eeprom.pst_autorun)
		{
			pst_auto_to++;
			// HERE PST в часах
			if (pst_auto_to > (data_eeprom.pst_autorun * 60 * 60 * FADC))
			// if(pst_auto_to > (data_eeprom.pst_autorun * 60 * FADC))
			{
				pst_auto_to = 0;
				pst_to = 0;
				pst_te = data_eeprom.pst_te * FADC;
				pst_last = ihmc_pos_t;
				pst_cur = pst_last + data_eeprom.pst_dx * 10;
				pst_result = 0;
				pst_t = data_eeprom.pst_t * FADC;
			}
		}
		else
			pst_auto_to = 0;
	}

	if (pst_event_fail)
		pst_event_fail--;

	//---------------------------------------
	//	LOF-тест
	//---------------------------------------
	if (lof_time && (lof_period > 0))
	{
		ihmc_pos_t = lof_position + lof_amplitude * sinf(M_TWOPI * lof_to / lof_period);
		lof_to++;
		if (lof_to >= lof_time)
		{
			lof_position = 0;
			lof_amplitude = 0;
			lof_period = 0;
			lof_to = 0;
			lof_time = 0;
		}
	}

	//---------------------------------------
	//	HERE ПИД-регулятор coil_pwm
	//---------------------------------------
	if (0)
	{
	}
	else if ((ihmc_pos_t != -1) && (!step_test_n) && ((data_eeprom.hard_pos_min != 0) && (ihmc_pos_t < data_eeprom.hard_pos_min)))
	{
		coil_pwm = (data_eeprom.flags.coil_inverse == 0) ? 0 : MAX_PWM_COIL;
		hmc_d_int = 0.0;
		fhmc_pos_th = fhmc_pos;
	}
	else if ((ihmc_pos_t != -1) && (!step_test_n) && ((data_eeprom.hard_pos_max != 0) && (ihmc_pos_t > (1000 - data_eeprom.hard_pos_max))))
	{
		coil_pwm = (data_eeprom.flags.coil_inverse == 0) ? MAX_PWM_COIL : 0;
		hmc_d_int = 0.0;
		fhmc_pos_th = fhmc_pos;
	}
	else
	{
		// PID
		if (pid_int_clear)
		{
			pid_int_clear = 0;
			hmc_d_int = 0.0;
		}

		if (ihmc_pos_t < (0 + LIMIT_DELTA))
			ihmc_pos_t = 0 + LIMIT_DELTA;
		if (ihmc_pos_t > (1000 - LIMIT_DELTA))
			ihmc_pos_t = 1000 - LIMIT_DELTA;

		if (0)
		{
		}
		else if ((ihmc_pos_t * 0.1) > fhmc_pos_th) // up
		{
			if ((tempeature != TEMPERATURE_BAD) && (tempeature < data_eeprom.slow_LT_edge))
				fhmc_pos_th = fhmc_pos_th + (ihmc_pos_t * 0.1 - fhmc_pos_th) * data_eeprom.t_lp_up * powf(data_eeprom.slow_LT_val, abs(tempeature - data_eeprom.slow_LT_edge) / 10);
			else
				fhmc_pos_th = fhmc_pos_th + (ihmc_pos_t * 0.1 - fhmc_pos_th) * data_eeprom.t_lp_up;
		}
		else if ((ihmc_pos_t * 0.1) < fhmc_pos_th) // down
		{
			if ((tempeature != TEMPERATURE_BAD) && (tempeature < data_eeprom.slow_LT_edge))
				fhmc_pos_th = fhmc_pos_th + (ihmc_pos_t * 0.1 - fhmc_pos_th) * data_eeprom.t_lp_down * powf(data_eeprom.slow_LT_val, abs(tempeature - data_eeprom.slow_LT_edge) / 10);
			else
				fhmc_pos_th = fhmc_pos_th + (ihmc_pos_t * 0.1 - fhmc_pos_th) * data_eeprom.t_lp_down;
		}
		hmc_d = fhmc_pos_th - fhmc_pos;

		if (fabs(hmc_d) < data_eeprom.si_off)
			hmc_d_int += hmc_d;

		if (hmc_d_int < data_eeprom.pid_int_min)
			hmc_d_int = data_eeprom.pid_int_min;
		if (hmc_d_int > data_eeprom.pid_int_max)
			hmc_d_int = data_eeprom.pid_int_max;
		hmc_d_dif = hmc_d - hmc_d_prev;
		hmc_d_prev = hmc_d;

		{
			if (!data_eeprom.flags.coil_inverse)
				coil_pwm_f = coil_pwm_def + data_eeprom.kp * hmc_d + data_eeprom.ki * hmc_d_int + data_eeprom.kd * hmc_d_dif;
			else
				coil_pwm_f = coil_pwm_def - data_eeprom.kp * hmc_d - data_eeprom.ki * hmc_d_int - data_eeprom.kd * hmc_d_dif;
		}

		info_hmc_d = hmc_d;
		info_hmc_d_pr = data_eeprom.kp * hmc_d;
		info_hmc_d_int = data_eeprom.ki * hmc_d_int;
		info_hmc_d_dif = data_eeprom.kd * hmc_d_dif;
		info_coil = coil_pwm_f;

		pid_int = hmc_d_int;

		if (0)
		{
		}
		else if (coil_pwm_f < 0.0f)
			coil_pwm = 0;
		else if (coil_pwm_f > ((float)MAX_PWM_COIL))
			coil_pwm = MAX_PWM_COIL;
		else
			coil_pwm = (int)coil_pwm_f;
	}

	//-------------------------------------------
	//	Высокое трение
	//-------------------------------------------
	if (abs(ihmc_pos_t - ihmc_pos) < (data_eeprom.hif_dx * 10))
		hif_to = 0;
	else
	{
		if (hif_to < (data_eeprom.hif_t * FADC))
			hif_to++;
	}

	if ((0 <= lof_pos) && (lof_pos < LOF_SIZE))
	{
		lof_arr[0][lof_pos] = fhmc_pos;
		lof_arr[1][lof_pos] = fhmc_pos_th;
		lof_pos++;
	}

	switch (menu_calib_pwm)
	{
	case MENU_CALIB_PWM_0:
		coil_pwm = 0;
		break;
	case MENU_CALIB_PWM_50:
		coil_pwm = data_eeprom.midpoint;
		break;
	case MENU_CALIB_PWM_100:
		coil_pwm = MAX_PWM_COIL;
		break;
	}

	i = data_eeprom.coil_pwm_limit;
	if (i < 0)
		i = 0;
	if (i > 90)
		i = 90;
	if (coil_pwm < (MAX_PWM_COIL * i / 200))
		coil_pwm = MAX_PWM_COIL * i / 200;
	if (coil_pwm > (MAX_PWM_COIL - MAX_PWM_COIL * i / 200))
		coil_pwm = MAX_PWM_COIL - MAX_PWM_COIL * i / 200;

	if (to_power_fail < TO_POWER_FAIL)
		PWM_COIL = 0;
	//////////////////////////////
	else if (test_pwm == 0)
		PWM_COIL = coil_pwm;
	//////////////////////////////
	if (hmc_d < 0.0f)
		signal_delta = hmc_d * -10.0f;
	else
		signal_delta = hmc_d * 10.0f;

	if (get_pos_flag)
	{
		con_str("x=");
		con_float(fhmc_pos);
		con_str(",");
		con_dec(ihmc_pos_t);
		con_str(",");
		// con_dec(roundf(fhmc_pos_th * 10.0));
		con_dec(adc_pos);
		con_str(",");
		// con_dec(coil_pwm);
		con_float(fhmc_pos_th);
		con_str("\n\r");

		//		con_str("p=");
		//		con_dec(info_hmc_d_pr);
		//		con_str(",");
		//		con_dec(info_hmc_d_int);
		//		con_str(",");
		//		con_dec(info_hmc_d_dif);
		//		con_str("\n\r");
		con_start();
	}

	if (test_a_out == 0)
		switch (data_eeprom.flags.aout_sig)
		{
		case OUT_SIG_NONE:
			break;

		case OUT_SIG_POS:
			i = ihmc_pos;
			if (i < data_eeprom.aout_pos_min)
				i = data_eeprom.aout_pos_min;
			if (i > data_eeprom.aout_pos_max)
				i = data_eeprom.aout_pos_max;

			aout_ma = data_eeprom.aout_ma_min + (i - data_eeprom.aout_pos_min) * (data_eeprom.aout_ma_max - data_eeprom.aout_ma_min) / (data_eeprom.aout_pos_max - data_eeprom.aout_pos_min);

			i = data_eeprom.aout_pwm_min + (aout_ma - 40) * (data_eeprom.aout_pwm_max - data_eeprom.aout_pwm_min) / (200 - 40);

			PWM_AOUT = i;
			break;

		case OUT_SIG_AIN:
			i = ain_ma / 10;
			if (i < data_eeprom.aout_ma_min)
				i = data_eeprom.aout_ma_min;
			if (i > data_eeprom.aout_ma_max)
				i = data_eeprom.aout_ma_max;

			aout_ma = i;

			i = data_eeprom.aout_pwm_min + (aout_ma - 40) * (data_eeprom.aout_pwm_max - data_eeprom.aout_pwm_min) / (200 - 40);

			PWM_AOUT = i;
			break;

		case OUT_SIG_DELTA:
			i = signal_delta;
			if (i < data_eeprom.aout_pos_min)
				i = data_eeprom.aout_pos_min;
			if (i > data_eeprom.aout_pos_max)
				i = data_eeprom.aout_pos_max;

			aout_ma = data_eeprom.aout_ma_min + (i - data_eeprom.aout_pos_min) * (data_eeprom.aout_ma_max - data_eeprom.aout_ma_min) / (data_eeprom.aout_pos_max - data_eeprom.aout_pos_min);

			i = data_eeprom.aout_pwm_min + (aout_ma - 40) * (data_eeprom.aout_pwm_max - data_eeprom.aout_pwm_min) / (200 - 40);

			PWM_AOUT = i;
			break;
		}

	if (test_namur == 0)
		switch (data_eeprom.flags.dout2_sig)
		{
		case OUT_SIG_NONE:
			namur_out[1] = 0;
			break;

		case OUT_SIG_POS:
			if ((namur_out[1] == 0) && (data_eeprom.flags.dout2_dir == DOUT_DIR_UP) && (ihmc_pos > (data_eeprom.dout2_pos + data_eeprom.dout2_gis / 2)))
				namur_out[1] = 1;
			else if ((namur_out[1] == 1) && (data_eeprom.flags.dout2_dir == DOUT_DIR_UP) && (ihmc_pos < (data_eeprom.dout2_pos - data_eeprom.dout2_gis / 2)))
				namur_out[1] = 0;
			else if ((namur_out[1] == 0) && (data_eeprom.flags.dout2_dir == DOUT_DIR_DOWN) && (ihmc_pos < (data_eeprom.dout2_pos - data_eeprom.dout2_gis / 2)))
				namur_out[1] = 1;
			else if ((namur_out[1] == 1) && (data_eeprom.flags.dout2_dir == DOUT_DIR_DOWN) && (ihmc_pos > (data_eeprom.dout2_pos + data_eeprom.dout2_gis / 2)))
				namur_out[1] = 0;
			break;

		case OUT_SIG_AIN:
			if ((namur_out[1] == 0) && (data_eeprom.flags.dout2_dir == DOUT_DIR_UP) && (ihmc_pos_t > (data_eeprom.dout2_pos + data_eeprom.dout2_gis / 2)))
				namur_out[1] = 1;
			else if ((namur_out[1] == 1) && (data_eeprom.flags.dout2_dir == DOUT_DIR_UP) && (ihmc_pos_t < (data_eeprom.dout2_pos - data_eeprom.dout2_gis / 2)))
				namur_out[1] = 0;
			else if ((namur_out[1] == 0) && (data_eeprom.flags.dout2_dir == DOUT_DIR_DOWN) && (ihmc_pos_t < (data_eeprom.dout2_pos - data_eeprom.dout2_gis / 2)))
				namur_out[1] = 1;
			else if ((namur_out[1] == 1) && (data_eeprom.flags.dout2_dir == DOUT_DIR_DOWN) && (ihmc_pos_t > (data_eeprom.dout2_pos + data_eeprom.dout2_gis / 2)))
				namur_out[1] = 0;
			break;

		case OUT_SIG_DELTA:
			if ((namur_out[1] == 0) && (data_eeprom.flags.dout2_dir == DOUT_DIR_UP) && (signal_delta > (data_eeprom.dout2_pos + data_eeprom.dout2_gis / 2)))
				namur_out[1] = 1;
			else if ((namur_out[1] == 1) && (data_eeprom.flags.dout2_dir == DOUT_DIR_UP) && (signal_delta < (data_eeprom.dout2_pos - data_eeprom.dout2_gis / 2)))
				namur_out[1] = 0;
			else if ((namur_out[1] == 0) && (data_eeprom.flags.dout2_dir == DOUT_DIR_DOWN) && (signal_delta < (data_eeprom.dout2_pos - data_eeprom.dout2_gis / 2)))
				namur_out[1] = 1;
			else if ((namur_out[1] == 1) && (data_eeprom.flags.dout2_dir == DOUT_DIR_DOWN) && (signal_delta > (data_eeprom.dout2_pos + data_eeprom.dout2_gis / 2)))
				namur_out[1] = 0;
			break;
		}

	if (test_namur == 0)
		switch (data_eeprom.flags.dout3_sig)
		{
		case OUT_SIG_NONE:
			namur_out[2] = 0;
			break;

		case OUT_SIG_POS:
			if ((namur_out[2] == 0) && (data_eeprom.flags.dout3_dir == DOUT_DIR_UP) && (ihmc_pos > (data_eeprom.dout3_pos + data_eeprom.dout3_gis / 2)))
				namur_out[2] = 1;
			else if ((namur_out[2] == 1) && (data_eeprom.flags.dout3_dir == DOUT_DIR_UP) && (ihmc_pos < (data_eeprom.dout3_pos - data_eeprom.dout3_gis / 2)))
				namur_out[2] = 0;
			else if ((namur_out[2] == 0) && (data_eeprom.flags.dout3_dir == DOUT_DIR_DOWN) && (ihmc_pos < (data_eeprom.dout3_pos - data_eeprom.dout3_gis / 2)))
				namur_out[2] = 1;
			else if ((namur_out[2] == 1) && (data_eeprom.flags.dout3_dir == DOUT_DIR_DOWN) && (ihmc_pos > (data_eeprom.dout3_pos + data_eeprom.dout3_gis / 2)))
				namur_out[2] = 0;
			break;

		case OUT_SIG_AIN:
			if ((namur_out[2] == 0) && (data_eeprom.flags.dout3_dir == DOUT_DIR_UP) && (ihmc_pos_t > (data_eeprom.dout3_pos + data_eeprom.dout3_gis / 2)))
				namur_out[2] = 1;
			else if ((namur_out[2] == 1) && (data_eeprom.flags.dout3_dir == DOUT_DIR_UP) && (ihmc_pos_t < (data_eeprom.dout3_pos - data_eeprom.dout3_gis / 2)))
				namur_out[2] = 0;
			else if ((namur_out[2] == 0) && (data_eeprom.flags.dout3_dir == DOUT_DIR_DOWN) && (ihmc_pos_t < (data_eeprom.dout3_pos - data_eeprom.dout3_gis / 2)))
				namur_out[2] = 1;
			else if ((namur_out[2] == 1) && (data_eeprom.flags.dout3_dir == DOUT_DIR_DOWN) && (ihmc_pos_t > (data_eeprom.dout3_pos + data_eeprom.dout3_gis / 2)))
				namur_out[2] = 0;
			break;

		case OUT_SIG_DELTA:
			if ((namur_out[2] == 0) && (data_eeprom.flags.dout2_dir == DOUT_DIR_UP) && (signal_delta > (data_eeprom.dout2_pos + data_eeprom.dout2_gis / 2)))
				namur_out[2] = 1;
			else if ((namur_out[2] == 1) && (data_eeprom.flags.dout2_dir == DOUT_DIR_UP) && (signal_delta < (data_eeprom.dout2_pos - data_eeprom.dout2_gis / 2)))
				namur_out[2] = 0;
			else if ((namur_out[2] == 0) && (data_eeprom.flags.dout2_dir == DOUT_DIR_DOWN) && (signal_delta < (data_eeprom.dout2_pos - data_eeprom.dout2_gis / 2)))
				namur_out[2] = 1;
			else if ((namur_out[2] == 1) && (data_eeprom.flags.dout2_dir == DOUT_DIR_DOWN) && (signal_delta > (data_eeprom.dout2_pos + data_eeprom.dout2_gis / 2)))
				namur_out[2] = 0;
			break;
		}

	if (pid_debug)
	{
		if (pid_debug_n < 30)
			pid_debug_n++;
		else
		{
			pid_debug_n = 0;

			con_str("PID_coil_pwm:");
			con_str("\n\r  hmc_d = ");
			con_float(hmc_d);
			con_str(", hmc_int = ");
			con_float(hmc_d_int);
			con_str(", hmc_dif = ");
			con_float(hmc_d_dif);
			con_str(", coil_pwm_f = ");
			con_float(coil_pwm_f);
			con_str(", coil_value = ");
			con_float(TIM3->CCR1);
			con_str("\n\r  Full = ");
			con_float(full_angle);
			con_str(", curr = ");
			con_float(curr_angle);
			con_str(", linrz = ");
			con_float(linearize);
			con_str(", ihmc_adc_s = ");
			con_dec(ihmc_adc_s);
			con_str(", lim_adc_0 = ");
			con_dec(hmc_adc_lim_0);
			con_str(", lim_adc_100 = ");
			con_dec(hmc_adc_lim_100);
			con_str("\n\r");
			con_start();
		}
	}

	//---------------------------------------
	//	Запись в HART-переменные
	//---------------------------------------
	// hart_value[0] = ((float)ain_ma) * 0.01f;
	// hart_value[1] = ((float)hmc_pos) * 0.1f;
}

//-----------------------------------------------------------------------------
//	void ADC1_IRQHandler(void)
//-----------------------------------------------------------------------------
void ADC1_IRQHandler(void)
{

	if (ADC1->SR & (1 << ADC_SR_JEOC))
	{
		//---------------------------------------
		//	Получение данных АЦП
		//---------------------------------------
		//		hmc_adc_s = ADC1->JDR1;
		//		ain_adc_s = ADC1->JDR2;
		//		tempeature_adc = ADC1->JDR3;
		//		btn_adc = ADC1->JDR4;

		//		con_str("x=");
		//		con_dec(hmc_adc_s);
		//		con_str("\n\r");
		//		con_start();

		WORD pos = ADC1->JDR1;
		// WORD pos = get_data_via_spi(to_power_fail < TO_POWER_FAIL); // если используется spi датчик

		if (st_auto == ST_AUTO_IDLE)
		{

			new_samples(
				ADC1->JDR2,	 // ain
				pos,		 // pos
				ADC1->JDR4,	 // btn
				ADC1->JDR3); // temp
		}
		else
		{
			auto_update(pos, ADC1->JDR4);
		}

		ADC1->SR = ~(1 << ADC_SR_JEOC);
	}
}

//-----------------------------------------------------------------------------
//	void TIM6_IRQHandler(void)
//-----------------------------------------------------------------------------
void TIM6_IRQHandler(void)
{
	if (TIM6->SR & (1 << TIM_SR_UIF))
	{
		TIM6->SR = ~(1 << TIM_SR_UIF);

		// con_str(".");
		// con_start();

#ifdef ADC_OVER
		if (ADC1->SR & (1 << ADC_SR_OVR))
		{
			ADC1->SR = ~(1 << ADC_SR_OVR);
			con_str("[ADC_OVR ");
			con_dec(uptime);
			con_str("]");
			con_start();
		}
		// if(ADC1->SR & (1 << ADC_SR_EOC))
		//{
		// ADC1->SR = ~(1 << ADC_SR_EOC);
		// con_str("[ADC_EOC]");
		// con_start();
		//}
		DMA1_Channel1->CCR = 0;
		DMA1->IFCR = (1 << DMA_IFCR_CGIF1);
		DMA1_Channel1->CMAR = (DWORD)adc_buf;
		DMA1_Channel1->CPAR = (DWORD)&ADC1->DR;
		DMA1_Channel1->CNDTR = 16;
		DMA1_Channel1->CCR = 0 | (1 << DMA_CCR_MSIZE) | (1 << DMA_CCR_PSIZE) | (1 << DMA_CCR_MINC) | (0 << DMA_CCR_CIRC) | (0 << DMA_CCR_HTIE) | (1 << DMA_CCR_TCIE) | (1 << DMA_CCR_EN);
		ADC1->CR2 = 0 | (1 << ADC_CR2_DMA) | (1 << ADC_CR2_DDS) | (0 << ADC_CR2_EOCS) | (1 << ADC_CR2_SWSTART) | (7 << ADC_CR2_DELS) | (1 << ADC_CR2_ADON);
#else
		ADC1->CR2 = 0 | (1 << ADC_CR2_JSWSTART) | (0 << ADC_CR2_DELS) | (1 << ADC_CR2_ADON);
#endif // ADC_OVER
	}
}

//-----------------------------------------------------------------------------
//	void USART1_IRQHandler(void)
//-----------------------------------------------------------------------------
void USART1_IRQHandler(void)
{
	while (USART1->SR & (1 << USART_SR_RXNE))
	{
		// новый символ в приемнике
		con.rx_tm = 0;
		con_rx_push(USART1->DR);
	}

	if (USART1->SR & (1 << USART_SR_TXE))
	{
		// передатчик пуст
		if (con.tx_t != con.tx_b)
			USART1->DR = con_tx_pop();
		else
			USART1->CR1 &= ~(1 << USART_CR1_TXEIE);
	}

	if (USART1->SR & (1 << USART_SR_ORE))
		USART1->DR;
}

//-----------------------------------------------------------------------------
//	void SysTickHandler(void)
//-----------------------------------------------------------------------------
void SysTickHandler(void)
{
	static tTIMER to_uptime = 0;

	//---------------------------------------------------------------------------
	//	uptime
	//---------------------------------------------------------------------------
	tick(&to_uptime);
	if (to_uptime >= 1000UL)
	{
		uptime++;
		if (worktime != -1)
			worktime++;
		to_uptime = 0;
	}

	//---------------------------------------------------------------------------
	//	NAMUR
	//---------------------------------------------------------------------------
	set_pin(NAMUR1_PIN, namur_out[0]);
	set_pin(NAMUR2_PIN, namur_out[1]);
	set_pin(NAMUR3_PIN, namur_out[2]);

	//---------------------------------------------------------------------------
	//	HART
	//---------------------------------------------------------------------------
	hart_timer();

	if (to_power_fail < TO_POWER_FAIL)
		tick(&to_power_fail);

	//---------------------------------------------------------------------------
	//	DISP
	//---------------------------------------------------------------------------
	if (to_disp_init < TO_DISP_INIT)
	{
		tick(&to_disp_init);
		if (to_disp_init == TO_DISP_INIT)
			disp_init_flag = 1;
	}

	// menu_update();
	menu_v2_update();

	//---------------------------------------------------------------------------
	// Светодиод "РАБОТА"
	//---------------------------------------------------------------------------
	workled_blink();

	IWDG_TIMER;
}

//-----------------------------------------------------------------------------
//	void workled_blink(void)
//-----------------------------------------------------------------------------
void workled_blink(void)
{
#ifdef AIN_AVG
	tick(&to_ain_avg);
#endif // AIN_AVG

	tick(&t_workled);
	if (t_workled >= (TO_WORKLED / 2))
	{
		t_workled = 0;
		// blink_var = !blink_var;

		// if(blink_var) set_pin(WORKLED_PIN, 1);
		// else set_pin(WORKLED_PIN, 0);

		// con_str("DMA1_ISR=");
		// con_dword(DMA1->ISR);
		// con_str(", CNDTR=");
		// con_dword(DMA1_Channel1->CNDTR);
		// con_str(", CCR=");
		// con_dword(DMA1_Channel1->CCR);
		// con_str(", MAR=");
		// con_dword(DMA1_Channel1->CMAR);
		// con_str(", PAR=");
		// con_dword(DMA1_Channel1->CPAR);
		// DMA1_Channel1->CMAR
		// con_str(", ADC_SR=");
		// con_dword(ADC1->SR);
		// con_str("{");
		// con_dec(ht_cnt);
		// con_str(",");
		// con_dec(tc_cnt);
		// con_str("}\n\r");
		// con_start();

		//		if(ADC1->SR & (1 << ADC_SR_OVR))
		//		{
		//			ADC1->SR = ~(1 << ADC_SR_OVR);
		//			con_str("[ADC_OVR]");
		//			con_start();
		//		}

		disp_flag = 1;

		//		con_str("HIF=");
		//		con_dec(abs(ihmc_pos_t - ihmc_pos));
		//		con_str(" @ ");
		//		con_dec(hif_to);
		//		con_str("\n\r");
		//		con_start();
	}
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
