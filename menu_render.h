/**
 * @file menu_render.h
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @brief ������������ ���� ������� ������������ ������� ���� ������������.
 * @version 1.0
 * @date 2023-03-15
 * 
 * @copyright (C) ���������, 2023
 */
#ifndef _MENU_RENDER_H_
#define _MENU_RENDER_H_

#include "type.h"

#pragma region  ������� �� ��������� ��������

extern void FromPosition(BYTE *str, const int value);
extern void FromFloat(BYTE *str, const int value, const int decimals);

#pragma endregion

/// @brief ��������� ��������� ������ "��������� �����".
extern void Menu_RenderView_Start(void);

/// @brief ��������� ��������� ������ "��������� ����".
extern void Menu_RenderView_Service(void);

/// @brief ��������� ��������� ������ "�������� �����".
extern void Menu_RenderView_Home(void);

/// @brief ��������� ��������� ������ "���������-1".
extern void Menu_RenderView_Info1(void);

/// @brief ��������� ��������� ������ "���������-2".
extern void Menu_RenderView_Info2(void);

/// @brief ��������� ��������� ������ "���������-3".
extern void Menu_RenderView_Info3(void);

/// @brief ��������� ��������� ������ "���������".
extern void Menu_RenderView_Setup(void);

/// @brief ��������� ��������� ������ "������ �������".
extern void Menu_RenderView_Log(void);

/// @brief ��������� ��������� ������ "�������� ������� �������". 
extern void Menu_RenderView_ViewLog(void); 

/// @brief ��������� ��������� ������ "�������� ������� �������: ��������". 
extern void Menu_RenderView_ViewLogEdit(void); 

/// @brief ��������� ��������� ������ "������� ������� �������".
extern void Menu_RenderView_ClearingLog(void);

/// @brief ��������� ��������� ������ "������� ������� �������: �������".
extern void Menu_RenderView_ClearingLogEdit(void);

/// @brief ��������� ��������� ������ "������������".
extern void Menu_RenderView_Signal(void);

/// @brief ��������� ��������� ������ "�����������".
extern void Menu_RenderView_Diag(void);

/// @brief ��������� ��������� ������ "HART".
extern void Menu_RenderView_Hart(void);

/// @brief ��������� ��������� ������ "�������".
extern void Menu_RenderView_Display(void);

/// @brief ��������� ��������� ������ "������ �����".
extern void Menu_RenderView_Hand(void);

/// @brief ��������� ��������� ��������� ������ � ������ ������� ����������.
extern void Menu_RenderView_HandEnable(void);

/// @brief ��������� ��������� ������ "����������".
extern void Menu_RenderView_Calib(void);

/// @brief ��������� ��������� ������ "�������������".
extern void Menu_RenderView_Autotune(void);

/// @brief ��������� ��������� ������ "�����".
extern void Menu_RenderView_Scale(void);

/// @brief ��������� ��������� ������ "����� - ��������������".
extern void Menu_RenderView_ScaleEdit(void);

/// @brief ��������� ��������� ������ "������������".
extern void Menu_RenderView_Linearization(void);

/// @brief ��������� ��������� ������ "������������ - ��������������".
extern void Menu_RenderView_LinearizationEdit(void);

/// @brief ��������� ��������� ������ "�������� �����".
extern void Menu_RenderView_InputRange(void);

/// @brief ��������� ��������� ������ "������� ��������".
extern void Menu_RenderView_TightClosing(void);

/// @brief ��������� ��������� ������ "������������ ��������������".
extern void Menu_RenderView_TransferCharacteristic(void);

/// @brief ��������� ��������� ������ "������������ �������������� - ��������������".
extern void Menu_RenderView_TransferCharacteristicEdit(void);

/// @brief ��������� ��������� ������ "�������� ����".
extern void Menu_RenderView_StrokeSpeed(void);

/// @brief ��������� ��������� ������ "��� ���������".
extern void Menu_RenderView_PID(void);

/// @brief ��������� ��������� ������ "���������� ���������".
extern void Menu_RenderView_ExpertSetup(void);

/// @brief ��������� ��������� ������ "C���� �������� ������������".
extern void Menu_RenderView_Reset(void);

/// @brief ��������� ��������� ������ "C���� �������� ������������ - ��������������".
extern void Menu_RenderView_ResetEdit(void);

