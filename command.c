/**
 * @file command.c
 * @author ����� �.�. (dnega@mail.ru)
 * @brief ������������� ������.
 * @version 0.1
 * @date 20.05.2016
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "command.h"
#include "watchdog.h"
#include "hart.h"
#include "stm32l1xx.h"

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
const char s_undef[] = "undef command. try \"help\" command.\n\r";
sEXEC_COMMAND const *console_command_table = console_command_init;

//-----------------------------------------------------------------------------
//! \fn void func_reboot(void *p)
//! \brief ������� \b "reboot". ���������� �����������
//-----------------------------------------------------------------------------
void func_reboot(void *p)
{
	NVIC_SystemReset();
}

//-----------------------------------------------------------------------------
//!	\fn void func_help(void *p)
//! \brief ������� \b "help". ������� ��������� �������
//-----------------------------------------------------------------------------
void func_help(void *p)
{
	int i = 0;
	con_str("\n\rAvailable commands:\n\r");
	con_start();
	while (console_command_table[i].pattern[0])
	{
		con_str("  ");
		con_str(console_command_table[i].pattern);
		con_str("\n\r");
		con_start();
		i++;
	}
	con_str("done.\n\r");
	con_start();
}

//-----------------------------------------------------------------------------
//!	\fn	void func_who(void	*p)
//! \brief ������� \b "who". ������� ������ ����������� � ��������
//-----------------------------------------------------------------------------
void func_who(void *p)
{
	con_str((BYTE *)s_who);
	con_str("FCPU = ");
	con_dec(FMSI);
	con_str(" Hz\n\r");
	con_start();
}

//-----------------------------------------------------------------------------
//! \fn void func_factory_reset(void *p)
//! \brief ������� \b "get adc". ������ �������� ���
//-----------------------------------------------------------------------------
void func_factory_reset(void *p)
{
	flag_factory_reset = 1;
}

//-----------------------------------------------------------------------------
//! \fn void func_pwm_coil(void *p)
//! \brief ������� \b "pwm_coil={�������� ������ ���}". ��������� ���������������
//! \brief \a "�������� ������ ���" �� \b 0 �� #MAX_PWM_COIL
//-----------------------------------------------------------------------------
void func_pwm_coil(void *p)
{
	PWM_COIL = *(int *)p;
}

//-----------------------------------------------------------------------------
//! \fn void func_pwm_aout(void *p)
//! \brief ������� \b "pwm_aout={�������� ������ ���}". ��������� ��������� �������
//! \brief \a "�������� ������ ���" �� \b 0 �� #MAX_PWM_AOUT
//-----------------------------------------------------------------------------
void func_pwm_aout(void *p)
{
	PWM_AOUT = *(int *)p;
}

//-----------------------------------------------------------------------------
//! \fn void func_namur(void *p)
//! \brief ������� \b "namur{����� ������}={��������}". ��������� ���������� �������
//! \brief \a "����� ������" �� \b 1 �� \b 3, \a "��������" �� \b 0 �� \b 1
//-----------------------------------------------------------------------------
void func_namur(void *p)
{
	typedef struct s_f
	{
		int n;
		int v;
	} s_f;
	s_f *f = (s_f *)p;

	if ((1 <= f->n) && (f->n <= 3) && (0 <= f->v) && (f->v <= 1))
		namur_out[f->n - 1] = f->v;
}

//-----------------------------------------------------------------------------
//! \fn void func_get_aout_ma(void *p)
//! \brief ������� \b "aout_ma?". ������ �������� ����������� ������ � ��
//-----------------------------------------------------------------------------
void func_get_aout_ma(void *p)
{
	con_str("aout_ma = ");
	con_float(aout_ma * 0.01f);
	con_str("\n\r");
	con_start();
}

//-----------------------------------------------------------------------------
//! \fn void func_set_aout_ma(void *p)
//! \brief ������� \b "aout_ma={��� ������ � ��}". ������������� ��� ����������� ������
//! \brief \a "��� ������ � ��" �� \b 4.00 �� �� \b 20.00 ��
//-----------------------------------------------------------------------------
void func_set_aout_ma(void *p)
{
	aout_ma = 100.0f * (*(float *)p);
}

//-----------------------------------------------------------------------------
//! \fn void func_pid_debug(void *p)
//! \brief ������� \b "pid_debug={��������}". ������� ���
//! \brief \a "��������": \b 0 - �� ��������, \b 1 - �������� ���������� ���������� ���
//-----------------------------------------------------------------------------
void func_pid_debug(void *p)
{
	pid_debug = *(int *)p;
}

//-----------------------------------------------------------------------------
//! \fn void func_get_adc(void *p)
//! \brief ������� \b "get adc". ������ �������� ���
//-----------------------------------------------------------------------------
void func_get_adc(void *p)
{
	get_adc_flag = 1;
}

//-----------------------------------------------------------------------------
//! \fn void func_clear_hmc_path(void *p)
//! \brief ������� \b "clear hmc_path". �������� ������� ������ "��"
//-----------------------------------------------------------------------------
void func_clear_hmc_path(void *p)
{
	ihmc_path = 0;
	save_maj(data_eeprom.hmc_path_nv, 0);
}

//-----------------------------------------------------------------------------
//! \fn void func_clear_worktime(void *p)
//! \brief ������� \b "clear worktime={����� � ��������}". ���������� ����� ������� "��"
//-----------------------------------------------------------------------------
void func_clear_worktime(void *p)
{
	int i = *(int *)p;
	worktime = i;
	save_maj(data_eeprom.worktime_nv, i);
}

//-----------------------------------------------------------------------------
//! \fn void func_clear_event(void *p)
//! \brief ������� \b "clear event". �������� ������ �������
//-----------------------------------------------------------------------------
void func_clear_event(void *p)
{
	int i;
	eeprom_unlock();
	for (i = 0; i < EVENT_LOG_NUM; i++)
	{
		data_eeprom.events[i].time = 0;
		data_eeprom.events[i].event = 0;
	}
	eeprom_lock();
	event_pos = 0;
	event_num = 0;
}

//-----------------------------------------------------------------------------
//! \fn void func_clear_event_flag(void *p)
//! \brief ������� \b "clear event flag". �������� �������� �������
//-----------------------------------------------------------------------------
void func_clear_event_flag(void *p)
{
	event_flag_last = 0;
}

//-----------------------------------------------------------------------------
//	��������� data_eeprom
//-----------------------------------------------------------------------------
//! \fn void func_set_ain_adc_min(void *p)
//! \brief ������� \b "ain_adc_min={��������}". ������������� �������� ��� ��� ������ ����� ������� ���� � \a "��.���"
FUNC_SET_INT(ain_adc_min);

//! \fn void func_get_ain_adc_min(void *p)
//! \brief ������� \b "ain_adc_min?". ��������� ������� �������� ��� ��� ������ ����� ������� ���� � \a "��.���"
FUNC_GET_INT(ain_adc_min);

//! \fn void func_set_ain_adc_max(void *p)
//! \brief ������� \b "ain_adc_max={��������}". ������������� �������� ��� ��� ������� ����� ������� ���� � \a "��.���"
FUNC_SET_INT(ain_adc_max);

//! \fn void func_get_ain_adc_max(void *p)
//! \brief ������� \b "ain_adc_max?". ��������� ������� �������� ��� ��� ������� ����� ������� ���� � \a "��.���"
FUNC_GET_INT(ain_adc_max);

//! \fn void func_set_ain_ma_min(void *p)
//! \brief ������� \b "ain_ma_min={��������}". ������������� �������� ���� ��� ������ ����� ������� ���� � \a "0.01 ��"
FUNC_SET_INT(ain_ma_min);

//! \fn void func_get_ain_ma_min(void *p)
//! \brief ������� \b "ain_ma_min?". ��������� ������� �������� ���� ��� ������ ����� ������� ���� � \a "0.01 ��"
FUNC_GET_INT(ain_ma_min);

//! \fn void func_set_ain_ma_max(void *p)
//! \brief ������� \b "ain_ma_max={��������}". ������������� �������� ���� ��� ������� ����� ������� ���� � \a "0.01 ��"
FUNC_SET_INT(ain_ma_max);

//! \fn void func_get_ain_ma_max(void *p)
//! \brief ������� \b "ain_ma_max?". ��������� ������� �������� ���� ��� ������� ����� ������� ���� � \a "0.01 ��"
FUNC_GET_INT(ain_ma_max);

//! \fn void func_set_hmc_pos_t_min(void *p)
//! \brief ������� \b "hmc_pos_t_min={��������}". ������������� ������� ��������� ��� ������ ����� � \a "0.1%.���"
FUNC_SET_INT(hmc_pos_t_min);

//! \fn void func_get_hmc_pos_t_min(void *p)
//! \brief ������� \b "hmc_pos_t_min?". ��������� ������� ������� ��������� ��� ������ ����� � \a "0.1%.���"
FUNC_GET_INT(hmc_pos_t_min);

//! \fn void func_set_hmc_pos_t_max(void *p)
//! \brief ������� \b "hmc_pos_t_max={��������}". ������������� ������� ��������� ��� ������� ����� � \a "0.1%.���"
FUNC_SET_INT(hmc_pos_t_max);

//! \fn void func_get_hmc_pos_t_max(void *p)
//! \brief ������� \b "hmc_pos_t_max?". ��������� ������� ������� ��������� ��� ������� ����� � \a "0.1%.���"
FUNC_GET_INT(hmc_pos_t_max);

//! \fn void func_set_ain_ma_t_min(void *p)
//! \brief ������� \b "ain_ma_t_min={��������}". ������������� �������� ���� � ����� ��� ������ ����� � \a "0.01 ��"
FUNC_SET_INT(ain_ma_t_min);

//! \fn void func_get_ain_ma_t_min(void *p)
//! \brief ������� \b "ain_ma_t_min?". ��������� ������� �������� ���� � ����� ��� ������ ����� � \a "0.01 ��"
FUNC_GET_INT(ain_ma_t_min);

//! \fn void func_set_ain_ma_t_max(void *p)
//! \brief ������� \b "ain_ma_t_max={��������}". ������������� �������� ���� � ����� ��� ������� ����� � \a "0.01 ��"
FUNC_SET_INT(ain_ma_t_max);

//! \fn void func_get_ain_ma_t_max(void *p)
//! \brief ������� \b "ain_ma_t_max?". ��������� ������� �������� ���� � ����� ��� ������� ����� � \a "0.01 ��"
FUNC_GET_INT(ain_ma_t_max);

//! \fn void func_set_aout_pwm_min(void *p)
//! \brief ������� \b "aout_pwm_min={��������}". ������������� �������� ��� ����������� ������ ��� \a "4.00 ��"
FUNC_SET_INT(aout_pwm_min);

//! \fn void func_get_aout_pwm_min(void *p)
//! \brief ������� \b "aout_pwm_min?". ��������� ������� �������� ��� ����������� ������ ��� \a "4.00 ��"
FUNC_GET_INT(aout_pwm_min);

//! \fn void func_set_aout_pwm_max(void *p)
//! \brief ������� \b "aout_pwm_max={��������}". ������������� �������� ��� ����������� ������ ��� \a "20.00 ��"
FUNC_SET_INT(aout_pwm_max);

//! \fn void func_get_aout_pwm_max(void *p)
//! \brief ������� \b "aout_pwm_max?". ��������� ������� �������� ��� ����������� ������ ��� \a "20.00 ��"
FUNC_GET_INT(aout_pwm_max);

//! \fn void func_set_hmc_adc_0(void *p)
//! \brief ������� \b "hmc_adc_0={��������}". ������������� �������� ��� ��� ����� \a "0%.���"
FUNC_SET_INT(hmc_adc_0);

//! \fn void func_get_hmc_adc_0(void *p)
//! \brief ������� \b "hmc_adc_0?". ��������� ������� �������� ��� ��� ����� \a "0%.���"
FUNC_GET_INT(hmc_adc_0);

//! \fn void func_set_hmc_adc_100(void *p)
//! \brief ������� \b "hmc_adc_100={��������}". ������������� �������� ��� ��� ����� \a "100%.���"
FUNC_SET_INT(hmc_adc_100);

//! \fn void func_get_hmc_adc_100(void *p)
//! \brief ������� \b "hmc_adc_100?". ��������� ������� �������� ��� ��� ����� \a "100%.���"
FUNC_GET_INT(hmc_adc_100);

//! \fn void func_set_hmc_lim_0(void *p)
//! \brief ������� \b "hmc_lim_0={��������}". ������������� ���������� ��������� ��� \a "0%.���" � \a "0.1%.���"
FUNC_SET_INT(hmc_lim_0);

//! \fn void func_get_hmc_lim_0(void *p)
//! \brief ������� \b "hmc_lim_0?". ��������� ������� �������� ����������� ��������� ��� \a "0%.���" � \a "0.1%.���"
FUNC_GET_INT(hmc_lim_0);

//! \fn void func_set_hmc_lim_100(void *p)
//! \brief ������� \b "hmc_lim_100={��������}". ������������� ���������� ��������� ��� \a "100%.���" � \a "0.1%.���"
FUNC_SET_INT(hmc_lim_100);

//! \fn void func_get_hmc_lim_100(void *p)
//! \brief ������� \b "hmc_lim_100?". ��������� ������� �������� ����������� ��������� ��� \a "100%.���" � \a "0.1%.���"
FUNC_GET_INT(hmc_lim_100);

//! \fn void func_set_hmc_g_0(void *p)
//! \brief ������� \b "hmc_g_0={��������}". ������������� ���� ��� \a "0%.���" � \a "0.1 �������"
FUNC_SET_INT(hmc_g_0);

//! \fn void func_get_hmc_g_0(void *p)
//! \brief ������� \b "hmc_g_0?". ��������� ������� �������� ���� ��� \a "0%.���" � \a "0.1 �������"
FUNC_GET_INT(hmc_g_0);

//! \fn void func_set_hmc_g_100(void *p)
//! \brief ������� \b "hmc_g_100={��������}". ������������� ���� ��� \a "100%.���" � \a "0.1 �������"
FUNC_SET_INT(hmc_g_100);

//! \fn void func_get_hmc_g_100(void *p)
//! \brief ������� \b "hmc_g_100?". ��������� ������� �������� ���� ��� \a "100%.���" � \a "0.1 �������"
FUNC_GET_INT(hmc_g_100);

//! \fn void func_set_hmc_lp(void *p)
//! \brief ������� \b "lpf_pos={������������ ��������}". ������������� �������� ��� ��� ������� ��������� �� \b 0.0 �� \b 1.0
FUNC_SET_FLOAT(lpf_pos);

//! \fn void func_get_hmc_lp(void *p)
//! \brief ������� \b "lpf_pos?". ��������� ������� �������� ��������� ��� ��� ������� ���������
FUNC_GET_FLOAT(lpf_pos);

//! \fn void func_set_ain_lp(void *p)
//! \brief ������� \b "lpf_a_in={������������ ��������}". ������������� �������� ��� ��� �������� ���� �� \b 0.0 �� \b 1.0
FUNC_SET_FLOAT(lpf_a_in);

//! \fn void func_get_ain_lp(void *p)
//! \brief ������� \b "lpf_a_in?". ��������� ������� �������� ��������� ��� ��� �������� ����
FUNC_GET_FLOAT(lpf_a_in);

//! \fn void func_set_kp(void *p)
//! \brief ������� \b "kp={������������ ��������}". ������������� ����������� ��� \a "��"
FUNC_SET_FLOAT(kp);

//! \fn void func_get_kp(void *p)
//! \brief ������� \b "kp?". ��������� ������� �������� ������������ ��� \a "��"
FUNC_GET_FLOAT(kp);

//! \fn void func_set_ki(void *p)
//! \brief ������� \b "ki={������������ ��������}". ������������� ����������� ��� \a "��"
FUNC_SET_FLOAT(ki);

//! \fn void func_get_ki(void *p)
//! \brief ������� \b "ki?". ��������� ������� �������� ������������ ��� \a "��"
FUNC_GET_FLOAT(ki);

//! \fn void func_set_kd(void *p)
//! \brief ������� \b "kd={������������ ��������}". ������������� ����������� ��� \a "��"
FUNC_SET_FLOAT(kd);

//! \fn void func_get_kd(void *p)
//! \brief ������� \b "kd?". ��������� ������� �������� ������������ ��� \a "��"
FUNC_GET_FLOAT(kd);

//! \fn void func_set_pid_int_min(void *p)
//! \brief ������� \b "pid_int_min={������������ ��������}". ������������� ����������� �-������������ �����
FUNC_SET_FLOAT(pid_int_min);

//! \fn void func_get_pid_int_min(void *p)
//! \brief ������� \b "pid_int_min?". ��������� ������� �������� ����������� �-������������ �����
FUNC_GET_FLOAT(pid_int_min);

//! \fn void func_set_pid_int_max(void *p)
//! \brief ������� \b "pid_int_max={������������ ��������}". ������������� ����������� �-������������ ������
FUNC_SET_FLOAT(pid_int_max);

//! \fn void func_get_pid_int_max(void *p)
//! \brief ������� \b "pid_int_max?". ��������� ������� �������� ����������� �-������������ ������
FUNC_GET_FLOAT(pid_int_max);

//! \fn void func_set_t_lp_up(void *p)
//! \brief ������� \b "t_lp_up={������������ ��������}". ������������� �������� ���������� �����
FUNC_SET_FLOAT(t_lp_up);

//! \fn void func_get_t_lp_up(void *p)
//! \brief ������� \b "t_lp_up?". ��������� ������� �������� �������� ���������� �����
FUNC_GET_FLOAT(t_lp_up);

//! \fn void func_set_t_lp_down(void *p)
//! \brief ������� \b "t_lp_down={������������ ��������}". ������������� �������� ���������� ����
FUNC_SET_FLOAT(t_lp_down);

//! \fn void func_get_t_lp_down(void *p)
//! \brief ������� \b "t_lp_down?". ��������� ������� �������� �������� ���������� ����
FUNC_GET_FLOAT(t_lp_down);

//! \fn void func_set_si_off(void *p)
//! \brief ������� \b "si_off={������������ ��������}". ������������� ������� ���������� �-������������
FUNC_SET_FLOAT(si_off);

//! \fn void func_get_si_off(void *p)
//! \brief ������� \b "si_off?". ��������� ������� �������� ������� ���������� �-������������
FUNC_GET_FLOAT(si_off);

//! \fn void func_set_slow_LT_edge(void *p)
//! \brief ������� \b "slow_LT_edge={��������}". ������������� ������� ���������� ���� ������� ��-�� �����������
FUNC_SET_INT(slow_LT_edge);

//! \fn void func_get_slow_LT_edge(void *p)
//! \brief ������� \b "slow_LT_edge?". ��������� ������� �������� ������� ���������� ���� ������� ��-�� �����������
FUNC_GET_INT(slow_LT_edge);

//! \fn void func_set_slow_LT_val(void *p)
//! \brief ������� \b "slow_LT_val={������������ ��������}". ������������� ����������� ���������� �������� ��-�� �
FUNC_SET_FLOAT(slow_LT_val);

//! \fn void func_get_slow_LT_val(void *p)
//! \brief ������� \b "slow_LT_val?". ��������� ������� �������� ������������ ���������� �������� ��-�� �
FUNC_GET_FLOAT(slow_LT_val);

//! \fn void func_get_midpoint(void *p)
//! \brief ������� \b "midpoint?". ��������� ������� �������� ������� ����� ��� ��������� ��
FUNC_GET_INT(midpoint);

//! \fn void func_set_midpoint(void *p)
//! \brief ������� \b "midpoint={��������}". ������������� �������� ������� ����� ��� ��������� ��
FUNC_SET_INT(midpoint);

#ifdef FAIL_DEBUG
//-----------------------------------------------------------------------------
//! \fn void func_failsystick(void *p)
//! \brief ������� \b "fail systick". �������� ���� ���������� �������
//-----------------------------------------------------------------------------
void func_failsystick(void *p)
{
	fail_systick = 1;
}

//-----------------------------------------------------------------------------
//! \fn void func_failmainloop(void *p)
//! \brief ������� \b "fail mainloop=1". �������� ���� �������� �����
//-----------------------------------------------------------------------------
void func_failmainloop(void *p)
{
	fail_mainloop = *(int *)p;
}
#endif // FAIL_DEBUG

//-----------------------------------------------------------------------------
//! \fn void func_hart_active(void *p)
//! \brief ������� \b "hart active". �������� ���������� HART
//-----------------------------------------------------------------------------
void func_hart_active(void *p)
{
	to_hart = 0;
}

//-----------------------------------------------------------------------------
//	void flash_write_dword(DWORD *dst, DWORD src)
//-----------------------------------------------------------------------------
void flash_write_dword(DWORD *dst, DWORD src)
{
	if (FLASH->PECR & (1 << FLASH_PECR_PELOCK))
	{
		FLASH->PEKEYR = 0x89ABCDEF;
		FLASH->PEKEYR = 0x02030405;
	}

	if (FLASH->PECR & (1 << FLASH_PECR_PRGLOCK))
	{
		FLASH->PRGKEYR = 0x8C9DAEBF;
		FLASH->PRGKEYR = 0x13141516;
	}

	*dst = src;

	FLASH->PECR = (1 << FLASH_PECR_PRGLOCK);
	FLASH->PECR = (1 << FLASH_PECR_PELOCK);
}

//-----------------------------------------------------------------------------
//! \fn void func_set_serial(void *p)
//! \brief ������� \b "serial={�������� �����}". ������������� ���������� �������� �����
//-----------------------------------------------------------------------------
void func_set_serial(void *p)
{
	int s = *(int *)p;

	con_str("\n\rnew serial = ");
	con_dec(s);
	con_str("\n\r");
	con_start();

	if (!image.serial)
	{
		flash_write_dword((DWORD *)&image.serial, (DWORD)s);
		con_str("update serial done\n\r");
		con_start();
	}
}

//-----------------------------------------------------------------------------
//! \fn void func_set_date(void *p)
//! \brief ������� \b "date={����}.{�����}.{���}". ������������� ���������� ���� ������������
//! \brief \a "����" �� \b 1 �� \b 31, \a "�����" �� \b 1 �� \b 12, \a "���" �� \b 1900 �� \b 2099
//-----------------------------------------------------------------------------
void func_set_date(void *p)
{
	typedef struct s_f
	{
		int d;
		int m;
		int y;
	} s_f;
	s_f *f = (s_f *)p;
	DWORD d;
	if (!image.date)
	{
		d = 0 | ((f->d & 0xFF) << 0) | ((f->m & 0xFF) << 8) | ((f->y & 0xFFFF) << 16);
		flash_write_dword((DWORD *)&image.date, (DWORD)d);
		con_str("update date done\n\r");
		con_start();
	}
}

//-----------------------------------------------------------------------------
//! \fn void func_get_date(void *p)
//! \brief ������� \b "date?". ������ ���� ������������
//! \brief \a "����" �� \b 1 �� \b 31, \a "�����" �� \b 1 �� \b 12, \a "���" �� \b 1900 �� \b 2099
//-----------------------------------------------------------------------------
void func_get_date(void *p)
{
	con_str("date:");
	con_dec((image.date >> 0) & 0xFF);
	con_str("-");
	con_dec((image.date >> 8) & 0xFF);
	con_str("-");
	con_dec((image.date >> 16) & 0xFFFF);
	con_str("\n\r");
	con_start();
}

//-----------------------------------------------------------------------------
//! \fn void func_hmc_set_0(void *p)
//! \brief ������� \b "hmc_set_0". ������������� ������� ��������� ������� ��� \b 0%.���
//-----------------------------------------------------------------------------
void func_hmc_set_0(void *p)
{
	eeprom_unlock();
	data_eeprom.hmc_adc_0 = ihmc_adc;
	eeprom_lock();
}

//-----------------------------------------------------------------------------
//! \fn void func_hmc_set_100(void *p)
//! \brief ������� \b "hmc_set_100". ������������� ������� ��������� ������� ��� \b 100%.���
//-----------------------------------------------------------------------------
void func_hmc_set_100(void *p)
{
	eeprom_unlock();
	data_eeprom.hmc_adc_100 = ihmc_adc;
	eeprom_lock();
}

//-----------------------------------------------------------------------------
//! \fn void func_login(void *p)
//! \brief ������� \b "login={������������}". ����������� ������������
//-----------------------------------------------------------------------------
void func_login(void *p)
{
	if (!strcmp(p, "admin")) // HERE ������ ��������������
		console_command_table = console_command_login;
	else
		console_command_table = console_command_init;
}

//-----------------------------------------------------------------------------
//! \fn void func_show_ph(void *p)
//! \brief ������� \b "show_ph={��������}". ���������� �� ������ "���������� �����" ������������� ������� ���
//! \brief \a "��������": \b 0 - �� ����������, \b 1 - ����������
//-----------------------------------------------------------------------------
void func_show_ph(void *p)
{
	show_ph = *(int *)p;
}

//-----------------------------------------------------------------------------
//	void func_log(void *p)
//-----------------------------------------------------------------------------
void func_log(void *p)
{
	get_pos_flag = *(int *)p;
}

//-----------------------------------------------------------------------------
//	void func_auto_kp(void *p)
//-----------------------------------------------------------------------------
void func_auto_kp(void *p)
{
	stat_kp = *(float *)p;
}

//-----------------------------------------------------------------------------
//	void func_auto_ki(void *p)
//-----------------------------------------------------------------------------
void func_auto_ki(void *p)
{
	stat_ki = *(float *)p;
}

//-----------------------------------------------------------------------------
//	void func_int_delta(void *p)
//-----------------------------------------------------------------------------
void func_int_delta(void *p)
{
	// pid_int_delta = *(float *)p;
}

//-----------------------------------------------------------------------------
//	void func_step_test(void *p)
//-----------------------------------------------------------------------------
void func_step_test(void *p)
{
	typedef struct s_f
	{
		int n;
		int t;
		int min;
		int max;
	} s_f;

	s_f *f = (s_f *)p;

	if ((1 <= f->n) && (f->n <= 20) && (3 <= f->t) && (f->t <= 30) && (0 <= f->min) && (f->min <= 1000) && (0 <= f->max) && (f->max <= 1000))
	{
		step_test_min = f->min;
		step_test_max = f->max;
		step_test_c = 0;
		step_test_n = f->n;
		step_test_to = 0;
		step_test_t = f->t * FADC;
	}
}

//-----------------------------------------------------------------------------
//	void func_pst(void *p)
//-----------------------------------------------------------------------------
void func_pst(void *p)
{
	typedef struct s_f
	{
		int dx;
		int t;
		int te;
	} s_f;

	s_f *f = (s_f *)p;

	if ((4 <= f->dx) && (f->dx <= 15) && (2 <= f->t) && (f->t <= 60) && (2 <= f->te) && (f->te <= 60))
	{
		pst_to = 0;
		pst_te = f->te * FADC;
		pst_last = ihmc_pos_t;
		pst_cur = pst_last + f->dx * 10;
		pst_result = 0;
		pst_t = f->t * FADC;
	}
}

//-----------------------------------------------------------------------------
//	void func_hart(void *p)
//-----------------------------------------------------------------------------
void func_hart(void *p)
{
	hart_act_count = *(int *)p;
}

//-----------------------------------------------------------------------------
//	void func_lof(void *p)
//-----------------------------------------------------------------------------
void func_lof(void *p)
{
	typedef struct s_f
	{
		int amplitude;
		int period;
		int time;
	} s_f;

	s_f *f = (s_f *)p;

	lof_position = ihmc_pos;
	lof_amplitude = f->amplitude;
	lof_period = f->period;
	lof_to = 0;
	lof_time = f->time;
}

//-----------------------------------------------------------------------------
//	void func_get_display(void *p)
//-----------------------------------------------------------------------------
void func_get_display(void *p)
{
	get_display = 1;
}

//-----------------------------------------------------------------------------
//! \fn void func_con_pwm(void *p)
//! \brief ������� \b "pwm_console={��������}". ��������� ������ ��������������� ���������� �������� � �������
//! \brief \a "��������": \b 0 - ���������� ����������, \b 1 - �� �������� "pwm_coil={�������� ������ ���}" � �������
//-----------------------------------------------------------------------------
void func_con_pwm(void *p)
{
	test_pwm = *(int *)p;
}

//-----------------------------------------------------------------------------
//! \fn void func_con_aout(void *p)
//! \brief ������� \b "aout_console={��������}". ��������� ������ ��������������� ���������� ���������� ������� � �������
//! \brief \a "��������": \b 0 - ���������� ����������, \b 1 - �� �������� "pwm_aout={�������� ������ ���}" � �������
//-----------------------------------------------------------------------------
void func_con_aout(void *p)
{
	test_a_out = *(int *)p;
}

//-----------------------------------------------------------------------------
//! \fn void func_con_namur(void *p)
//! \brief ������� \b "namur_console={��������}". ��������� ������ ��������������� ���������� ����������� �������� � �������
//! \brief \a "��������": \b 0 - ���������� ����������, \b 1 - �� �������� "namur{����� ������}={��������}" � �������
//-----------------------------------------------------------------------------
void func_con_namur(void *p)
{
	test_namur = *(int *)p;
}

//-----------------------------------------------------------------------------
//! \fn void func_linearize_en(void *p)
//! \brief ������� \b "linearize_en={��������}". ��������� ������ ������������ ��������� ��� ����������� ��������
//! \brief \a "��������": \b 0 - ���������, \b 1 - ��������
//-----------------------------------------------------------------------------
void func_linearize_en(void *p)
{
	eeprom_unlock();
	data_eeprom.linear_en = *(int *)p;
	eeprom_lock();
}

//-----------------------------------------------------------------------------
//! \page PAGE_CONSOLE_INIT ���������������� �������:
//! \li �������:
//! - \copydoc func_help
//! - \copydoc func_who
//! - \copydoc func_login
//-----------------------------------------------------------------------------
const sEXEC_COMMAND console_command_init[] =
	{
		{"help", func_help},
		{"who", func_who},
		{"login=%s", func_login},
		{"log=%d", func_log},
		{"auto_kp=%f", func_auto_kp},
		{"auto_ki=%f", func_auto_ki},
		{"hart=%d", func_hart},
		{"lof=%d,%d,%d", func_lof},
		{"date?", func_get_date},
		{"get display", func_get_display},
		{"", 0}};

//-----------------------------------------------------------------------------
//! \page PAGE_CONSOLE_LOGIN ����������������� �������:
//! \li �������:
//! - \copydoc func_help
//! - \copydoc func_who
//! - \copydoc func_login
//! \li ����������:
//! - \copydoc func_reboot
//! - \copydoc func_pwm_coil
//! - \copydoc func_pwm_aout
//! - \copydoc func_namur
//! - \copydoc func_get_adc
//! - \copydoc func_set_aout_ma
//! - \copydoc func_get_aout_ma
//! - \copydoc func_pid_debug
//! - \copydoc func_show_ph
//! - \copydoc func_hart_active
//! - \copydoc func_failsystick
//! - \copydoc func_failmainloop
//! \li ����������:
//! - \copydoc func_clear_hmc_path
//! - \copydoc func_clear_worktime
//! - \copydoc func_clear_event
//! - \copydoc func_clear_event_flag
//! - \copydoc func_set_serial
//! - \copydoc func_set_date
//! - \copydoc func_con_pwm
//! - \copydoc func_con_aout
//! - \copydoc func_con_namur
//! - \copydoc func_linearize_en
//! \li ��������� ����������:
//! - \copydoc func_hmc_set_0
//! - \copydoc func_hmc_set_100
//! - \copydoc func_set_hmc_adc_0
//! - \copydoc func_get_hmc_adc_0
//! - \copydoc func_set_hmc_adc_100
//! - \copydoc func_get_hmc_adc_100
//! - \copydoc func_set_hmc_g_0
//! - \copydoc func_get_hmc_g_0
//! - \copydoc func_set_hmc_g_100
//! - \copydoc func_get_hmc_g_100
//! - \copydoc func_set_ain_adc_min
//! - \copydoc func_get_ain_adc_min
//! - \copydoc func_set_ain_adc_max
//! - \copydoc func_get_ain_adc_max
//! - \copydoc func_set_ain_ma_min
//! - \copydoc func_get_ain_ma_min
//! - \copydoc func_set_ain_ma_max
//! - \copydoc func_get_ain_ma_max
//! - \copydoc func_set_aout_pwm_min
//! - \copydoc func_get_aout_pwm_min
//! - \copydoc func_set_aout_pwm_max
//! - \copydoc func_get_aout_pwm_max
//! \li ���������������� ���������:
//! - \copydoc func_set_hmc_lim_0
//! - \copydoc func_get_hmc_lim_0
//! - \copydoc func_set_hmc_lim_100
//! - \copydoc func_get_hmc_lim_100
//! - \copydoc func_set_hmc_pos_t_min
//! - \copydoc func_get_hmc_pos_t_min
//! - \copydoc func_set_hmc_pos_t_max
//! - \copydoc func_get_hmc_pos_t_max
//! - \copydoc func_set_ain_ma_t_min
//! - \copydoc func_get_ain_ma_t_min
//! - \copydoc func_set_ain_ma_t_max
//! - \copydoc func_get_ain_ma_t_max
//! - \copydoc func_set_hmc_lp
//! - \copydoc func_get_hmc_lp
//! - \copydoc func_set_ain_lp
//! - \copydoc func_get_ain_lp
//! \li ��������� ���:
//! - \copydoc func_set_kp
//! - \copydoc func_get_kp
//! - \copydoc func_set_ki
//! - \copydoc func_get_ki
//! - \copydoc func_set_kd
//! - \copydoc func_get_kd
//! - \copydoc func_set_pid_int_min
//! - \copydoc func_get_pid_int_min
//! - \copydoc func_set_pid_int_max
//! - \copydoc func_get_pid_int_max
//! - \copydoc func_set_t_lp_up
//! - \copydoc func_get_t_lp_up
//! - \copydoc func_set_t_lp_down
//! - \copydoc func_get_t_lp_down
//! - \copydoc func_set_si_off
//! - \copydoc func_get_si_off
//-----------------------------------------------------------------------------
const sEXEC_COMMAND console_command_login[] =
	{
		{"help", func_help},
		{"who", func_who},
		{"reboot", func_reboot},
		{"factory_reset", func_factory_reset},
		{"login=%s", func_login},
		{"hart active", func_hart_active},
		{"pwm_coil=%d", func_pwm_coil},
		{"pwm_aout=%d", func_pwm_aout},
		{"namur%d=%d", func_namur},
		{"get adc", func_get_adc},
		{"aout_ma?", func_get_aout_ma},
		{"aout_ma=%f", func_set_aout_ma},
		{"pid_debug=%d", func_pid_debug},
		{"clear hmc_path", func_clear_hmc_path},
		{"clear worktime=%d", func_clear_worktime},
		{"clear event", func_clear_event},
		{"clear event flag", func_clear_event_flag},
		{"serial=%d", func_set_serial},
		{"date=%d.%d.%d", func_set_date},
		{"date?", func_get_date},
		{"hmc_set_0", func_hmc_set_0},
		{"hmc_set_100", func_hmc_set_100},
		{"show_ph=%d", func_show_ph},
		{"log=%d", func_log},
		{"int_delta=%f", func_int_delta},
		{"step_test=%d,%d,%d,%d", func_step_test},
		{"pst=%d,%d,%d", func_pst},
		{"lof=%d,%d,%d", func_lof},

		FUNC_TABLE_INT(ain_adc_min),
		FUNC_TABLE_INT(ain_adc_max),
		FUNC_TABLE_INT(ain_ma_min),
		FUNC_TABLE_INT(ain_ma_max),
		FUNC_TABLE_INT(hmc_pos_t_min),
		FUNC_TABLE_INT(hmc_pos_t_max),
		FUNC_TABLE_INT(ain_ma_t_min),
		FUNC_TABLE_INT(ain_ma_t_max),
		FUNC_TABLE_INT(aout_pwm_min),
		FUNC_TABLE_INT(aout_pwm_max),

		FUNC_TABLE_INT(hmc_adc_0),
		FUNC_TABLE_INT(hmc_adc_100),
		FUNC_TABLE_INT(hmc_lim_0),
		FUNC_TABLE_INT(hmc_lim_100),
		FUNC_TABLE_INT(hmc_g_0),
		FUNC_TABLE_INT(hmc_g_100),

		FUNC_TABLE_FLOAT(kp),
		FUNC_TABLE_FLOAT(ki),
		FUNC_TABLE_FLOAT(kd),
		FUNC_TABLE_FLOAT(pid_int_min),
		FUNC_TABLE_FLOAT(pid_int_max),
		FUNC_TABLE_FLOAT(lpf_pos),
		FUNC_TABLE_FLOAT(lpf_a_in),
		FUNC_TABLE_FLOAT(t_lp_up),
		FUNC_TABLE_FLOAT(t_lp_down),
		FUNC_TABLE_FLOAT(si_off),
		FUNC_TABLE_INT(slow_LT_edge),
		FUNC_TABLE_FLOAT(slow_LT_val),
		FUNC_TABLE_INT(midpoint),
		{"pwm_console=%d", func_con_pwm},
		{"aout_console=%d", func_con_aout},
		{"namur_console=%d", func_con_namur},
		{"linearize_en=%d", func_linearize_en},

#ifdef FAIL_DEBUG
		{"fail systick", func_failsystick},
		{"fail mainloop=%d", func_failmainloop},
#endif // FAIL_DEBUG
		{"", 0},
};

//-----------------------------------------------------------------------------
//	void command_mainloop(void)
//-----------------------------------------------------------------------------
void command_mainloop(void)
{
	BYTE x;

	while (con.rx_b != con.rx_t)
	{
		x = con_rx_pop();
		switch (x)
		{
		case COM_END:
			console_command[console_command_pos] = 0;
			con_str("\n\r");
			con_start();
			if (!console_command[0] || command_exec(console_command_table, console_command, console_exec_buf))
			{
				con_str(">");
				con_start();
			}
			else
			{
				con_str((BYTE *)s_undef);
				con_start();
			}
			console_command_pos = 0;
			break;

		case 0x0A:
			break;

		case COM_BS:
			if (console_command_pos > 0)
			{
				console_command_pos--;
				con_push(x);
				con_start();
			}
			console_command[console_command_pos] = 0;
			break;

		case COM_ESC:
			while (console_command_pos > 0)
			{
				console_command_pos--;
				con_push(COM_BS);
				con_start();
			}
			break;

		default:
			if (console_command_pos < (CONSOLE_COMMAND_SIZE - 1))
			{
				con_push(x);
				con_start();
				console_command[console_command_pos] = x;
				console_command_pos++;
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	int command_exec(const sEXEC_COMMAND *table, const BYTE *in, BYTE *out)
//-----------------------------------------------------------------------------
int command_exec(const sEXEC_COMMAND *table, const BYTE *in, BYTE *out)
{
	int cmd;
	int p_in, p_table;
	int p_size, p_cnt;
	int p_out;
	int sign;

	int f_num;
	int f_mult;

	cmd = 0;
	while (table[cmd].pattern[0])
	{
		p_in = 0;
		p_table = 0;
		p_out = 0;
		while (1)
		{
			if (table[cmd].pattern[p_table] == '%')
			{
				p_table++;
				if ((table[cmd].pattern[p_table] == 'b') | (table[cmd].pattern[p_table] == 'w') | (table[cmd].pattern[p_table] == 'q'))
				{
					switch (table[cmd].pattern[p_table])
					{
					case 'b':
						p_size = 1 << 1;
						break;
					case 'w':
						p_size = 2 << 1;
						break;
					case 'q':
						p_size = 4 << 1;
						break;
					}
					for (p_cnt = 0; p_cnt < p_size; p_cnt++)
					{
						if (IS_HEX(in[p_in]))
						{
							if (p_cnt & 1)
								out[p_out + p_size / 2 - 1 - p_cnt / 2] |= TO_HEX(in[p_in]);
							else
								out[p_out + p_size / 2 - 1 - p_cnt / 2] = TO_HEX(in[p_in]) << 4;
							p_in++;
						}
						else
							break;
					}
					if (p_cnt == p_size)
						p_out += p_size / 2;
					else
						break;
				}
				else if (table[cmd].pattern[p_table] == 's')
				{
					while (IS_CHAR(in[p_in]))
					{
						out[p_out] = in[p_in];
						p_in++;
						p_out++;
					}
					out[p_out] = 0;
					p_out++;
				}
				else if (table[cmd].pattern[p_table] == 'f')
				{
					sign = 1;
					if (in[p_in] == '-')
					{
						sign = -1;
						p_in++;
					}
					f_num = 0;
					f_mult = 0;
					if (IS_DIGIT(in[p_in]) || (in[p_in] == '.'))
					{
						while (IS_DIGIT(in[p_in]) || ((f_mult == 0) && (in[p_in] == '.')))
						{
							if (in[p_in] == '.')
							{
								f_mult = 1;
							}
							else
							{
								f_num = f_num * 10 + (in[p_in] - '0');
								if (f_mult)
									f_mult *= 10;
							}
							p_in++;
						}
						if (f_mult == 0)
							*(float *)&out[p_out] = ((float)sign) * ((float)f_num);
						else
							*(float *)&out[p_out] = ((float)sign) * ((float)f_num) / ((float)f_mult);
						p_out += sizeof(int);
					}
					else
						break;
				}
				else if (table[cmd].pattern[p_table] == 'd')
				{
					sign = 1;
					if (in[p_in] == '-')
					{
						sign = -1;
						p_in++;
					}
					if (IS_DIGIT(in[p_in]))
					{
						*(int *)&out[p_out] = 0;
						while (IS_DIGIT(in[p_in]))
						{
							*(int *)&out[p_out] *= 10;
							*(int *)&out[p_out] += (in[p_in] - '0');
							p_in++;
						}
						*(int *)&out[p_out] *= sign;
						p_out += sizeof(int);
					}
					else
						break;
				}
				else if (IS_DIGIT(table[cmd].pattern[p_table]))
				{
					p_size = table[cmd].pattern[p_table] - '0';
					p_table++;
					if (table[cmd].pattern[p_table] == 'd')
					{
						*(int *)&out[p_out] = 0;
						sign = 1;
						if (in[p_in] == '-')
						{
							sign = -1;
							p_in++;
						}
						for (p_cnt = 0; p_cnt < p_size; p_cnt++)
						{
							if (IS_DIGIT(in[p_in]))
							{
								*(int *)&out[p_out] *= 10;
								*(int *)&out[p_out] += (in[p_in] - '0');
								p_in++;
							}
							else
								break;
						}
						if (p_cnt == p_size)
						{
							*(int *)&out[p_out] *= sign;
							p_out += sizeof(int);
						}
						else
							break;
					}
					else
						break;
				} // formats
				else
					break;
			}
			else
			{
				if (table[cmd].pattern[p_table] == in[p_in])
				{
					if (in[p_in] == 0)
					{
						table[cmd].func(out);
						return 1;
					}
				}
				else
					break;
				p_in++;
			}
			p_table++;
		}
		cmd++;
	}
	return 0;
}

/******************************************************************************
 * ����� �����
 *****************************************************************************/
