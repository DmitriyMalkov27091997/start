/**
 * @file menu.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Библиотека меню.
 * @version 2.1
 * @date 10.03.2023
 *
 * @copyright (C) Ярославль, 2016
 *
 */
#include "menu.h"
#include "menu_v2.h"
#include "menu_render.h"
#include "menu_navigate.h"
#include "picto.h"
#include "define.h"
#include "font.h"
#include <math.h>
#include "hart.h"
#include "isr.h"

volatile int menu_view = MENU_VIEW_START;
volatile int menu_setup = 0;
volatile int menu_subitem = 0;
volatile int menu_subitem_edit = 0;
volatile int menu_lof_pos = 0;
volatile int menu_lof_amp = 0;
volatile int menu_lof_size = 0;
volatile int menu_lof_view = 0;
volatile int menu_kp, menu_ki, menu_kd;
volatile int menu_event_last = 0;


const BYTE *menu_setup_items[MENU_SETUP_NUM] =
	{
		[MENU_SETUP_SIGNAL] = "Сигнализация",
		[MENU_SETUP_DISPLAY] = "Дисплей",
		[MENU_SETUP_HART] = "HART",
		[MENU_SETUP_IM] = "Настройка ИМ",
		[MENU_SETUP_DIAG] = "Диагностика",
		[MENU_SETUP_LOG] = "Журнал событий",
		[MENU_SETUP_HAND] = "Ручной режим",
};

const BYTE *menu_display_items[MENU_DISPLAY_NUM] =
	{
		[MENU_DISPLAY_IM] = "Единицы ИМ",
		[MENU_DISPLAY_AIN] = "Единицы входа",
		[MENU_DISPLAY_CONTRAST] = "Контрастность",
		[MENU_DISPLAY_ROTATE] = "Поворот экрана",
		[MENU_DISPLAY_FILTER] = "Сглаживание ПК",
};

const BYTE *menu_hart_items[MENU_HART_NUM] =
	{
		[MENU_HART_ID] = "Идентификатор",
		[MENU_HART_ENABLE] = "Отключение",
		[MENU_HART_PRIO] = "Приоритет",
};

const BYTE *menu_signal_items[MENU_SIGNAL_NUM] =
	{
		[MENU_SIGNAL_AOUT] = "Аналог. выход",
		[MENU_SIGNAL_DOUT1] = "Диск. выход 1",
		[MENU_SIGNAL_DOUT2] = "Диск. выход 2",
		[MENU_SIGNAL_DOUT3] = "Диск. выход 3",
};

const BYTE *menu_signal_aout_items[MENU_SIGNAL_AOUT_NUM] =
	{
		[MENU_SIGNAL_AOUT_SIG] = "Сигнал",
		[MENU_SIGNAL_AOUT_UP] = "Верх. диап",
		[MENU_SIGNAL_AOUT_DOWN] = "Ниж. диап",
		[MENU_SIGNAL_AOUT_MAX] = "Макс. ток",
		[MENU_SIGNAL_AOUT_MIN] = "Мин. ток",
};

const BYTE *menu_signal_dout_items[MENU_SIGNAL_DOUT_NUM] =
	{
		[MENU_SIGNAL_DOUT_SIG] = "Сигнал",
		[MENU_SIGNAL_DOUT_VAL] = "Положение",
		[MENU_SIGNAL_DOUT_DIR] = "Направление",
		[MENU_SIGNAL_DOUT_GIS] = "Гистерезис",
};

const BYTE *menu_im_items[MENU_IM_NUM] =
	{
		[MENU_IM_CALIB] = "Калибровка",
		[MENU_IM_AUTO] = "Автонастройка",
		[MENU_IM_HAND] = "ПИД регулятор",
		[MENU_IM_T_LP] = "Скорость хода",
		[MENU_IM_HAR] = "Передаточные",
		[MENU_IM_HARD] = "Плотное закрытие",
		[MENU_IM_DIAP] = "Диапазон входа",
		[MENU_IM_LP] = "ФНЧ входа",
		[MENU_IM_LINEAR] = "Линеаризация",
		[MENU_IM_RESET] = "Полный сброс",
};

const BYTE *menu_im_calib_items[MENU_IM_CALIB_NUM] =
	{
		[MENU_IM_CALIB_TEST] = "ТЕСТ",
		[MENU_IM_CALIB_MIN] = "Минимум",
		[MENU_IM_CALIB_MAX] = "Максимум",
		[MENU_IM_CALIB_RST] = "Сброс времени",
};

const BYTE *menu_im_hand_items[MENU_IM_HAND_NUM] =
	{
		[MENU_IM_HAND_KP] = "Пропорц. Кп",
		[MENU_IM_HAND_KI] = "Интеграл. Ки",
		[MENU_IM_HAND_KD] = "Диффер. Кд",
		[MENU_IM_HAND_SAVE] = "Сохранить",
		[MENU_IM_HAND_INV] = "Инвертировать",
		[MENU_IM_HAND_SI_OFF] = "Зона накопления",
};

const BYTE *menu_im_t_lp_items[MENU_IM_T_LP_NUM] =
	{
		[MENU_IM_T_LP_UP] = "Скорость вверх",
		[MENU_IM_T_LP_DOWN] = "Скорость вниз",
};

const BYTE *menu_im_har_items[MENU_IM_HAR_NUM] =
	{
		[MENU_IM_HAR_PER] = "Передаточная хар.",
		[MENU_IM_HAR_LIM] = "Ограничение расхода"};

const BYTE *menu_im_hard_items[MENU_IM_HARD_NUM] =
	{
		[MENU_IM_HARD_MIN] = "Минимум",
		[MENU_IM_HARD_MAX] = "Максимум",
};

const BYTE *menu_im_diap_items[MENU_IM_DIAP_NUM] =
	{
		[MENU_IM_DIAP_MIN] = "Минимум",
		[MENU_IM_DIAP_MAX] = "Максимум",
};

const BYTE *menu_im_none_items[1] =
	{
		[0] = "ФНЧ входа",
};

const BYTE *menu_im_linear_items[1] =
	{
		[0] = "Линеаризация",
};

const BYTE *menu_im_reset_items[1] =
	{
		[0] = "Полный сброс",
};

const BYTE *menu_diag_items[MENU_DIAG_NUM] =
	{
		[MENU_DIAG_PST] = "PST-тест",
		[MENU_DIAG_FRIC] = "Высокое трение",
		[MENU_DIAG_GIST] = "Гистограммы",
		[MENU_DIAG_LOF] = "Трение страгивания",
};

const BYTE *menu_pst_items[MENU_PST_NUM] =
	{
		[MENU_PST_DX] = "Процент хода, %",
		[MENU_PST_T] = "Время теста, с",
		[MENU_PST_TE] = "Задержка ошибки, с",
		[MENU_PST_AUTORUN] = "Автозапуск, час",
		[MENU_PST_START] = "PST тест",
};

const BYTE *menu_fric_items[MENU_FRIC_NUM] =
	{
		[MENU_FRIC_DX] = "Рассогласование, %",
		[MENU_FRIC_T] = "Время, с",
};

const BYTE *menu_diag_gist_items[MENU_DIAG_GIST_NUM] =
	{
		[MENU_DIAG_GIST_POS] = "пол",
		[MENU_DIAG_GIST_TGT] = "зад",
		[MENU_DIAG_GIST_TMP] = "тем",
};

const BYTE *menu_out_sig_items[MENU_OUT_SIG_NUM] =
	{
		[MENU_OUT_SIG_NONE] = "Нет",
		[MENU_OUT_SIG_POS] = "ПК",
		[MENU_OUT_SIG_AIN] = "ВС",
		[MENU_OUT_SIG_DELTA] = "ОШ",
};

const BYTE *menu_out_sig_des_items[MENU_OUT_SIG_NUM] =
	{
		[MENU_OUT_SIG_NONE] = "Выключено",
		[MENU_OUT_SIG_POS] = "Полож. клапана",
		[MENU_OUT_SIG_AIN] = "Входной сигнал",
		[MENU_OUT_SIG_DELTA] = "Рассогласование",
};

const BYTE *menu_dout_dir_items[MENU_DOUT_DIR_NUM] =
	{
		[MENU_DOUT_DIR_UP] = "Увелич.",
		[MENU_DOUT_DIR_DOWN] = "Уменьш.",
};

const BYTE *menu_calib_pwm_items[4] =
	{
		[MENU_CALIB_PWM_NONE] = "Нет",
		[MENU_CALIB_PWM_0] = "минимум",
		[MENU_CALIB_PWM_50] = "середина",
		[MENU_CALIB_PWM_100] = "максимум",
};

const BYTE *menu_im_auto_items[] =
	{
		"ВНИМАНИЕ!!! Привод",	 // 0
		"будет двигаться",		 // 1
		"Определение минимума",	 // 2
		"Определение максимума", // 3
		"идет сбор статистики",	 // 4
		"Получены границы",		 // 5
		"Время хода вниз",		 // 6
		"Время хода вверх",		 // 7
		"Время хода итог",		 // 8
		"Установка 50%",		 // 9
		"Нажмите кнопку",		 // 10
		"Выберите тип",			 // 11
		"НЗ-клапан",			 // 12
		"НО-клапан",			 // 13
		"HART автонастройка",	 // 14
};

const BYTE *menu_dout1_disable[2] =
	{
		[DOUT1_ENABLE] = "Вкл",
		[DOUT1_DISABLE] = "Выкл",
};

const sMENU_VALUE menu_signal_value_item[] =
	{
		{MENU_SIGNAL_AOUT, MENU_SIGNAL_AOUT_SIG, 0, &data_eeprom.flags.i, 0, 0},
		{MENU_SIGNAL_AOUT, MENU_SIGNAL_AOUT_UP, 0, &data_eeprom.aout_pos_max, -50, 1050},
		{MENU_SIGNAL_AOUT, MENU_SIGNAL_AOUT_DOWN, 0, &data_eeprom.aout_pos_min, -50, 1050},
		{MENU_SIGNAL_AOUT, MENU_SIGNAL_AOUT_MAX, 0, &data_eeprom.aout_ma_max, 40, 200},
		{MENU_SIGNAL_AOUT, MENU_SIGNAL_AOUT_MIN, 0, &data_eeprom.aout_ma_min, 40, 200},
		{MENU_SIGNAL_DOUT1, MENU_SIGNAL_DOUT_SIG, 0, &data_eeprom.flags.i, 0, 0},
		{MENU_SIGNAL_DOUT2, MENU_SIGNAL_DOUT_SIG, 0, &data_eeprom.flags.i, 0, 0},
		{MENU_SIGNAL_DOUT2, MENU_SIGNAL_DOUT_VAL, 0, &data_eeprom.dout2_pos, 0, 1000},
		{MENU_SIGNAL_DOUT2, MENU_SIGNAL_DOUT_DIR, 0, &data_eeprom.flags.i, 0, 0},
		{MENU_SIGNAL_DOUT2, MENU_SIGNAL_DOUT_GIS, 0, &data_eeprom.dout2_gis, 0, 200},
		{MENU_SIGNAL_DOUT3, MENU_SIGNAL_DOUT_SIG, 0, &data_eeprom.flags.i, 0, 0},
		{MENU_SIGNAL_DOUT3, MENU_SIGNAL_DOUT_VAL, 0, &data_eeprom.dout3_pos, 0, 1000},
		{MENU_SIGNAL_DOUT3, MENU_SIGNAL_DOUT_DIR, 0, &data_eeprom.flags.i, 0, 0},
		{MENU_SIGNAL_DOUT3, MENU_SIGNAL_DOUT_GIS, 0, &data_eeprom.dout3_gis, 0, 200},
		{0, 0, 0, 0, 0, 0},
};

const sMENU_VALUE menu_im_hand_value_item[] =
	{
		{MENU_IM_HAND, MENU_IM_HAND_KP, 0, &menu_kp, 0, 999999},
		{MENU_IM_HAND, MENU_IM_HAND_KI, 0, &menu_ki, 0, 999999},
		{MENU_IM_HAND, MENU_IM_HAND_KD, 0, &menu_kd, 0, 999999},
		{MENU_IM_HARD, MENU_IM_HARD_MIN, 0, &data_eeprom.hard_pos_min, 0, 200},
		{MENU_IM_HARD, MENU_IM_HARD_MAX, 0, &data_eeprom.hard_pos_max, 0, 200},
		{MENU_IM_DIAP, MENU_IM_DIAP_MIN, 0, &data_eeprom.ain_ma_t_min, 400, 2000},
		{MENU_IM_DIAP, MENU_IM_DIAP_MAX, 0, &data_eeprom.ain_ma_t_max, 400, 2000},
		{MENU_IM_LP, 0, 0, (int *)&data_eeprom.lpf_a_in, 0, 100},
		{MENU_IM_HAR, MENU_IM_HAR_LIM, 0, &data_eeprom.coil_pwm_limit, 0, 90},
		{MENU_IM_HAND, MENU_IM_HAND_INV, 0, &data_eeprom.flags.i, 0, 0},
		{MENU_IM_HAND, MENU_IM_HAND_SI_OFF, 0, value_f : &data_eeprom.si_off, 3, 1000},
		{MENU_IM_T_LP, MENU_IM_T_LP_UP, 0, value_f : &data_eeprom.t_lp_up, 0, 1000},
		{MENU_IM_T_LP, MENU_IM_T_LP_DOWN, 0, value_f : &data_eeprom.t_lp_down, 0, 1000},
		{0, 0, 0, 0, 0, 0},
};

const sMENU_VALUE menu_diag_value_item[] =
	{
		{MENU_DIAG_PST, MENU_PST_DX, 0, &data_eeprom.pst_dx, -15, 15},
		{MENU_DIAG_PST, MENU_PST_T, 0, &data_eeprom.pst_t, 2, 60},
		{MENU_DIAG_PST, MENU_PST_TE, 0, &data_eeprom.pst_te, 2, 60},
		{MENU_DIAG_PST, MENU_PST_AUTORUN, 0, &data_eeprom.pst_autorun, 0, 4096},
		{MENU_DIAG_FRIC, MENU_FRIC_DX, 0, &data_eeprom.hif_dx, 3, 25},
		{MENU_DIAG_FRIC, MENU_FRIC_T, 0, &data_eeprom.hif_t, 3, 60},
		{0, 0, 0, 0, 0, 0},
};

const BYTE *menu_event[EVENT_NUM] =
	{
		[EVENT_FREE] = "пусто",
		[EVENT_SW_FAIL] = "сбой ПО",
		[EVENT_AIN_MIN] = "вход мин",
		[EVENT_AIN_MAX] = "вход макс",
		[EVENT_POWER_FAIL] = "питание",
		[EVENT_ANGLE_MAX] = "угол макс",
		[EVENT_ANGLE_MIN] = "угол мин",
		[EVENT_TA_MIN] = "темп -40",
		[EVENT_TA_MAX] = "темп +85",
		[EVENT_AOUT_FAIL] = "аналог. выход",
		[EVENT_DOUT1_FAIL] = "диск. выход1",
		[EVENT_DOUT2_FAIL] = "диск. выход2",
		[EVENT_DOUT3_FAIL] = "диск. выход3",
		[EVENT_IM_FAIL] = "ИМ сбой",
		[EVENT_IM_FAIL_ONCE] = "ИМ сбой Р",
		[EVENT_HMC_MAX] = "ПК макс",
		[EVENT_HMC_MIN] = "ПК мин",
		[EVENT_WDT_RESET] = "вотчдог",
		[EVENT_TAMPER] = "тампер",
		[EVENT_BP_FAIL] = "блок питания",
		[EVENT_FULL_RST] = "сброс",
		[EVENT_PST_FAIL] = "ошибка PST",
		[EVENT_HIF_FAIL] = "высокое трение",
};

const BYTE *menu_disp_filter_item[8] =
	{
		[0] = "След. за ПИД",
		[1] = "Без фильтра",
		[2] = "Коэфф. 0.50",
		[3] = "Коэфф. 0.30",
		[4] = "Коэфф. 0.20",
		[5] = "Коэфф. 0.10",
		[6] = "Коэфф. 0.08",
		[7] = "Коэфф. 0.05",
};

const BYTE *menu_im_ph_item[11] =
	{
		[0] = "Равнопроц +50%",
		[1] = "Равнопроц +40%",
		[2] = "Равнопроц +30%",
		[3] = "Равнопроц +20%",
		[4] = "Равнопроц +10%",
		[5] = "Линейная",
		[6] = "Обратнопроц -10%",
		[7] = "Обратнопроц -20%",
		[8] = "Обратнопроц -30%",
		[9] = "Обратнопроц -40%",
		[10] = "Обратнопроц -50%",
};



//-----------------------------------------------------------------------------
//	void load_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
//-----------------------------------------------------------------------------
void load_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
{
	int i;
	while (value_item[i].value)
	{
		if ((value_item[i].menu_setup == setup) && (value_item[i].menu_subitem == subitem))
		{
			menu_data.i = *value_item[i].value;
			return;
		}
		i++;
	}
}

//-----------------------------------------------------------------------------
//	void load_value_float(const sMENU_VALUE *value_item, const int setup, const int subitem, const float mult)
//-----------------------------------------------------------------------------
void load_value_float(const sMENU_VALUE *value_item, const int setup, const int subitem, const float mult)
{
	int i;
	while (value_item[i].value)
	{
		if ((value_item[i].menu_setup == setup) && (value_item[i].menu_subitem == subitem))
		{
			menu_data.i = roundf(*value_item[i].value_f * mult);
			return;
		}
		i++;
	}
}