/// @brief ��������� ��������� ������ "�������� ������� �������".

/// @brief ��������� ��������� ������ "���������� �����".
extern void Menu_RenderView_AO(void);

/// @brief ��������� ��������� ������ "���������� ����� 1".
extern void Menu_RenderView_DO1(void);

/// @brief ��������� ��������� ������ "���������� ����� 2".
extern void Menu_RenderView_DO2(void);

/// @brief ��������� ��������� ������ "���������� ����� 3".
extern void Menu_RenderView_DO3(void);

/// @brief ��������� ��������� ������ "������� ������� �������".
extern void Menu_RenderView_ClearingLog(void);

/// @brief ��������� ��������� ������ "PST-����".
extern void Menu_RenderView_PST(void);

/// @brief ��������� ��������� ������ "������ �����������".
extern void Menu_RenderView_BreakawayFriction(void);

/// @brief ��������� ��������� ������ "�����������".
extern void Menu_RenderView_Histograms(void);

/// @brief ��������� ��������� ������ "������� ������".
extern void Menu_RenderView_HighFriction(void);

/// @brief ��������� ��������� ������ "HART: �������������".
extern void Menu_RenderView_HartIdentifier(void);

/// @brief ��������� ��������� ������ "HART: ���������".
extern void Menu_RenderView_HartPriority(void);

/// @brief ��������� ��������� ������ "HART: ����������".
extern void Menu_RenderView_HartShutdown(void);

/// @brief ��������� ��������� ������ "������� ��".
extern void Menu_RenderView_UnitsIM(void);

/// @brief ��������� ��������� ������ "������� �����".
extern void Menu_RenderView_UnitsInput(void);

/// @brief ��������� ��������� ������ "�������������".
extern void Menu_RenderView_Contrast(void);

/// @brief ��������� ��������� ������ "������� ������".
extern void Menu_RenderView_ScreenRotation(void);

/// @brief ��������� ��������� ������ "����������� ��".
extern void Menu_RenderView_PcSmoothing(void);

/// @brief ��������� ��������� ������ "����������: ����".
extern void Menu_RenderView_CalibTest(void);

/// @brief ��������� ��������� ������ "����������: ���� - ��������������".
extern void Menu_RenderView_CalibTestEdit(void);

/// @brief ��������� ��������� ������ "����������: �������".
extern void Menu_RenderView_CalibMin(void);

/// @brief ��������� ��������� ������ "����������: ������� - ��������������".
extern void Menu_RenderView_CalibMinEdit(void);

/// @brief ��������� ��������� ������ "����������: ��������".
extern void Menu_RenderView_CalibMax(void);

/// @brief ��������� ��������� ������ "����������: �������� - ��������������".
extern void Menu_RenderView_CalibMaxEdit(void);

/// @brief ��������� ��������� ������ "�������� �����: �������".
extern void Menu_RenderView_InputRangeMin(void);

/// @brief ��������� ��������� ������ "�������� �����: ������� - ��������������".
extern void Menu_RenderView_InputRangeMinEdit(void);

/// @brief ��������� ��������� ������ "�������� �����: ��������".
extern void Menu_RenderView_InputRangeMax(void);

/// @brief ��������� ��������� ������ "�������� �����: �������� - ��������������".
extern void Menu_RenderView_InputRangeMaxEdit(void);

/// @brief ��������� ��������� ������ "������� ��������: �������".
extern void Menu_RenderView_TightClosingMin(void);

/// @brief ��������� ��������� ������ "������� ��������: ������� - ��������������".
extern void Menu_RenderView_TightClosingMinEdit(void);

/// @brief ��������� ��������� ������ "������� ��������: ��������".
extern void Menu_RenderView_TightClosingMax(void);

/// @brief ��������� ��������� ������ "������� ��������: �������� - ��������������".
extern void Menu_RenderView_TightClosingMaxEdit(void);

/// @brief ��������� ��������� ������ "�������� ����: �������� �������".
extern void Menu_RenderView_StrokeSpeedPump(void);

/// @brief ��������� ��������� ������ "�������� ����: �������� ������� - ��������������".
extern void Menu_RenderView_StrokeSpeedPumpEdit(void);

/// @brief ��������� ��������� ������ "�������� ����: �������� ������".
extern void Menu_RenderView_StrokeSpeedBleed(void);

