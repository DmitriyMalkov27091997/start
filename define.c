/**
 * @file define.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Файл настроек проекта.
 * @version 0.1
 * @date 20.05.2016
 *
 * @copyright (C) Ярославль, 2016
 *
 */
#include "define.h"

volatile tTIMER t_workled = 0;
// volatile int		blink_var = 0;

const BYTE s_who[] = SW_NAME " @ STM32L152RCT6 [" SW_DATE "]\n\r";
BYTE console_tx_buf[CONSOLE_TX_SIZE];
BYTE console_rx_buf[CONSOLE_RX_SIZE];
sSERIAL_PORT con;
BYTE console_command[CONSOLE_COMMAND_SIZE];
BYTE console_exec_buf[CONSOLE_COMMAND_SIZE];
volatile int console_command_pos = 0;

volatile int disp_init_flag = 0;
tTIMER to_disp_init = 0;
volatile int disp_flag = 0;

volatile DWORD uptime = 0;
volatile int worktime = -1;
volatile int tempeature = TEMPERATURE_BAD;
volatile int tempeature_adc = -1;

volatile int get_adc_flag = 0;
volatile int get_pos_flag = 0;
const sIMAGE_INFO image;

volatile int st_auto = ST_AUTO_IDLE;
volatile int auto_type = TYPE_DIRECT;
volatile int hart_autoset_flag = 0;
volatile int hart_autoset_done_flag = 0;
tTIMER to_auto = 0;

WORD stat_pos_arr[STAT_POS_SIZE];
volatile int stat_pos = 0;
volatile int stat_pos_arr_valid = 0;
volatile int stat_pos_min = -1;
volatile int stat_pos_max = -1;
volatile int stat_pos_delta = -1;
volatile int stat_pos_avg = -1;

volatile int stat_pos_adc_min = -1;
volatile int stat_pos_adc_max = -1;
volatile int stat_time_down = -1;
volatile int stat_time_up = -1;

volatile float stat_kp = 5.0f;
volatile float stat_ki = 0.01f;
volatile float stat_coil_pwm = MAX_PWM_COIL * 0.5f;

#ifdef AIN_AVG
volatile int ain_avg_min = -1;
volatile int ain_avg_max = -1;
volatile int ain_avg_avg = -1;
volatile int ain_avg_tmin = -1;
volatile int ain_avg_tmax = -1;
volatile int ain_avg_sum = -1;
volatile int ain_avg_cnt = -1;
volatile int ain_avg_done = 0;
tTIMER to_ain_avg = 0;
#endif

volatile int pos_value = -1;
volatile int ihmc_adc_s = -1;
volatile float fhmc_adc = -1.0;
volatile int ihmc_adc = -1;
volatile float fhmc_pos_a = -1.0;
volatile int ihmc_pos_a = -1;
volatile float fhmc_pos = -1.0;
volatile int ihmc_pos = -1;
volatile int ihmc_adc_d = -1;
volatile int ihmc_pos_da = -1;
volatile int ihmc_pos_d = -1;
volatile int ihmc_g = 0;
volatile int ihmc_path = -1;
volatile int ihmc_pos_t = -1;

volatile int ain_adc_s = -1;
volatile float fain_adc = -1;
volatile int ain_ma_s = -1;
volatile int ain_ma = -1;
volatile int ain_pos_t = -1;

volatile int hart_ma = -1;
volatile int aout_ma = -1;
volatile int aout_diag = 1;

volatile int test_pwm = 0;	 //!< Управление электромагнитом с консоли
volatile int test_a_out = 0; //!< Управление аналоговым выходом с консоли
volatile int test_namur = 0; //!< Управление дискретными выходами с консоли (0 - запрещено, иначе - разрешено)

volatile int namur_out[3];
volatile int namur_diag[3] = {-1, -1, -1};

volatile int signal_delta = 0;

volatile float pid_int = 0.0f;

volatile int coil_pwm = -1;
volatile int coil_pwm_def = MAX_PWM_COIL / 2;

volatile int pid_debug = 0;
volatile int pid_int_clear = 0;

volatile int btn_adc = -1;

sDATA_EEPROM data_eeprom;

tTIMER to_power_fail = 0; // TO_POWER_FAIL;

tTIMER to_button_pressed = 0;
volatile int button_pressed = 0;

volatile int pid_source = PID_SOURCE_SIGNAL; // Источник сигнала для ПИД-регулятора.
volatile int display_contrast = -1;
volatile int disp_update_count = 0;