//-----------------------------------------------------------------------------
//	int get_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
//-----------------------------------------------------------------------------
int get_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
{
	int i;
	while (value_item[i].value)
	{
		if ((value_item[i].menu_setup == setup) && (value_item[i].menu_subitem == subitem))
		{
			return *value_item[i].value;
		}
		i++;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//	void save_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
//-----------------------------------------------------------------------------
void save_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
{
	int i;
	while (value_item[i].value)
	{
		if ((value_item[i].menu_setup == setup) && (value_item[i].menu_subitem == subitem))
		{
			eeprom_unlock();
			*value_item[i].value = menu_data.i;
			eeprom_lock();
			return;
		}
		i++;
	}
}

//-----------------------------------------------------------------------------
//	void save_value_float(const sMENU_VALUE *value_item, const int setup, const int subitem)
//-----------------------------------------------------------------------------
void save_value_float(const sMENU_VALUE *value_item, const int setup, const int subitem, const float mult)
{
	int i;
	while (value_item[i].value)
	{
		if ((value_item[i].menu_setup == setup) && (value_item[i].menu_subitem == subitem))
		{
			eeprom_unlock();
			*(float *)(value_item[i].value) = menu_data.i * mult;
			eeprom_lock();
			return;
		}
		i++;
	}
}

//-----------------------------------------------------------------------------
//	void set_value(const sMENU_VALUE *value_item, const int setup, const int subitem, const int x)
//-----------------------------------------------------------------------------
void set_value(const sMENU_VALUE *value_item, const int setup, const int subitem, const int x)
{
	int i;
	while (value_item[i].value)
	{
		if ((value_item[i].menu_setup == setup) && (value_item[i].menu_subitem == subitem))
		{
			*value_item[i].value = x;
			return;
		}
		i++;
	}
}

//-----------------------------------------------------------------------------
//	int min_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
//-----------------------------------------------------------------------------
int min_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
{
	int i;
	while (value_item[i].value)
	{
		if ((value_item[i].menu_setup == setup) && (value_item[i].menu_subitem == subitem))
		{
			return value_item[i].min;
		}
		i++;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//	int max_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
//-----------------------------------------------------------------------------
int max_value(const sMENU_VALUE *value_item, const int setup, const int subitem)
{
	int i;
	while (value_item[i].value)
	{
		if ((value_item[i].menu_setup == setup) && (value_item[i].menu_subitem == subitem))
		{
			return value_item[i].max;
		}
		i++;
	}
	return 0;
}



//-----------------------------------------------------------------------------
//	void to_ma(BYTE *str, const int x)
//-----------------------------------------------------------------------------
void to_ma(BYTE *str, const int x)
{
	str[0] = 0;
	mstr_add_int(str, x / 10);
	mstr_add(str, ".");
	mstr_add_int(str, x % 10);
	mstr_add(str, "мА");
}

/// @brief Перейти к указанному экрану.
/// @param view Номер экрана.
void menu_go_to(int view)
{
	to_menu = 0;
	menu_view = view;
}

/// @brief Перейти к указанному экрану с последующей его очисткой.
/// @param view Номер экрана.
void menu_go_to_view_and_clear(int view)
{
	menu_go_to(view);
	pcf8531_clear(0, 0, 128, 32);
}

/// @brief Перейти к указанному экрану с последующей устновкой изображения.
/// @param view Номер экрана.
/// @param pic Изображение.
void menu_go_to_view_and_set_pic(int view, const BYTE *pic)
{
	menu_go_to(view);
	pcf8531_pic(0, 0, pic, 128, 32);
}

/// @brief Автоматический выход.
void menu_auto_exit(void)
{
	to_auto = 0;
	st_auto = ST_AUTO_IDLE;
	hart_autoset_flag = 0;
	menu_setup = MENU_IM_AUTO;

	menu_go_to_view_and_clear(MENU_VIEW_IM);

	con_str("[MENU_AUTO_EXIT]");
	con_start();
}

/// @brief Выполнить отрисовку основного экрана в указанном режиме работы.
/// @param mode Режим работы: \n 0-автоматический; \n 1-ручной
void redraw_menu_view_main(const int mode)
{
	int i, s;

	//-------------------------------------
	//	Положение. Аналог
	//-------------------------------------
	// pcf8531_pic(0, 0, picto_normal, 128, 32);
	// return;

	pcf8531_clear(0, 23, 3, 5);
	pcf8531_clear(125, 23, 3, 5);
	pcf8531_pic(0, 24, picto_line, 128, 8);

	i = ihmc_pos_d;
	if (i != -1)
		pcf8531_pica(2 + i * 120 / 1000, 25, picto_pointer, 4, 7);

	i = ihmc_pos_t;
	if (i != -1)
		pcf8531_pica(2 + i * 122 / 1000, 25, picto_pointer_t, 2, 7);

	if (mode == 0)
	{
		if (hart_ma == -1)
		{
			// AIN
			if (data_eeprom.flags.display_ain == DISPLAY_AIN_MA)
			{
				//-------------------------------------
				//	Входной сигнал мА
				//-------------------------------------
				pcf8531_pic(81, 16, picto_ma_small_place, 35, 8);
				i = ain_ma_s;
				if (i < 0)
					i = 0;
				if (i >= 1000)
					pcf8531_pic(83, 16, picto_digit_small[(i / 1000) % 10], 5, 8);
				pcf8531_pic(89, 16, picto_digit_small[(i / 100) % 10], 5, 8);
				pcf8531_pic(97, 16, picto_digit_small[(i / 10) % 10], 5, 8);
			}
			else
			{
				//-------------------------------------
				//	Входной сигнал проценты
				//-------------------------------------
				pcf8531_pic(81, 16, picto_proc_small_place, 35, 8);
				i = ain_ma_s;
				if (0)
				{
				}
				else if (i < data_eeprom.ain_ma_t_min)
					i = data_eeprom.hmc_pos_t_min;
				else if (i > data_eeprom.ain_ma_t_max)
					i = data_eeprom.hmc_pos_t_max;
				else
				{
					i = data_eeprom.hmc_pos_t_min + (i - data_eeprom.ain_ma_t_min) * (data_eeprom.hmc_pos_t_max - data_eeprom.hmc_pos_t_min) / (data_eeprom.ain_ma_t_max - data_eeprom.ain_ma_t_min);
				}
				if (i < 0)
					i = 0;
				if (i > 1000)
					i = 1000;
				if (i >= 1000)
					pcf8531_pic(82, 16, picto_digit_small[(i / 1000) % 10], 5, 8);
				if (i >= 100)
					pcf8531_pic(88, 16, picto_digit_small[(i / 100) % 10], 5, 8);
				pcf8531_pic(94, 16, picto_digit_small[(i / 10) % 10], 5, 8);
				pcf8531_pic(102, 16, picto_digit_small[(i / 1) % 10], 5, 8);
			}
		}
		else
		{
			// HART
			if (data_eeprom.flags.display_ain == DISPLAY_AIN_MA)
			{
				//-------------------------------------
				//	Входной сигнал мА
				//-------------------------------------
				pcf8531_pic(81, 16, picto_ma_small_place, 35, 8);
				i = hart_ma;
				if (i < 0)
					i = 0;
				if (i >= 1000)
					pcf8531_pic(83, 16, picto_digit_small[(i / 1000) % 10], 5, 8);
				pcf8531_pic(89, 16, picto_digit_small[(i / 100) % 10], 5, 8);
				pcf8531_pic(97, 16, picto_digit_small[(i / 10) % 10], 5, 8);
			}
			else
			{
				//-------------------------------------
				//	Входной сигнал проценты
				//-------------------------------------
				pcf8531_pic(81, 16, picto_proc_small_place, 35, 8);
				if (0)
				{
				}
				else if (hart_ma < data_eeprom.ain_ma_t_min)
					i = data_eeprom.hmc_pos_t_min;
				else if (hart_ma > data_eeprom.ain_ma_t_max)
					i = data_eeprom.hmc_pos_t_max;
				else
				{
					i = data_eeprom.hmc_pos_t_min + (hart_ma - data_eeprom.ain_ma_t_min) * (data_eeprom.hmc_pos_t_max - data_eeprom.hmc_pos_t_min) / (data_eeprom.ain_ma_t_max - data_eeprom.ain_ma_t_min);
				}
				if (i < 0)
					i = 0;
				if (i > 1000)
					i = 1000;
				if (i >= 1000)
					pcf8531_pic(82, 16, picto_digit_small[(i / 1000) % 10], 5, 8);
				if (i >= 100)
					pcf8531_pic(88, 16, picto_digit_small[(i / 100) % 10], 5, 8);
				pcf8531_pic(94, 16, picto_digit_small[(i / 10) % 10], 5, 8);
				pcf8531_pic(102, 16, picto_digit_small[(i / 1) % 10], 5, 8);
			}
		}
	}
	else
	{
		//-------------------------------------
		//	Ручное задание проценты
		//-------------------------------------
		i = ihmc_pos_t;
		pcf8531_pic(77, 16, picto_hand_pos, 39, 8);

		if (i >= 1000)
			pcf8531_pic(77, 16, picto_digit_small[(i / 1000) % 10], 5, 8);

		if (i >= 100)
			pcf8531_pic(83, 16, picto_digit_small[(i / 100) % 10], 5, 8);

		pcf8531_pic(89, 16, picto_digit_small[(i / 10) % 10], 5, 8);
		pcf8531_pic(97, 16, picto_digit_small[(i / 1) % 10], 5, 8);
	}

	//-------------------------------------
	//	Положение. Цифры
	//-------------------------------------
#ifdef AM800W
	pcf8531_clear(12, 5, 63, 17);
#else
	pcf8531_pic(12, 5, picto_pos_large, 63, 17);
#endif

	if (data_eeprom.flags.display_im == DISPLAY_IM_GRAD)
	{
		i = ihmc_g;
#ifndef AM800W
		pcf8531_pic(66, 4, picto_main_grad, 9, 18);
#endif
	}
	else
	{
		i = ihmc_pos_d;
	}

#ifdef AM800W
	if (i < 0)
	{
		i = -i;
		pcf8531_fill(11, 4, 64, 19);
		pcf8531_pic_invert(43 + 9, 17, picto_dot_large, 5, 5);
		if (i < 1000)
		{
			if (i >= 100)
				pcf8531_pic_invert(12 + 9, 5, picto_digit_large[(i / 100) % 10], 14, 17);
			pcf8531_pic_invert(27 + 9, 5, picto_digit_large[(i / 10) % 10], 14, 17);
			pcf8531_pic_invert(51 + 9, 5, picto_digit_large[(i / 1) % 10], 14, 17);
		}
		else
		{
			pcf8531_pic_invert(21 + 9, 5, picto_digit_large[(i / 1000) % 10], 14, 17);
			pcf8531_pic_invert(36 + 9, 5, picto_digit_large[(i / 100) % 10], 14, 17);
			pcf8531_pic_invert(51 + 9, 5, picto_digit_large[(i / 10) % 10], 14, 17);
		}
	}
	else
	{
		pcf8531_clear(11, 4, 55 + 9, 19);
		pcf8531_pic(43 + 9, 17, picto_dot_large, 5, 5);
		if (i < 1000)
		{
			if (i >= 100)
				pcf8531_pic(12 + 9, 5, picto_digit_large[(i / 100) % 10], 14, 17);
			pcf8531_pic(27 + 9, 5, picto_digit_large[(i / 10) % 10], 14, 17);
			pcf8531_pic(51 + 9, 5, picto_digit_large[(i / 1) % 10], 14, 17);
		}
		else
		{
			pcf8531_pic(21 + 9, 5, picto_digit_large[(i / 1000) % 10], 14, 17);
			pcf8531_pic(36 + 9, 5, picto_digit_large[(i / 100) % 10], 14, 17);
			pcf8531_pic(51 + 9, 5, picto_digit_large[(i / 10) % 10], 14, 17);
		}
	}
#else
	if (i < 0)
	{
		i = -i;
		pcf8531_fill(11, 4, 55, 19);
		pcf8531_pic_invert(43, 17, picto_dot_large, 5, 5);
		if (i < 1000)
		{
			if (i >= 100)
				pcf8531_pic_invert(12, 5, picto_digit_large[(i / 100) % 10], 14, 17);
			pcf8531_pic_invert(27, 5, picto_digit_large[(i / 10) % 10], 14, 17);
			pcf8531_pic_invert(51, 5, picto_digit_large[(i / 1) % 10], 14, 17);
		}
		else
		{
			pcf8531_pic_invert(21, 5, picto_digit_large[(i / 1000) % 10], 14, 17);
			pcf8531_pic_invert(36, 5, picto_digit_large[(i / 100) % 10], 14, 17);
			pcf8531_pic_invert(51, 5, picto_digit_large[(i / 10) % 10], 14, 17);
		}
	}
	else
	{
		pcf8531_clear(11, 4, 55, 19);
		pcf8531_pic(43, 17, picto_dot_large, 5, 5);
		if (i < 1000)
		{
			if (i >= 100)
				pcf8531_pic(12, 5, picto_digit_large[(i / 100) % 10], 14, 17);
			pcf8531_pic(27, 5, picto_digit_large[(i / 10) % 10], 14, 17);
			pcf8531_pic(51, 5, picto_digit_large[(i / 1) % 10], 14, 17);
		}
		else
		{
			pcf8531_pic(21, 5, picto_digit_large[(i / 1000) % 10], 14, 17);
			pcf8531_pic(36, 5, picto_digit_large[(i / 100) % 10], 14, 17);
			pcf8531_pic(51, 5, picto_digit_large[(i / 10) % 10], 14, 17);
		}
	}
#endif
	//-------------------------------------
	//	Пиктограммы
	//-------------------------------------
	if (mode == 0)
	{
		if (!show_ph)
		{
			if (hart_ma == -1)
				pcf8531_pic(77, 3, picto_auto, 39, 10);
			else
				pcf8531_pic(77, 3, picto_hart, 39, 10);
		}
		else
		{
			// ain_ma
			pcf8531_clear(77, 3, 39, 10);
			pcf8531_pic(81, 5, picto_ma_small_place, 35, 8);
			i = ain_ma;
			if (i < 0)
				i = 0;
			if (i >= 1000)
				pcf8531_pic(83, 5, picto_digit_small[(i / 1000) % 10], 5, 8);
			pcf8531_pic(89, 5, picto_digit_small[(i / 100) % 10], 5, 8);
			pcf8531_pic(97, 5, picto_digit_small[(i / 10) % 10], 5, 8);
		}
	}
	else
		pcf8531_pic(77, 3, picto_hand, 39, 10);

	if ((data_eeprom.flags.hart_enable == HART_ENABLE_ON) && ((to_hart >= TO_HART) || (disp_update_count & 1)))
		pcf8531_pic(1, 2, picto_hart_flag, 9, 9);
	else
		pcf8531_pic(1, 2, picto_flag, 9, 9);

	if (0)
		pcf8531_pic(1, 12, picto_limits_flag, 9, 9);
	else
		pcf8531_pic(1, 12, picto_flag, 9, 9);

	if ((data_eeprom.hard_pos_min != 0) || (data_eeprom.hard_pos_max != 0))
	{
		pcf8531_pic(118, 2, picto_press_flag, 9, 9);

		i = data_eeprom.hard_pos_min;
		if ((0 < i) && (i < 1000))
			pcf8531_fill(2, 28, i * 124 / 1000, 2);

		if (ihmc_pos_t < (0 + data_eeprom.hard_pos_min))
			pcf8531_pic(0, 23, picto_hard_min, 3, 5);

		i = data_eeprom.hard_pos_max;
		if ((0 < i) && (i < 1000))
			pcf8531_fill(126 - i * 124 / 1000, 28, i * 124 / 1000, 2);

		if (ihmc_pos_t > (1000 - data_eeprom.hard_pos_max))
			pcf8531_pic(125, 23, picto_hard_max, 3, 5);
	}

	if (event_flag)
		pcf8531_pic(118, 12, picto_status_err, 9, 9);
	else if (event_flag_last)
		pcf8531_pic(118, 12, picto_status_err_last, 9, 9);
	else
		pcf8531_pic(118, 12, picto_status_ok, 9, 9);
}

/// @brief Действия для экрана "Стартовый экран"
void selected_menu_view_start(void)
{
	tick(&to_menu);

	if (to_menu >= TO_MENU_START)
	{
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}
	else
	{
		
	}
}

/// @brief Выполнить отрисовку экрана "Стартовый экран".
void redraw_menu_view_start(void)
{
	BYTE str[32];

	pcf8531_pic(0, 0, picto_start, 128, 32);

	mstr_cpy(str, "AM800, вер. ПО ");
	mstr_add_int(str, SW_VER_MAJ);
	mstr_add(str, ".");
	mstr_add_int(str, SW_VER_MIN);
	mstr_add(str, ".");
	mstr_add_int(str, SW_VER_BUILD);
	pcf8531_str_to(0, 23, str, 127, font9);
}

/// @brief Действия для экрана "Основной экран"
void selected_menu_view_home(void)
{
	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		menu_go_to_view_and_clear(MENU_VIEW_INFO_1);
	}

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		menu_go_to_view_and_clear(MENU_VIEW_INFO_3);
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		menu_setup = 0;
		menu_go_to_view_and_set_pic(MENU_VIEW_SETUP, picto_menu);
	}

	if ((button_pressed == 5) && (to_button_pressed > 6000))
	{

		menu_go_to_view_and_set_pic(MENU_VIEW_IM, picto_menu);

		menu_setup = MENU_IM_AUTO;
		to_auto = 0;
		st_auto = ST_AUTO_IDLE;

		con_str("[MENU_AUTO_ENTER]");
		con_start();
	}
}

/// @brief Выполнить отрисовку экрана "Основной экран".
void redraw_menu_view_home(void)
{
	redraw_menu_view_main(0);
}

/// @brief Действия для экрана "Инфорежим-1"
void selected_menu_view_info1(void)
{
	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		menu_go_to_view_and_clear(MENU_VIEW_INFO_2);
	}

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}
}

/// @brief Выполнить отрисовку экрана "Инфорежим-1".
void redraw_menu_view_info1(void)
{
	int i;
	BYTE str[32];

	pcf8531_pic(0, 0, picto_info_1, 128, 32);

	str[0] = 0;
	i = uptime;
	if (i < 60)
	{
		mstr_add_int(str, i);
		mstr_add(str, "с");
	}
	else if (i < (60 * 60))
	{
		mstr_add_int(str, i / 60);
		mstr_add(str, "м");
		mstr_add_int(str, i % 60);
		mstr_add(str, "с");
	}
	else if (i < (24 * 60 * 60))
	{
		i /= 60;
		mstr_add_int(str, i / 60);
		mstr_add(str, "ч");
		mstr_add_int(str, i % 60);
		mstr_add(str, "м");
	}
	else
	{
		i /= 60 * 60;
		mstr_add_int(str, i / 24);
		mstr_add(str, "д");
		mstr_add_int(str, i % 24);
		mstr_add(str, "ч");
	}
	pcf8531_str_to(18, 2, str, 88, font9);

	str[0] = 0;
	i = worktime;
	if (i < 60)
	{
		mstr_add_int(str, i);
		mstr_add(str, "с");
	}
	else if (i < (60 * 60))
	{
		mstr_add_int(str, i / 60);
		mstr_add(str, "м");
		mstr_add_int(str, i % 60);
		mstr_add(str, "с");
	}
	else if (i < (24 * 60 * 60))
	{
		i /= 60;
		mstr_add_int(str, i / 60);
		mstr_add(str, "ч");
		mstr_add_int(str, i % 60);
		mstr_add(str, "м");
	}
	else
	{
		i /= 60 * 60;
		mstr_add_int(str, i / 24);
		mstr_add(str, "д");
		mstr_add_int(str, i % 24);
		mstr_add(str, "ч");
	}
	pcf8531_str_to(18, 12, str, 81, font9);

	str[0] = 0;
	i = ihmc_path / 200;
	mstr_add_int(str, i / 10);
	mstr_add(str, ".");
	mstr_add_int(str, i % 10);
	mstr_add(str, "ц");
	pcf8531_str_to(21, 22, str, 74, font9);

	i = tempeature;
	if (i < 0)
	{
		pcf8531_pic(104, 2, picto_minus_small, 3, 8);
		i = -i;
	}
	if (i >= 10)
		pcf8531_pic(108, 2, picto_digit_small[(i / 10) % 10], 5, 8);
	pcf8531_pic(114, 2, picto_digit_small[(i / 1) % 10], 5, 8);

	mstr_cpy(str, "ПО v");
	mstr_add_int(str, SW_VER_MAJ);
	mstr_add(str, ".");
	mstr_add_int(str, SW_VER_MIN);
	mstr_add(str, ".");
	mstr_add_int(str, SW_VER_BUILD);
	i = pcf8531_str_len(str, font9);
	pcf8531_str(128 - 1 - i, 12, str, font9);

	mstr_cpy(str, "sn:");

	if (!image.serial)
		mstr_add(str, " не задан");
	else
		mstr_add_int(str, image.serial);

	pcf8531_clear(64, 22, 63, 9);
	i = pcf8531_str_len(str, font9);
	pcf8531_str(128 - 1 - i, 22, str, font9);
}

/// @brief Действия для экрана "Инфорежим-2"
void selected_menu_view_info2(void)
{
	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		menu_go_to_view_and_clear(MENU_VIEW_INFO_3);
	}

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		menu_go_to_view_and_clear(MENU_VIEW_INFO_1);
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}
}