/// @brief ��������� ��������� ������ "�������� ����: �������� ������ - ��������������".
extern void Menu_RenderView_StrokeSpeedBleedEdit(void);

/// @brief ��������� ��������� ������ "�������� ����: ����������� �������".
extern void Menu_RenderView_StrokeSpeedLimit(void);

/// @brief ��������� ��������� ������ "�������� ����: ����������� ������� - ��������������".
extern void Menu_RenderView_StrokeSpeedLimitEdit(void);

/// @brief ��������� ��������� ������ "���: ��".
extern void Menu_RenderView_PidKp(void);

/// @brief ��������� ��������� ������ "���: �� - ��������������".
extern void Menu_RenderView_PidKpEdit(void);

/// @brief ��������� ��������� ������ "���: ��".
extern void Menu_RenderView_PidKd(void);

/// @brief ��������� ��������� ������ "���: �� - ��������������".
extern void Menu_RenderView_PidKdEdit(void);

/// @brief ��������� ��������� ������ "���: ��".
extern void Menu_RenderView_PidKi(void);

/// @brief ��������� ��������� ������ "���: �� - ��������������".
extern void Menu_RenderView_PidKiEdit(void);

/// @brief ��������� ��������� ������ "���������� ���������: ����������".
extern void Menu_RenderView_ExpertCalib(void);

/// @brief ��������� ��������� ������ "���������� ���������: ����".
extern void Menu_RenderView_ExpertTest(void);

/// @brief ��������� ��������� ������ "���������� ���������: ���".
extern void Menu_RenderView_ExpertLpf(void);

/// @brief ��������� ��������� ������ "���������� ���������: ������������".
extern void Menu_RenderView_ExpertLinearization(void);

/// @brief ��������� ��������� ������ "���������� ���������: �������".
extern void Menu_RenderView_ExpertDisplay(void);

/// @brief ��������� ��������� ������ "���������� ���������: ������� - ��������������".
extern void Menu_RenderView_ExpertDisplayEdit(void);

/// @brief ��������� ��������� ������ "���������� ���������: ������������� ����������".
extern void Menu_RenderView_ExpertTempVersion(void);

/// @brief ��������� ��������� ������ "���������� ���������: ������������� ���������� - ��������������".
extern void Menu_RenderView_ExpertTempVersionEdit(void);

/// @brief ��������� ��������� ������ "���������� ���������: ���� ���������� �".
extern void Menu_RenderView_ExpertAccumZone(void);

/// @brief ��������� ��������� ������ "���������� ���������: ���� ���������� � - ��������������".
extern void Menu_RenderView_ExpertAccumZoneEdit(void);

/// @brief ��������� ��������� ������ "���������� ���������: ������ ����� �������".
extern void Menu_RenderView_ExpertFactoryReset(void);

/// @brief ��������� ��������� ������ "���������� ���������: ������ ����� ������� - ��������������".
extern void Menu_RenderView_ExpertFactoryResetEdit(void);

/// @brief ��������� ��������� ������ "���������� �����: ������".
extern void Menu_RenderView_AoSignal(void);

/// @brief ��������� ��������� ������ "���������� �����: ����������� ���".
extern void Menu_RenderView_AoMin(void);

/// @brief ��������� ��������� ������ "���������� �����: ������������ ���".
extern void Menu_RenderView_AoMax(void);

/// @brief ��������� ��������� ������ "���������� �����: ������ ��������".
extern void Menu_RenderView_AoLower(void);

/// @brief ��������� ��������� ������ "���������� �����: ������� ��������".
extern void Menu_RenderView_AoUpper(void);

/// @brief ��������� ��������� ������ "���������� ����� 1: ������".
extern void Menu_RenderView_Do1Signal(void);

/// @brief ��������� ��������� ������ "���������� ����� 2: ������".
extern void Menu_RenderView_Do2Signal(void);

/// @brief ��������� ��������� ������ "���������� ����� 2: ����������".
extern void Menu_RenderView_Do2Hysteresis(void);

/// @brief ��������� ��������� ������ "���������� ����� 2: �����������".
extern void Menu_RenderView_Do2Direction(void);

/// @brief ��������� ��������� ������ "���������� ����� 2: ���������".
extern void Menu_RenderView_Do2Position(void);