volatile int event_pos = 0;
volatile int event_num = 0;

volatile DWORD event_flag_current = 0;
volatile DWORD event_flag = 0;
volatile DWORD event_flag_last = 0;

volatile float info_hmc_d;
volatile float info_hmc_d_pr;
volatile float info_hmc_d_int;
volatile float info_hmc_d_dif;
volatile float info_coil;
volatile int info_coil_ofs;

volatile float lof_arr[2][LOF_SIZE];
volatile int lof_gist[2][LOF_GIST_SIZE] =
	{
		[0] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 31, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0},
		[1] = {0, 4, 8, 12, 16, 20, 24, 28, 31, 28, 24, 20, 16, 12, 8, 4, 4, 8, 12, 16, 20, 24, 28, 31, 28, 24, 20, 16, 12, 8, 4, 0},
};
volatile int lof_pos = -1;

volatile int tamper = 0;

const float hmc_disp_fs[6] =
	{
		0.5f,
		0.3f,
		0.2f,
		0.1f,
		0.08f,
		0.05f,
};

volatile int show_ph = 0;

volatile int flag_calib_reset = 0;
volatile int flag_im_reset = 0;
volatile int flag_factory_reset = 0;

const float im_ph_k[11] =
	{
		[0] = 50.0f,
		[1] = 40.0f,
		[2] = 30.0f,
		[3] = 20.0f,
		[4] = 10.0f,
		[5] = 0.0f,
		[6] = 1.0f / 10.0f,
		[7] = 1.0f / 20.0f,
		[8] = 1.0f / 30.0f,
		[9] = 1.0f / 40.0f,
		[10] = 1.0f / 50.0f,
};

#ifdef ADC_OVER
volatile WORD adc_buf[16];
#endif // ADC_OVER

volatile DWORD gist_pos[GIST_POS_SIZE];
volatile DWORD gist_tgt[GIST_TGT_SIZE];
volatile DWORD gist_tmp[GIST_TMP_SIZE];

volatile int step_test_n = 0;
volatile int step_test_c = 0;
volatile int step_test_t = 0;
volatile int step_test_to = 0;
volatile int step_test_min = 0;
volatile int step_test_max = 1000;

volatile int pst_to = 0;
volatile int pst_t = 0;
volatile int pst_te = 0;
volatile int pst_last = 0;
volatile int pst_cur = 0;
volatile int pst_result = 0;
volatile int pst_event_fail = 0;
volatile int pst_auto_to = 0;

volatile int hif_to = 0;

volatile int lof_to = 0;
volatile int lof_amplitude = 0;
volatile int lof_period = 0;
volatile int lof_time = 0;
volatile int lof_position = 0;

volatile int hart_act_count = 0;
volatile int hart_act_data_pos = 0;
volatile int hart_act_data_page_send = -1;
volatile WORD hart_act_data[64];
volatile int hart_act_data_type = HART_ACT_DATA_POS;

volatile int get_display = 0;
volatile int linear_temp_disable = 0;

void calib_reset(void)
{
	int i;

	con_str("Сброс времени\n\r");
	con_start();

	// сбросить "Кц"
	ihmc_path = 0;
	save_maj(data_eeprom.hmc_path_nv, 0);

	// сбросить "Тк"
	worktime = 0;
	save_maj(data_eeprom.worktime_nv, 0);

	// очистить журнал событий
	for (i = 0; i < EVENT_LOG_NUM; i++)
	{
		eeprom_unlock();
		data_eeprom.events[i].time = 0;
		data_eeprom.events[i].event = 0;
		eeprom_lock();
	}
	load_events();
}

