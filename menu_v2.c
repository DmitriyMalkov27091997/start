/**
 * @file menu_v2.c
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @brief ���������� ����.
 * @version 2.1
 * @date 10.03.2023
 *
 * @copyright (C) ���������, 2023
 */

#include "define.h"
#include "menu_v2.h"
#include "menu_helper.h"
#include "menu_navigate.h"
#include "menu_render.h"

MENU_ITEM(START, NULL_MENU, NULL_MENU, NULL_MENU, HOME, Menu_NavigateByView_Start, Menu_RenderView_Start);            // ���������
MENU_ITEM(SERVICE, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, Menu_NavigateByView_Service, Menu_RenderView_Service); // ��������� ����

MENU_ITEM(HOME, INFO1, INFO3, NULL_MENU, SETUP, Menu_NavigateBase, Menu_RenderView_Home); // ��������
MENU_ITEM(INFO1, INFO2, HOME, HOME, HOME, Menu_NavigateBase, Menu_RenderView_Info1);      // ���������-1
MENU_ITEM(INFO2, INFO3, INFO1, HOME, HOME, Menu_NavigateBase, Menu_RenderView_Info2);     // ���������-2
MENU_ITEM(INFO3, HOME, INFO2, HOME, HOME, Menu_NavigateBase, Menu_RenderView_Info3);      // ���������-3

#pragma region ���������

MENU_ITEM(SETUP, LOG, HAND, HOME, CALIB, Menu_NavigateBase, Menu_RenderView_Setup); // 1. ��������� ��

#pragma region ��������� ��

MENU_ITEM(CALIB, AUTOTUNE, RESET, SETUP, CALIB_TEST, Menu_NavigateBase, Menu_RenderView_Calib); // 1.1. ����������

#pragma region ����������
     
MENU_ITEM(CALIB_TEST, CALIB_MIN, CALIB_MAX, CALIB, CALIB_TEST_EDIT, Menu_NavigateBase, Menu_RenderView_CalibTest);                     // 1.1.1. ����
MENU_ITEM(CALIB_TEST_EDIT, NULL_MENU, NULL_MENU, CALIB_TEST, NULL_MENU, Menu_NavigateByView_CalibTest, Menu_RenderView_CalibTestEdit); // 1.1.1. ���� - ��������������
MENU_ITEM(CALIB_MIN, CALIB_MAX, CALIB_TEST, CALIB, CALIB_MIN_EDIT, Menu_NavigateBase, Menu_RenderView_CalibMin);                       // 1.1.2. �������
MENU_ITEM(CALIB_MIN_EDIT, NULL_MENU, NULL_MENU, CALIB_MIN, NULL_MENU, Menu_NavigateByView_CalibMin, Menu_RenderView_CalibMinEdit);     // 1.1.2. ������� - ��������������
MENU_ITEM(CALIB_MAX, CALIB_TEST, CALIB_MIN, CALIB, CALIB_MAX_EDIT, Menu_NavigateBase, Menu_RenderView_CalibMax);                       // 1.1.3. ��������
MENU_ITEM(CALIB_MAX_EDIT, NULL_MENU, NULL_MENU, CALIB_MAX, NULL_MENU, Menu_NavigateByView_CalibMax, Menu_RenderView_CalibMaxEdit);     // 1.1.3. �������� - ��������������

#pragma endregion

MENU_ITEM(AUTOTUNE, SCALE, CALIB, SETUP, AUTOTUNE_EDIT, Menu_NavigateBase, Menu_RenderView_Autotune);                                                // 1.2. �������������
MENU_ITEM(AUTOTUNE_EDIT, NULL_MENU, NULL_MENU, AUTOTUNE, NULL_MENU, Menu_NavigateByView_Autotune, Menu_RenderView_Autotune);                         // 1.2. ������������� - ��������������
MENU_ITEM(SCALE, LINEARIZATION, AUTOTUNE, SETUP, SCALE_EDIT, Menu_NavigateBase, Menu_RenderView_Scale);                                              // 1.3. �����
MENU_ITEM(SCALE_EDIT, NULL_MENU, NULL_MENU, SCALE, NULL_MENU, Menu_NavigateByView_Scale, Menu_RenderView_ScaleEdit);                                 // 1.3. ����� - ��������������
MENU_ITEM(LINEARIZATION, INPUT_RANGE, SCALE, SETUP, LINEARIZATION_EDIT, Menu_NavigateBase, Menu_RenderView_Linearization);                           // 1.4. ������������
MENU_ITEM(LINEARIZATION_EDIT, NULL_MENU, NULL_MENU, LINEARIZATION, NULL_MENU, Menu_NavigateByView_Linearization, Menu_RenderView_LinearizationEdit); // 1.4. ������������ - ��������������
MENU_ITEM(INPUT_RANGE, TIGHT_CLOSING, LINEARIZATION, SETUP, INPUT_RANGE_MIN, Menu_NavigateBase, Menu_RenderView_InputRange);                         // 1.5. �������� �����

#pragma region �������� �����

MENU_ITEM(INPUT_RANGE_MIN, INPUT_RANGE_MAX, INPUT_RANGE_MAX, INPUT_RANGE, INPUT_RANGE_MIN_EDIT, Menu_NavigateBase, Menu_RenderView_InputRangeMin);       // 1.5.1. �������
MENU_ITEM(INPUT_RANGE_MIN_EDIT, NULL_MENU, NULL_MENU, INPUT_RANGE_MIN, NULL_MENU, Menu_NavigateByView_InputRangeMin, Menu_RenderView_InputRangeMinEdit); // 1.5.1. ������� - ��������������
MENU_ITEM(INPUT_RANGE_MAX, INPUT_RANGE_MIN, INPUT_RANGE_MIN, INPUT_RANGE, INPUT_RANGE_MAX_EDIT, Menu_NavigateBase, Menu_RenderView_InputRangeMax);       // 1.5.2. ��������
MENU_ITEM(INPUT_RANGE_MAX_EDIT, NULL_MENU, NULL_MENU, INPUT_RANGE_MAX, NULL_MENU, Menu_NavigateByView_InputRangeMax, Menu_RenderView_InputRangeMaxEdit); // 1.5.2. �������� - ��������������

