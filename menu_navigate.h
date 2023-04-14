/**
 * @file menu_navigate.h
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @brief ������������ ���� ������� ��������� �� ������� ���� ������������.
 * @version 1.0
 * @date 2023-03-15
 *
 * @copyright (C) ���������, 2023
 */

#ifndef _MENU_NAVIGATE_H_
#define _MENU_NAVIGATE_H_

#include "menu_helper.h"

/// @brief ������� ���������� ��������� �� ������ ����.
extern void Menu_NavigateBase(void);

/// @brief �������� ������� ��������� ������.
extern void Menu_ClearButtonStates(void);

/// @brief ��������� ��������� �� ������ "���������".
extern void Menu_NavigateByView_Start(void);

/**
 * @brief �������� ������ � ������ "��������� ����".
 * @remark ��� ��������� ������� � ������ "��������� ����" ���������� ��������� ��������� ��������:
 *      \n \b 1. ������ � ���������� ������ "������� ����" + "����".
 *      \n \b 2. ������������� ������ � ���������� ������ "������� �����"
 *      \n \b 3. ��������� � �������� ������ ������ "������� ����"
 * @param serviceMenu ��������� �� ����� "�������� ����".
 */
extern void Menu_ServiceMenuEntry(Menu_Item_t *serviceMenu);

/// @brief �������� ��������� ������.
extern void Menu_GetButtonState(void);

/// @brief ��������� ���������� � ���������� ������.
extern void Menu_NavigateByView_Service(void);

/// @brief ��������� ���������� �������� ������� � ������ ������.
extern void Menu_NavigateByView_HandEnable(void);

/// @brief ��������� ���������� �� ������ "����������: ���� - ��������������".
extern void Menu_NavigateByView_CalibTest(void);

/// @brief ��������� ���������� �� ������ "����������: ������� - ��������������".
extern void Menu_NavigateByView_CalibMin(void);

/// @brief ��������� ���������� �� ������ "����������: �������� - ��������������".
extern void Menu_NavigateByView_CalibMax(void);

/// @brief ��������� ���������� �� ������ "�������������".
extern void Menu_NavigateByView_Autotune(void);

/// @brief ��������� ���������� �� ������ "�����".
extern void Menu_NavigateByView_Scale(void);

/// @brief ��������� ���������� �� ������ "������������".
extern void Menu_NavigateByView_Linearization(void);

/// @brief ��������� ���������� �� ������ "�������� �����: �������".
extern void Menu_NavigateByView_InputRangeMin(void);

/// @brief ��������� ���������� �� ������ "�������� �����: ��������".
extern void Menu_NavigateByView_InputRangeMax(void);

/// @brief ��������� ���������� �� ������ "������� ��������: �������".
extern void Menu_NavigateByView_TightClosingMin(void);

/// @brief ��������� ���������� �� ������ "������� ��������: ��������".
extern void Menu_NavigateByView_TightClosingMax(void);

/// @brief ��������� ���������� �� ������ "������������ ��������������".
extern void Menu_NavigateByView_TransferCharacteristic(void);

/// @brief ��������� ���������� �� ������ "�������� �������".
extern void Menu_NavigateByView_StrokeSpeedPump(void);

/// @brief ��������� ���������� �� ������ "�������� ������".
extern void Menu_NavigateByView_StrokeSpeedBleed(void);

/// @brief ��������� ���������� �� ������ "����������� �������".
extern void Menu_NavigateByView_StrokeSpeedLimit(void);

/// @brief ��������� ���������� �� ������ "���: ��".
extern void Menu_NavigateByView_PidKp(void);

/// @brief ��������� ���������� �� ������ "���: ��".
extern void Menu_NavigateByView_PidKd(void);

/// @brief ��������� ���������� �� ������ "���: ��".
extern void Menu_NavigateByView_PidKi(void);

/// @brief ��������� ���������� �� ������ "������ ���������: ��� �������".
extern void Menu_NavigateByView_SensorType(void);

/// @brief ��������� ���������� �� ������ "���: ��".
extern void Menu_NavigateByView_PositionSensorMin_Auto(void);

/// @brief ��������� ���������� �� ������ "���: ��".
extern void Menu_NavigateByView_PositionSensorMin_Manual(void);

/// @brief ��������� ���������� �� ������ "���: ��".
extern void Menu_NavigateByView_PositionSensorMax_Auto(void);

/// @brief ��������� ���������� �� ������ "������ ���������: ��� �������".
extern void Menu_NavigateByView_PositionSensorMax_Manual(void);

/// @brief ��������� ���������� �� ������ "������� ���� 4��: ������".
extern void Menu_NavigateByView_CurrentInput4mA_Auto(void);

/// @brief ��������� ���������� �� ������ "������� ���� 4��: �������".
extern void Menu_NavigateByView_CurrentInput4mA_Manual(void);

/// @brief ��������� ���������� �� ������ "������� ���� 20��: ������".
extern void Menu_NavigateByView_CurrentInput20mA_Auto(void);

/// @brief ��������� ���������� �� ������ "������� ���� 20��: �������".
extern void Menu_NavigateByView_CurrentInput20mA_Manual(void);

/// @brief ��������� ���������� �� ������ "������� ����� 4��: �������".
extern void Menu_NavigateByView_CurrentOutput4mA(void);

/// @brief ��������� ���������� �� ������ "������� ����� 20��: �������".
extern void Menu_NavigateByView_CurrentOutput20mA(void);

/// @brief ��������� ���������� �� ������ "����: ���������� �����".
extern void Menu_NavigateByView_TestAO(void);

/// @brief ��������� ���������� �� ������ "����: ���������� ����� 1".
extern void Menu_NavigateByView_TestDO1(void);

/// @brief ��������� ���������� �� ������ "����: ���������� ����� 2".
extern void Menu_NavigateByView_TestDO2(void);

/// @brief ��������� ���������� �� ������ "����: ���������� ����� 3".
extern void Menu_NavigateByView_TestDO3(void);

/// @brief ��������� ���������� �� ������ "���: ����".
extern void Menu_NavigateByView_LpfInput(void);

/// @brief ��������� ���������� �� ������ "���: �����".
extern void Menu_NavigateByView_LpfOutput(void);

/// @brief ��������� ���������� �� ������ "������������: �����������".
extern void Menu_NavigateByView_LinearizationCoef(void);

/// @brief ��������� ���������� �� ������ "������������: ����� ����".
extern void Menu_NavigateByView_LinearizationZeroShift(void);

/// @brief ��������� ���������� �� ������ "�������".
extern void Menu_NavigateByView_ExpertDisplay(void);

/// @brief ��������� ���������� �� ������ "������������� ����������".
extern void Menu_NavigateByView_ExpertTempVersion(void);

/// @brief ��������� ���������� �� ������ "���� ���������� �".
extern void Menu_NavigateByView_ExpertAccumZone(void);

/// @brief ��������� ���������� �� ������ "������ ����� �������".
extern void Menu_NavigateByView_ExpertFactoryReset(void);

/// @brief ��������� ���������� �� ������ "C���� �������� ������������".
extern void Menu_NavigateByView_Reset(void);

/// @brief ��������� ���������� �� ������ "������ �������: ��������". 
extern void Menu_NavigateByView_ViewLogEdit(void);

/// @brief ��������� ���������� �� ������ "������ �������: ��������" 
extern void Menu_NavigateByView_ClearingLogEdit(void);

#endif // _MENU_NAVIGATE_H_