void im_reset(void)
{
	int i;

	con_str("Full reset\n\r");
	con_start();

	eeprom_unlock();

	// Сброс типа датчика положения.
	data_eeprom.sensor_type = 0;

	// Сброс формата экрана.
	data_eeprom.display_format = 0;

	data_eeprom.temp_version = 1;

	// Сброс переходной характеристики - линейная
	data_eeprom.flags.im_ph = 5;

	// Сброс линеаризации - выключена
	data_eeprom.linear_en = 0;
	data_eeprom.linear_coef = 0;
	data_eeprom.linear_shift = 0;

	// Выключено плотное закрытие
	data_eeprom.hard_pos_max = 0;
	data_eeprom.hard_pos_min = 0;

	// Выключаю ограничение быстродействия
	data_eeprom.coil_pwm_limit = 0;

	// Сброс фильтра экрана - 0.1, фильтра аналогового входа - 0.5
	data_eeprom.flags.display_filter = 5;
	data_eeprom.lpf_a_in = 0.5;

	// Сбрасываю настройки выходов и выключаю их
	data_eeprom.flags.dout3_sig = OUT_SIG_NONE;
	data_eeprom.flags.dout2_sig = OUT_SIG_NONE;
	data_eeprom.flags.aout_sig = OUT_SIG_NONE;
	data_eeprom.flags.dout1_disable = DOUT1_DISABLE;
	data_eeprom.flags.dout2_dir = DOUT_DIR_DOWN;
	data_eeprom.flags.dout3_dir = DOUT_DIR_UP;
	data_eeprom.dout2_pos = 50;
	data_eeprom.dout3_pos = 950;
	data_eeprom.dout2_gis = 10;
	data_eeprom.dout3_gis = 10;
	data_eeprom.aout_pos_max = 1000;
	data_eeprom.aout_pos_min = 0;
	data_eeprom.aout_ma_max = 200;
	data_eeprom.aout_ma_min = 40;

	// Сбрасываю настройки HART
	data_eeprom.flags.hart_id = 0;
	data_eeprom.flags.hart_enable = HART_ENABLE_ON;
	data_eeprom.flags.hart_prio = HART_PRIO_AIN;

	// Сбрасываю настройки дисплея
	data_eeprom.flags.display_im = DISPLAY_IM_PROC;
	data_eeprom.flags.display_ain = DISPLAY_AIN_MA;
	data_eeprom.flags.display_contrast = 0;

	// Сбрасываю настройки ПИД-регулятора
	data_eeprom.kp = 50.0;
	data_eeprom.ki = 0.1;
	data_eeprom.kd = 0.0;
	data_eeprom.pid_int_min = -500.0 / data_eeprom.ki;
	data_eeprom.pid_int_max = 500.0 / data_eeprom.ki;
	data_eeprom.flags.coil_inverse = 0;
	data_eeprom.t_lp_up = 0.8;
	data_eeprom.t_lp_down = 0.8;
	data_eeprom.si_off = 25.0;
	data_eeprom.midpoint = 350;
	data_eeprom.slow_LT_edge = -30;
	data_eeprom.slow_LT_val = 0.5;

	// Сбрасываю настройки диагностики
	data_eeprom.pst_dx = 5;
	data_eeprom.pst_t = 10;
	data_eeprom.pst_te = 8;
	data_eeprom.pst_autorun = 0;
	data_eeprom.hif_dx = 5;
	data_eeprom.hif_t = 10;

	eeprom_lock();

	// сбросить "Кц"
	ihmc_path = 0;
	save_maj(data_eeprom.hmc_path_nv, 0);

	// сбросить "Тк"
	worktime = 0;
	save_maj(data_eeprom.worktime_nv, 0);

	// очистить журнал событий
	eeprom_unlock();
	for (i = 0; i < EVENT_LOG_NUM; i++)
	{
		data_eeprom.events[i].time = 0;
		data_eeprom.events[i].event = 0;
	}
	eeprom_lock();
	load_events();

	event_save(EVENT_FULL_RST); // Записываю событие "Полный сброс"
}