/// @brief Выполнить отрисовку экрана "Инфорежим-2".
void redraw_menu_view_info2(void)
{
	int i;
	BYTE str[32];

	pcf8531_pic(0, 0, picto_info_2, 128, 32);

	//-------------------------------------
	//	Положение ИМ проценты
	//-------------------------------------
	i = ihmc_pos_d;
	if (i < 1000)
	{
		if (i >= 100)
			pcf8531_pic(2, 2, picto_digit_small[(i / 100) % 10], 5, 8);

		pcf8531_pic(8, 2, picto_digit_small[(i / 10) % 10], 5, 8);
		pcf8531_pic(14, 2, picto_digit_small_dot, 1, 8);
		pcf8531_pic(16, 2, picto_digit_small[(i / 1) % 10], 5, 8);
	}
	else
	{
		pcf8531_pic(4, 2, picto_digit_small[(i / 1000) % 10], 5, 8);
		pcf8531_pic(10, 2, picto_digit_small[(i / 100) % 10], 5, 8);
		pcf8531_pic(16, 2, picto_digit_small[(i / 10) % 10], 5, 8);
	}

	//-------------------------------------
	//	Положение ИМ градусы
	//-------------------------------------
	i = ihmc_g;

	if (i < 0)
	{
		i = -i;
		pcf8531_fill(1, 12, 21, 10);
		if (i < 1000)
		{
			if (i >= 100)
				pcf8531_pic_invert(2, 13, picto_digit_small[(i / 100) % 10], 5, 8);

			pcf8531_pic_invert(8, 13, picto_digit_small[(i / 10) % 10], 5, 8);
			pcf8531_pic_invert(14, 13, picto_digit_small_dot, 1, 8);
			pcf8531_pic_invert(16, 13, picto_digit_small[(i / 1) % 10], 5, 8);
		}
		else
		{
			pcf8531_pic_invert(4, 13, picto_digit_small[(i / 1000) % 10], 5, 8);
			pcf8531_pic_invert(10, 13, picto_digit_small[(i / 100) % 10], 5, 8);
			pcf8531_pic_invert(16, 13, picto_digit_small[(i / 10) % 10], 5, 8);
		}
	}
	else
	{
		pcf8531_clear(1, 12, 21, 10);
		if (i < 1000)
		{
			if (i >= 100)
				pcf8531_pic(2, 13, picto_digit_small[(i / 100) % 10], 5, 8);

			pcf8531_pic(8, 13, picto_digit_small[(i / 10) % 10], 5, 8);
			pcf8531_pic(14, 13, picto_digit_small_dot, 1, 8);
			pcf8531_pic(16, 13, picto_digit_small[(i / 1) % 10], 5, 8);
		}
		else
		{
			pcf8531_pic(4, 13, picto_digit_small[(i / 1000) % 10], 5, 8);
			pcf8531_pic(10, 13, picto_digit_small[(i / 100) % 10], 5, 8);
			pcf8531_pic(16, 13, picto_digit_small[(i / 10) % 10], 5, 8);
		}
	}

	//-------------------------------------
	//	Входной сигнал мА
	//-------------------------------------
	i = ain_ma_s;

	if (i < 0)
		i = 0;

	if (i >= 1000)
		pcf8531_pic(51, 13, picto_digit_small[(i / 1000) % 10], 5, 8);

	pcf8531_pic(57, 13, picto_digit_small[(i / 100) % 10], 5, 8);
	pcf8531_pic(63, 13, picto_digit_small_dot, 1, 8);
	pcf8531_pic(65, 13, picto_digit_small[(i / 10) % 10], 5, 8);

	//-------------------------------------
	//	Положение ИМ-цель проценты
	//-------------------------------------
	i = ihmc_pos_t;

	if (i < 1000)
	{
		if (i >= 100)
			pcf8531_pic(57, 2, picto_digit_small[(i / 100) % 10], 5, 8);

		pcf8531_pic(63, 2, picto_digit_small[(i / 10) % 10], 5, 8);
		pcf8531_pic(69, 2, picto_digit_small_dot, 1, 8);
		pcf8531_pic(71, 2, picto_digit_small[(i / 1) % 10], 5, 8);
	}
	else
	{
		pcf8531_pic(59, 2, picto_digit_small[(i / 1000) % 10], 5, 8);
		pcf8531_pic(65, 2, picto_digit_small[(i / 100) % 10], 5, 8);
		pcf8531_pic(71, 2, picto_digit_small[(i / 10) % 10], 5, 8);
	}

	//-------------------------------------
	//	Выходной сигнал мА
	//-------------------------------------
	i = aout_ma;
	if (i < 0)
		i = 0;

	i = (i + 5) / 10;

	str[0] = 0;
	mstr_add(str, "ао:");

	if (data_eeprom.flags.aout_sig == OUT_SIG_NONE)
	{
		pcf8531_clear(87, 1, 40, 10);
		pcf8531_str(88, 2, str, font9);
		pcf8531_str(105, 2, "вык", font9);
	}
	else
	{
		if (aout_diag)
		{
			pcf8531_fill(87, 1, 40, 10);
			pcf8531_str_inverse(88, 2, str, font9);
			str[0] = 0;
			mstr_add_int(str, i);
			mstr_add(str, "мА");
			pcf8531_str_inverse(127 - pcf8531_str_len(str, font9), 2, str, font9);
		}
		else
		{
			pcf8531_clear(87, 1, 40, 10);
			pcf8531_str(88, 2, str, font9);
			str[0] = 0;
			mstr_add_int(str, i);
			mstr_add(str, "мА");
			pcf8531_str(127 - pcf8531_str_len(str, font9), 2, str, font9);
		}
	}

	//-------------------------------------
	//	Сигналы NAMUR
	//-------------------------------------
	for (i = 0; i < 3; i++)
	{
		if (((i == 0) && (data_eeprom.flags.dout1_disable == DOUT1_DISABLE)) || ((i == 1) && (data_eeprom.flags.dout2_sig == OUT_SIG_NONE)) || ((i == 2) && (data_eeprom.flags.dout3_sig == OUT_SIG_NONE)))
		{
			// none
			pcf8531_pic(104 + i * 8 + 1, 13, picto_minus_small, 3, 8);
		}
		else
		{
			if (namur_out[i])
			{
				pcf8531_pic(103 + i * 8, 12, picto_namur_invert, 7, 10);
				if (namur_diag[i])
					pcf8531_pic_invert(104 + i * 8, 13, picto_digit_small[10], 5, 8);
				else
					pcf8531_pic_invert(104 + i * 8, 13, picto_digit_small[i + 1], 5, 8);
			}
			else
			{
				if (namur_diag[i])
					pcf8531_pic(104 + i * 8, 13, picto_digit_small[10], 5, 8);
				else
					pcf8531_pic(104 + i * 8, 13, picto_digit_small[i + 1], 5, 8);
			}
		}
	}

	//-------------------------------------
	//	Положение ИМ аналог
	//-------------------------------------
	i = ihmc_pos;
	if (i != -1)
		pcf8531_pica(2 + i * 120 / 1000, 25, picto_pointer, 4, 7);

	//-------------------------------------
	//	Положение ИМ-цель аналог
	//-------------------------------------
	i = ihmc_pos_t;
	if (i != -1)
		pcf8531_pica(2 + i * 122 / 1000, 25, picto_pointer_t, 2, 7);
}

/// @brief Действия для экрана "Инфорежим-3"
void selected_menu_view_info3(void)
{
	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		menu_go_to_view_and_clear(MENU_VIEW_INFO_2);
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}
}

/// @brief Выполнить отрисовку экрана "Инфорежим-3".
void redraw_menu_view_info3(void)
{
	BYTE str[32];

	pcf8531_clear(0, 0, 128, 32);

	FromPosition(str, ihmc_pos);
	pcf8531_str(0, 0, str, Font_Single);

	FromPosition(str, ihmc_pos_t);
	pcf8531_str(40, 0, str, Font_Single);

	str[0] = 0;
	mstr_add_int(str, PWM_COIL);
	mstr_add(str, "Э/");
	mstr_add_int(str, info_coil_ofs);
	pcf8531_str(80, 0, str, Font_Single);

	FromFloat(str, roundf(info_hmc_d * 1000.0f),3);
	mstr_add(str, "Е");
	pcf8531_str(0, 8, str, Font_Single);

	if (to_power_fail < TO_POWER_FAIL)
		mstr_cpy(str, "БП сбой");
	else
		mstr_cpy(str, "БП норм");

	pcf8531_str(0, 16, str, Font_Single);

	FromFloat(str, roundf(info_coil * 1000.0f),3);
	mstr_add(str, "В");
	pcf8531_str(0, 24, str, Font_Single);

	FromFloat(str, roundf(info_hmc_d_pr * 1000.0f),3);
	mstr_add(str, "П");
	pcf8531_str(64, 8, str, Font_Single);

	FromFloat(str, roundf(info_hmc_d_int * 1000.0f),3);
	mstr_add(str, "И");
	pcf8531_str(64, 16, str, Font_Single);

	FromFloat(str, roundf(info_hmc_d_dif * 1000.0f),3);
	mstr_add(str, "Д");
	pcf8531_str(64, 24, str, Font_Single);
}

/// @brief Действия для экрана "Ручной режим"
void selected_menu_view_hand(void)
{
	
}

/// @brief Выполнить отрисовку экрана "Ручной режим".
void redraw_menu_view_hand(void)
{
	redraw_menu_view_main(1);
}

/// @brief Действия для экрана "Настройки"
void selected_menu_view_setup(void)
{
	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_setup < (MENU_SETUP_NUM - 1))
		{
			menu_setup++;
			to_menu = 0;
		}
		else
		{
			menu_setup = 0;
			to_menu = 0;
		}
	}

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_setup > 0)
		{
			menu_setup--;
			to_menu = 0;
		}
		else
		{
			to_menu = 0;
			menu_setup = MENU_SETUP_NUM - 1;
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_go_to_view_and_clear(MENU_VIEW_HOME);
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;

		switch (menu_setup)
		{
		case MENU_SETUP_HAND:
		{
		
			break;
		}
		case MENU_SETUP_IM:
		{
			menu_setup = 0;
			menu_subitem = 0;

			to_menu = 0;
			menu_view = MENU_VIEW_IM;
			break;
		}
		case MENU_SETUP_DISPLAY:
		case MENU_SETUP_HART:
		{
			menu_subitem = 0;
			menu_data.f.dw = data_eeprom.flags.dw;
			menu_go_to_view_and_clear(MENU_VIEW_SETUP_EDIT);
			break;
		}
		case MENU_SETUP_SIGNAL:
		{
			menu_setup = 0;
			menu_subitem = 0;
			load_value(menu_signal_value_item, menu_setup, menu_subitem);
			to_menu = 0;
			menu_view = MENU_VIEW_SIGNAL;
			break;
		}
		case MENU_SETUP_DIAG:
		{
			menu_setup = 0;
			menu_subitem = 0;
			to_menu = 0;
			menu_view = MENU_VIEW_DIAG;
			break;
		}
		case MENU_SETUP_LOG:
		{
			menu_setup = 0;
			menu_subitem = event_pos;
			if (event_num)
			{
				if (menu_subitem > 0)
					menu_subitem--;
				else
					menu_subitem = event_num - 1;
			}
			else
				menu_subitem = 0;
			to_menu = 0;
			menu_view = MENU_VIEW_LOG;
			break;
		}
		}
	}
}

/// @brief Выполнить отрисовку экрана "Настройки".
void redraw_menu_view_setup(void)
{
	pcf8531_pic(0, 0, picto_menu, 128, 32);
	pcf8531_str_to(
		(128 - pcf8531_str_len(menu_setup_items[menu_setup], font9)) / 2,
		16, menu_setup_items[menu_setup], 128, font9);
}

/// @brief Действия для экрана "Настройки - Редактирование"
void selected_menu_view_setup_edit(void)
{
	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_subitem_edit)
		{
			if (menu_setup == MENU_SETUP_DISPLAY)
			{
				switch (menu_subitem)
				{
				case MENU_DISPLAY_IM:
					menu_data.f.display_im = DISPLAY_IM_PROC;
					break;
				case MENU_DISPLAY_AIN:
					menu_data.f.display_ain = DISPLAY_AIN_PROC;
					break;
				case MENU_DISPLAY_CONTRAST:
					if (menu_data.f.display_contrast > 5)
						menu_data.f.display_contrast--;
					display_contrast = menu_data.f.display_contrast;
					break;
				case MENU_DISPLAY_ROTATE:
					menu_data.f.display_rotate = DISPLAY_ROTATE_OFF;
					break;
				case MENU_DISPLAY_FILTER:
					if (menu_data.f.display_filter < 7)
						menu_data.f.display_filter++;
					break;
				}
			}
			else if (menu_setup == MENU_SETUP_HART)
			{
				switch (menu_subitem)
				{
				case MENU_HART_ID:
					if (menu_data.f.hart_id > 0)
						menu_data.f.hart_id--;
					break;
				case MENU_HART_ENABLE:
					menu_data.f.hart_enable = HART_ENABLE_ON;
					break;
				case MENU_HART_PRIO:
					menu_data.f.hart_prio = HART_PRIO_AIN;
					break;
				}
			}
		}
		else
		{
			if (menu_setup == MENU_SETUP_DISPLAY)
			{
				if (menu_subitem < (MENU_DISPLAY_NUM - 1))
				{
					menu_subitem++;
					to_menu = 0;
				}
				else
				{
					menu_subitem = 0;
					to_menu = 0;
				}
			}
			else if (menu_setup == MENU_SETUP_HART)
			{
				if (menu_subitem < (MENU_HART_NUM - 1))
				{
					menu_subitem++;
					to_menu = 0;
				}
				else
				{
					menu_subitem = 0;
					to_menu = 0;
				}
			}
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_subitem_edit)
		{
			if (menu_setup == MENU_SETUP_DISPLAY)
			{
				switch (menu_subitem)
				{
				case MENU_DISPLAY_IM:
					menu_data.f.display_im = DISPLAY_IM_GRAD;
					break;
				case MENU_DISPLAY_AIN:
					menu_data.f.display_ain = DISPLAY_AIN_MA;
					break;
				case MENU_DISPLAY_CONTRAST:
					if (menu_data.f.display_contrast < 25)
						menu_data.f.display_contrast++;
					display_contrast = menu_data.f.display_contrast;
					break;
				case MENU_DISPLAY_ROTATE:
					menu_data.f.display_rotate = DISPLAY_ROTATE_ON;
					break;
				case MENU_DISPLAY_FILTER:
					if (menu_data.f.display_filter > 0)
						menu_data.f.display_filter--;
					break;
				}
			}
			else if (menu_setup == MENU_SETUP_HART)
			{
				switch (menu_subitem)
				{
				case MENU_HART_ID:
					if (menu_data.f.hart_id < 15)
						menu_data.f.hart_id++;
					break;
				case MENU_HART_ENABLE:
					menu_data.f.hart_enable = HART_ENABLE_OFF;
					break;
				case MENU_HART_PRIO:
					menu_data.f.hart_prio = HART_PRIO_HART;
					break;
				}
			}
		}
		else
		{
			if (menu_subitem > 0)
			{
				menu_subitem--;
				to_menu = 0;
			}
			else
			{
				switch (menu_setup)
				{
				case MENU_SETUP_DISPLAY:
					menu_subitem = MENU_DISPLAY_NUM - 1;
					break;
				case MENU_SETUP_HART:
					menu_subitem = MENU_HART_NUM - 1;
					break;
				}
			}
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		if (menu_subitem_edit)
		{
			display_contrast = -1;
			menu_data.f.dw = data_eeprom.flags.dw;
			to_menu = 0;
			menu_subitem_edit = 0;
		}
		else
		{
			display_contrast = -1;
			menu_go_to_view_and_clear(MENU_VIEW_SETUP);
		}
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		if (menu_subitem_edit)
		{
			eeprom_unlock();
			data_eeprom.flags.dw = menu_data.f.dw;
			eeprom_lock();
			to_menu = 0;
			menu_subitem_edit = 0;
			display_contrast = -1;
		}
		else
		{
			display_contrast = menu_data.f.display_contrast;
			to_menu = 0;
			menu_subitem_edit = 1;
		}
	}
}

/// @brief Выполнить отрисовку экрана "Настройки - Редактирование".
void redraw_menu_view_setup_edit(void)
{
	int i;
	BYTE str[32];
	BYTE const **menu_items;

	switch (menu_setup)
	{
	case MENU_SETUP_DISPLAY:
		menu_items = menu_display_items;
		break;
	case MENU_SETUP_HART:
		menu_items = menu_hart_items;
		break;
	case MENU_SETUP_SIGNAL:
		menu_items = menu_signal_items;
		break;
	}
	i = pcf8531_str_len(menu_items[menu_subitem], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2,
				menu_items[menu_subitem], font9);

	if (menu_setup == MENU_SETUP_DISPLAY)
	{
		switch (menu_subitem)
		{
		case MENU_DISPLAY_IM:
			if (menu_data.f.display_im == DISPLAY_IM_PROC)
			{
				// Проценты
				pcf8531_fill(7, 12, 56, 12);
				pcf8531_str_inverse(10, 13, "Проценты", font9);
				pcf8531_str(70, 13, "Градусы", font9);
			}
			else
			{
				// Градусы
				pcf8531_str(10, 13, "Проценты", font9);
				pcf8531_fill(67, 12, 50, 12);
				pcf8531_str_inverse(70, 13, "Градусы", font9);
			}
			break;

		case MENU_DISPLAY_AIN:
			if (menu_data.f.display_ain == DISPLAY_AIN_PROC)
			{
				// Проценты
				pcf8531_fill(7, 12, 56, 12);
				pcf8531_str_inverse(10, 13, "Проценты", font9);
				pcf8531_str(85, 13, "мА", font9);
			}
			else
			{
				// Градусы
				pcf8531_str(10, 13, "Проценты", font9);
				pcf8531_fill(67, 12, 50, 12);
				pcf8531_str_inverse(85, 13, "мА", font9);
			}
			break;

		case MENU_DISPLAY_CONTRAST:
			i = menu_data.f.display_contrast - 15;
			if ((i < -10) || (i > 10))
			{
				i = 0;
				menu_data.f.display_contrast = 15;
			}
			if (i < 0)
			{
				i = -i;
				str[0] = '-';
				if (i >= 10)
				{
					str[1] = i / 10 + '0';
					str[2] = i % 10 + '0';
					str[3] = 0;
				}
				else
				{
					str[1] = i % 10 + '0';
					str[2] = 0;
				}
			}
			else
			{
				if (i >= 10)
				{
					str[0] = i / 10 + '0';
					str[1] = i % 10 + '0';
					str[2] = 0;
				}
				else
				{
					str[0] = i % 10 + '0';
					str[1] = 0;
				}
			}
			i = pcf8531_str_len(str, font9);
			pcf8531_fill(44, 12, 40, 12);
			pcf8531_str_inverse((128 - i) / 2, 13, str, font9);
			break;

		case MENU_DISPLAY_ROTATE:
			if (menu_data.f.display_rotate == DISPLAY_ROTATE_OFF)
			{
				// Нормал
				pcf8531_fill(7, 12, 56, 12);
				pcf8531_str_inverse(16, 13, "Нормал", font9);
				pcf8531_str(71, 13, "Поворот", font9);
			}
			else
			{
				// Поворот
				pcf8531_str(16, 13, "Нормал", font9);
				pcf8531_fill(67, 12, 50, 12);
				pcf8531_str_inverse(71, 13, "Поворот", font9);
			}
			break;

		case MENU_DISPLAY_FILTER:
			i = pcf8531_str_len(menu_disp_filter_item[menu_data.f.display_filter], font9);
			pcf8531_str((128 - i) / 2, 13, menu_disp_filter_item[menu_data.f.display_filter], font9);
			break;
		}
	}
	else if (menu_setup == MENU_SETUP_HART)
	{
		switch (menu_subitem)
		{
		case MENU_HART_ID:
			i = menu_data.f.hart_id;
			if (i >= 10)
			{
				str[0] = i / 10 + '0';
				str[1] = i % 10 + '0';
				str[2] = 0;
			}
			else
			{
				str[0] = i % 10 + '0';
				str[1] = 0;
			}
			pcf8531_str(32, 13, "Адрес", font9);
			pcf8531_str(70, 13, str, font9);
			break;

		case MENU_HART_ENABLE:
			if (menu_data.f.hart_enable == HART_ENABLE_ON)
			{
				// On
				pcf8531_fill(7, 12, 56, 12);
				pcf8531_str_inverse(25, 13, "Вкл", font9);
				pcf8531_str(79, 13, "Выкл", font9);
			}
			else
			{
				// Off
				pcf8531_str(25, 13, "Вкл", font9);
				pcf8531_fill(67, 12, 50, 12);
				pcf8531_str_inverse(79, 13, "Выкл", font9);
			}
			break;

		case MENU_HART_PRIO:
			if (menu_data.f.hart_prio == HART_PRIO_AIN)
			{
				// AIN
				pcf8531_fill(7, 12, 56, 12);
				pcf8531_str_inverse(17, 13, "Аналог", font9);
				pcf8531_str(75, 13, "Модем", font9);
			}
			else
			{
				// HART
				pcf8531_str(17, 13, "Аналог", font9);
				pcf8531_fill(67, 12, 50, 12);
				pcf8531_str_inverse(75, 13, "Модем", font9);
			}
			break;
		}
	}
	if (!menu_subitem_edit)
		pcf8531_clear(0, 32 - 7, 128, 7);
}

