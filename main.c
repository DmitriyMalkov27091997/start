/**
 *	\file			main.c
 *	\brief		Главный файл проекта.
 *
 *	\mainpage Позиционер "AM800".
 *	\author		Шанин А.М.
 *	\author		email: adnega@mail.ru
 *	\author		(C) Ярославль, 2016
 *	\version	v0.1
 *	\date			20.05.2016
 *
 *	\line _
 *	\section SEC_MAIN Основные определения, переменные и функции заданы в файлах:
 *	- \ref define.h \copybrief define.h
 *	- \ref type.h \copybrief type.h
 *
 *	\line _
 *	\section SEC_EVENTS Коды событий:
 *	- \ref eEVENT \copydoc eEVENT
 *
 *	\line _
 *	\section SEC_CONSOLE Команды консоли:
 *	- \ref PAGE_CONSOLE_INIT
 *		\copydoc PAGE_CONSOLE_INIT
 *	- \ref PAGE_CONSOLE_LOGIN
 *		\copydoc PAGE_CONSOLE_LOGIN
 * 
 */
#include "stm32l1xx.h"

#include "type.h"
#include "define.h"
#include "init.h"
#include "isr.h"
#include "serial_port.h"
#include "command.h"
#include "math.h"
#include "pcf8531.h"
#include "picto.h"
#include "hart.h"
#include "watchdog.h"
#include "font.h"
#include "spi_sensor.h"

DWORD		sw_crc = 0;
DWORD		sw_crc_value = 0;
DWORD		sw_crc_addr = 0;

int 	flag_temp_min = 0;
int 	flag_temp_max = 0;
int 	flag_ain_min = 0;
int 	flag_ain_max = 0;
int 	flag_ain_fail = 0;