void factory_reset(void)
{
	int i;

	con_str("Factory reset\n\r");
	con_start();
	eeprom_unlock();
	// Сброс заводских уставок
	data_eeprom.ain_adc_min = 50;
	data_eeprom.ain_adc_max = 2050;
	data_eeprom.ain_ma_min = 400;
	data_eeprom.ain_ma_max = 2000;
	data_eeprom.ain_ma_t_min = 400;
	data_eeprom.ain_ma_t_max = 2000;
	data_eeprom.hmc_pos_t_min = 0;
	data_eeprom.hmc_pos_t_max = 1000;
	data_eeprom.aout_pwm_min = 165;
	data_eeprom.aout_pwm_max = 820;
	data_eeprom.hmc_adc_0 = 0;
	data_eeprom.hmc_adc_100 = 4096;
	data_eeprom.hmc_lim_0 = 0;
	data_eeprom.hmc_lim_100 = 1000;
	data_eeprom.hmc_g_0 = -212;
	data_eeprom.hmc_g_100 = 204;
	//	data_eeprom.coil_pwm_limit

	data_eeprom.lpf_max_samples = 64;
	data_eeprom.lpf_pos = 1;
	data_eeprom.lpf_a_in = 1;
	data_eeprom.lpf_a_out = 1;

	// Сброс пользовательских настроек

	data_eeprom.flags.im_ph = 5;
	data_eeprom.hard_pos_max = 0;
	data_eeprom.hard_pos_min = 0;
	data_eeprom.coil_pwm_limit = 0;
	data_eeprom.flags.display_filter = 5;

	data_eeprom.flags.dout3_sig = OUT_SIG_NONE;
	data_eeprom.flags.dout2_sig = OUT_SIG_NONE;
	data_eeprom.flags.aout_sig = OUT_SIG_NONE;
	data_eeprom.flags.dout1_disable = DOUT1_DISABLE;
	data_eeprom.flags.dout2_dir = DOUT_DIR_DOWN;
	data_eeprom.flags.dout3_dir = DOUT_DIR_UP;
	data_eeprom.dout2_pos = 50;
	data_eeprom.dout3_pos = 950;
	data_eeprom.dout2_gis = 10;
	data_eeprom.dout3_gis = 10;
	data_eeprom.aout_pos_max = 1000;
	data_eeprom.aout_pos_min = 0;
	data_eeprom.aout_ma_max = 200;
	data_eeprom.aout_ma_min = 40;
	data_eeprom.flags.hart_id = 0;
	data_eeprom.flags.hart_enable = HART_ENABLE_ON;
	data_eeprom.flags.hart_prio = HART_PRIO_AIN;
	data_eeprom.flags.display_im = DISPLAY_IM_PROC;
	data_eeprom.flags.display_ain = DISPLAY_AIN_MA;
	data_eeprom.flags.display_contrast = 0;

	data_eeprom.kp = 25.0;
	data_eeprom.ki = 0.05;
	data_eeprom.kd = 0.0;
	data_eeprom.pid_int_min = -500.0 / data_eeprom.ki;
	data_eeprom.pid_int_max = 500.0 / data_eeprom.ki;
	data_eeprom.flags.coil_inverse = 0;
	data_eeprom.t_lp_up = 0.05;
	data_eeprom.t_lp_down = 0.05;
	data_eeprom.si_off = 25.0;
	data_eeprom.pst_dx = 5;
	data_eeprom.pst_t = 10;
	data_eeprom.pst_te = 8;
	data_eeprom.pst_autorun = 0;
	data_eeprom.hif_dx = 5;
	data_eeprom.hif_t = 10;
	data_eeprom.midpoint = 350;
	data_eeprom.slow_LT_edge = -30;
	data_eeprom.slow_LT_val = 0.5;

	data_eeprom.linear_en = 0;
	data_eeprom.linear_coef = 0;
	data_eeprom.linear_shift = 0;
	data_eeprom.display_format = FROM_D;
	data_eeprom.sensor_type = 0;
	data_eeprom.temp_version = 1;

	eeprom_lock();

	// Сброс "Кц","Тк"(!, журнала событий)
	ihmc_path = 0;
	save_maj(data_eeprom.hmc_path_nv, 0);
	worktime = 0;
	save_maj(data_eeprom.worktime_nv, 0);

	// eeprom_unlock();
	// for (i = 0; i < EVENT_LOG_NUM; i++) //Очистка журнала событий
	// {
	// 	data_eeprom.events[event_pos].time = 0; //Малков: считаю, что нужно изменить event_pos на i
	// 	data_eeprom.events[event_pos].event = 0;
	// }
	// eeprom_lock();
	load_events();

	event_save(EVENT_FULL_RST); // Записываю событие "Полный сброс"

	con_str("Reset completed\n\r");
	con_start();
}

/// @brief Очистка журнала событий
void log_reset(void)
{
	int i;

	eeprom_unlock();
	for (i = 0; i < EVENT_LOG_NUM; i++)
	{
		data_eeprom.events[i].time = 0;
		data_eeprom.events[i].event = 0;
	}
	eeprom_lock();
	load_events();
}

void eeprom_unlock(void)
{
	if (FLASH->PECR & (1 << FLASH_PECR_PELOCK))
	{
		FLASH->PEKEYR = 0x89ABCDEF;
		FLASH->PEKEYR = 0x02030405;
	}
}

void eeprom_lock(void)
{
	FLASH->PECR = (1 << FLASH_PECR_PELOCK);
}