/// @brief Действия для экрана "Сигнализация"
void selected_menu_view_signal(void)
{
	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_setup < (MENU_SIGNAL_NUM - 1))
		{
			menu_setup++;
			to_menu = 0;
		}
		else
		{
			menu_setup = 0;
			to_menu = 0;
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_setup > 0)
		{
			menu_setup--;
			to_menu = 0;
		}
		else
		{
			menu_setup = MENU_SIGNAL_NUM - 1;
			to_menu = 0;
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_go_to_view_and_clear(MENU_VIEW_SETUP);
		menu_setup = MENU_SETUP_SIGNAL;
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;

		switch (menu_setup)
		{
		case MENU_SIGNAL_AOUT:
		case MENU_SIGNAL_DOUT1:
		case MENU_SIGNAL_DOUT2:
		case MENU_SIGNAL_DOUT3:
			menu_subitem = 0;
			load_value(menu_signal_value_item, menu_setup, menu_subitem);
			menu_go_to_view_and_clear(MENU_VIEW_SIGNAL_EDIT);
			break;
		}
	}
}

/// @brief Выполнить отрисовку экрана "Сигнализация".
void redraw_menu_view_signal(void)
{
	int i;
	BYTE str[32];

	i = pcf8531_str_len(menu_signal_items[menu_setup], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2,
				menu_signal_items[menu_setup], font9);
	pcf8531_clear(0, 32 - 7, 128, 7);

	if (menu_setup == MENU_SIGNAL_AOUT)
	{
		pcf8531_str(2, 18, menu_out_sig_items[data_eeprom.flags.aout_sig], font9);
		FromPosition(str, data_eeprom.aout_pos_max);
		pcf8531_str(30, 13, str, font9); // 100.0%
		FromPosition(str, data_eeprom.aout_pos_min);
		pcf8531_str(30, 23, str, font9); // 0.0%
		to_ma(str, data_eeprom.aout_ma_max);
		pcf8531_str(80, 13, str, font9); // 20.0mA
		to_ma(str, data_eeprom.aout_ma_min);
		pcf8531_str(80, 23, str, font9); // 4.0mA
	}
	else if (menu_setup == MENU_SIGNAL_DOUT1)
	{
		if (data_eeprom.flags.dout1_disable == DOUT1_ENABLE)
		{
			// enable
			i = pcf8531_str_len("Включен", font9);
			pcf8531_str((128 - i) / 2, 18, "Включен", font9);
		}
		else
		{
			// disable
			i = pcf8531_str_len("Выключен", font9);
			pcf8531_str((128 - i) / 2, 18, "Выключен", font9);
		}
	}
	else if (menu_setup == MENU_SIGNAL_DOUT2)
	{
		pcf8531_str(2, 18, menu_out_sig_items[data_eeprom.flags.dout2_sig], font9);
		FromPosition(str, data_eeprom.dout2_pos);
		pcf8531_str(30, 13, str, font9); // 90.0%
		FromPosition(str, data_eeprom.dout2_gis);
		pcf8531_str(80, 13, str, font9); // 2.0%
		i = pcf8531_str_len(menu_dout_dir_items[data_eeprom.flags.dout2_dir], font9);
		pcf8531_str((128 - i) / 2, 23, menu_dout_dir_items[data_eeprom.flags.dout2_dir], font9);
	}
	else if (menu_setup == MENU_SIGNAL_DOUT3)
	{
		pcf8531_str(2, 18, menu_out_sig_items[data_eeprom.flags.dout3_sig], font9);
		FromPosition(str, data_eeprom.dout3_pos);
		pcf8531_str(30, 13, str, font9); // 20.0%
		FromPosition(str, data_eeprom.dout3_gis);
		pcf8531_str(80, 13, str, font9); // 3.0%
		i = pcf8531_str_len(menu_dout_dir_items[data_eeprom.flags.dout3_dir], font9);
		pcf8531_str((128 - i) / 2, 23, menu_dout_dir_items[data_eeprom.flags.dout3_dir], font9);
	}
}

/// @brief Действия для экрана "Сигнализация - Редактирование"
void selected_menu_view_signal_edit(void)
{
	int i;

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_subitem_edit)
		{
			if (menu_setup == MENU_SIGNAL_AOUT)
			{
				switch (menu_subitem)
				{
				case MENU_SIGNAL_AOUT_SIG:
					if (menu_data.f.aout_sig < OUT_SIG_DELTA)
						menu_data.f.aout_sig++;
					break;
				case MENU_SIGNAL_AOUT_UP:
				case MENU_SIGNAL_AOUT_DOWN:
				case MENU_SIGNAL_AOUT_MAX:
				case MENU_SIGNAL_AOUT_MIN:
					if (menu_data.i > min_value(menu_signal_value_item, menu_setup, menu_subitem))
						menu_data.i--;
					else
						menu_data.i = min_value(menu_signal_value_item, menu_setup, menu_subitem);
					break;
				}
			}
			else if (menu_setup == MENU_SIGNAL_DOUT1)
			{
				switch (menu_subitem)
				{
				case MENU_SIGNAL_DOUT_SIG:
					menu_data.f.dout1_disable = DOUT1_ENABLE;
					break;
				}
			}
			else if (menu_setup == MENU_SIGNAL_DOUT2)
			{
				switch (menu_subitem)
				{
				case MENU_SIGNAL_DOUT_SIG:
					if (menu_data.f.dout2_sig < OUT_SIG_DELTA)
						menu_data.f.dout2_sig++;
					break;
				case MENU_SIGNAL_DOUT_DIR:
					menu_data.f.dout2_dir = DOUT_DIR_UP;
					break;
				case MENU_SIGNAL_DOUT_VAL:
				case MENU_SIGNAL_DOUT_GIS:
					if (menu_data.i > min_value(menu_signal_value_item, menu_setup, menu_subitem))
						menu_data.i--;
					else
						menu_data.i = min_value(menu_signal_value_item, menu_setup, menu_subitem);
					break;
				}
			}
			else if (menu_setup == MENU_SIGNAL_DOUT3)
			{
				switch (menu_subitem)
				{
				case MENU_SIGNAL_DOUT_SIG:
					if (menu_data.f.dout3_sig < OUT_SIG_DELTA)
						menu_data.f.dout3_sig++;
					break;
				case MENU_SIGNAL_DOUT_DIR:
					menu_data.f.dout3_dir = DOUT_DIR_UP;
					break;
				case MENU_SIGNAL_DOUT_VAL:
				case MENU_SIGNAL_DOUT_GIS:
					if (menu_data.i > min_value(menu_signal_value_item, menu_setup, menu_subitem))
						menu_data.i--;
					else
						menu_data.i = min_value(menu_signal_value_item, menu_setup, menu_subitem);
					break;
				}
			}
		}
		else
		{
			if (menu_setup == MENU_SIGNAL_AOUT)
			{
				if (menu_subitem < (MENU_SIGNAL_AOUT_NUM - 1))
				{
					menu_subitem++;
					load_value(menu_signal_value_item, menu_setup, menu_subitem);
					to_menu = 0;
				}
				else
				{
					menu_subitem = 0;
					load_value(menu_signal_value_item, menu_setup, menu_subitem);
					to_menu = 0;
				}
			}
			else if ((menu_setup == MENU_SIGNAL_DOUT2) || (menu_setup == MENU_SIGNAL_DOUT3))
			{
				if (menu_subitem < (MENU_SIGNAL_DOUT_NUM - 1))
				{
					menu_subitem++;
					load_value(menu_signal_value_item, menu_setup, menu_subitem);
					to_menu = 0;
				}
				else
				{
					menu_subitem = 0;
					load_value(menu_signal_value_item, menu_setup, menu_subitem);
					to_menu = 0;
				}
			}
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_subitem_edit)
		{
			if (menu_setup == MENU_SIGNAL_AOUT)
			{
				switch (menu_subitem)
				{
				case MENU_SIGNAL_AOUT_SIG:
					if (menu_data.f.aout_sig > OUT_SIG_NONE)
						menu_data.f.aout_sig--;
					break;
				case MENU_SIGNAL_AOUT_UP:
				case MENU_SIGNAL_AOUT_DOWN:
				case MENU_SIGNAL_AOUT_MAX:
				case MENU_SIGNAL_AOUT_MIN:
					if (menu_data.i < max_value(menu_signal_value_item, menu_setup, menu_subitem))
						menu_data.i++;
					else
						menu_data.i = max_value(menu_signal_value_item, menu_setup, menu_subitem);
					break;
				}
			}
			else if (menu_setup == MENU_SIGNAL_DOUT1)
			{
				switch (menu_subitem)
				{
				case MENU_SIGNAL_DOUT_SIG:
					menu_data.f.dout1_disable = DOUT1_DISABLE;
					break;
				}
			}
			else if (menu_setup == MENU_SIGNAL_DOUT2)
			{
				switch (menu_subitem)
				{
				case MENU_SIGNAL_DOUT_SIG:
					if (menu_data.f.dout2_sig > OUT_SIG_NONE)
						menu_data.f.dout2_sig--;
					break;
				case MENU_SIGNAL_DOUT_DIR:
					menu_data.f.dout2_dir = DOUT_DIR_DOWN;
					break;
				case MENU_SIGNAL_DOUT_VAL:
				case MENU_SIGNAL_DOUT_GIS:
					if (menu_data.i < max_value(menu_signal_value_item, menu_setup, menu_subitem))
						menu_data.i++;
					else
						menu_data.i = max_value(menu_signal_value_item, menu_setup, menu_subitem);
					break;
				}
			}
			else if (menu_setup == MENU_SIGNAL_DOUT3)
			{
				switch (menu_subitem)
				{
				case MENU_SIGNAL_DOUT_SIG:
					if (menu_data.f.dout3_sig > OUT_SIG_NONE)
						menu_data.f.dout3_sig--;
					break;
				case MENU_SIGNAL_DOUT_DIR:
					menu_data.f.dout3_dir = DOUT_DIR_DOWN;
					break;
				case MENU_SIGNAL_DOUT_VAL:
				case MENU_SIGNAL_DOUT_GIS:
					if (menu_data.i < max_value(menu_signal_value_item, menu_setup, menu_subitem))
						menu_data.i++;
					else
						menu_data.i = max_value(menu_signal_value_item, menu_setup, menu_subitem);
					break;
				}
			}
		}
		else
		{
			if (menu_subitem > 0)
			{
				menu_subitem--;
				load_value(menu_signal_value_item, menu_setup, menu_subitem);
				to_menu = 0;
			}
			else
			{
				switch (menu_setup)
				{
				case MENU_SIGNAL_AOUT:
					menu_subitem = MENU_SIGNAL_AOUT_NUM - 1;
					break;
				case MENU_SIGNAL_DOUT2:
					menu_subitem = MENU_SIGNAL_DOUT_NUM - 1;
					break;
				case MENU_SIGNAL_DOUT3:
					menu_subitem = MENU_SIGNAL_DOUT_NUM - 1;
					break;
				}
				load_value(menu_signal_value_item, menu_setup, menu_subitem);
				to_menu = 0;
			}
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		if (menu_subitem_edit)
		{
			load_value(menu_signal_value_item, menu_setup, menu_subitem);
			to_menu = 0;
			menu_subitem_edit = 0;
		}
		else
		{
			menu_go_to_view_and_clear(MENU_VIEW_SIGNAL);
		}
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		if (menu_subitem_edit)
		{
			save_value(menu_signal_value_item, menu_setup, menu_subitem);
			to_menu = 0;
			menu_subitem_edit = 0;
		}
		else
		{
			load_value(menu_signal_value_item, menu_setup, menu_subitem);
			to_menu = 0;
			menu_subitem_edit = 1;
		}
	}

	if ((button_pressed == BUTTON_DOWN) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 5;
		else
			i = 50;

		if (min_value(menu_signal_value_item, menu_setup, menu_subitem) != max_value(menu_signal_value_item, menu_setup, menu_subitem))
		{
			if ((menu_data.i - i) > min_value(menu_signal_value_item, menu_setup, menu_subitem))
				menu_data.i -= i;
			else
				menu_data.i = min_value(menu_signal_value_item, menu_setup, menu_subitem);
		}
	}

	if ((button_pressed == BUTTON_UP) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 5;
		else
			i = 50;

		if (min_value(menu_signal_value_item, menu_setup, menu_subitem) != max_value(menu_signal_value_item, menu_setup, menu_subitem))
		{
			if ((menu_data.i + i) < max_value(menu_signal_value_item, menu_setup, menu_subitem))
				menu_data.i += i;
			else
				menu_data.i = max_value(menu_signal_value_item, menu_setup, menu_subitem);
		}
	}
}

/// @brief Выполнить отрисовку экрана "Сигнализация - Редактирование".
void redraw_menu_view_signal_edit(void)
{
	int i;
	BYTE str[32];
	BYTE const **menu_items;

	switch (menu_setup)
	{
	case MENU_SIGNAL_AOUT:
		menu_items = menu_signal_aout_items;
		break;
	case MENU_SIGNAL_DOUT1:
		menu_items = menu_signal_dout_items;
		break;
	case MENU_SIGNAL_DOUT2:
		menu_items = menu_signal_dout_items;
		break;
	case MENU_SIGNAL_DOUT3:
		menu_items = menu_signal_dout_items;
		break;
	}
	i = pcf8531_str_len(menu_items[menu_subitem], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2,
				menu_items[menu_subitem], font9);

	if (menu_setup == MENU_SIGNAL_AOUT)
	{
		switch (menu_subitem)
		{
		case MENU_SIGNAL_AOUT_SIG:
			i = pcf8531_str_len(menu_out_sig_des_items[menu_data.f.aout_sig], font9);
			pcf8531_str((128 - i) / 2, 14, menu_out_sig_des_items[menu_data.f.aout_sig], font9);
			break;
		case MENU_SIGNAL_AOUT_UP:
		case MENU_SIGNAL_AOUT_DOWN:
			FromPosition(str, menu_data.i);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		case MENU_SIGNAL_AOUT_MAX:
		case MENU_SIGNAL_AOUT_MIN:
			to_ma(str, menu_data.i);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		}
	}
	else if (menu_setup == MENU_SIGNAL_DOUT1)
	{
		if (menu_data.f.dout1_disable == DOUT1_ENABLE)
		{
			// enable
			pcf8531_fill(8, 12, 52, 12);
			i = pcf8531_str_len(menu_dout1_disable[0], font9);
			pcf8531_str_inverse(8 + (52 - i) / 2, 14, menu_dout1_disable[0], font9);
			i = pcf8531_str_len(menu_dout1_disable[1], font9);
			pcf8531_str(68 + (52 - i) / 2, 14, menu_dout1_disable[1], font9);
		}
		else
		{
			// disable
			i = pcf8531_str_len(menu_dout1_disable[0], font9);
			pcf8531_str(8 + (52 - i) / 2, 14, menu_dout1_disable[0], font9);
			pcf8531_fill(68, 12, 52, 12);
			i = pcf8531_str_len(menu_dout1_disable[1], font9);
			pcf8531_str_inverse(68 + (52 - i) / 2, 14, menu_dout1_disable[1], font9);
		}
	}
	else if (menu_setup == MENU_SIGNAL_DOUT2)
	{
		switch (menu_subitem)
		{
		case MENU_SIGNAL_DOUT_SIG:
			i = pcf8531_str_len(menu_out_sig_des_items[menu_data.f.dout2_sig], font9);
			pcf8531_str((128 - i) / 2, 14, menu_out_sig_des_items[menu_data.f.dout2_sig], font9);
			break;
		case MENU_SIGNAL_DOUT_DIR:
			if (menu_data.f.dout2_dir == DOUT_DIR_UP)
			{
				// UP
				pcf8531_fill(7, 12, 56, 12);
				pcf8531_str_inverse(17, 13, menu_dout_dir_items[0], font9);
				pcf8531_str(75, 13, menu_dout_dir_items[1], font9);
			}
			else
			{
				// DOWN
				pcf8531_str(17, 13, menu_dout_dir_items[0], font9);
				pcf8531_fill(67, 12, 50, 12);
				pcf8531_str_inverse(75, 13, menu_dout_dir_items[1], font9);
			}
			break;
		case MENU_SIGNAL_DOUT_VAL:
		case MENU_SIGNAL_DOUT_GIS:
			FromPosition(str, menu_data.i);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		}
	}
	else if (menu_setup == MENU_SIGNAL_DOUT3)
	{
		switch (menu_subitem)
		{
		case MENU_SIGNAL_DOUT_SIG:
			i = pcf8531_str_len(menu_out_sig_des_items[menu_data.f.dout3_sig], font9);
			pcf8531_str((128 - i) / 2, 14, menu_out_sig_des_items[menu_data.f.dout3_sig], font9);
			break;
		case MENU_SIGNAL_DOUT_DIR:
			if (menu_data.f.dout3_dir == DOUT_DIR_UP)
			{
				// UP
				pcf8531_fill(7, 12, 56, 12);
				pcf8531_str_inverse(17, 13, menu_dout_dir_items[0], font9);
				pcf8531_str(75, 13, menu_dout_dir_items[1], font9);
			}
			else
			{
				// DOWN
				pcf8531_str(17, 13, menu_dout_dir_items[0], font9);
				pcf8531_fill(67, 12, 50, 12);
				pcf8531_str_inverse(75, 13, menu_dout_dir_items[1], font9);
			}
			break;
		case MENU_SIGNAL_DOUT_VAL:
		case MENU_SIGNAL_DOUT_GIS:
			FromPosition(str, menu_data.i);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		}
	}

	if (!menu_subitem_edit)
		pcf8531_clear(0, 32 - 7, 128, 7);
}

