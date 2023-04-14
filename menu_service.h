/**
 * @file menu_service.h
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @brief ������������ ����, ����������, ������ ��������� � ��������� ����������� ����.
 * @version 1.0
 * @date 2023-03-21
 *
 * @copyright (C) ���������, 2023
 */

#ifndef _MENU_SERVICE_H_
#define _MENU_SERVICE_H_

#include "menu_helper.h"

//-----------------------------------------------------------------------------
//	enum eSERVICE_ITEM_TYPE
//-----------------------------------------------------------------------------
typedef enum eSERVICE_ITEM_TYPE
{
	ESERVICE_TYPE_NONE = 0, // ����� ������
	ESERVICE_TYPE_COMMAND,	// �������
	ESERVICE_TYPE_INT_U,	// ��. ����� 0000
	ESERVICE_TYPE_INT_P,	// ��. ����� 0000
	ESERVICE_TYPE_FLOAT_U,	// ��. � ������ 000.000
	ESERVICE_TYPE_FLOAT_P,	// ��. � ������ 000.000
	ESERVICE_TYPE_FLOAT_PB, // ��. � ������ s000000.0
	ESERVICE_TYPE_FLAGS,	// �����
} eSERVICE_ITEM_TYPE;

/// @brief ��������� ���������� ��������
typedef struct sSERVICE_ITEM
{
	BYTE *name;
	DWORD type;
	union
	{
		volatile void *addr;
		void (*command)(void);
	};
	WORD pos;
	WORD size;
} sSERVICE_ITEM;

/// @brief ��������� ���� � ���������� ����.
/// @param serviceMenu ������� �� ����� ����������� ����.
extern void Service_Entry(Menu_Item_t *serviceMenu);

/// @brief ��������� ��������� �� ����������� ����.
extern void Service_Navigate(void);

/// @brief ��������� ��������� ����������� ����.
extern void Service_Render(void);

/// @brief �������� �������� ������� ��������� ��������������� ��������� 0%.
extern void Service_Command_SetPosition_0(void);

/// @brief �������� �������� ������� ��������� ��������������� ��������� 100%.
extern void Service_Command_SetPosition_100(void);

/// @brief �������� �������� �������� �������� ������� ��������������� 4��.
extern void Service_Command_SetAIn_4(void);

/// @brief �������� �������� �������� �������� ������� ��������������� 20��.
extern void Service_Command_SetAIn_20(void);

/// @brief ��������� ������������.
extern void Service_Command_Reboot(void);

/// @brief ��������� ������ ����� ����������.
extern void Service_Command_FactoryReset(void);

/// @brief ��������� ����� �������� ������������.
extern void Service_Command_FullReset(void);

/// @brief �������� ������������� ���������� �����������. 
/// @param newVersion ����� �������� ����������.
extern void Service_Command_ChangeTempVersion(int newVersion);

#endif // _MENU_SERVICE_H_