void hart_tx_enable(void)
{
	set_pin(HART_RTS_PIN, 0); // HART_RTS = 0
	USART3->CR1 |= (1 << USART_CR1_TXEIE);
}

void set_pin(const WORD pin, const BYTE value)
{
	GPIO_TypeDef *port;
	switch (pin & 0xF0)
	{
	case GPIOA_PORT:
		port = GPIOA;
		break;
	case GPIOB_PORT:
		port = GPIOB;
		break;
	case GPIOC_PORT:
		port = GPIOC;
		break;
	case GPIOD_PORT:
		port = GPIOD;
		break;
	case GPIOE_PORT:
		port = GPIOE;
		break;
	case GPIOF_PORT:
		port = GPIOF;
		break;
	default:
		return;
	}
	if (value)
		port->BSRRL = 1 << (pin & 15);
	else
		port->BSRRH = 1 << (pin & 15);
}

int get_pin(const WORD pin)
{
	GPIO_TypeDef *port;
	switch (pin & 0xF0)
	{
	case GPIOA_PORT:
		port = GPIOA;
		break;
	case GPIOB_PORT:
		port = GPIOB;
		break;
	case GPIOC_PORT:
		port = GPIOC;
		break;
	case GPIOD_PORT:
		port = GPIOD;
		break;
	case GPIOE_PORT:
		port = GPIOE;
		break;
	case GPIOF_PORT:
		port = GPIOF;
		break;
	default:
		return 0;
	}
	return (port->IDR >> (pin & 15)) & 1;
}

void set_pupd(const WORD pin, const BYTE value)
{
	GPIO_TypeDef *port;
	switch (pin & 0xF0)
	{
	case GPIOA_PORT:
		port = GPIOA;
		break;
	case GPIOB_PORT:
		port = GPIOB;
		break;
	case GPIOC_PORT:
		port = GPIOC;
		break;
	case GPIOD_PORT:
		port = GPIOD;
		break;
	case GPIOE_PORT:
		port = GPIOE;
		break;
	case GPIOF_PORT:
		port = GPIOF;
		break;
	default:
		return;
	}
	port->PUPDR = (port->PUPDR & (~(3 << ((pin & 15) * 2)))) | ((value & 3) << ((pin & 15) * 2));
}

int load_maj(volatile int x[3], volatile int *v)
{
	int h[3];

	h[0] = x[0];
	h[1] = x[1];
	h[2] = x[2];

	if ((h[0] == h[1]) && (h[1] == h[2]) && (h[0] == h[2]))
	{
		*v = h[0];
		return 1;
	}
	else if ((h[0] != h[1]) && (h[1] != h[2]) && (h[0] != h[2]))
	{
		return 0;
	}
	if (h[0] == h[1])
		*v = h[0];
	else if (h[0] == h[2])
		*v = h[0];
	else if (h[1] == h[2])
		*v = h[1];

	return save_maj(x, *v);
}

int save_maj(volatile int x[3], volatile int v)
{
	int h = v;
	eeprom_unlock();
	x[0] = h;
	eeprom_lock();
	eeprom_unlock();
	x[1] = h;
	eeprom_lock();
	eeprom_unlock();
	x[2] = h;
	eeprom_lock();
	if ((x[0] == x[1]) && (x[1] == x[2]) && (x[0] == x[2]))
		return 1;
	else
		return 0;
}

void load_events(void)
{
	int i;
	DWORD event_time = 0;

	event_pos = 0;
	event_num = 0;

	event_time = data_eeprom.events[0].time;

	if ((event_time == 0) || ((data_eeprom.events[0].event & (~(1 << EVENT_FLAG))) <= EVENT_FREE) || ((data_eeprom.events[0].event & (~(1 << EVENT_FLAG))) >= EVENT_NUM))
	{
		return;
	}

	i = 0;
	while ((i < EVENT_LOG_NUM) && ((data_eeprom.events[i].event & (~(1 << EVENT_FLAG))) > EVENT_FREE) && ((data_eeprom.events[i].event & (~(1 << EVENT_FLAG))) < EVENT_NUM) && (data_eeprom.events[i].time >= event_time))
	{
		event_time = data_eeprom.events[i].time;
		event_pos++;
		i++;
	}
	event_num = event_pos;

	if (i < EVENT_LOG_NUM)
	{
		if ((data_eeprom.events[i].time == 0) || ((data_eeprom.events[i].event & (~(1 << EVENT_FLAG))) <= EVENT_FREE) || ((data_eeprom.events[i].event & (~(1 << EVENT_FLAG))) >= EVENT_NUM))
		{
			return;
		}

		event_time = data_eeprom.events[i].time;

		while ((i < EVENT_LOG_NUM) && ((data_eeprom.events[i].event & (~(1 << EVENT_FLAG))) > EVENT_FREE) && ((data_eeprom.events[i].event & (~(1 << EVENT_FLAG))) < EVENT_NUM) && (data_eeprom.events[i].time >= event_time))
		{
			event_time = data_eeprom.events[i].time;
			i++;
		}

		if (i == EVENT_LOG_NUM)
			event_num = EVENT_LOG_NUM;
	}
}