/// @brief Действия для экрана "Настройка ИМ"
void selected_menu_view_im(void)
{
	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_setup < (MENU_IM_NUM - 1))
			menu_setup++;
		else
			menu_setup = 0;
		to_menu = 0;
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_setup > 0)
			menu_setup--;
		else
			menu_setup = MENU_IM_NUM - 1;
		to_menu = 0;
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_setup = MENU_SETUP_IM;
		menu_go_to_view_and_clear(MENU_VIEW_SETUP);
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;

		switch (menu_setup)
		{
		case MENU_IM_CALIB:
			menu_subitem = 0;
			menu_go_to_view_and_clear(MENU_VIEW_IM_EDIT);
			break;
		case MENU_IM_AUTO:
			menu_subitem = 0;
			to_auto = 0;
			st_auto = ST_AUTO_SELECT;
			menu_go_to_view_and_clear(MENU_VIEW_AUTO);
			break;
		case MENU_IM_HAND:
			menu_kp = roundf(data_eeprom.kp * 1000.0f);
			menu_ki = roundf(data_eeprom.ki * 1000.0f);
			menu_kd = roundf(data_eeprom.kd * 1000.0f);
			menu_subitem = 0;
			menu_go_to_view_and_clear(MENU_VIEW_IM_EDIT);
			break;
		case MENU_IM_T_LP:
			menu_subitem = 0;
			menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_up * FADC));
			menu_go_to_view_and_clear(MENU_VIEW_IM_EDIT);
			break;
		case MENU_IM_HAR:
			menu_data.f = data_eeprom.flags;
			if (menu_data.f.im_ph >= 11)
				menu_data.f.im_ph = 5;
			menu_subitem = 0;
			menu_go_to_view_and_clear(MENU_VIEW_IM_EDIT);
			break;
		case MENU_IM_HARD:
		case MENU_IM_DIAP:
			menu_subitem = 0;
			load_value(menu_im_hand_value_item, menu_setup, menu_subitem);
			menu_go_to_view_and_clear(MENU_VIEW_IM_EDIT);
			break;
		case MENU_IM_LP:
			menu_subitem = 0;
			menu_data.i = data_eeprom.lpf_a_in * 100.0f;

			if (menu_data.i < 0)
				menu_data.i = 0;
			if (menu_data.i > 100)
				menu_data.i = 100;

			menu_subitem_edit = 1;
			menu_go_to_view_and_clear(MENU_VIEW_IM_EDIT);
			break;

		case MENU_IM_LINEAR:
			menu_subitem = 0;

			if (data_eeprom.linear_en > 0)
				menu_data.i = 1;
			else
				menu_data.i = 0;

			menu_subitem_edit = 1;
			menu_go_to_view_and_clear(MENU_VIEW_IM_EDIT);
			break;

		case MENU_IM_RESET:
			menu_subitem = 0;
			menu_subitem_edit = 1;
			menu_data.i = 0;
			menu_go_to_view_and_clear(MENU_VIEW_IM_EDIT);
			break;
		}
	}
}

/// @brief Выполнить отрисовку экрана "Настройка ИМ".
void redraw_menu_view_im(void)
{
	int i;
	BYTE str[32];

	i = pcf8531_str_len(menu_im_items[menu_setup], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2,
				menu_im_items[menu_setup], font9);
	pcf8531_clear(0, 32 - 7, 128, 7);

	switch (menu_setup)
	{
	case MENU_IM_CALIB:
		i = data_eeprom.hmc_lim_0;
		str[0] = 0;
		mstr_add_int(str, i / 10);
		mstr_add(str, ".");
		mstr_add_int(str, i % 10);
		mstr_add(str, "% .. ");

		i = data_eeprom.hmc_lim_100;
		mstr_add_int(str, i / 10);
		mstr_add(str, ".");
		mstr_add_int(str, i % 10);
		mstr_add(str, "%");

		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
		break;

	case MENU_IM_AUTO:
		// disp im_auto
		i = pcf8531_str_len(menu_im_auto_items[0], font9);
		pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[0], font9);
		i = pcf8531_str_len(menu_im_auto_items[1], font9);
		pcf8531_str((128 - i) / 2, 24, menu_im_auto_items[1], font9);
		break;

	case MENU_IM_HAND:
		mstr_cpy(str, "Кп");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(0 + (42 - i) / 2, 14, str, font9);
		FromFloat(str, roundf(data_eeprom.kp * 1000.0f),3);
		i = pcf8531_str_len(str, font9);
		pcf8531_str(0 + (42 - i) / 2, 24, str, font9);

		mstr_cpy(str, "Ки");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(43 + (42 - i) / 2, 14, str, font9);
		FromFloat(str, roundf(data_eeprom.ki * 1000.0f),3);
		i = pcf8531_str_len(str, font9);
		pcf8531_str(43 + (42 - i) / 2, 24, str, font9);

		mstr_cpy(str, "Кд");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(86 + (42 - i) / 2, 14, str, font9);
		FromFloat(str, roundf(data_eeprom.kd * 1000.0f),3);
		i = pcf8531_str_len(str, font9);
		pcf8531_str(86 + (42 - i) / 2, 24, str, font9);
		break;

	case MENU_IM_T_LP:
		mstr_cpy(str, "Вверх");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(0 + (64 - i) / 2, 14, str, font9);
		FromFloat(str, roundf(T_LP_M * 10.0f / (data_eeprom.t_lp_up * FADC)),1);
		mstr_add(str, "c");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(0 + (64 - i) / 2, 24, str, font9);
		mstr_cpy(str, "Вниз");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(65 + (64 - i) / 2, 14, str, font9);
		FromFloat(str, roundf(T_LP_M * 10.0f / (data_eeprom.t_lp_down * FADC)),1);
		mstr_add(str, "c");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(65 + (64 - i) / 2, 24, str, font9);
		break;

	case MENU_IM_HAR:
		if (data_eeprom.flags.im_ph < 11)
		{
			pcf8531_pic(2, 12, picto_ph[data_eeprom.flags.im_ph], 27, 19);
			pcf8531_clear(30, 12, 128 - 20, 9);
			i = pcf8531_str_len(menu_im_ph_item[data_eeprom.flags.im_ph], font9);
			pcf8531_str(30 + (128 - 30 - i) / 2, 12, menu_im_ph_item[data_eeprom.flags.im_ph], font9);
		}
		else
		{
			pcf8531_pic(2, 12, picto_ph[5], 27, 19);
			pcf8531_clear(30, 12, 128 - 30, 9);
			i = pcf8531_str_len(menu_im_ph_item[5], font9);
			pcf8531_str(30 + (128 - 30 - i) / 2, 12, menu_im_ph_item[5], font9);
		}

		mstr_cpy(str, "Расход ");
		if (data_eeprom.coil_pwm_limit == 0)
			mstr_add(str, "не огр.");
		else
		{
			mstr_add(str, "огр. ");
			mstr_add_int(str, data_eeprom.coil_pwm_limit);
			mstr_add(str, "%");
		}
		i = pcf8531_str_len(str, font9);
		pcf8531_str(30 + (128 - 30 - i) / 2, 22, str, font9);
		break;

	case MENU_IM_HARD:
		pcf8531_pic(14, 14, picto_hard_im, 100, 8);
		if (data_eeprom.hard_pos_min)
		{
			str[0] = 0;
			mstr_add_int(str, data_eeprom.hard_pos_min / 10);
			mstr_add(str, ".");
			mstr_add_int(str, data_eeprom.hard_pos_min % 10);
			mstr_add(str, "%");
		}
		else
		{
			str[0] = 0;
			mstr_add(str, "выкл.");
		}
		pcf8531_str(14, 24, str, font9);
		if (data_eeprom.hard_pos_max)
		{
			str[0] = 0;
			mstr_add_int(str, data_eeprom.hard_pos_max / 10);
			mstr_add(str, ".");
			mstr_add_int(str, data_eeprom.hard_pos_max % 10);
			mstr_add(str, "%");
		}
		else
		{
			str[0] = 0;
			mstr_add(str, "выкл.");
		}
		i = pcf8531_str_len(str, font9);
		pcf8531_str(128 - 1 - 14 - i, 24, str, font9);

		pcf8531_fill(14, 14, data_eeprom.hard_pos_min / 10, 7);
		pcf8531_fill(113 - data_eeprom.hard_pos_max / 10, 14, data_eeprom.hard_pos_max / 10, 7);
		break;

	case MENU_IM_DIAP:
		str[0] = 0;
		mstr_add_int(str, data_eeprom.ain_ma_t_min / 100);
		mstr_add(str, ".");
		mstr_add_int2(str, data_eeprom.ain_ma_t_min % 100);
		mstr_add(str, " мА");
		i = pcf8531_str_len(str, font9);
		pcf8531_str((64 - i) / 2, 24, str, font9);

		str[0] = 0;
		mstr_add(str, "мин");
		i = pcf8531_str_len(str, font9);
		pcf8531_str((64 - i) / 2, 14, str, font9);

		str[0] = 0;
		mstr_add_int(str, data_eeprom.ain_ma_t_max / 100);
		mstr_add(str, ".");
		mstr_add_int2(str, data_eeprom.ain_ma_t_max % 100);
		mstr_add(str, " мА");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(64 + (64 - i) / 2, 24, str, font9);

		str[0] = 0;
		mstr_add(str, "макс");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(64 + (64 - i) / 2, 14, str, font9);
		break;

	case MENU_IM_LINEAR:
		str[0] = 0;
		if (data_eeprom.linear_en > 0)
			mstr_add(str, "Включена");
		else
			mstr_add(str, "Выключена");
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
		break;

	case MENU_IM_LP:
		FromFloat(str, data_eeprom.lpf_a_in * 1000.0f,3);
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
		break;

	case MENU_IM_RESET:
		mstr_cpy(str, "пользов. настроек");
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
		break;
	}
}