#pragma endregion

MENU_ITEM(TIGHT_CLOSING, TRANSFER_CHARACTERISTIC, INPUT_RANGE, SETUP, TIGHT_CLOSING_MIN, Menu_NavigateBase, Menu_RenderView_TightClosing); // 1.6. ������� ��������

#pragma region ������� ��������

MENU_ITEM(TIGHT_CLOSING_MIN, TIGHT_CLOSING_MAX, TIGHT_CLOSING_MAX, TIGHT_CLOSING, TIGHT_CLOSING_MIN_EDIT, Menu_NavigateBase, Menu_RenderView_TightClosingMin);   // 1.6.1. �������
MENU_ITEM(TIGHT_CLOSING_MIN_EDIT, NULL_MENU, NULL_MENU, TIGHT_CLOSING_MIN, NULL_MENU, Menu_NavigateByView_TightClosingMin, Menu_RenderView_TightClosingMinEdit); // 1.6.1. ������� - ��������������
MENU_ITEM(TIGHT_CLOSING_MAX, TIGHT_CLOSING_MIN, TIGHT_CLOSING_MIN, TIGHT_CLOSING, TIGHT_CLOSING_MAX_EDIT, Menu_NavigateBase, Menu_RenderView_TightClosingMax);   // 1.6.2. ��������
MENU_ITEM(TIGHT_CLOSING_MAX_EDIT, NULL_MENU, NULL_MENU, TIGHT_CLOSING_MAX, NULL_MENU, Menu_NavigateByView_TightClosingMax, Menu_RenderView_TightClosingMaxEdit); // 1.6.2. �������� - ��������������

#pragma endregion

MENU_ITEM(TRANSFER_CHARACTERISTIC, STROKE_SPEED, TIGHT_CLOSING, SETUP, TRANSFER_CHARACTERISTIC_EDIT, Menu_NavigateBase, Menu_RenderView_TransferCharacteristic);                           // 1.7. ������������ ��������������
MENU_ITEM(TRANSFER_CHARACTERISTIC_EDIT, NULL_MENU, NULL_MENU, TRANSFER_CHARACTERISTIC, NULL_MENU, Menu_NavigateByView_TransferCharacteristic, Menu_RenderView_TransferCharacteristicEdit); // 1.7. ������������ �������������� - ��������������
MENU_ITEM(STROKE_SPEED, PID, TRANSFER_CHARACTERISTIC, SETUP, STROKE_SPEED_PUMP, Menu_NavigateBase, Menu_RenderView_StrokeSpeed);                                                           // 1.8. �������� ����

#pragma region �������� ����

MENU_ITEM(STROKE_SPEED_PUMP, STROKE_SPEED_BLEED, STROKE_SPEED_LIMIT, STROKE_SPEED, STROKE_SPEED_PUMP_EDIT, Menu_NavigateBase, Menu_RenderView_StrokeSpeedPump);      // 1.8.1. �������� �������
MENU_ITEM(STROKE_SPEED_PUMP_EDIT, NULL_MENU, NULL_MENU, STROKE_SPEED_PUMP, NULL_MENU, Menu_NavigateByView_StrokeSpeedPump, Menu_RenderView_StrokeSpeedPumpEdit);     // 1.8.1. �������� ������� - ��������������
MENU_ITEM(STROKE_SPEED_BLEED, STROKE_SPEED_LIMIT, STROKE_SPEED_PUMP, STROKE_SPEED, STROKE_SPEED_BLEED_EDIT, Menu_NavigateBase, Menu_RenderView_StrokeSpeedBleed);    // 1.8.2. �������� ������
MENU_ITEM(STROKE_SPEED_BLEED_EDIT, NULL_MENU, NULL_MENU, STROKE_SPEED_BLEED, NULL_MENU, Menu_NavigateByView_StrokeSpeedBleed, Menu_RenderView_StrokeSpeedBleedEdit); // 1.8.2. �������� ������ - ��������������
MENU_ITEM(STROKE_SPEED_LIMIT, STROKE_SPEED_PUMP, STROKE_SPEED_BLEED, STROKE_SPEED, STROKE_SPEED_LIMIT_EDIT, Menu_NavigateBase, Menu_RenderView_StrokeSpeedLimit);    // 1.8.3. ����������� �������
MENU_ITEM(STROKE_SPEED_LIMIT_EDIT, NULL_MENU, NULL_MENU, STROKE_SPEED_LIMIT, NULL_MENU, Menu_NavigateByView_StrokeSpeedLimit, Menu_RenderView_StrokeSpeedLimitEdit); // 1.8.3. ����������� ������� - ��������������

#pragma endregion

MENU_ITEM(PID, EXPERT_SETUP, STROKE_SPEED, SETUP, PID_KP, Menu_NavigateBase, Menu_RenderView_PID); // 1.9. ��� ���������

#pragma region ��� ���������

MENU_ITEM(PID_KP, PID_KD, PID_KI, PID, PID_KP_EDIT, Menu_NavigateBase, Menu_RenderView_PidKp);                         // 1.9.1. ��
MENU_ITEM(PID_KP_EDIT, NULL_MENU, NULL_MENU, PID_KP, NULL_MENU, Menu_NavigateByView_PidKp, Menu_RenderView_PidKpEdit); // 1.9.1. �� - ��������������
MENU_ITEM(PID_KD, PID_KI, PID_KP, PID, PID_KD_EDIT, Menu_NavigateBase, Menu_RenderView_PidKd);                         // 1.9.2. ��
MENU_ITEM(PID_KD_EDIT, NULL_MENU, NULL_MENU, PID_KD, NULL_MENU, Menu_NavigateByView_PidKd, Menu_RenderView_PidKdEdit); // 1.9.2. �� - ��������������
MENU_ITEM(PID_KI, PID_KP, PID_KD, PID, PID_KI_EDIT, Menu_NavigateBase, Menu_RenderView_PidKi);                         // 1.9.3. ��
MENU_ITEM(PID_KI_EDIT, NULL_MENU, NULL_MENU, PID_KI, NULL_MENU, Menu_NavigateByView_PidKi, Menu_RenderView_PidKiEdit); // 1.9.3. �� - ��������������

#pragma endregion

MENU_ITEM(EXPERT_SETUP, RESET, PID, SETUP, EXPERT_CALIB, Menu_NavigateBase, Menu_RenderView_ExpertSetup); // 1.10. ���������� ���������

#pragma region ���������� ���������

MENU_ITEM(EXPERT_CALIB, EXPERT_TEST, EXPERT_FACTORY_RESET, EXPERT_SETUP, CALIB_POSITION_SENSOR, Menu_NavigateBase, Menu_RenderView_ExpertCalib); // 1.10.1. ����������

#pragma region ����������

MENU_ITEM(CALIB_POSITION_SENSOR, CALIB_CURRENT_INPUT, CALIB_CURRENT_OUTPUT, EXPERT_CALIB, POSITION_SENSOR_TYPE, Menu_NavigateBase, Menu_RenderView_PositionSensor); // 1.10.1.1. ������ ���������

#pragma region ������ ���������

MENU_ITEM(POSITION_SENSOR_TYPE, POSITION_SENSOR_SET_MIN, POSITION_SENSOR_SET_MAX, CALIB_POSITION_SENSOR, POSITION_SENSOR_TYPE_EDIT, Menu_NavigateBase, Menu_RenderView_PositionSensorType); // 1.10.1.1.1. ������ ���������: ��� �������
MENU_ITEM(POSITION_SENSOR_TYPE_EDIT, NULL_MENU, NULL_MENU, POSITION_SENSOR_TYPE, NULL_MENU, Menu_NavigateByView_SensorType, Menu_RenderView_PositionSensorTypeEdit);                        // 1.10.1.1.1. ������ ���������: ��� ������� - ��������������

MENU_ITEM(POSITION_SENSOR_SET_MIN, POSITION_SENSOR_SET_MAX, POSITION_SENSOR_TYPE, CALIB_POSITION_SENSOR, POSITION_SENSOR_SET_MIN_AUTO, Menu_NavigateBase, Menu_RenderView_PositionSensorMin); // 1.10.1.1.2. ������ ���������: �������

#pragma region ������ ��������� : �������

MENU_ITEM(POSITION_SENSOR_SET_MIN_AUTO, POSITION_SENSOR_SET_MIN_MANUAL, POSITION_SENSOR_SET_MIN_MANUAL, POSITION_SENSOR_SET_MIN, POSITION_SENSOR_SET_MIN_AUTO_EDIT, Menu_NavigateBase, Menu_RenderView_PositionSensorMin_Auto);   // 1.10.1.1.2.1. ������ ��������� - �������: �������������
MENU_ITEM(POSITION_SENSOR_SET_MIN_AUTO_EDIT, NULL_MENU, NULL_MENU, POSITION_SENSOR_SET_MIN_AUTO, NULL_MENU, Menu_NavigateByView_PositionSensorMin_Auto, Menu_RenderView_PositionSensorMin_AutoEdit);                              // 1.10.1.1.2.1. ������ ��������� - �������: ������������� - ��������������
MENU_ITEM(POSITION_SENSOR_SET_MIN_MANUAL, POSITION_SENSOR_SET_MIN_AUTO, POSITION_SENSOR_SET_MIN_AUTO, POSITION_SENSOR_SET_MIN, POSITION_SENSOR_SET_MIN_MANUAL_EDIT, Menu_NavigateBase, Menu_RenderView_PositionSensorMin_Manual); // 1.10.1.1.2.1. ������ ��������� - �������: �������
MENU_ITEM(POSITION_SENSOR_SET_MIN_MANUAL_EDIT, NULL_MENU, NULL_MENU, POSITION_SENSOR_SET_MIN_MANUAL, NULL_MENU, Menu_NavigateByView_PositionSensorMin_Manual, Menu_RenderView_PositionSensorMin_ManualEdit);                      // 1.10.1.1.2.1. ������ ��������� - �������: ������� - ��������������

#pragma endregion

MENU_ITEM(POSITION_SENSOR_SET_MAX, POSITION_SENSOR_TYPE, POSITION_SENSOR_SET_MIN, CALIB_POSITION_SENSOR, POSITION_SENSOR_SET_MAX_AUTO, Menu_NavigateBase, Menu_RenderView_PositionSensorMax); // 1.10.1.1.3. ������ ���������: ��������

#pragma region ������ ��������� : ��������

MENU_ITEM(POSITION_SENSOR_SET_MAX_AUTO, POSITION_SENSOR_SET_MAX_MANUAL, POSITION_SENSOR_SET_MAX_MANUAL, POSITION_SENSOR_SET_MAX, POSITION_SENSOR_SET_MAX_AUTO_EDIT, Menu_NavigateBase, Menu_RenderView_PositionSensorMax_Auto);   // 1.10.1.1.3.1. ������ ��������� - ��������: �������������
MENU_ITEM(POSITION_SENSOR_SET_MAX_AUTO_EDIT, NULL_MENU, NULL_MENU, POSITION_SENSOR_SET_MAX_AUTO, NULL_MENU, Menu_NavigateByView_PositionSensorMax_Auto, Menu_RenderView_PositionSensorMax_AutoEdit);                              // 1.10.1.1.3.1. ������ ��������� - ��������: ������������� - ��������������
MENU_ITEM(POSITION_SENSOR_SET_MAX_MANUAL, POSITION_SENSOR_SET_MAX_AUTO, POSITION_SENSOR_SET_MAX_AUTO, POSITION_SENSOR_SET_MAX, POSITION_SENSOR_SET_MAX_MANUAL_EDIT, Menu_NavigateBase, Menu_RenderView_PositionSensorMax_Manual); // 1.10.1.1.3.2. ������ ��������� - ��������: �������
MENU_ITEM(POSITION_SENSOR_SET_MAX_MANUAL_EDIT, NULL_MENU, NULL_MENU, POSITION_SENSOR_SET_MAX_MANUAL, NULL_MENU, Menu_NavigateByView_PositionSensorMax_Manual, Menu_RenderView_PositionSensorMax_ManualEdit);                      // 1.10.1.1.3.2. ������ ��������� - ��������: ������� - ��������������

#pragma endregion

#pragma endregion

MENU_ITEM(CALIB_CURRENT_INPUT, CALIB_CURRENT_OUTPUT, CALIB_POSITION_SENSOR, EXPERT_CALIB, CURRENT_INPUT_4MA, Menu_NavigateBase, Menu_RenderView_CalibCurrentInput); // 1.10.1.2. ������� ����

#pragma region ������� ����

MENU_ITEM(CURRENT_INPUT_4MA, CURRENT_INPUT_20MA, CURRENT_INPUT_20MA, CALIB_CURRENT_INPUT, CURRENT_INPUT_4MA_AUTO, Menu_NavigateBase, Menu_RenderView_CurrentInput4mA); // 1.10.1.2.1. ������� ����: 4��

#pragma region ������� ���� : 4��

MENU_ITEM(CURRENT_INPUT_4MA_AUTO, CURRENT_INPUT_4MA_MANUAL, CURRENT_INPUT_4MA_MANUAL, CURRENT_INPUT_4MA, CURRENT_INPUT_4MA_AUTO_EDIT, Menu_NavigateBase, Menu_RenderView_CurrentInput4mA_Auto);   // 1.10.1.2.1.1. ������� ���� 4��: ������
MENU_ITEM(CURRENT_INPUT_4MA_AUTO_EDIT, NULL_MENU, NULL_MENU, CURRENT_INPUT_4MA_AUTO, NULL_MENU, Menu_NavigateByView_CurrentInput4mA_Auto, Menu_RenderView_CurrentInput4mA_AutoEdit);              // 1.10.1.2.1.1. ������� ���� 4��: ������ - ��������������
MENU_ITEM(CURRENT_INPUT_4MA_MANUAL, CURRENT_INPUT_4MA_AUTO, CURRENT_INPUT_4MA_AUTO, CURRENT_INPUT_4MA, CURRENT_INPUT_4MA_MANUAL_EDIT, Menu_NavigateBase, Menu_RenderView_CurrentInput4mA_Manual); // 1.10.1.2.1.2. ������� ���� 4��: �������
MENU_ITEM(CURRENT_INPUT_4MA_MANUAL_EDIT, NULL_MENU, NULL_MENU, CURRENT_INPUT_4MA_MANUAL, NULL_MENU, Menu_NavigateByView_CurrentInput4mA_Manual, Menu_RenderView_CurrentInput4mA_ManualEdit);      // 1.10.1.2.1.2. ������� ���� 4��: ������� - ��������������

#pragma endregion

MENU_ITEM(CURRENT_INPUT_20MA, CURRENT_INPUT_4MA, CURRENT_INPUT_4MA, CALIB_CURRENT_INPUT, CURRENT_INPUT_20MA_AUTO, Menu_NavigateBase, Menu_RenderView_CurrentInput20mA); // 1.10.1.2.2. ������� ����: 20��

#pragma region ������� ���� : 20��

MENU_ITEM(CURRENT_INPUT_20MA_AUTO, CURRENT_INPUT_20MA_MANUAL, CURRENT_INPUT_20MA_MANUAL, CURRENT_INPUT_20MA, CURRENT_INPUT_20MA_AUTO_EDIT, Menu_NavigateBase, Menu_RenderView_CurrentInput20mA_Auto);   // 1.10.1.2.2.1. ������� ���� 20��: ������
MENU_ITEM(CURRENT_INPUT_20MA_AUTO_EDIT, NULL_MENU, NULL_MENU, CURRENT_INPUT_20MA_AUTO, NULL_MENU, Menu_NavigateByView_CurrentInput20mA_Auto, Menu_RenderView_CurrentInput20mA_AutoEdit);                // 1.10.1.2.2.1. ������� ���� 20��: ������
MENU_ITEM(CURRENT_INPUT_20MA_MANUAL, CURRENT_INPUT_20MA_AUTO, CURRENT_INPUT_20MA_AUTO, CURRENT_INPUT_20MA, CURRENT_INPUT_20MA_MANUAL_EDIT, Menu_NavigateBase, Menu_RenderView_CurrentInput20mA_Manual); // 1.10.1.2.2.2. ������� ���� 20��: �������
MENU_ITEM(CURRENT_INPUT_20MA_MANUAL_EDIT, NULL_MENU, NULL_MENU, CURRENT_INPUT_20MA_MANUAL, NULL_MENU, Menu_NavigateByView_CurrentInput20mA_Manual, Menu_RenderView_CurrentInput20mA_ManualEdit);        // 1.10.1.2.2.2. ������� ���� 20��: �������

#pragma endregion

#pragma endregion

MENU_ITEM(CALIB_CURRENT_OUTPUT, CALIB_POSITION_SENSOR, CALIB_CURRENT_INPUT, EXPERT_CALIB, CURRENT_OUTPUT_4MA, Menu_NavigateBase, Menu_RenderView_CalibCurrentOutput); // 1.10.1.3. ������� �����

#pragma region ������� �����

MENU_ITEM(CURRENT_OUTPUT_4MA, CURRENT_OUTPUT_20MA, CURRENT_OUTPUT_20MA, CALIB_CURRENT_OUTPUT, CURRENT_OUTPUT_4MA_EDIT, Menu_NavigateBase, Menu_RenderView_CurrentOutput4mA);  // 1.10.1.3.1. ������� �����: 4��
MENU_ITEM(CURRENT_OUTPUT_4MA_EDIT, NULL_MENU, NULL_MENU, CURRENT_OUTPUT_4MA, NULL_MENU, Menu_NavigateByView_CurrentOutput4mA, Menu_RenderView_CurrentOutput4mAEdit);          // 1.10.1.3.1. ������� �����: 4��
MENU_ITEM(CURRENT_OUTPUT_20MA, CURRENT_OUTPUT_4MA, CURRENT_OUTPUT_4MA, CALIB_CURRENT_OUTPUT, CURRENT_OUTPUT_20MA_EDIT, Menu_NavigateBase, Menu_RenderView_CurrentOutput20mA); // 1.10.1.3.2. ������� �����: 20��
MENU_ITEM(CURRENT_OUTPUT_20MA_EDIT, NULL_MENU, NULL_MENU, CURRENT_OUTPUT_20MA, NULL_MENU, Menu_NavigateByView_CurrentOutput20mA, Menu_RenderView_CurrentOutput20mAEdit);      // 1.10.1.3.2. ������� �����: 20��

#pragma endregion

#pragma endregion

MENU_ITEM(EXPERT_TEST, EXPERT_LPF, EXPERT_CALIB, EXPERT_SETUP, TEST_AO, Menu_NavigateBase, Menu_RenderView_ExpertTest); // 1.10.2. ����

#pragma region ����

MENU_ITEM(TEST_AO, TEST_DO1, TEST_DO3, EXPERT_TEST, TEST_AO_EDIT, Menu_NavigateBase, Menu_RenderView_TestAO);                  // 1.10.2.1. ����: ���������� �����
MENU_ITEM(TEST_AO_EDIT, NULL_MENU, NULL_MENU, TEST_AO, NULL_MENU, Menu_NavigateByView_TestAO, Menu_RenderView_TestAOEdit);     // 1.10.2.1. ����: ���������� ����� - ��������������
MENU_ITEM(TEST_DO1, TEST_DO2, TEST_AO, EXPERT_TEST, TEST_DO1_EDIT, Menu_NavigateBase, Menu_RenderView_TestDO1);                // 1.10.2.2. ����: ���������� ����� 1
MENU_ITEM(TEST_DO1_EDIT, NULL_MENU, NULL_MENU, TEST_DO1, NULL_MENU, Menu_NavigateByView_TestDO1, Menu_RenderView_TestDO1Edit); // 1.10.2.2. ����: ���������� ����� 1 - ��������������
MENU_ITEM(TEST_DO2, TEST_DO3, TEST_DO1, EXPERT_TEST, TEST_DO2_EDIT, Menu_NavigateBase, Menu_RenderView_TestDO2);               // 1.10.2.3. ����: ���������� ����� 2
MENU_ITEM(TEST_DO2_EDIT, NULL_MENU, NULL_MENU, TEST_DO2, NULL_MENU, Menu_NavigateByView_TestDO2, Menu_RenderView_TestDO2Edit); // 1.10.2.3. ����: ���������� ����� 2 - ��������������
MENU_ITEM(TEST_DO3, TEST_AO, TEST_DO2, EXPERT_TEST, TEST_DO3_EDIT, Menu_NavigateBase, Menu_RenderView_TestDO3);                // 1.10.2.4. ����: ���������� ����� 3
MENU_ITEM(TEST_DO3_EDIT, NULL_MENU, NULL_MENU, TEST_DO3, NULL_MENU, Menu_NavigateByView_TestDO3, Menu_RenderView_TestDO3Edit); // 1.10.2.4. ����: ���������� ����� 3 - ��������������

#pragma endregion

MENU_ITEM(EXPERT_LPF, EXPERT_LINEARIZATION, EXPERT_TEST, EXPERT_SETUP, LPF_INPUT, Menu_NavigateBase, Menu_RenderView_ExpertLpf); // 1.10.3. ���

#pragma region ���

MENU_ITEM(LPF_INPUT, LPF_OUTPUT, LPF_OUTPUT, EXPERT_LPF, LPF_INPUT_EDIT, Menu_NavigateBase, Menu_RenderView_LpfInput);                 // 1.10.3.1. ���: ����
MENU_ITEM(LPF_INPUT_EDIT, NULL_MENU, NULL_MENU, LPF_INPUT, NULL_MENU, Menu_NavigateByView_LpfInput, Menu_RenderView_LpfInputEdit);     // 1.10.3.1. ���: ���� - ��������������
MENU_ITEM(LPF_OUTPUT, LPF_INPUT, LPF_INPUT, EXPERT_LPF, LPF_OUTPUT_EDIT, Menu_NavigateBase, Menu_RenderView_LpfOutput);                // 1.10.3.2. ���: �����
MENU_ITEM(LPF_OUTPUT_EDIT, NULL_MENU, NULL_MENU, LPF_OUTPUT, NULL_MENU, Menu_NavigateByView_LpfOutput, Menu_RenderView_LpfOutputEdit); // 1.10.3.2. ���: ����� - ��������������

#pragma endregion

MENU_ITEM(EXPERT_LINEARIZATION, EXPERT_DISPLAY, EXPERT_LPF, EXPERT_SETUP, LINEARIZATION_COEF, Menu_NavigateBase, Menu_RenderView_ExpertLinearization); // 1.10.4. ������������

#pragma region ������������

MENU_ITEM(LINEARIZATION_COEF, LINEARIZATION_ZERO_SHIFT, LINEARIZATION_ZERO_SHIFT, EXPERT_LINEARIZATION, LINEARIZATION_COEF_EDIT, Menu_NavigateBase, Menu_RenderView_LinearizationCoef);      // 1.10.4.1. �����������
MENU_ITEM(LINEARIZATION_COEF_EDIT, NULL_MENU, NULL_MENU, LINEARIZATION_COEF, NULL_MENU, Menu_NavigateByView_LinearizationCoef, Menu_RenderView_LinearizationCoefEdit);                       // 1.10.4.1. ����������� - ��������������
MENU_ITEM(LINEARIZATION_ZERO_SHIFT, LINEARIZATION_COEF, LINEARIZATION_COEF, EXPERT_LINEARIZATION, LINEARIZATION_ZERO_SHIFT_EDIT, Menu_NavigateBase, Menu_RenderView_LinearizationZeroShift); // 1.10.4.2. ����� ����
MENU_ITEM(LINEARIZATION_ZERO_SHIFT_EDIT, NULL_MENU, NULL_MENU, LINEARIZATION_ZERO_SHIFT, NULL_MENU, Menu_NavigateByView_LinearizationZeroShift, Menu_RenderView_LinearizationZeroShiftEdit); // 1.10.4.2. ����� ���� - ��������������

#pragma endregion

MENU_ITEM(EXPERT_DISPLAY, EXPERT_TEMP_VERSION, EXPERT_LINEARIZATION, EXPERT_SETUP, EXPERT_DISPLAY_EDIT, Menu_NavigateBase, Menu_RenderView_ExpertDisplay);                   // 1.10.5. �������
MENU_ITEM(EXPERT_DISPLAY_EDIT, NULL_MENU, NULL_MENU, EXPERT_DISPLAY, NULL_MENU, Menu_NavigateByView_ExpertDisplay, Menu_RenderView_ExpertDisplayEdit);                       // 1.10.5. ������� - ��������������
MENU_ITEM(EXPERT_TEMP_VERSION, EXPERT_ACCUM_ZONE, EXPERT_DISPLAY, EXPERT_SETUP, EXPERT_TEMP_VERSION_EDIT, Menu_NavigateBase, Menu_RenderView_ExpertTempVersion);             // 1.10.6. ������������� ����������
MENU_ITEM(EXPERT_TEMP_VERSION_EDIT, NULL_MENU, NULL_MENU, EXPERT_TEMP_VERSION, NULL_MENU, Menu_NavigateByView_ExpertTempVersion, Menu_RenderView_ExpertTempVersionEdit);     // 1.10.6. ������������� ���������� - ��������������
MENU_ITEM(EXPERT_ACCUM_ZONE, EXPERT_FACTORY_RESET, EXPERT_TEMP_VERSION, EXPERT_SETUP, EXPERT_ACCUM_ZONE_EDIT, Menu_NavigateBase, Menu_RenderView_ExpertAccumZone);           // 1.10.7. ���� ���������� �
MENU_ITEM(EXPERT_ACCUM_ZONE_EDIT, NULL_MENU, NULL_MENU, EXPERT_ACCUM_ZONE, NULL_MENU, Menu_NavigateByView_ExpertAccumZone, Menu_RenderView_ExpertAccumZoneEdit);             // 1.10.7. ���� ���������� � - ��������������
MENU_ITEM(EXPERT_FACTORY_RESET, EXPERT_CALIB, EXPERT_ACCUM_ZONE, EXPERT_SETUP, EXPERT_FACTORY_RESET_EDIT, Menu_NavigateBase, Menu_RenderView_ExpertFactoryReset);            // 1.10.8. ����� � ��������� ����������
MENU_ITEM(EXPERT_FACTORY_RESET_EDIT, NULL_MENU, NULL_MENU, EXPERT_FACTORY_RESET, NULL_MENU, Menu_NavigateByView_ExpertFactoryReset, Menu_RenderView_ExpertFactoryResetEdit); // 1.10.8. ����� � ��������� ���������� - ��������������

#pragma endregion

MENU_ITEM(RESET, CALIB, EXPERT_SETUP, SETUP, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Reset);      // 1.11. ������ ����� ���������������� ��������
MENU_ITEM(RESET_EDIT, NULL_MENU, NULL_MENU, SETUP, NULL_MENU, Menu_NavigateByView_Reset, Menu_RenderView_ResetEdit); // 1.11. ������ ����� ���������������� �������� - ��������������

#pragma endregion

MENU_ITEM(LOG, SIGNAL, SETUP, HOME, VIEW_LOG, Menu_NavigateBase, Menu_RenderView_Log); // 2. ������ �������

#pragma region ������ �������

#pragma region �������� ������� �������

MENU_ITEM(VIEW_LOG, CLEARING_LOG, CLEARING_LOG, LOG, VIEW_LOG_EDIT, Menu_NavigateBase, Menu_RenderView_ViewLog); // 2.1. �������� ������� �������
MENU_ITEM(VIEW_LOG_EDIT, NULL_MENU, NULL_MENU, VIEW_LOG, NULL_MENU, Menu_NavigateByView_ViewLogEdit, Menu_RenderView_ViewLogEdit); // 2.1.1. ���������� ��������� ������� �������
#pragma endregion 

#pragma region ������� ������� �������

MENU_ITEM(CLEARING_LOG, VIEW_LOG, VIEW_LOG, LOG, CLEARING_LOG_EDIT, Menu_NavigateBase, Menu_RenderView_ClearingLog); // 2.2. ������� ������� �������
MENU_ITEM(CLEARING_LOG_EDIT, NULL_MENU, NULL_MENU, CLEARING_LOG, NULL_MENU, Menu_NavigateByView_ClearingLogEdit, Menu_RenderView_ClearingLogEdit); // 2.2.1. ������� ������� �������

#pragma endregion

#pragma endregion

MENU_ITEM(SIGNAL, DIAG, LOG, HOME, AO, Menu_NavigateBase, Menu_RenderView_Signal); // 3. ������������

#pragma region ������������

MENU_ITEM(AO, DO1, DO3, SIGNAL, AO_SIGNAL, Menu_NavigateBase, Menu_RenderView_AO); // 3.1. ���������� �����

#pragma region ���������� �����

MENU_ITEM(AO_SIGNAL, AO_MIN, AO_UPPER, AO, NULL_MENU, Menu_NavigateBase, Menu_RenderView_AoSignal);  // 3.1.1. ������
MENU_ITEM(AO_MIN, AO_MAX, AO_SIGNAL, AO, NULL_MENU, Menu_NavigateBase, Menu_RenderView_AoMin);       // 3.1.2. ����������� ���
MENU_ITEM(AO_MAX, AO_LOWER, AO_MIN, AO, NULL_MENU, Menu_NavigateBase, Menu_RenderView_AoMax);        // 3.1.3. ������������ ���
MENU_ITEM(AO_LOWER, AO_UPPER, AO_MAX, AO, NULL_MENU, Menu_NavigateBase, Menu_RenderView_AoLower);    // 3.1.4. ������ ��������
MENU_ITEM(AO_UPPER, AO_SIGNAL, AO_LOWER, AO, NULL_MENU, Menu_NavigateBase, Menu_RenderView_AoUpper); // 3.1.5. ������� ��������

#pragma endregion

MENU_ITEM(DO1, DO2, AO, SIGNAL, DO1_SIGNAL, Menu_NavigateBase, Menu_RenderView_DO1); // 3.2. ���������� ����� 1

#pragma region ���������� ����� 1

MENU_ITEM(DO1_SIGNAL, NULL_MENU, NULL_MENU, DO1, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do1Signal); // 3.2.1. ������

#pragma endregion

MENU_ITEM(DO2, DO3, DO1, SIGNAL, DO2_SIGNAL, Menu_NavigateBase, Menu_RenderView_DO2); // 3.3. ���������� ����� 2

#pragma region ���������� ����� 2

MENU_ITEM(DO2_SIGNAL, DO2_HYSTERESIS, DO2_POSITION, DO2, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do2Signal);       // 3.3.1. ������
MENU_ITEM(DO2_HYSTERESIS, DO2_DIRECTION, DO2_SIGNAL, DO2, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do2Hysteresis);  // 3.3.2. ����������
MENU_ITEM(DO2_DIRECTION, DO2_POSITION, DO2_HYSTERESIS, DO2, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do2Direction); // 3.3.3. �����������
MENU_ITEM(DO2_POSITION, DO2_SIGNAL, DO2_DIRECTION, DO2, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do2Position);      // 3.3.4. ���������

#pragma endregion

MENU_ITEM(DO3, AO, DO2, SIGNAL, DO3_SIGNAL, Menu_NavigateBase, Menu_RenderView_DO3); // 3.4. ���������� ����� 3

#pragma region ���������� ����� 3

MENU_ITEM(DO3_SIGNAL, DO3_HYSTERESIS, DO3_POSITION, DO3, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do3Signal);       // 3.4.1. ������
MENU_ITEM(DO3_HYSTERESIS, DO3_DIRECTION, DO3_SIGNAL, DO3, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do3Hysteresis);  // 3.4.2. ����������
MENU_ITEM(DO3_DIRECTION, DO3_POSITION, DO3_HYSTERESIS, DO3, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do3Direction); // 3.4.3. �����������
MENU_ITEM(DO3_POSITION, DO3_SIGNAL, DO3_DIRECTION, DO3, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Do3Position);      // 3.4.4. ���������

#pragma endregion

#pragma endregion

MENU_ITEM(DIAG, HART, SIGNAL, HOME, PST, Menu_NavigateBase, Menu_RenderView_Diag); // 4. �����������

#pragma region �����������

MENU_ITEM(PST, BREAKAWAY_FRICTION, HIGH_FRICTION, DIAG, PST_STROKE, Menu_NavigateBase, Menu_RenderView_PST); // 4.1. PST-����

#pragma region PST -����

MENU_ITEM(PST_STROKE, PST_RUN, PST_TIME, PST, NULL_MENU, Menu_NavigateBase, Menu_RenderView_PsStroke);               // 4.1.1. ������� ����
MENU_ITEM(PST_RUN, PST_AUTORUN, PST_STROKE, PST, NULL_MENU, Menu_NavigateBase, Menu_RenderView_PstRun);              // 4.1.2. ������ �����
MENU_ITEM(PST_AUTORUN, PST_ERROR_DELAY, PST_RUN, PST, NULL_MENU, Menu_NavigateBase, Menu_RenderView_PstAutorun);     // 4.1.3. ����������, ���
MENU_ITEM(PST_ERROR_DELAY, PST_TIME, PST_AUTORUN, PST, NULL_MENU, Menu_NavigateBase, Menu_RenderView_PstErrorDelay); // 4.1.4. �������� ������, �
MENU_ITEM(PST_TIME, PST_STROKE, PST_ERROR_DELAY, PST, NULL_MENU, Menu_NavigateBase, Menu_RenderView_PstTime);        // 4.1.5. ����� �����, �

#pragma endregion

MENU_ITEM(BREAKAWAY_FRICTION, HISTOGRAMS, PST, DIAG, BREAKAWAY_FRICTION_RANGE, Menu_NavigateBase, Menu_RenderView_BreakawayFriction); // 4.2. ������ �����������

#pragma region ������ �����������

MENU_ITEM(BREAKAWAY_FRICTION_RANGE, BREAKAWAY_FRICTION_COUNT, BREAKAWAY_FRICTION_DATA, BREAKAWAY_FRICTION, NULL_MENU, Menu_NavigateBase, Menu_RenderView_BreakawayFrictionRange); // 4.2.1. �������� �����
MENU_ITEM(BREAKAWAY_FRICTION_COUNT, BREAKAWAY_FRICTION_RUN, BREAKAWAY_FRICTION_RANGE, BREAKAWAY_FRICTION, NULL_MENU, Menu_NavigateBase, Menu_RenderView_BreakawayFrictionCount);  // 4.2.2. ���������� ��������
MENU_ITEM(BREAKAWAY_FRICTION_RUN, BREAKAWAY_FRICTION_DATA, BREAKAWAY_FRICTION_COUNT, BREAKAWAY_FRICTION, NULL_MENU, Menu_NavigateBase, Menu_RenderView_BreakawayFrictionRun);     // 4.2.3. ������
MENU_ITEM(BREAKAWAY_FRICTION_DATA, BREAKAWAY_FRICTION_RANGE, BREAKAWAY_FRICTION_RUN, BREAKAWAY_FRICTION, NULL_MENU, Menu_NavigateBase, Menu_RenderView_BreakawayFrictionData);    // 4.2.4. ������������ ������

#pragma endregion

MENU_ITEM(HISTOGRAMS, HIGH_FRICTION, BREAKAWAY_FRICTION, DIAG, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Histograms); // 4.3. �����������
MENU_ITEM(HIGH_FRICTION, PST, HISTOGRAMS, DIAG, HIGH_FRICTION_ERROR, Menu_NavigateBase, Menu_RenderView_HighFriction);    // 4.4. ������� ������

#pragma region ������� ������

MENU_ITEM(HIGH_FRICTION_ERROR, HIGH_FRICTION_TIME, HIGH_FRICTION_TIME, HIGH_FRICTION, NULL_MENU, Menu_NavigateBase, Menu_RenderView_HighFrictionError); // 4.4.1. ���������������, %
MENU_ITEM(HIGH_FRICTION_TIME, HIGH_FRICTION_ERROR, HIGH_FRICTION_ERROR, HIGH_FRICTION, NULL_MENU, Menu_NavigateBase, Menu_RenderView_HighFrictionTime); // 4.4.2. �����, �

#pragma endregion

#pragma endregion

MENU_ITEM(HART, DISPLAY, DIAG, HOME, HART_IDENTIFIER, Menu_NavigateBase, Menu_RenderView_Hart); // 5. HART

#pragma region

MENU_ITEM(HART_IDENTIFIER, HART_PRIORITY, HART_SHUTDOWN, HART, NULL_MENU, Menu_NavigateBase, Menu_RenderView_HartIdentifier); // 5.1. �������������
MENU_ITEM(HART_PRIORITY, HART_SHUTDOWN, HART_IDENTIFIER, HART, NULL_MENU, Menu_NavigateBase, Menu_RenderView_HartPriority);   // 5.2. ���������
MENU_ITEM(HART_SHUTDOWN, HART_IDENTIFIER, HART_PRIORITY, HART, NULL_MENU, Menu_NavigateBase, Menu_RenderView_HartShutdown);   // 5.3. ����������

#pragma endregion

MENU_ITEM(DISPLAY, HAND, HART, HOME, UNITS_IM, Menu_NavigateBase, Menu_RenderView_Display); // 6. �������

#pragma region

MENU_ITEM(UNITS_IM, UNITS_INPUT, PC_SMOORHING, DISPLAY, NULL_MENU, Menu_NavigateBase, Menu_RenderView_UnitsIM);            // 6.1. ������� ��
MENU_ITEM(UNITS_INPUT, CONTRAST, UNITS_IM, DISPLAY, NULL_MENU, Menu_NavigateBase, Menu_RenderView_UnitsInput);             // 6.2. ������� �����
MENU_ITEM(CONTRAST, SCREEN_ROTATION, UNITS_INPUT, DISPLAY, NULL_MENU, Menu_NavigateBase, Menu_RenderView_Contrast);        // 6.3. �������������
MENU_ITEM(SCREEN_ROTATION, PC_SMOORHING, CONTRAST, DISPLAY, NULL_MENU, Menu_NavigateBase, Menu_RenderView_ScreenRotation); // 6.4. ������� ������
MENU_ITEM(PC_SMOORHING, UNITS_IM, SCREEN_ROTATION, DISPLAY, NULL_MENU, Menu_NavigateBase, Menu_RenderView_PcSmoothing);    // 6.5. ����������� ��

#pragma endregion

MENU_ITEM(HAND, SETUP, DISPLAY, HOME, HAND_ENABLE, Menu_NavigateBase, Menu_RenderView_Hand);                               // 7. ������ �����
MENU_ITEM(HAND_ENABLE, NULL_MENU, NULL_MENU, HOME, NULL_MENU, Menu_NavigateByView_HandEnable, Menu_RenderView_HandEnable); // 7. ������ �����

#pragma endregion

sMENU_BTN menu_btn;
eMENU_CALIB_PWM menu_calib_pwm = MENU_CALIB_PWM_NONE;

volatile sMENU_DATA menu_data;
int i_old;
volatile int menu_hand_pos_t = 0;
volatile int menu_switch = -1;

void menu_v2_update(void)
{
    Menu_Item_t *currentMenuItem = Menu_CurrentMenuItem();

    if (Menu_IsNullOrEmpty(currentMenuItem))
        Menu_GoTo(&START);

    Menu_GetButtonState();
    Menu_ServiceMenuEntry(&SERVICE);
    Menu_NavigateCurrent();
    Menu_ClearButtonStates();
}

void menu_v2_redraw(void)
{
    Menu_RenderCurrent();
}