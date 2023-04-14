/**
 * @file menu_v2.h
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @brief ������������ ���� ����.
 * @version 2.1
 * @date 10.03.2023
 *
 * @copyright (C) ���������, 2023
 *
 */
#ifndef __MENU_V2_H__
#define __MENU_V2_H__

#include "type.h"
#include "define.h"

#define TO_MENU_V2_START (3000UL) // ������� ������ ���������� ������, ��

/// @brief ��������� ������������ ��������� ������.
typedef struct sMENU_BTN
{
	union
	{
		BYTE btn; // ����� ��������
		struct
		{
			BYTE btn_up : 1;	 // ������� "�����"
			BYTE btn_down : 1;	 // ������� "����"
			BYTE btn_ok : 1;	 // ������� "��"
			BYTE btn_cancel : 1; // ������� "�����"
			BYTE clr_up : 1;	 // ������� "�����"
			BYTE clr_down : 1;	 // ������� "����"
			BYTE clr_ok : 1;	 // ������� "��"
			BYTE clr_cancel : 1; // ������� "�����"
		};
	};
} sMENU_BTN;

/// @brief �������� ������� ���������� ���������������.
typedef enum eMENU_CALIB_PWM
{
	/// @brief �� �����������
	MENU_CALIB_PWM_NONE = 0,

	/// @brief ����� ��������
	MENU_CALIB_PWM_0,

	/// @brief ��������
	MENU_CALIB_PWM_50,

	/// @brief ����� ��������
	MENU_CALIB_PWM_100,
} eMENU_CALIB_PWM;

/// @brief ������������� ������
typedef struct sMENU_DATA
{
	union
	{
		/// @brief ������������� "�����"
		sEEPROM_FLAGS f;

		/// @brief ������������� 32-������ �������� ����� �����
		int i;

		/// @brief ������������� ����������� �������� ���������� "i"
		int str;
	};
} sMENU_DATA;

/// @brief ��������� ������� ������ � �������� ��������� ����
extern void menu_v2_update(void);

/// @brief ��������� ��������� ����
extern void menu_v2_redraw(void);

/// @brief ������� ��������� ������.
extern sMENU_BTN menu_btn;

/// @brief ������ ���������� ���������������
extern eMENU_CALIB_PWM menu_calib_pwm;

extern volatile sMENU_DATA menu_data;

/// @brief ������� ��������� � ������ ������
extern volatile int menu_hand_pos_t;

/// @brief ��������� ��������� ��� ������������ ���������� ��������� menu_data.i
extern int i_old; 

/// @brief ��������� ����������, ������������ � �������� ������������� � ������� ����.
extern volatile int menu_switch;

/// @brief ��������� ����������, ������������ � �������� �������� � "������� �������".
extern volatile int counterItem;

extern tTIMER to_menu; // ��������� �� ��������� ��������.

#endif // __MENU_V2_H__