/// @brief Действия для экрана "Настройка ИМ - Редактирование"
void selected_menu_view_im_edit(void)
{
	int i;

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_subitem_edit)
		{
			if (menu_setup == MENU_IM_CALIB)
			{
				switch (menu_subitem)
				{
				case MENU_IM_CALIB_TEST:
					menu_calib_pwm = MENU_CALIB_PWM_0;
					break;
				case MENU_IM_CALIB_RST:
					menu_data.i = 1;
					break;
				}
			}
			else if (menu_setup == MENU_IM_HAND)
			{
				if (menu_subitem == MENU_IM_HAND_INV)
				{
					menu_data.f.coil_inverse = 0;
				}
				else if (menu_subitem == MENU_IM_HAND_SI_OFF)
				{
					if (menu_data.i > min_value(menu_im_hand_value_item, menu_setup, menu_subitem))
						menu_data.i--;
					else
						menu_data.i = min_value(menu_im_hand_value_item, menu_setup, menu_subitem);
				}
				else if (min_value(menu_im_hand_value_item, menu_setup, menu_subitem) != max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
				{
					if (get_value(menu_im_hand_value_item, menu_setup, menu_subitem) > min_value(menu_im_hand_value_item, menu_setup, menu_subitem))
						set_value(menu_im_hand_value_item, menu_setup, menu_subitem,
								  get_value(menu_im_hand_value_item, menu_setup, menu_subitem) - 1);
					else
						set_value(menu_im_hand_value_item, menu_setup, menu_subitem,
								  min_value(menu_im_hand_value_item, menu_setup, menu_subitem));
				}
			}
			else if (menu_setup == MENU_IM_T_LP)
			{
				if ((menu_subitem == MENU_IM_T_LP_UP) || (menu_subitem == MENU_IM_T_LP_DOWN))
				{
					if (menu_data.i > 1)
						menu_data.i--;
					else
						menu_data.i = 1;
				}
			}
			else if (menu_setup == MENU_IM_HAR)
			{
				if (menu_subitem == MENU_IM_HAR_PER)
				{
					if (menu_data.f.im_ph < 10)
						menu_data.f.im_ph++;
					else
						menu_data.f.im_ph = 10;
				}
				else
				{
					if (menu_data.i > min_value(menu_im_hand_value_item, menu_setup, menu_subitem))
						menu_data.i--;
					else
						menu_data.i = min_value(menu_im_hand_value_item, menu_setup, menu_subitem);
				}
			}
			else if ((menu_setup == MENU_IM_HARD) || (menu_setup == MENU_IM_DIAP) || (menu_setup == MENU_IM_LP))
			{
				if (menu_data.i > min_value(menu_im_hand_value_item, menu_setup, menu_subitem))
					menu_data.i--;
				else
					menu_data.i = min_value(menu_im_hand_value_item, menu_setup, menu_subitem);
			}
			else if (menu_setup == MENU_IM_RESET || menu_setup == MENU_IM_LINEAR)
			{
				if (menu_data.i > 0)
					menu_data.i = 0;
				else
					menu_data.i = 1;
			}
		}
		else
		{
			if (menu_setup == MENU_IM_CALIB)
			{
				if (menu_subitem < (MENU_IM_CALIB_NUM - 1))
					menu_subitem++;
				else
					menu_subitem = 0;
			}
			else if (menu_setup == MENU_IM_HAND)
			{
				if (menu_subitem < (MENU_IM_HAND_NUM - 1))
					menu_subitem++;
				else
					menu_subitem = 0;
			}
			else if (menu_setup == MENU_IM_T_LP)
			{
				if (menu_subitem < (MENU_IM_T_LP_NUM - 1))
					menu_subitem++;
				else
					menu_subitem = 0;
			}
			else if (menu_setup == MENU_IM_HAR)
			{
				if (menu_subitem < (MENU_IM_HAR_NUM - 1))
					menu_subitem++;
				else
					menu_subitem = 0;
			}
			else if (menu_setup == MENU_IM_HARD)
			{
				if (menu_subitem < (MENU_IM_HARD_NUM - 1))
					menu_subitem++;
				else
					menu_subitem = 0;
			}
			else if (menu_setup == MENU_IM_DIAP)
			{
				if (menu_subitem < (MENU_IM_DIAP_NUM - 1))
					menu_subitem++;
				else
					menu_subitem = 0;
			}

			if ((menu_setup == MENU_IM_HAR) && (menu_subitem == MENU_IM_HAR_PER))
			{
				menu_data.f = data_eeprom.flags;
				if (menu_data.f.im_ph >= 11)
					menu_data.f.im_ph = 5;
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_UP))
			{
				menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_up * FADC));
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_DOWN))
			{
				menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_down * FADC));
			}
			else if ((menu_setup == MENU_IM_HAND) && (menu_subitem == MENU_IM_HAND_SI_OFF))
			{
				menu_data.i = roundf(data_eeprom.si_off * 10.0f);
			}
			else
				load_value(menu_im_hand_value_item, menu_setup, menu_subitem);

			to_menu = 0;
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_subitem_edit)
		{
			if (menu_setup == MENU_IM_CALIB)
			{
				switch (menu_subitem)
				{
				case MENU_IM_CALIB_TEST:
					menu_calib_pwm = MENU_CALIB_PWM_100;
					break;
				case MENU_IM_CALIB_RST:
					menu_data.i = 0;
					break;
				}
			}
			else if (menu_setup == MENU_IM_HAND)
			{
				if (menu_subitem == MENU_IM_HAND_INV)
				{
					menu_data.f.coil_inverse = 1;
				}
				else if (menu_subitem == MENU_IM_HAND_SI_OFF)
				{
					if (menu_data.i < max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
						menu_data.i++;
					else
						menu_data.i = max_value(menu_im_hand_value_item, menu_setup, menu_subitem);
				}
				else if (min_value(menu_im_hand_value_item, menu_setup, menu_subitem) != max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
				{
					if (get_value(menu_im_hand_value_item, menu_setup, menu_subitem) < max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
						set_value(menu_im_hand_value_item, menu_setup, menu_subitem,
								  get_value(menu_im_hand_value_item, menu_setup, menu_subitem) + 1);
					else
						set_value(menu_im_hand_value_item, menu_setup, menu_subitem,
								  max_value(menu_im_hand_value_item, menu_setup, menu_subitem));
				}
			}
			else if (menu_setup == MENU_IM_T_LP)
			{
				if (menu_data.i < 70)
					menu_data.i++;
				else
					menu_data.i = 70;
			}
			else if (menu_setup == MENU_IM_HAR)
			{
				if (menu_subitem == MENU_IM_HAR_PER)
				{
					if (menu_data.f.im_ph > 0)
						menu_data.f.im_ph--;
					else
						menu_data.f.im_ph = 0;
				}
				else
				{
					if (menu_data.i < max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
						menu_data.i++;
					else
						menu_data.i = max_value(menu_im_hand_value_item, menu_setup, menu_subitem);
				}
			}
			else if ((menu_setup == MENU_IM_HARD) || (menu_setup == MENU_IM_DIAP) || (menu_setup == MENU_IM_LP))
			{
				if (menu_data.i < max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
					menu_data.i++;
				else
					menu_data.i = max_value(menu_im_hand_value_item, menu_setup, menu_subitem);
			}
			else if (menu_setup == MENU_IM_RESET || menu_setup == MENU_IM_LINEAR)
			{
				if (menu_data.i > 0)
					menu_data.i = 0;
				else
					menu_data.i = 1;
			}
		}
		else
		{
			if (menu_subitem > 0)
				menu_subitem--;
			else
			{
				switch (menu_setup)
				{
				case MENU_IM_CALIB:
					menu_subitem = MENU_IM_CALIB_NUM - 1;
					break;
				case MENU_IM_HAND:
					menu_subitem = MENU_IM_HAND_NUM - 1;
					break;
				case MENU_IM_T_LP:
					menu_subitem = MENU_IM_T_LP_NUM - 1;
					break;
				case MENU_IM_HAR:
					menu_subitem = MENU_IM_HAR_NUM - 1;
					break;
				case MENU_IM_HARD:
					menu_subitem = MENU_IM_HARD_NUM - 1;
					break;
				case MENU_IM_DIAP:
					menu_subitem = MENU_IM_DIAP_NUM - 1;
					break;
				}
			}
			if ((menu_setup == MENU_IM_HAR) && (menu_subitem == MENU_IM_HAR_PER))
			{
				menu_data.f = data_eeprom.flags;
				if (menu_data.f.im_ph >= 11)
					menu_data.f.im_ph = 5;
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_UP))
			{
				menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_up * FADC));
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_DOWN))
			{
				menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_down * FADC));
			}
			else if ((menu_setup == MENU_IM_HAND) && (menu_subitem == MENU_IM_HAND_SI_OFF))
			{
				menu_data.i = roundf(data_eeprom.si_off * 10.0f);
			}
			else
				load_value(menu_im_hand_value_item, menu_setup, menu_subitem);
			to_menu = 0;
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_calib_pwm = MENU_CALIB_PWM_NONE;
		if (menu_subitem_edit)
		{
			to_menu = 0;
			menu_subitem_edit = 0;
			if ((menu_setup == MENU_IM_HAR) && (menu_subitem == MENU_IM_HAR_PER))
			{
				menu_data.f = data_eeprom.flags;
				if (menu_data.f.im_ph >= 11)
					menu_data.f.im_ph = 5;
			}
			else if (menu_setup == MENU_IM_LP)
			{
				load_value(menu_im_hand_value_item, menu_setup, menu_subitem);
				menu_data.i = data_eeprom.lpf_a_in * 100.0f;

				if (menu_data.i < 0)
					menu_data.i = 0;
				if (menu_data.i > 100)
					menu_data.i = 100;

				menu_go_to_view_and_clear(MENU_VIEW_IM);
			}
			else if (menu_setup == MENU_IM_LINEAR)
			{
				menu_go_to_view_and_clear(MENU_VIEW_IM);
			}
			else if (menu_setup == MENU_IM_RESET)
			{
				menu_go_to_view_and_clear(MENU_VIEW_IM);
			}
			// сохранение ПИД-коэффициентов
			else if ((menu_setup == MENU_IM_HAND) && ((menu_subitem == MENU_IM_HAND_KP) || (menu_subitem == MENU_IM_HAND_KI) || (menu_subitem == MENU_IM_HAND_KD)))
			{
				save_value(menu_im_hand_value_item, menu_setup, menu_subitem);
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_UP))
			{
				menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_up * FADC));
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_DOWN))
			{
				menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_down * FADC));
			}
			else if ((menu_setup == MENU_IM_HAND) && (menu_subitem == MENU_IM_HAND_SI_OFF))
			{
				menu_data.i = roundf(data_eeprom.si_off * 10.0f);
			}
			else
				load_value(menu_im_hand_value_item, menu_setup, menu_subitem);
		}
		else
		{
			menu_go_to_view_and_clear(MENU_VIEW_IM);
		}
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		if (menu_subitem_edit)
		{
			to_menu = 0;
			menu_subitem_edit = 0;
			if ((menu_setup == MENU_IM_CALIB) && (menu_subitem == MENU_IM_CALIB_TEST))
			{
				menu_subitem_edit = 1;
				menu_calib_pwm = MENU_CALIB_PWM_50;
			}
			else if ((menu_setup == MENU_IM_CALIB) && (menu_subitem == MENU_IM_CALIB_MIN))
			{
				eeprom_unlock();
				data_eeprom.hmc_lim_0 = get_hmc_pos();
				eeprom_lock();
			}
			else if ((menu_setup == MENU_IM_CALIB) && (menu_subitem == MENU_IM_CALIB_MAX))
			{
				eeprom_unlock();
				data_eeprom.hmc_lim_100 = get_hmc_pos();
				eeprom_lock();
			}
			else if ((menu_setup == MENU_IM_CALIB) && (menu_subitem == MENU_IM_CALIB_RST))
			{
				if (menu_data.i)
					flag_calib_reset = 1;
			}
			else if ((menu_setup == MENU_IM_HAND) && (menu_subitem == MENU_IM_HAND_SAVE))
			{
				con_str("SAVE PID coeff\n\r");
				con_start();

				eeprom_unlock();
				data_eeprom.kp = 0.001f * menu_kp;
				data_eeprom.ki = 0.001f * menu_ki;
				data_eeprom.kd = 0.001f * menu_kd;
				eeprom_lock();
			}
			else if ((menu_setup == MENU_IM_HAR) && (menu_subitem == MENU_IM_HAR_PER))
			{
				eeprom_unlock();
				data_eeprom.flags = menu_data.f;
				eeprom_lock();
			}
			else if (menu_setup == MENU_IM_LP)
			{
				eeprom_unlock();
				data_eeprom.lpf_a_in = menu_data.i * 0.01f;
				eeprom_lock();
				menu_go_to_view_and_clear(MENU_VIEW_IM);
			}
			else if (menu_setup == MENU_IM_LINEAR)
			{
				eeprom_unlock();
				data_eeprom.linear_en = menu_data.i;
				eeprom_lock();
				menu_go_to_view_and_clear(MENU_VIEW_IM);
			}
			else if (menu_setup == MENU_IM_RESET)
			{
				if (menu_data.i)
					flag_im_reset = 1; // HERE Запуск сброса настроек

				menu_go_to_view_and_clear(MENU_VIEW_IM);
			}
			// сохранение ПИД-коэффициентов
			else if ((menu_setup == MENU_IM_HAND) && ((menu_subitem == MENU_IM_HAND_KP) || (menu_subitem == MENU_IM_HAND_KI) || (menu_subitem == MENU_IM_HAND_KD)))
			{
				load_value(menu_im_hand_value_item, menu_setup, menu_subitem);
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_UP))
			{
				eeprom_unlock();
				data_eeprom.t_lp_up = T_LP_M * 2.0f / ((float)(menu_data.i * FADC));
				eeprom_lock();
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_DOWN))
			{
				eeprom_unlock();
				data_eeprom.t_lp_down = T_LP_M * 2.0f / ((float)(menu_data.i * FADC));
				eeprom_lock();
			}
			else if ((menu_setup == MENU_IM_HAND) && (menu_subitem == MENU_IM_HAND_SI_OFF))
			{
				eeprom_unlock();
				data_eeprom.si_off = ((float)(menu_data.i)) * 0.1f;
				eeprom_lock();
			}
			else
			{
				save_value(menu_im_hand_value_item, menu_setup, menu_subitem);
			}
		}
		else
		{
			if ((menu_setup == MENU_IM_HAR) && (menu_subitem == MENU_IM_HAR_PER))
			{
				menu_data.f = data_eeprom.flags;
				if (menu_data.f.im_ph >= 11)
					menu_data.f.im_ph = 5;
			}
			else if ((menu_setup == MENU_IM_CALIB) && (menu_subitem == MENU_IM_CALIB_RST))
			{
				menu_data.i = 0;
			}
			else if (menu_setup == MENU_IM_RESET)
			{
				menu_data.i = 0;
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_UP))
			{
				menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_up * FADC));
			}
			else if ((menu_setup == MENU_IM_T_LP) && (menu_subitem == MENU_IM_T_LP_DOWN))
			{
				menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_down * FADC));
			}
			else if ((menu_setup == MENU_IM_HAND) && (menu_subitem == MENU_IM_HAND_SI_OFF))
			{
				menu_data.i = roundf(data_eeprom.si_off * 10.0f);
			}
			else
				load_value(menu_im_hand_value_item, menu_setup, menu_subitem);
			to_menu = 0;
			menu_subitem_edit = 1;
		}
	}

	if ((menu_setup == MENU_IM_HAND) && ((menu_subitem == MENU_IM_HAND_KP) || (menu_subitem == MENU_IM_HAND_KI) || (menu_subitem == MENU_IM_HAND_KD)) && (button_pressed == BUTTON_DOWN) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 5000UL)
			i = 100;
		else if (to_button_pressed < 10000UL)
			i = 1000;
		else
			i = 10000;

		if (min_value(menu_im_hand_value_item, menu_setup, menu_subitem) != max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
		{
			if ((get_value(menu_im_hand_value_item, menu_setup, menu_subitem) - i) > min_value(menu_im_hand_value_item, menu_setup, menu_subitem))
				set_value(menu_im_hand_value_item, menu_setup, menu_subitem,
						  get_value(menu_im_hand_value_item, menu_setup, menu_subitem) - i);
			else
				set_value(menu_im_hand_value_item, menu_setup, menu_subitem,
						  min_value(menu_im_hand_value_item, menu_setup, menu_subitem));
		}
	}

	if ((menu_setup == MENU_IM_T_LP) && ((menu_subitem == MENU_IM_T_LP_UP) || (menu_subitem == MENU_IM_T_LP_DOWN)) && (button_pressed == BUTTON_DOWN) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 2;
		else
			i = 10;

		if ((menu_data.i - i) > 1)
			menu_data.i -= i;
		else
			menu_data.i = 1;
	}

	if ((menu_setup == MENU_IM_T_LP) && ((menu_subitem == MENU_IM_T_LP_UP) || (menu_subitem == MENU_IM_T_LP_DOWN)) && (button_pressed == BUTTON_UP) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 2;
		else
			i = 10;

		if ((menu_data.i + i) < 70)
			menu_data.i += i;
		else
			menu_data.i = 70;
	}

	if ((menu_setup == MENU_IM_HAND) && (menu_subitem == MENU_IM_HAND_SI_OFF) && (button_pressed == BUTTON_DOWN) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 2;
		else
			i = 10;

		if ((menu_data.i - i) > 3)
			menu_data.i -= i;
		else
			menu_data.i = 3;
	}

	if ((menu_setup == MENU_IM_HAND) && (menu_subitem == MENU_IM_HAND_SI_OFF) && (button_pressed == BUTTON_UP) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 2;
		else
			i = 10;

		if ((menu_data.i + i) < 1000)
			menu_data.i += i;
		else
			menu_data.i = 1000;
	}

	if ((menu_setup == MENU_IM_HAND) && ((menu_subitem == MENU_IM_HAND_KP) || (menu_subitem == MENU_IM_HAND_KI) || (menu_subitem == MENU_IM_HAND_KD)) && (button_pressed == BUTTON_UP) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 5000UL)
			i = 20;
		else if (to_button_pressed < 10000UL)
			i = 500;
		else
			i = 2500;

		if (min_value(menu_im_hand_value_item, menu_setup, menu_subitem) != max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
		{
			if ((get_value(menu_im_hand_value_item, menu_setup, menu_subitem) + i) < max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
				set_value(menu_im_hand_value_item, menu_setup, menu_subitem,
						  get_value(menu_im_hand_value_item, menu_setup, menu_subitem) + i);
			else
				set_value(menu_im_hand_value_item, menu_setup, menu_subitem,
						  max_value(menu_im_hand_value_item, menu_setup, menu_subitem));
		}
	}

	if ((menu_setup != MENU_IM_HAND) && (menu_setup != MENU_IM_T_LP) && (button_pressed == BUTTON_DOWN) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 5;
		else
			i = 10;

		if (min_value(menu_im_hand_value_item, menu_setup, menu_subitem) != max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
		{
			if ((menu_data.i - i) > min_value(menu_im_hand_value_item, menu_setup, menu_subitem))
				menu_data.i -= i;
			else
				menu_data.i = min_value(menu_im_hand_value_item, menu_setup, menu_subitem);
		}
	}

	if ((menu_setup != MENU_IM_HAND) && (menu_setup != MENU_IM_T_LP) && (button_pressed == BUTTON_UP) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 5;
		else
			i = 10;

		if (min_value(menu_im_hand_value_item, menu_setup, menu_subitem) != max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
		{
			if ((menu_data.i + i) < max_value(menu_im_hand_value_item, menu_setup, menu_subitem))
				menu_data.i += i;
			else
				menu_data.i = max_value(menu_im_hand_value_item, menu_setup, menu_subitem);
		}
	}

	if ((menu_setup == MENU_IM_CALIB) && ((menu_subitem == MENU_IM_CALIB_MIN) || (menu_subitem == MENU_IM_CALIB_MAX)))
	{
		if (menu_subitem_edit)
		{
			if (0)
			{
			}
			else if (button_pressed == BUTTON_DOWN)
				menu_calib_pwm = MENU_CALIB_PWM_0;
			else if (button_pressed == BUTTON_UP)
				menu_calib_pwm = MENU_CALIB_PWM_100;
			else
				menu_calib_pwm = MENU_CALIB_PWM_50;
		}
		else
			menu_calib_pwm = MENU_CALIB_PWM_NONE;
	}
}

/// @brief Выполнить отрисовку экрана "Настройка ИМ - Редактирование".
void redraw_menu_view_im_edit(void)
{
	int i;
	BYTE str[32];
	BYTE const **menu_items;

	switch (menu_setup)
	{
	case MENU_IM_CALIB:
		menu_items = menu_im_calib_items;
		break;
	case MENU_IM_HAND:
		menu_items = menu_im_hand_items;
		break;
	case MENU_IM_T_LP:
		menu_items = menu_im_t_lp_items;
		break;
	case MENU_IM_HAR:
		menu_items = menu_im_har_items;
		break;
	case MENU_IM_HARD:
		menu_items = menu_im_hard_items;
		break;
	case MENU_IM_DIAP:
		menu_items = menu_im_diap_items;
		break;
	case MENU_IM_LP:
		menu_items = menu_im_none_items;
		break;
	case MENU_IM_LINEAR:
		menu_items = menu_im_linear_items;
		break;
	case MENU_IM_RESET:
		menu_items = menu_im_reset_items;
		break;
	}
	i = pcf8531_str_len(menu_items[menu_subitem], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2, menu_items[menu_subitem], font9);
	if (!menu_subitem_edit)
		pcf8531_clear(0, 32 - 7, 128, 7);

	if (menu_setup == MENU_IM_CALIB)
	{
		if (menu_subitem == MENU_IM_CALIB_TEST)
		{
			if (!menu_subitem_edit)
				pcf8531_clear(0, 32 - 7, 128, 7);
			else
				pcf8531_pic(0, 24, picto_calib_pwm, 128, 8);
			i = pcf8531_str_len(menu_calib_pwm_items[menu_calib_pwm], font9);
			pcf8531_str((128 - i) / 2, 14, menu_calib_pwm_items[menu_calib_pwm], font9);
		}
		else if (menu_subitem == MENU_IM_CALIB_MIN)
		{
			i = get_hmc_pos();
			str[0] = 0;
			mstr_add_int(str, i / 10);
			mstr_add(str, ".");
			mstr_add_int(str, i % 10);
			mstr_add(str, "% -> ");

			i = data_eeprom.hmc_lim_0;
			mstr_add_int(str, i / 10);
			mstr_add(str, ".");
			mstr_add_int(str, i % 10);
			mstr_add(str, "%");

			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
		}
		else if (menu_subitem == MENU_IM_CALIB_MAX)
		{
			i = get_hmc_pos();
			str[0] = 0;
			mstr_add_int(str, i / 10);
			mstr_add(str, ".");
			mstr_add_int(str, i % 10);
			mstr_add(str, "% -> ");

			i = data_eeprom.hmc_lim_100;
			mstr_add_int(str, i / 10);
			mstr_add(str, ".");
			mstr_add_int(str, i % 10);
			mstr_add(str, "%");

			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
		}
		else if (menu_subitem == MENU_IM_CALIB_RST)
		{
			if (!menu_subitem_edit)
			{
				mstr_cpy(str, "и журнала событий");
				i = pcf8531_str_len(str, font9);
				pcf8531_str((128 - i) / 2, 14, str, font9);
			}
			else
			{
				mstr_cpy(str, "Сбросить? - ");
				if (menu_data.i)
					mstr_add(str, "ДА");
				else
					mstr_add(str, "нет");
				i = pcf8531_str_len(str, font9);
				pcf8531_str((128 - i) / 2, 14, str, font9);
			}
		}
	}
	else if (menu_setup == MENU_IM_HAND)
	{
		str[0] = 0;
		switch (menu_subitem)
		{
		case MENU_IM_HAND_KP:
			FromFloat(str, menu_kp,3);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		case MENU_IM_HAND_KI:
			FromFloat(str, menu_ki,3);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		case MENU_IM_HAND_KD:
			FromFloat(str, menu_kd,3);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		case MENU_IM_HAND_SAVE:
			if (menu_subitem_edit)
				mstr_add(str, "Вы уверены?");
			else
				mstr_add(str, "коэффициенты ПИД");
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		case MENU_IM_HAND_INV:
			if (menu_data.f.coil_inverse)
				mstr_cpy(str, "Обратная шкала");
			else
				mstr_cpy(str, "Прямая шкала");
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		case MENU_IM_HAND_SI_OFF:
			FromFloat(str, menu_data.i,1);
			mstr_add(str, "%");
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		}
	}
	else if (menu_setup == MENU_IM_T_LP)
	{
		str[0] = 0;
		switch (menu_subitem)
		{
		case MENU_IM_T_LP_UP:
		case MENU_IM_T_LP_DOWN:
			FromFloat(str, menu_data.i * 5,1);
			mstr_add(str, "c");
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		}
	}
	else if (menu_setup == MENU_IM_HAR)
	{
		switch (menu_subitem)
		{
		case MENU_IM_HAR_PER:
			pcf8531_clear(0, 32 - 7, 128, 7);
			pcf8531_pic(2, 12, picto_ph[menu_data.f.im_ph], 27, 19);
			pcf8531_clear(30, 12, 128 - 30, 9);
			i = pcf8531_str_len(menu_im_ph_item[menu_data.f.im_ph], font9);
			pcf8531_str(30 + (128 - 30 - i) / 2, 12, menu_im_ph_item[menu_data.f.im_ph], font9);
			if (menu_subitem_edit)
				pcf8531_pic(43, 32 - 7, picto_ph_edit, 71, 7);
			break;
		case MENU_IM_HAR_LIM:
			str[0] = 0;
			i = menu_data.i;
			if (i < 0)
				i = 0;
			if (i > 90)
				i = 90;
			mstr_add_int(str, i);
			mstr_add(str, "%");
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 12, str, font9);
			break;
		}
	}
	else if (menu_setup == MENU_IM_HARD)
	{
		switch (menu_subitem)
		{
		case MENU_IM_HARD_MIN:
			pcf8531_pic(27, 14, picto_hard_im, 100, 8);
			str[0] = 0;
			mstr_add_int(str, menu_data.i / 10);
			mstr_add(str, ".");
			mstr_add_int(str, menu_data.i % 10);
			// mstr_add(str, "%");
			pcf8531_str(0, 14, str, font9);

			pcf8531_fill(27, 14, menu_data.i / 10, 7);
			pcf8531_fill(127 - data_eeprom.hard_pos_max / 10, 14, data_eeprom.hard_pos_max / 10, 7);
			break;

		case MENU_IM_HARD_MAX:
			pcf8531_pic(0, 14, picto_hard_im, 100, 8);
			str[0] = 0;
			mstr_add_int(str, menu_data.i / 10);
			mstr_add(str, ".");
			mstr_add_int(str, menu_data.i % 10);
			// mstr_add(str, "%");
			i = pcf8531_str_len(str, font9);
			pcf8531_str(127 - i, 14, str, font9);

			pcf8531_fill(0, 14, data_eeprom.hard_pos_min / 10, 7);
			pcf8531_fill(99 - menu_data.i / 10, 14, menu_data.i / 10, 7);
			break;
		}
	}
	else if (menu_setup == MENU_IM_DIAP)
	{
		switch (menu_subitem)
		{
		case MENU_IM_DIAP_MIN:
			str[0] = 0;
			mstr_add_int(str, menu_data.i / 100);
			mstr_add(str, ".");
			mstr_add_int2(str, menu_data.i % 100);
			mstr_add(str, " мА");
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;

		case MENU_IM_DIAP_MAX:
			str[0] = 0;
			mstr_add_int(str, menu_data.i / 100);
			mstr_add(str, ".");
			mstr_add_int2(str, menu_data.i % 100);
			mstr_add(str, " мА");
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 14, str, font9);
			break;
		}
	}
	else if (menu_setup == MENU_IM_LP)
	{
		str[0] = 0;
		mstr_add_int(str, menu_data.i / 100);
		mstr_add(str, ".");
		mstr_add_int(str, (menu_data.i / 10) % 10);
		mstr_add_int(str, (menu_data.i / 1) % 10);
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
	}
	else if (menu_setup == MENU_IM_LINEAR)
	{
		if (menu_data.i > 0)
			mstr_cpy(str, "Включить");
		else
			mstr_cpy(str, "Выключить");

		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
	}
	else if (menu_setup == MENU_IM_RESET)
	{
		mstr_cpy(str, "Сбросить? - ");
		if (menu_data.i)
			mstr_add(str, "ДА");
		else
			mstr_add(str, "нет");
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
	}
}

/// @brief Действия для экрана "Диагностика"
void selected_menu_view_diag(void)
{
	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_setup < (MENU_DIAG_NUM - 1))
		{
			menu_setup++;
			to_menu = 0;
		}
		else
		{
			menu_setup = 0;
			to_menu = 0;
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_setup > 0)
		{
			menu_setup--;
			to_menu = 0;
		}
		else
		{
			to_menu = 0;
			menu_setup = MENU_DIAG_NUM - 1;
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_setup = MENU_SETUP_DIAG;
		menu_go_to_view_and_clear(MENU_VIEW_SETUP);
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;

		switch (menu_setup)
		{
		case MENU_DIAG_PST:
			menu_subitem = 0;
			menu_setup = MENU_DIAG_PST;
			menu_go_to_view_and_clear(MENU_VIEW_PST);
			load_value(menu_diag_value_item, menu_setup, menu_subitem);
			break;

		case MENU_DIAG_FRIC:
			menu_subitem = 0;
			menu_setup = MENU_DIAG_FRIC;
			menu_go_to_view_and_clear(MENU_VIEW_FRIC);
			load_value(menu_diag_value_item, menu_setup, menu_subitem);
			break;

		case MENU_DIAG_GIST:
			menu_setup = 0;
			menu_go_to_view_and_clear(MENU_VIEW_DIAG_GIST);
			break;

		case MENU_DIAG_LOF:
			menu_setup = 0;
			menu_go_to_view_and_clear(MENU_VIEW_LOF);
			break;
		}
	}
}