/// @brief ��������� ��������� ������ "���������� ����� 3: ������".
extern void Menu_RenderView_Do3Signal(void);

/// @brief ��������� ��������� ������ "���������� ����� 3: ����������".
extern void Menu_RenderView_Do3Hysteresis(void);

/// @brief ��������� ��������� ������ "���������� ����� 3: �����������".
extern void Menu_RenderView_Do3Direction(void);

/// @brief ��������� ��������� ������ "���������� ����� 3: ���������".
extern void Menu_RenderView_Do3Position(void);

/// @brief ��������� ��������� ������ "PST-����: ������� ����".
extern void Menu_RenderView_PsStroke(void);

/// @brief ��������� ��������� ������ "PST-����: ������ �����".
extern void Menu_RenderView_PstRun(void);

/// @brief ��������� ��������� ������ "PST-����: ����������, ���".
extern void Menu_RenderView_PstAutorun(void);

/// @brief ��������� ��������� ������ "PST-����: �������� ������, �".
extern void Menu_RenderView_PstErrorDelay(void);

/// @brief ��������� ��������� ������ "PST-����: ����� �����, �".
extern void Menu_RenderView_PstTime(void);

/// @brief ��������� ��������� ������ "������ �����������: �������� �����".
extern void Menu_RenderView_BreakawayFrictionRange(void);

/// @brief ��������� ��������� ������ "������ �����������: ���������� ��������".
extern void Menu_RenderView_BreakawayFrictionCount(void);

/// @brief ��������� ��������� ������ "������ �����������: ������ �����".
extern void Menu_RenderView_BreakawayFrictionRun(void);

/// @brief ��������� ��������� ������ "������ �����������: ������������ ������".
extern void Menu_RenderView_BreakawayFrictionData(void);

/// @brief ��������� ��������� ������ "������� ������: ���������������".
extern void Menu_RenderView_HighFrictionError(void);

/// @brief ��������� ��������� ������ "������� ������: �����".
extern void Menu_RenderView_HighFrictionTime(void);

/// @brief ��������� ��������� ������ "����������: ������ ���������".
extern void Menu_RenderView_PositionSensor(void);

/// @brief ��������� ��������� ������ "����������: ������� ����".
extern void Menu_RenderView_CalibCurrentInput(void);

/// @brief ��������� ��������� ������ "����������: ������� �����".
extern void Menu_RenderView_CalibCurrentOutput(void);

/// @brief ��������� ��������� ������ "����: ���������� �����".
extern void Menu_RenderView_TestAO(void);

/// @brief ��������� ��������� ������ "����: ���������� ����� - ��������������".
extern void Menu_RenderView_TestAOEdit(void);

/// @brief ��������� ��������� ������ "����: ���������� ����� 1".
extern void Menu_RenderView_TestDO1(void);

/// @brief ��������� ��������� ������ "����: ���������� ����� 1 - ��������������".
extern void Menu_RenderView_TestDO1Edit(void);

/// @brief ��������� ��������� ������ "����: ���������� ����� 2".
extern void Menu_RenderView_TestDO2(void);

/// @brief ��������� ��������� ������ "����: ���������� ����� 2 - ��������������".
extern void Menu_RenderView_TestDO2Edit(void);

/// @brief ��������� ��������� ������ "����: ���������� ����� 3".
extern void Menu_RenderView_TestDO3(void);

/// @brief ��������� ��������� ������ "����: ���������� ����� 3 - ��������������".
extern void Menu_RenderView_TestDO3Edit(void);

/// @brief ��������� ��������� ������ "���: ����".
extern void Menu_RenderView_LpfInput(void);

/// @brief ��������� ��������� ������ "���: ���� - ��������������".
extern void Menu_RenderView_LpfInputEdit(void);

/// @brief ��������� ��������� ������ "���: �����".
extern void Menu_RenderView_LpfOutput(void);

/// @brief ��������� ��������� ������ "���: ����� - ��������������".
extern void Menu_RenderView_LpfOutputEdit(void);

/// @brief ��������� ��������� ������ "������������: �����������".
extern void Menu_RenderView_LinearizationCoef(void);

/// @brief ��������� ��������� ������ "������������: ����������� - ��������������".
extern void Menu_RenderView_LinearizationCoefEdit(void);