void event_save(const DWORD event)
{
	eeprom_unlock();
	data_eeprom.events[event_pos].time = worktime;
	data_eeprom.events[event_pos].event = event;
	if (event_num < EVENT_LOG_NUM)
		event_num++;
	eeprom_lock();

	con_str("EVENT [e=");
	con_dword(data_eeprom.events[event_pos].event);
	con_str(", t=");
	con_dword(data_eeprom.events[event_pos].time);
	con_str("] saved ");
	con_dec(event_pos);
	con_str("/");
	con_dec(event_num);
	con_str("\n\r");
	con_start();

	if (event_pos < EVENT_LOG_NUM)
		event_pos++;
	else
		event_pos = 0;
}

void tick(tTIMER *t)
{
	tTIMER temp;
	temp = *t;
	temp += TO_TICK;
	if (temp > (*t))
		*t = temp;
}

void print_fault(sFAULT *fault)
{
	con_str("FAULT:\n\r");
	con_str("r0   = ");
	con_dword(fault->r0);
	con_str(", ");
	con_str("r1   = ");
	con_dword(fault->r1);
	con_str(", ");
	con_str("r2   = ");
	con_dword(fault->r2);
	con_str(", ");
	con_str("r3   = ");
	con_dword(fault->r3);
	con_str("\n\r");
	con_str("r12  = ");
	con_dword(fault->r12);
	con_str(", ");
	con_str("lr   = ");
	con_dword(fault->lr);
	con_str(", ");
	con_str("pc   = ");
	con_dword(fault->pc);
	con_str(", ");
	con_str("xpsr = ");
	con_dword(fault->xpsr);
	con_str("\n\r");
	con_start();
}

void mstr_cpy(char *dst, const char *src)
{
	while ((*src))
	{
		(*dst) = (*src);
		dst++;
		src++;
	}
	(*dst) = 0;
}

void mstr_add(char *dst, const char *src)
{
	while ((*dst))
		dst++;

	mstr_cpy(dst, src);
}

void mstr_copy_int(char *dst, int x)
{
	DWORD h;
	DWORD t;
	BYTE b[10];
	BYTE c;

	if (x == 0)
	{
		(*dst) = '0';
		dst++;
	}
	else
	{
		if (x < 0)
		{
			(*dst) = '-';
			dst++;
			t = -x;
		}
		else
			t = x;
		h = 10;
		c = 0;
		while (t > 0)
		{
			b[c++] = t % h;
			t /= 10;
		}
		while (c > 0)
		{
			(*dst) = b[--c] | '0';
			dst++;
		}
	}
	(*dst) = 0;
}

void mstr_add_int(char *dst, int x)
{
	while ((*dst))
		dst++;

	mstr_copy_int(dst, x);
}

void mstr_add_int2(char *dst, int x)
{
	while (*dst)
		dst++;

	if (x < 0)
		x = -x;

	*dst++ = ((x / 10) % 10) | '0';
	*dst++ = ((x / 1) % 10) | '0';

	*dst = 0;
}

void mstr_add_dword(char *dst, DWORD x)
{
	int i;
	while (*dst)
		dst++;
	for (i = 0; i < 8; i++)
		*dst++ = Hex[(x >> ((7 - i) * 4)) & 15];
	*dst = 0;
}

void Change_PWM_Aout(int value)
{
	if (value > 0)
		PWM_AOUT = value;
	else
		PWM_AOUT = 0;
}

float Get_LpfValue(int filtrationDegree)
{
	float proportion = data_eeprom.lpf_max_samples / 4;

	switch (filtrationDegree)
	{
	case LPF_MAX:
		return 0.25f;

	case LPF_MID:
		return 0.5f;

	case LPF_MIN:
		return 0.75f;

	default:
		return 1.0f;
	}
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