/// @brief Выполнить отрисовку экрана "Диагностика".
void redraw_menu_view_diag(void)
{
	int i;

	i = pcf8531_str_len(menu_setup_items[MENU_SETUP_DIAG], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2,
				menu_setup_items[MENU_SETUP_DIAG], font9);
	pcf8531_str_to(
		(128 - pcf8531_str_len(menu_diag_items[menu_setup], font9)) / 2,
		16, menu_diag_items[menu_setup], 128, font9);
	pcf8531_clear(0, 32 - 7, 128, 7);
}

/// @brief Действия для экрана "PST-тест"
void selected_menu_view_pst(void)
{
	int i;

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_subitem_edit)
		{
			if (menu_data.i > min_value(menu_diag_value_item, menu_setup, menu_subitem))
				menu_data.i--;
			else
				menu_data.i = min_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
		else
		{
			if (menu_subitem < (MENU_PST_NUM - 1))
				menu_subitem++;
			else
				menu_subitem = 0;
			to_menu = 0;
			load_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_subitem_edit)
		{
			if (menu_data.i < max_value(menu_diag_value_item, menu_setup, menu_subitem))
				menu_data.i++;
			else
				menu_data.i = max_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
		else
		{
			if (menu_subitem > 0)
				menu_subitem--;
			else
				menu_subitem = MENU_PST_NUM - 1;
			to_menu = 0;
			load_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;

		if (menu_subitem_edit)
		{
			menu_subitem_edit = 0;
			to_menu = 0;
			if (menu_subitem != MENU_PST_START)
				load_value(menu_diag_value_item, menu_setup, menu_subitem);
			else
			{
				pst_auto_to = 0;
				pst_to = 0;
				pst_te = 0;
				pst_last = 0;
				pst_cur = 0;
				pst_result = 0;
				pst_t = 0;
			}
		}
		else
		{
			menu_setup = MENU_DIAG_PST;
			menu_go_to_view_and_clear(MENU_VIEW_DIAG);
		}
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		if (menu_subitem_edit)
		{
			if (menu_subitem != MENU_PST_START)
			{
				menu_subitem_edit = 0;
				to_menu = 0;
				save_value(menu_diag_value_item, menu_setup, menu_subitem);

				if (menu_subitem == MENU_PST_AUTORUN)
					pst_auto_to = 0;
			}
			else if (!pst_t)
			{
				menu_subitem_edit = 0;
				to_menu = 0;
			}
		}
		else
		{
			menu_subitem_edit = 1;
			if (menu_subitem == MENU_PST_START)
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
	}

	if (menu_subitem_edit && (button_pressed == BUTTON_DOWN) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 5;
		else
			i = 50;

		if (min_value(menu_diag_value_item, menu_setup, menu_subitem) != max_value(menu_diag_value_item, menu_setup, menu_subitem))
		{
			if ((menu_data.i - i) > min_value(menu_diag_value_item, menu_setup, menu_subitem))
				menu_data.i -= i;
			else
				menu_data.i = min_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
	}

	if (menu_subitem_edit && (button_pressed == BUTTON_UP) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 5;
		else
			i = 50;

		if (min_value(menu_diag_value_item, menu_setup, menu_subitem) != max_value(menu_diag_value_item, menu_setup, menu_subitem))
		{
			if ((menu_data.i + i) < max_value(menu_diag_value_item, menu_setup, menu_subitem))
				menu_data.i += i;
			else
				menu_data.i = max_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
	}
}

/// @brief Выполнить отрисовку экрана "PST-тест".
void redraw_menu_view_pst(void)
{
	int i;
	BYTE str[32];

	i = pcf8531_str_len(menu_pst_items[menu_subitem], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2, menu_pst_items[menu_subitem], font9);
	if (!menu_subitem_edit)
		pcf8531_clear(0, 32 - 7, 128, 7);

	str[0] = 0;
	if (menu_subitem == MENU_PST_START)
	{
		pcf8531_clear(0, 32 - 7, 128, 7);
		if (menu_subitem_edit)
		{
			if (pst_t)
			{
				mstr_add(str, "Идет тест ");
				i = pst_to * 100 / (pst_t * 2);
				mstr_add_int(str, i);
				mstr_add(str, "%");
			}
			else
			{
				if (pst_result == 3)
					mstr_add(str, "PST тест OK");
				else
					mstr_add(str, "PST тест ОШИБКА!");
			}
		}
		else
			mstr_add(str, "Запуск теста");
	}
	else
	{
		mstr_add_int(str, menu_data.i);
	}
	i = pcf8531_str_len(str, font9);
	pcf8531_str((128 - i) / 2, 13, str, font9);
}

/// @brief Действия для экрана "Высокое трение"
void selected_menu_view_fric(void)
{
	int i;

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_subitem_edit)
		{
			if (menu_data.i > min_value(menu_diag_value_item, menu_setup, menu_subitem))
				menu_data.i--;
			else
				menu_data.i = min_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
		else
		{
			if (menu_subitem < (MENU_FRIC_NUM - 1))
				menu_subitem++;
			else
				menu_subitem = 0;
			to_menu = 0;
			load_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_subitem_edit)
		{
			if (menu_data.i < max_value(menu_diag_value_item, menu_setup, menu_subitem))
				menu_data.i++;
			else
				menu_data.i = max_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
		else
		{
			if (menu_subitem > 0)
				menu_subitem--;
			else
				menu_subitem = MENU_FRIC_NUM - 1;
			to_menu = 0;
			load_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;

		if (menu_subitem_edit)
		{
			menu_subitem_edit = 0;
			to_menu = 0;
			load_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
		else
		{
			menu_setup = MENU_DIAG_FRIC;
			menu_go_to_view_and_clear(MENU_VIEW_DIAG);
		}
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		if (menu_subitem_edit)
		{
			menu_subitem_edit = 0;
			to_menu = 0;
			save_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
		else
			menu_subitem_edit = 1;
	}

	if (menu_subitem_edit && (button_pressed == BUTTON_DOWN) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 5;
		else
			i = 50;

		if (min_value(menu_diag_value_item, menu_setup, menu_subitem) != max_value(menu_diag_value_item, menu_setup, menu_subitem))
		{
			if ((menu_data.i - i) > min_value(menu_diag_value_item, menu_setup, menu_subitem))
				menu_data.i -= i;
			else
				menu_data.i = min_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
	}

	if (menu_subitem_edit && (button_pressed == BUTTON_UP) && (to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
	{
		if (to_button_pressed < 1000UL)
			i = 1;
		else if (to_button_pressed < 3000UL)
			i = 5;
		else
			i = 50;

		if (min_value(menu_diag_value_item, menu_setup, menu_subitem) != max_value(menu_diag_value_item, menu_setup, menu_subitem))
		{
			if ((menu_data.i + i) < max_value(menu_diag_value_item, menu_setup, menu_subitem))
				menu_data.i += i;
			else
				menu_data.i = max_value(menu_diag_value_item, menu_setup, menu_subitem);
		}
	}
}

/// @brief Выполнить отрисовку экрана "Высокое трение".
void redraw_menu_view_fric(void)
{
	int i;
	BYTE str[32];

	i = pcf8531_str_len(menu_fric_items[menu_subitem], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2, menu_fric_items[menu_subitem], font9);
	if (!menu_subitem_edit)
		pcf8531_clear(0, 32 - 7, 128, 7);
	str[0] = 0;
	mstr_add_int(str, menu_data.i);
	i = pcf8531_str_len(str, font9);
	pcf8531_str((128 - i) / 2, 13, str, font9);
}

/// @brief Действия для экрана "Гистограммы"
void selected_menu_view_diag_gist(void)
{
	int i;

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (menu_setup < (MENU_DIAG_GIST_NUM - 1))
		{
			menu_setup++;
			to_menu = 0;
		}
		else
		{
			menu_setup = 0;
			to_menu = 0;
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (menu_setup > 0)
		{
			menu_setup--;
			to_menu = 0;
		}
		else
		{
			to_menu = 0;
			menu_setup = MENU_DIAG_GIST_NUM - 1;
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_setup = MENU_DIAG_GIST;
		menu_go_to_view_and_clear(MENU_VIEW_DIAG);
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;

		if (0)
		{
		}
		else if (menu_setup == MENU_DIAG_GIST_POS)
			for (i = 0; i < GIST_POS_SIZE; i++)
				gist_pos[i] = 0;
		else if (menu_setup == MENU_DIAG_GIST_TGT)
			for (i = 0; i < GIST_TGT_SIZE; i++)
				gist_tgt[i] = 0;
		else if (menu_setup == MENU_DIAG_GIST_TMP)
			for (i = 0; i < GIST_TMP_SIZE; i++)
				gist_tmp[i] = 0;
	}
}

/// @brief Выполнить отрисовку экрана "Гистограммы".
void redraw_menu_view_diag_gist(void)
{
	int i, j, n;

	if (menu_setup == MENU_DIAG_GIST_POS)
	{
		pcf8531_pic(0, 0, picto_gist, 128, 32);
		pcf8531_str(0, 32 - 8, menu_diag_gist_items[menu_setup], font9);
		n = 0;
		for (i = 0; i < GIST_POS_SIZE; i++)
			n += gist_pos[i];
		for (i = 0; i < GIST_POS_SIZE; i++)
		{
			j = gist_pos[i] * 40 / n;
			if (j <= 20)
				pcf8531_pic(2 + i * 6, 22 - j, picto_gist_col, 5, j);
			else
			{
				pcf8531_pic(2 + i * 6, 2, picto_gist_col, 5, 20);
				pcf8531_pic(3 + i * 6, 0, picto_gist_col_max, 3, 2);
			}
		}
		i = ihmc_pos * 120 / 1000;
		if (i < 0)
			i = 0;
		if (i > 120)
			i = 120;
		pcf8531_pic(4 + i, 0, picto_gist_col_pos, 1, 6);
	}
	else if (menu_setup == MENU_DIAG_GIST_TGT)
	{
		pcf8531_pic(0, 0, picto_gist, 128, 32);
		pcf8531_str(0, 32 - 8, menu_diag_gist_items[menu_setup], font9);
		n = 0;
		for (i = 0; i < GIST_TGT_SIZE; i++)
			n += gist_tgt[i];
		for (i = 0; i < GIST_TGT_SIZE; i++)
		{
			j = gist_tgt[i] * 40 / n;
			if (j <= 20)
				pcf8531_pic(2 + i * 6, 22 - j, picto_gist_col, 5, j);
			else
			{
				pcf8531_pic(2 + i * 6, 2, picto_gist_col, 5, 20);
				pcf8531_pic(3 + i * 6, 0, picto_gist_col_max, 3, 2);
			}
		}
		i = ihmc_pos_t * 120 / 1000;
		if (i < 0)
			i = 0;
		if (i > 120)
			i = 120;
		pcf8531_pic(4 + i, 0, picto_gist_col_pos, 1, 6);
	}
	else if (menu_setup == MENU_DIAG_GIST_TMP)
	{
		pcf8531_pic(0, 0, picto_gist_tmp, 128, 32);
		n = 0;
		for (i = 0; i < GIST_TMP_SIZE; i++)
			n += gist_tmp[i];
		for (i = 0; i < GIST_TMP_SIZE; i++)
		{
			j = gist_tmp[i] * 40 / n;
			if (j <= 20)
				pcf8531_pic(2 + i * 9, 22 - j, picto_gist_col, 8, j);
			else
			{
				pcf8531_pic(2 + i * 9, 2, picto_gist_col, 8, 20);
				pcf8531_pic(3 + i * 9, 0, picto_gist_col_max6, 6, 2);
			}
		}
		if (tempeature != TEMPERATURE_BAD)
		{
			i = (tempeature + 50) * 118 / (80 + 50);
			if (i < 0)
				i = 0;
			if (i > 118)
				i = 118;
			pcf8531_pic(5 + i, 0, picto_gist_col_pos, 1, 6);
		}
	}
}

/// @brief Действия для экрана "Журнал событий"
void selected_menu_view_log(void)
{
	DWORD t1, t2;

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		if (event_num)
		{
			t1 = data_eeprom.events[menu_subitem].time;
			if (menu_subitem > 0)
				menu_subitem--;
			else
				menu_subitem = event_num - 1;
			t2 = data_eeprom.events[menu_subitem].time;

			con_str("EV: ");
			con_dword(t1);
			con_str(" - ");
			con_dword(t2);
			if (t1 < t2)
			{
				menu_event_last = 1;
				con_str(" *");
			}
			else
				menu_event_last = 0;
			con_str("\n\r");
			con_start();
		}
	}
	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (event_num)
		{
			t1 = data_eeprom.events[menu_subitem].time;
			if (menu_subitem < (event_num - 1))
				menu_subitem++;
			else
				menu_subitem = 0;
			t2 = data_eeprom.events[menu_subitem].time;

			con_str("EV: ");
			con_dword(t1);
			con_str(" - ");
			con_dword(t2);
			if (t2 < t1)
			{
				menu_event_last = 1;
				con_str(" *");
			}
			else
				menu_event_last = 0;
			con_str("\n\r");
			con_start();
		}
	}
	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		to_menu = 0;
		menu_setup = MENU_SETUP_LOG;
		menu_view = MENU_VIEW_SETUP;
	}
	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		to_menu = 0;
		event_flag_last = 0;
		menu_setup = MENU_SETUP_LOG;
		menu_view = MENU_VIEW_SETUP;
	}
}

/// @brief Выполнить отрисовку экрана "Журнал событий".
void redraw_menu_view_log(void)
{
	int i;
	BYTE str[32];

	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear(0, 32 - 7, 128, 7);

	if (event_num)
	{
		str[0] = 0;
		mstr_add(str, "Журнал ");
		mstr_add_int(str, menu_subitem);
		mstr_add(str, "/");
		mstr_add_int(str, event_num);
		if (menu_event_last)
			mstr_add(str, " (К)");
		i = pcf8531_str_len(str, font9);
		pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
		pcf8531_str((128 - i) / 2, 2, str, font9);

		str[0] = 0;
		if (data_eeprom.events[menu_subitem].event & (1 << EVENT_FLAG))
			mstr_add(str, "П");
		else
			mstr_add(str, "Е");
		mstr_add_int(str, data_eeprom.events[menu_subitem].event & (~(1 << EVENT_FLAG)));
		mstr_add(str, ": ");
		mstr_add(str, menu_event[data_eeprom.events[menu_subitem].event & (~(1 << EVENT_FLAG))]);
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);

		str[0] = 0;
		mstr_add(str, "Т: ");
		i = data_eeprom.events[menu_subitem].time;
		if (i < 60)
		{
			mstr_add_int(str, i);
			mstr_add(str, "с");
		}
		else if (i < (60 * 60))
		{
			mstr_add_int(str, i / 60);
			mstr_add(str, "м");
			mstr_add_int2(str, i % 60);
			mstr_add(str, "с");
		}
		else if (i < (24 * 60 * 60))
		{
			mstr_add_int(str, i / (60 * 60));
			mstr_add(str, "ч");
			mstr_add_int2(str, (i / 60) % 60);
			mstr_add(str, "м");
			mstr_add_int2(str, i % 60);
			mstr_add(str, "с");
		}
		else
		{
			mstr_add_int(str, i / (24 * 60 * 60));
			mstr_add(str, "д");
			mstr_add_int2(str, (i / (60 * 60)) % 24);
			mstr_add(str, "ч");
			mstr_add_int2(str, (i / 60) % 60);
			mstr_add(str, "м");
			mstr_add_int2(str, i % 60);
			mstr_add(str, "с");
		}
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 24, str, font9);
	}
	else
	{
		str[0] = 0;
		mstr_add(str, "Журнал 0 / 0");
		i = pcf8531_str_len(str, font9);
		pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
		pcf8531_str((128 - i) / 2, 2, str, font9);

		str[0] = 0;
		mstr_add(str, "Нет событий");
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
	}
}

/// @brief Действия для экрана "Автонастройка"
void selected_menu_view_auto(void)
{
	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;

		if (st_auto == ST_AUTO_SELECT)
		{
			auto_type = TYPE_DIRECT;
		}
		else if (st_auto == ST_AUTO_TIME_DONE)
		{
			menu_auto_exit();
		}
	}

	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (st_auto == ST_AUTO_SELECT)
		{
			auto_type = TYPE_REVERSE;
		}
		else if (st_auto == ST_AUTO_TIME_DONE)
		{
			menu_auto_exit();
		}
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_auto_exit();
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		if (st_auto == ST_AUTO_SELECT)
		{ // HERE Запуск автонастройки с кнопок
			to_auto = 0;
			if (data_eeprom.linear_en == 0)
			{
			}
			else
			{
				data_eeprom.linear_en = 0;
				linear_temp_disable = 1;
			}
			st_auto = ST_AUTO_START;
		}
		else if (st_auto == ST_AUTO_TIME_DONE)
		{
			menu_auto_exit();
		}
	}
}

/// @brief Выполнить отрисовку экрана "Автонастройка".
void redraw_menu_view_auto(void)
{
	int i;
	BYTE str[32];

	i = pcf8531_str_len(menu_im_items[MENU_IM_AUTO], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2,
				menu_im_items[MENU_IM_AUTO], font9);
	pcf8531_clear(0, 32 - 7, 128, 7);
	if (st_auto == ST_AUTO_IDLE)
	{
	}
	else if (st_auto == ST_AUTO_SELECT)
	{
		i = pcf8531_str_len(menu_im_auto_items[11], font9);
		pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[11], font9);
		switch (auto_type)
		{
		case TYPE_DIRECT:
			i = pcf8531_str_len(menu_im_auto_items[12], font9);
			pcf8531_str_inverse((64 - i) / 2, 24, menu_im_auto_items[12], font9);
			i = pcf8531_str_len(menu_im_auto_items[13], font9);
			pcf8531_str(64 + (64 - i) / 2, 24, menu_im_auto_items[13], font9);
			break;
		case TYPE_REVERSE:
			i = pcf8531_str_len(menu_im_auto_items[12], font9);
			pcf8531_str((64 - i) / 2, 24, menu_im_auto_items[12], font9);
			i = pcf8531_str_len(menu_im_auto_items[13], font9);
			pcf8531_str_inverse(64 + (64 - i) / 2, 24, menu_im_auto_items[13], font9);
			break;
		}
		// auto_type
	}
	else if (st_auto == ST_AUTO_DOWN_LIMIT)
	{
		int avg, delta;
		i = pcf8531_str_len(menu_im_auto_items[2], font9);
		pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[2], font9);
		if (stat_pos_arr_valid)
		{
			avg = stat_pos_avg;
			delta = stat_pos_delta;
			str[0] = 0;
			mstr_add_int(str, avg);
			mstr_add(str, ", ");
			mstr_add_int(str, delta);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 24, str, font9);
		}
		else
		{
			i = pcf8531_str_len(menu_im_auto_items[4], font9);
			pcf8531_str((128 - i) / 2, 24, menu_im_auto_items[4], font9);
		}
	}
	else if (st_auto == ST_AUTO_UP_LIMIT)
	{
		int avg, delta;
		i = pcf8531_str_len(menu_im_auto_items[3], font9);
		pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[3], font9);
		if (stat_pos_arr_valid)
		{
			avg = stat_pos_avg;
			delta = stat_pos_delta;
			str[0] = 0;
			mstr_add_int(str, avg);
			mstr_add(str, ", ");
			mstr_add_int(str, delta);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 24, str, font9);
		}
		else
		{
			i = pcf8531_str_len(menu_im_auto_items[4], font9);
			pcf8531_str((128 - i) / 2, 24, menu_im_auto_items[4], font9);
		}
	}
	else if (st_auto == ST_AUTO_LIMIT_DONE)
	{
		int avg, delta;
		i = pcf8531_str_len(menu_im_auto_items[5], font9);
		pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[5], font9);
		str[0] = 0;
		mstr_add_int(str, stat_pos_adc_min);
		mstr_add(str, " .. ");
		mstr_add_int(str, stat_pos_adc_max);
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 24, str, font9);
	}
	else if (st_auto == ST_AUTO_DOWN_TIME)
	{
		i = pcf8531_str_len(menu_im_auto_items[6], font9);
		pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[6], font9);
		str[0] = 0;
		FromPosition(str, ihmc_pos);
		mstr_add(str, ", ");
		mstr_add_int(str, stat_time_down);
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 24, str, font9);
	}
	else if (st_auto == ST_AUTO_UP_TIME)
	{
		i = pcf8531_str_len(menu_im_auto_items[7], font9);
		pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[7], font9);
		str[0] = 0;
		FromPosition(str, ihmc_pos);
		mstr_add(str, ", ");
		mstr_add_int(str, stat_time_up);
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 24, str, font9);
	}
	else if (st_auto == ST_AUTO_TIME_DONE)
	{
		if (stat_pos_arr_valid)
		{
			if (to_auto < 10000)
			{
				i = pcf8531_str_len(menu_im_auto_items[9], font9);
				pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[9], font9);
			}
			else
			{
				i = pcf8531_str_len(menu_im_auto_items[10], font9);
				pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[10], font9);
			}
			str[0] = 0;
			FromPosition(str, ihmc_pos);
			mstr_add(str, ", ");
			i = stat_pos_delta;
			mstr_add_int(str, i / 10);
			mstr_add(str, ".");
			mstr_add_int(str, i % 10);
			mstr_add(str, "%, ");
			mstr_add_int(str, stat_coil_pwm);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 24, str, font9);
		}
		else
		{
			i = pcf8531_str_len(menu_im_auto_items[8], font9);
			pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[8], font9);
			str[0] = 0;
			mstr_add_int(str, stat_time_down);
			mstr_add(str, ", ");
			mstr_add_int(str, stat_time_up);
			i = pcf8531_str_len(str, font9);
			pcf8531_str((128 - i) / 2, 24, str, font9);
		}
	}
}