//-----------------------------------------------------------------------------
//! \fn int main(void)
//! \brief Главный цикл.
//-----------------------------------------------------------------------------
int main(void)
{
	/*TODO:
	Проверка состояние датчика, 1 раз при старте позиционера.
	*/
	int		i, j;
	WORD	adc_value;
	int		adc_channel = 1;
	int		count = 0;

	sEEPROM_FLAGS	flags;
	static DWORD	event_flags = 0;
	DWORD	rcc_csr = 0;

	static int pcf8531_page = 0;
	static int pf_count = 0;
	static int pg_count = 0;

	float	temp_f;
	int	iwdt_flag = 0;
	char	sth[32];

	rcc_csr = RCC->CSR;

	if(rcc_csr & (1 << RCC_CSR_IWDGRSTF)) iwdt_flag = 1;

	RCC->CSR |= (1 << RCC_CSR_RMVF);

	sp_init(&con,
		console_tx_buf, CONSOLE_TX_SIZE,
		console_rx_buf,	CONSOLE_RX_SIZE,
		&USART1->CR1);

	hart_init();

	//---------------------------------------------------------------------------
	//	INIT_ALL
	//---------------------------------------------------------------------------
	init_SYSTEM();
	init_GPIO();
	init_SYSTICK();
	init_USART1();
	init_USART3();
	init_TIM6();
	init_TIM3();
	init_TIM9();
	init_NVIC();

	IWDG_INIT;

	crc32_init(&sw_crc);
	crc32_next(&sw_crc, (const BYTE *)SW_CRC_START, SW_CRC_APP_SIZE);

	con_str("\n\nAPP: start = ");
	con_dword(SW_CRC_START);
	con_str(", size = ");
	con_dec(SW_CRC_APP_SIZE);
	con_str(", crc32 = ");
	con_dword(sw_crc);
	con_str(" / ");
	con_dword(image.crc32);
	con_str("\n\rserial = ");
	con_dec(image.serial);
	con_str("\n\r");
	con_start();

	con_str("\n\rRESET(");
	con_dword(rcc_csr);
	con_str("):\n\r");
	if(rcc_csr & (1 << RCC_CSR_LPWRRSTF))
		con_str(" - Low-power reset\n\r");
	if(rcc_csr & (1 << RCC_CSR_WWDGRSTF))
		con_str("- Window watchdog reset\n\r");
	if(rcc_csr & (1 << RCC_CSR_IWDGRSTF))
		con_str("- Independent watchdog reset\n\r");
	if(rcc_csr & (1 << RCC_CSR_SFTRSTF))
		con_str("- Software reset\n\r");
	if(rcc_csr & (1 << RCC_CSR_PORRSTF))
		con_str("- POR/PDR reset\n\r");
	if(rcc_csr & (1 << RCC_CSR_PINRSTF))
		con_str("- PIN reset\n\r");
	con_str("\n\r");
	con_start();

	if(!load_maj(data_eeprom.hmc_path_nv, &ihmc_path)) ihmc_path = 0;
	if(!load_maj(data_eeprom.worktime_nv, &worktime)) worktime = 0;
	else worktime += 120;

	if((0 > data_eeprom.hard_pos_min) || (data_eeprom.hard_pos_min > 200))
	{
		eeprom_unlock();
		data_eeprom.hard_pos_min = 0;
		eeprom_lock();
	}

	if((0 > data_eeprom.hard_pos_max) || (data_eeprom.hard_pos_max > 200))
	{
		eeprom_unlock();
		data_eeprom.hard_pos_max = 0;
		eeprom_lock();
	}

	if(data_eeprom.t_lp_up < 0.003f)
	{
		eeprom_unlock();
		data_eeprom.t_lp_up = 0.003f;
		eeprom_lock();
	}
	if(data_eeprom.t_lp_up > 0.270f)
	{
		eeprom_unlock();
		data_eeprom.t_lp_up = 0.270f;
		eeprom_lock();
	}
	if(data_eeprom.t_lp_down < 0.003f)
	{
		eeprom_unlock();
		data_eeprom.t_lp_down = 0.003f;
		eeprom_lock();
	}
	if(data_eeprom.t_lp_down > 0.270f)
	{
		eeprom_unlock();
		data_eeprom.t_lp_down = 0.270f;
		eeprom_lock();
	}

	con_str("EVENT LOG load... ");
	con_start();

	load_events();

	con_dec(event_pos);
	con_str("/");
	con_dec(event_num);
	con_str("\n\r");
	con_start();

	con_str((BYTE *)&s_who);
	con_str("\n\r");
	con_push(COM_BEGIN);
	con_start();

	while(1)
	{
		IWDG_MAINLOOP;

		__WFI();

		//-----------------------------------------
		// Контроль целостности SW
		//-----------------------------------------
		if(sw_crc_addr)
		{
			if((sw_crc_addr + SW_CRC_BLOCK_SIZE) >= (SW_CRC_START + SW_CRC_APP_SIZE))
			{
				crc32_next(&sw_crc_value, (BYTE *)sw_crc_addr, SW_CRC_BLOCK_SIZE - SW_CRC_BLOCK_END);
				sw_crc = sw_crc_value;

				if(sw_crc != image.crc32)
				{
					// sw crc fail
					con_str("crc32 = ");
					con_dword(sw_crc);
					con_str(" / ");
					con_dword(image.crc32);
					con_str("\n\r");
					con_start();
				}
				sw_crc_addr = 0;
			}
			else
			{
				// next
				crc32_next(&sw_crc_value, (BYTE *)sw_crc_addr, SW_CRC_BLOCK_SIZE);
				sw_crc_addr += SW_CRC_BLOCK_SIZE;
			}
		}
		else
		{
			sw_crc_addr = SW_CRC_START;
			crc32_init(&sw_crc_value);
		}

		//-------------------------------------------------------------------------
		//	Обновление энергонезависимых данных
		//-------------------------------------------------------------------------
		j = ihmc_path;
		if(load_maj(data_eeprom.hmc_path_nv, &i))
		{
			if((i + 2000) < j)
			save_maj(data_eeprom.hmc_path_nv, j);
		}
		else save_maj(data_eeprom.hmc_path_nv, j);

		j = worktime;
		if(load_maj(data_eeprom.worktime_nv, &i))
		{
			if((i + 60) < j)
			save_maj(data_eeprom.worktime_nv, j);
		}
		else save_maj(data_eeprom.worktime_nv, j);

		// HERE Сброс времени
		if(flag_calib_reset)
		{
			flag_calib_reset = 0;
			calib_reset();
		}

		// HERE Полный сброс
		if(flag_im_reset)
		{
			flag_im_reset = 0;
			im_reset();
		}

		// Сброс к заводским настройкам - консоль и HART
		if (flag_factory_reset)
		{
			flag_factory_reset = 0;
			factory_reset();
		}

		//-------------------------------------------------------------------------
		//	Диагностика. Проверки.
		//-------------------------------------------------------------------------
		set_pupd(FAIL_PIN, PUPD_UP);
		for(i = 0; i < 10; i++) __NOP();
		j = 0;
		for(i = 0; i < 10; i++) if(get_pin(FAIL_PIN)) j++;
		if(j == i)
		{
			pg_count = 0;
			if(pf_count < 50) pf_count++;
			else to_power_fail = 0;
		}
		else if(j == 0)
		{
			pf_count = 0;
			if(pg_count < 10) pg_count++;
			else to_power_fail = TO_POWER_FAIL;
		}
		else
		{
			pf_count = 0;
			pg_count = 0;
		}
		set_pupd(FAIL_PIN, PUPD_OFF);

		set_pupd(NAMUR1D_PIN, PUPD_UP);
		for(i = 0; i < 10; i++) __NOP();
		namur_diag[0] = get_pin(NAMUR1D_PIN);
		set_pupd(NAMUR1D_PIN, PUPD_OFF);

		set_pupd(NAMUR2D_PIN, PUPD_UP);
		for(i = 0; i < 10; i++) __NOP();
		namur_diag[1] = get_pin(NAMUR2D_PIN);
		set_pupd(NAMUR2D_PIN, PUPD_OFF);

		set_pupd(NAMUR3D_PIN, PUPD_UP);
		for(i = 0; i < 10; i++) __NOP();
		namur_diag[2] = get_pin(NAMUR3D_PIN);
		set_pupd(NAMUR3D_PIN, PUPD_OFF);

		set_pupd(AOUT_DIAG_PIN, PUPD_UP);
		for(i = 0; i < 10; i++) __NOP();
		aout_diag = get_pin(AOUT_DIAG_PIN);
		set_pupd(AOUT_DIAG_PIN, PUPD_OFF);

		// Гистерезис для событий, чтобы не было водопада событий на границе условия.
		if((tempeature != TEMPERATURE_BAD) && (tempeature < TEMP_C_MIN)) flag_temp_min = 1;
			  else if (tempeature >= (TEMP_C_MIN + 2)) flag_temp_min = 0;

		if((tempeature != TEMPERATURE_BAD) && (tempeature > TEMP_C_MAX)) flag_temp_max = 1;
			  else if (tempeature <= (TEMP_C_MAX - 2)) flag_temp_max = 0;

		if((ain_ma_s != -1) && (ain_ma_s < AIN_MA_MIN)) flag_ain_min =1;
				else if (ain_ma_s >= (AIN_MA_MIN + 5)) flag_ain_min = 0;

		if((ain_ma_s != -1) && (ain_ma_s > AIN_MA_MAX)) flag_ain_max =1;
				else if (ain_ma_s <= (AIN_MA_MAX - 5)) flag_ain_max = 0;

		if((ain_ma_s != -1) && (ain_ma_s < AIN_MA_POWER_FAIL)) flag_ain_fail =1;
						else if (ain_ma_s >= (AIN_MA_POWER_FAIL + 5)) flag_ain_fail = 0;

		//-------------------------------------------------------------------------
		//	Диагностика. Генерация событий
		//-------------------------------------------------------------------------
		event_flag_current = 0;

		if((data_eeprom.flags.dout1_disable == DOUT1_ENABLE) && (namur_diag[0] == 1))
			event_flag_current |= (1 << EVENT_DOUT1_FAIL);

		if((data_eeprom.flags.dout2_sig != OUT_SIG_NONE) && (namur_diag[1] == 1))
			event_flag_current |= (1 << EVENT_DOUT2_FAIL);

		if((data_eeprom.flags.dout3_sig != OUT_SIG_NONE) && (namur_diag[2] == 1))
			event_flag_current |= (1 << EVENT_DOUT3_FAIL);

		if((data_eeprom.flags.aout_sig != OUT_SIG_NONE) && (aout_diag == 1))
			event_flag_current |= (1 << EVENT_AOUT_FAIL);

		if(flag_ain_min > 0)
			event_flag_current |= (1 << EVENT_AIN_MIN);

		if(flag_ain_max > 0)
			event_flag_current |= (1 << EVENT_AIN_MAX);

		if(flag_ain_fail > 0)
			event_flag_current |= (1 << EVENT_POWER_FAIL);

		if(flag_temp_min > 0)
			event_flag_current |= (1 << EVENT_TA_MIN);

		if(flag_temp_max > 0)
			event_flag_current |= (1 << EVENT_TA_MAX);

		if(to_power_fail < TO_POWER_FAIL)
			event_flag_current |= (1 << EVENT_BP_FAIL);

		if(sw_crc != image.crc32)
			event_flag_current |= (1 << EVENT_SW_FAIL);

		if(iwdt_flag == 1)
		{
			event_flag_current |= (1 << EVENT_WDT_RESET);
			iwdt_flag = 0;
		}

		if(tamper == 1)
			event_flag_current |= (1 << EVENT_TAMPER);

		if(pst_event_fail)
			event_flag_current |= (1 << EVENT_PST_FAIL);

		if(hif_to >= (data_eeprom.hif_t * FADC))
			event_flag_current |= (1 << EVENT_HIF_FAIL);

		if (test_namur == 0)
			if(event_flag_current) namur_out[0] = 0;
			else namur_out[0] = 1;

		event_flags = event_flags ^ event_flag_current;

		for(i = 0; i < EVENT_NUM; i++)
		{
			if((event_flags >> i) & 1)
			{
				if((to_power_fail >= TO_POWER_FAIL) || (i == EVENT_BP_FAIL))
				{
					if(((event_flag_current | (~(EVENT_MASK_ONCE))) >> i) & 1)
						event_save(i | ((((~event_flag_current) >> i) & 1) << EVENT_FLAG));
				}
			}
		}

		event_flags = event_flag_current;
		event_flag_last |= event_flag_current;
		event_flag = event_flag_current;

		//-------------------------------------------------------------------------
		//	Дисплей. Инициализация
		//-------------------------------------------------------------------------
		if(disp_init_flag)
		{

			if((data_eeprom.flags.display_contrast < 5) || (data_eeprom.flags.display_contrast > 25))
			{
				flags.dw = data_eeprom.flags.dw;
				flags.display_contrast = 15;
				eeprom_unlock();
				data_eeprom.flags.dw = flags.dw;
				eeprom_lock();
			}

			disp_init_flag = 0;
			con_str("DISP init... ");
			con_start();
			pcf8531_restart();
			pcf8531_init();
			con_str("Ok\n\r");
			con_start();
		}

		//-------------------------------------------------------------------------
		//	Дисплей. Обновление
		//-------------------------------------------------------------------------
		if(disp_flag && (to_disp_init >= TO_DISP_INIT))
		{
			disp_flag = 0;
			#if 1
			
			//menu_redraw();
			menu_v2_redraw();


			#else
			i = (src_max - src_min) * 10000 / 4096;
			mstr_cpy(sth, "src=");
			mstr_add_int(sth, i / 100);
			mstr_add(sth, ".");
			mstr_add_int2(sth, i % 100);
			mstr_add(sth, "%   ");
			pcf8531_str(10, 0, sth, Font_Single);

			i = (avg_max - avg_min) * 10000 / 4096;
			mstr_cpy(sth, "avg=");
			mstr_add_int(sth, i / 100);
			mstr_add(sth, ".");
			mstr_add_int2(sth, i % 100);
			mstr_add(sth, "%   ");
			pcf8531_str(10, 8, sth, Font_Single);

			i = (flt_max - flt_min) * 10000 / 4096;
			mstr_cpy(sth, "flt=");
			mstr_add_int(sth, i / 100);
			mstr_add(sth, ".");
			mstr_add_int2(sth, i % 100);
			mstr_add(sth, "%   ");
			pcf8531_str(10, 16, sth, Font_Single);
			#endif
			disp_update_count++;

			if(get_display)
			{
				get_display = 0;
				for(i = 0; i < PCF8531_SIZEX; i++)
				{
					con_str("D");
					con_dec(i);
					con_str(":");
					con_dword(*(DWORD *)pcf8531_video[i]);
//					con_str(",");
//					con_byte(pcf8531_video[i][1]);
//					con_str(",");
//					con_byte(pcf8531_video[i][2]);
//					con_str(",");
//					con_byte(pcf8531_video[i][3]);
//					con_str(",");
//					con_byte(pcf8531_video[i][4]);
					con_str("\n\r");
					con_start();
					while(con.tx_b != con.tx_t);
				}
			}
		}

		//-------------------------------------------------------------------------
		//	Дисплей. Запись
		//-------------------------------------------------------------------------
		if(to_disp_init >= TO_DISP_INIT)
		{
			pcf8531_redraw(pcf8531_page);
			if(pcf8531_page < (PCF8531_PAGE_NUM - 1)) pcf8531_page++;
			else
			{
				pcf8531_page = 0;

				pcf8531_init_after();

				// update contrast
				if(display_contrast == -1) pcf8531_contrast(data_eeprom.flags.display_contrast);
				else pcf8531_contrast(display_contrast);
			}
		}

		//-------------------------------------------------------------------------
		//	Обработка команд с консоли
		//-------------------------------------------------------------------------
		command_mainloop();
		
		//-------------------------------------------------------------------------
	}
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