/// @brief ��������� ��������� ������ "������������: ����� ����".
extern void Menu_RenderView_LinearizationZeroShift(void);

/// @brief ��������� ��������� ������ "������������: ����� ���� - ��������������".
extern void Menu_RenderView_LinearizationZeroShiftEdit(void);

/// @brief ��������� ��������� ������ "������ ���������: ��� �������".
extern void Menu_RenderView_PositionSensorType(void);

/// @brief ��������� ��������� ������ "������ ���������: ��� ������� - ��������������".
extern void Menu_RenderView_PositionSensorTypeEdit(void);

/// @brief ��������� ��������� ������ "������ ���������: �������� �������".
extern void Menu_RenderView_PositionSensorMin(void);

/// @brief ��������� ��������� ������ "������ ��������� - �������: �������������".
extern void Menu_RenderView_PositionSensorMin_Auto(void);

/// @brief ��������� ��������� ������ "������ ��������� - �������: ������������� - ��������������".
extern void Menu_RenderView_PositionSensorMin_AutoEdit(void);

/// @brief ��������� ��������� ������ "������ ��������� - �������: �������".
extern void Menu_RenderView_PositionSensorMin_Manual(void);

/// @brief ��������� ��������� ������ "������ ��������� - �������: ������� - ��������������".
extern void Menu_RenderView_PositionSensorMin_ManualEdit(void);

/// @brief ��������� ��������� ������ "������ ���������: �������� ��������".
extern void Menu_RenderView_PositionSensorMax(void);

/// @brief ��������� ��������� ������ "������ ��������� - ��������: �������������".
extern void Menu_RenderView_PositionSensorMax_Auto(void);

/// @brief ��������� ��������� ������ "������ ��������� - ��������: ������������� - ��������������".
extern void Menu_RenderView_PositionSensorMax_AutoEdit(void);

/// @brief ��������� ��������� ������ "������ ��������� - ��������: �������".
extern void Menu_RenderView_PositionSensorMax_Manual(void);

/// @brief ��������� ��������� ������ "������ ��������� - ��������: ������� - ��������������".
extern void Menu_RenderView_PositionSensorMax_ManualEdit(void);

/// @brief ��������� ��������� ������ "������� ����: 4��".
extern void Menu_RenderView_CurrentInput4mA(void);

/// @brief ��������� ��������� ������ "������� ����: 20��".
extern void Menu_RenderView_CurrentInput20mA(void);

/// @brief ��������� ��������� ������ "������� �����: 4��".
extern void Menu_RenderView_CurrentOutput4mA(void);

/// @brief ��������� ��������� ������ "������� �����: 4�� - ��������������".
extern void Menu_RenderView_CurrentOutput4mAEdit(void);

/// @brief ��������� ��������� ������ "������� �����: 20��".
extern void Menu_RenderView_CurrentOutput20mA(void);

/// @brief ��������� ��������� ������ "������� �����: 20�� - ��������������".
extern void Menu_RenderView_CurrentOutput20mAEdit(void);

/// @brief ��������� ��������� ������ "������� ���� 4��: ������".
extern void Menu_RenderView_CurrentInput4mA_Auto(void);

/// @brief ��������� ��������� ������ "������� ���� 4��: ������ - ��������������".
extern void Menu_RenderView_CurrentInput4mA_AutoEdit(void);

/// @brief ��������� ��������� ������ "������� ���� 4��: �������".
extern void Menu_RenderView_CurrentInput4mA_Manual(void);

/// @brief ��������� ��������� ������ "������� ���� 4��: ������� - ��������������".
extern void Menu_RenderView_CurrentInput4mA_ManualEdit(void);

/// @brief ��������� ��������� ������ "������� ���� 20��: ������".
extern void Menu_RenderView_CurrentInput20mA_Auto(void);

/// @brief ��������� ��������� ������ "������� ���� 20��: ������ - ��������������".
extern void Menu_RenderView_CurrentInput20mA_AutoEdit(void);

/// @brief ��������� ��������� ������ "������� ���� 20��: �������".
extern void Menu_RenderView_CurrentInput20mA_Manual(void);

/// @brief ��������� ��������� ������ "������� ���� 20��: ������� - ��������������".
extern void Menu_RenderView_CurrentInput20mA_ManualEdit(void);

#endif // _MENU_RENDER_H_