/// @brief Действия для экрана "Инженерный режим"
void selected_menu_view_service(void)
{
	
}

/// @brief Выполнить отрисовку экрана "Инженерный режим".
void redraw_menu_view_service(void)
{
	
}

/// @brief Действия для экрана "Трение страгивания"
void selected_menu_view_lof(void)
{
	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		switch (menu_lof_pos)
		{
		case 0:
			menu_lof_amp = 0;
			break;
		case 1:
			menu_lof_size = 0;
			break;
		case 2:
			lof_pos = 0;
			lof_position = ihmc_pos;
			lof_amplitude = (menu_lof_amp == 0) ? 50 : 10;
			lof_period = LOF_SIZE;
			lof_to = 0;
			lof_time = LOF_SIZE;
			break;
		case 3:
			if (menu_lof_view < 2)
				menu_lof_view++;
			else
				menu_lof_view = 0;
			break;
		}
	}

	if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
		switch (menu_lof_pos)
		{
		case 0:
			menu_lof_amp = 1;
			break;
		case 1:
			menu_lof_size = 1;
			break;
		case 2:
			menu_setup = MENU_DIAG_LOF;
			menu_go_to_view_and_clear(MENU_VIEW_DIAG);
			break;
		case 3:
			if (menu_lof_view > 0)
				menu_lof_view--;
			else
				menu_lof_view = 2;
			break;
		}
	}

	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
		if (menu_lof_pos < 3)
			menu_lof_pos++;
		else
			menu_lof_pos = 0;
	}

	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		menu_setup = MENU_DIAG_LOF;
		menu_go_to_view_and_clear(MENU_VIEW_DIAG);
	}
}

/// @brief Выполнить отрисовку экрана "Трение страгивания".
void redraw_menu_view_lof(void)
{
	int i, j, n;
	BYTE str[32];
	static float lof_min, lof_max;

	pcf8531_clear(0, 0, 128, 32);

	pcf8531_str(10, 0, (menu_lof_amp == 0) ? "5%" : "1%", Font_Single);
	pcf8531_str(10, 8, (menu_lof_size == 0) ? "32" : "16", Font_Single);
	pcf8531_str(10, 16, "запуск", Font_Single);
	switch (menu_lof_view)
	{
	case 0:
		pcf8531_str(10, 24, "pos+tgt", Font_Single);
		break;
	case 1:
		pcf8531_str(10, 24, "pos", Font_Single);
		break;
	case 2:
		pcf8531_str(10, 24, "tgt", Font_Single);
		break;
	}

	pcf8531_str(0, menu_lof_pos * 8, ">", Font_Single);

	if (0)
	{
	}
	else if (lof_pos == -2)
	{
		//	view lof gist
		if (menu_lof_size == 0)
		{
			// 32 bars
			n = lof_gist[0][0];
			if (n < lof_gist[1][0])
				n = lof_gist[1][0];
			for (i = 0; i < LOF_GIST_SIZE; i++)
			{
				if (n < lof_gist[0][i])
					n = lof_gist[0][i];
				if (n < lof_gist[1][i])
					n = lof_gist[1][i];
			}
			if (0)
			{
			}
			else if (menu_lof_view == 0)
			{
				// pos+tgt
				for (i = 0; i < LOF_GIST_SIZE; i++)
				{
					pcf8531_pixel(64 + i * 2 + 0, 31 - lof_gist[1][i] * 31 / n);
					for (j = 0; j <= lof_gist[0][i] * 31 / n; j++)
						pcf8531_pixel(64 + i * 2 + 1, 31 - j);
				}
			}
			else if (menu_lof_view == 1)
			{
				// pos
				for (i = 0; i < LOF_GIST_SIZE; i++)
				{
					for (j = 0; j <= lof_gist[0][i] * 31 / n; j++)
					{
						pcf8531_pixel(64 + i * 2 + 0, 31 - j);
						pcf8531_pixel(64 + i * 2 + 1, 31 - j);
					}
				}
			}
			else if (menu_lof_view == 2)
			{
				// tgt
				for (i = 0; i < LOF_GIST_SIZE; i++)
				{
					pcf8531_pixel(64 + i * 2 + 0, 31 - lof_gist[1][i] * 31 / n);
					pcf8531_pixel(64 + i * 2 + 1, 31 - lof_gist[1][i] * 31 / n);
				}
			}
		}
		else
		{
			// 16 bars
			n = lof_gist[0][0] + lof_gist[0][1];
			if (n < (lof_gist[1][0] + lof_gist[1][1]))
				n = lof_gist[1][0] + lof_gist[1][1];
			for (i = 0; i < LOF_GIST_SIZE / 2; i++)
			{
				if (n < (lof_gist[0][i * 2 + 0] + lof_gist[0][i * 2 + 1]))
					n = lof_gist[0][i * 2 + 0] + lof_gist[0][i * 2 + 1];
				if (n < (lof_gist[1][i * 2 + 0] + lof_gist[1][i * 2 + 1]))
					n = lof_gist[1][i * 2 + 0] + lof_gist[1][i * 2 + 1];
			}
			if (0)
			{
			}
			else if (menu_lof_view == 0)
			{
				// pos+tgt
				for (i = 0; i < LOF_GIST_SIZE / 2; i++)
				{
					pcf8531_pixel(64 + i * 4 + 0, 31 - (lof_gist[1][i * 2 + 0] + lof_gist[1][i * 2 + 1]) * 31 / n);
					pcf8531_pixel(64 + i * 4 + 1, 31 - (lof_gist[1][i * 2 + 0] + lof_gist[1][i * 2 + 1]) * 31 / n);
					for (j = 0; j <= (lof_gist[0][i * 2 + 0] + lof_gist[0][i * 2 + 1]) * 31 / n; j++)
					{
						pcf8531_pixel(64 + i * 4 + 2, 31 - j);
						pcf8531_pixel(64 + i * 4 + 3, 31 - j);
					}
				}
			}
			else if (menu_lof_view == 1)
			{
				// pos
				for (i = 0; i < LOF_GIST_SIZE / 2; i++)
				{
					for (j = 0; j <= (lof_gist[0][i * 2 + 0] + lof_gist[0][i * 2 + 1]) * 31 / n; j++)
					{
						// pcf8531_pixel(64 + i * 4 + 0, 31 - j);
						pcf8531_pixel(64 + i * 4 + 1, 31 - j);
						pcf8531_pixel(64 + i * 4 + 2, 31 - j);
						pcf8531_pixel(64 + i * 4 + 3, 31 - j);
					}
				}
			}
			else if (menu_lof_view == 2)
			{
				// tgt
				for (i = 0; i < LOF_GIST_SIZE / 2; i++)
				{
					pcf8531_pixel(64 + i * 4 + 0, 31 - (lof_gist[1][i * 2 + 0] + lof_gist[1][i * 2 + 1]) * 31 / n);
					pcf8531_pixel(64 + i * 4 + 1, 31 - (lof_gist[1][i * 2 + 0] + lof_gist[1][i * 2 + 1]) * 31 / n);
					pcf8531_pixel(64 + i * 4 + 2, 31 - (lof_gist[1][i * 2 + 0] + lof_gist[1][i * 2 + 1]) * 31 / n);
					// pcf8531_pixel(64 + i * 4 + 3, 31 - (lof_gist[1][i * 2 + 0] + lof_gist[1][i * 2 + 1]) * 31 / n);
				}
			}
		}
	}
	else if (lof_pos == -1)
	{
		mstr_cpy(str, "нет данных");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(64 + (64 - i) / 2, 16, str, font9);
	}
	else if ((0 <= lof_pos) && (lof_pos < LOF_SIZE))
	{
		for (i = 0; i < 64; i++)
		{
			if ((lof_pos > (i * LOF_SIZE / 64)) || (i == 63) || (i == 0))
			{
				pcf8531_pixel(64 + i, 28);
				pcf8531_pixel(64 + i, 29);
				pcf8531_pixel(64 + i, 30);
				pcf8531_pixel(64 + i, 31);
			}
			else
			{
				pcf8531_pixel(64 + i, 28);
				// pcf8531_pixel(64 + i, 29);
				// pcf8531_pixel(64 + i, 30);
				pcf8531_pixel(64 + i, 31);
			}
		}
		str[0] = 0;
		mstr_add_int(str, lof_pos * 100 / LOF_SIZE);
		mstr_add(str, "%");
		i = pcf8531_str_len(str, font9);
		pcf8531_str(64 + (64 - i) / 2, 16, str, font9);
	}
	else if (lof_pos == LOF_SIZE)
	{
		// calc gist
		lof_min = lof_arr[0][0];
		lof_max = lof_arr[0][0];
		if (lof_min > lof_arr[1][0])
			lof_min = lof_arr[1][0];
		if (lof_max < lof_arr[1][0])
			lof_max = lof_arr[1][0];
		for (i = 1; i < LOF_SIZE; i++)
		{
			if (lof_min > lof_arr[0][i])
				lof_min = lof_arr[0][i];
			if (lof_max < lof_arr[0][i])
				lof_max = lof_arr[0][i];
			if (lof_min > lof_arr[1][i])
				lof_min = lof_arr[1][i];
			if (lof_max < lof_arr[1][i])
				lof_max = lof_arr[1][i];
		}

		con_str("lof_min = ");
		con_float(lof_min);
		con_str(", lof_max = ");
		con_float(lof_max);
		con_str("\n\r");
		con_start();

		for (i = 0; i < LOF_GIST_SIZE; i++)
		{
			lof_gist[0][i] = 0;
			lof_gist[1][i] = 0;
		}

		for (i = 0; i < LOF_SIZE; i++)
		{
			lof_gist[0][1 + (int)((lof_arr[0][i] - lof_min) * (LOF_GIST_SIZE - 2) / (lof_max - lof_min))]++;
			lof_gist[1][1 + (int)((lof_arr[1][i] - lof_min) * (LOF_GIST_SIZE - 2) / (lof_max - lof_min))]++;
		}

		/*
		n = lof_gist[0][0];
		if(n < lof_gist[1][0]) n = lof_gist[1][0];
		for(i = 0; i < LOF_GIST_SIZE; i++)
		{
			if(n < lof_gist[0][i]) n = lof_gist[0][i];
			if(n < lof_gist[1][i]) n = lof_gist[1][i];
		}

		con_str("lof_n = ");
		con_float(n);
		con_str("\n\r");
		con_start();

		for(i = 0; i < LOF_GIST_SIZE; i++)
		{
			lof_gist[0][i] = lof_gist[0][i] * 32 / n;
			if(lof_gist[0][i] > 31) lof_gist[0][i] = 31;
			lof_gist[1][i] = lof_gist[1][i] * 32 / n;
			if(lof_gist[1][i] > 31) lof_gist[1][i] = 31;
		}
		*/

		lof_pos = -2;
	}
}

/// @brief Действия для экрана "Автонастройка по HART"
void selected_menu_view_hartauto(void)
{
}

/// @brief Выполнить отрисовку экрана "Автонастройка по HART".
void redraw_menu_view_hartauto(void)
{
	int i;

	i = pcf8531_str_len(menu_im_items[MENU_IM_AUTO], font9);
	pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
	pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
	pcf8531_str((128 - i) / 2, 2, menu_im_items[MENU_IM_AUTO], font9);
	pcf8531_clear(0, 32 - 7, 128, 7);
	i = pcf8531_str_len(menu_im_auto_items[14], font9);
	pcf8531_str((128 - i) / 2, 14, menu_im_auto_items[14], font9);
}

/// @brief Определть нажатые кнопки и обновить состояние меню
void menu_update(void)
{
	switch (menu_view)
	{
	case (MENU_VIEW_START):
		selected_menu_view_start();
		break;
	case (MENU_VIEW_HOME):
		selected_menu_view_home();
		break;
	case (MENU_VIEW_INFO_1):
		selected_menu_view_info1();
		break;
	case (MENU_VIEW_INFO_2):
		selected_menu_view_info2();
		break;
	case (MENU_VIEW_INFO_3):
		selected_menu_view_info3();
		break;
	case (MENU_VIEW_HAND):
		selected_menu_view_hand();
		break;
	case (MENU_VIEW_SETUP):
		selected_menu_view_setup();
		break;
	case (MENU_VIEW_SETUP_EDIT):
		selected_menu_view_setup_edit();
		break;
	case (MENU_VIEW_SIGNAL):
		selected_menu_view_signal();
		break;
	case (MENU_VIEW_SIGNAL_EDIT):
		selected_menu_view_signal_edit();
		break;
	case (MENU_VIEW_IM):
		selected_menu_view_im();
		break;
	case (MENU_VIEW_IM_EDIT):
		selected_menu_view_im_edit();
		break;
	case (MENU_VIEW_DIAG):
		selected_menu_view_diag();
		break;
	case (MENU_VIEW_PST):
		selected_menu_view_pst();
		break;
	case (MENU_VIEW_FRIC):
		selected_menu_view_fric();
		break;
	case (MENU_VIEW_DIAG_GIST):
		selected_menu_view_diag_gist();
		break;
	case (MENU_VIEW_LOG):
		selected_menu_view_log();
		break;
	case (MENU_VIEW_AUTO):
		selected_menu_view_auto();
		break;
	case (MENU_VIEW_SERVICE):
		selected_menu_view_service();
		break;
	case (MENU_VIEW_LOF):
		selected_menu_view_lof();
		break;
	default:
		if (menu_btn.btn_up)
			menu_btn.clr_up = 1;

		if (menu_btn.btn_down)
			menu_btn.clr_down = 1;

		if (menu_btn.btn_ok)
			menu_btn.clr_ok = 1;

		if (menu_btn.btn_cancel)
			menu_btn.clr_cancel = 1;

		break;
	}

	if (menu_btn.clr_up)
	{
		menu_btn.btn_up = 0;
		menu_btn.clr_up = 0;
	}

	if (menu_btn.clr_down)
	{
		menu_btn.btn_down = 0;
		menu_btn.clr_down = 0;
	}

	if (menu_btn.clr_ok)
	{
		menu_btn.btn_ok = 0;
		menu_btn.clr_ok = 0;
	}

	if (menu_btn.clr_cancel)
	{
		menu_btn.btn_cancel = 0;
		menu_btn.clr_cancel = 0;
	}
}

/// @brief Выполнить отрисовку меню
void menu_redraw(void)
{
	if (hart_autoset_flag != 0)
	{
		hart_autoset_flag = 0;
		menu_view = MENU_VIEW_HARTAUTO;
	}

	if (hart_autoset_done_flag != 0)
	{
		hart_autoset_done_flag = 0;
		pcf8531_clear(0, 0, 128, 32);
		menu_view = MENU_VIEW_HOME;
	}

	switch (menu_view)
	{
	case MENU_VIEW_START:
		redraw_menu_view_start();
		break;
	case MENU_VIEW_HOME:
		redraw_menu_view_home();
		break;
	case MENU_VIEW_INFO_1:
		redraw_menu_view_info1();
		break;
	case MENU_VIEW_INFO_2:
		redraw_menu_view_info2();
		break;
	case MENU_VIEW_INFO_3:
		redraw_menu_view_info3();
		break;
	case MENU_VIEW_HAND:
		redraw_menu_view_hand();
		break;
	case MENU_VIEW_SETUP:
		redraw_menu_view_setup();
		break;
	case MENU_VIEW_SETUP_EDIT:
		redraw_menu_view_setup_edit();
		break;
	case MENU_VIEW_SIGNAL:
		redraw_menu_view_signal();
		break;
	case MENU_VIEW_SIGNAL_EDIT:
		redraw_menu_view_signal_edit();
		break;
	case MENU_VIEW_IM:
		redraw_menu_view_im();
		break;
	case MENU_VIEW_IM_EDIT:
		redraw_menu_view_im_edit();
		break;
	case MENU_VIEW_DIAG:
		redraw_menu_view_diag();
		break;
	case MENU_VIEW_PST:
		redraw_menu_view_pst();
		break;
	case MENU_VIEW_FRIC:
		redraw_menu_view_fric();
		break;
	case MENU_VIEW_DIAG_GIST:
		redraw_menu_view_diag_gist();
		break;
	case MENU_VIEW_LOG:
		redraw_menu_view_log();
		break;
	case MENU_VIEW_AUTO:
		redraw_menu_view_auto();
		break;
	case MENU_VIEW_SERVICE:
		redraw_menu_view_service();
		break;
	case MENU_VIEW_LOF:
		redraw_menu_view_lof();
		break;
	case MENU_VIEW_HARTAUTO:
		redraw_menu_view_hartauto();
		break;
	default:
		pcf8531_clear(0, 0, 128, 32);
		pcf8531_pic(59, 11, picto_status_ok, 9, 9);
		break;
	}
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
