/**
 * @file menu_render.h
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Заголовочный файл методов визуализации пунктов меню пользователя.
 * @version 1.0
 * @date 2023-03-15
 * 
 * @copyright (C) Ярославль, 2023
 */
#ifndef _MENU_RENDER_H_
#define _MENU_RENDER_H_

#include "type.h"

#pragma region  Удалить по окончании миграции

extern void FromPosition(BYTE *str, const int value);
extern void FromFloat(BYTE *str, const int value, const int decimals);

#pragma endregion

/// @brief Выполнить отрисовку экрана "Стартовый экран".
extern void Menu_RenderView_Start(void);

/// @brief Выполнить отрисовку экрана "Сервисное меню".
extern void Menu_RenderView_Service(void);

/// @brief Выполнить отрисовку экрана "Основной экран".
extern void Menu_RenderView_Home(void);

/// @brief Выполнить отрисовку экрана "Инфорежим-1".
extern void Menu_RenderView_Info1(void);

/// @brief Выполнить отрисовку экрана "Инфорежим-2".
extern void Menu_RenderView_Info2(void);

/// @brief Выполнить отрисовку экрана "Инфорежим-3".
extern void Menu_RenderView_Info3(void);

/// @brief Выполнить отрисовку экрана "Настройки".
extern void Menu_RenderView_Setup(void);

/// @brief Выполнить отрисовку экрана "Журнал событий".
extern void Menu_RenderView_Log(void);

/// @brief Выполнить отрисовку экрана "Просмотр журнала событий". 
extern void Menu_RenderView_ViewLog(void); 

/// @brief Выполнить отрисовку экрана "Просмотр журнала событий: просмотр". 
extern void Menu_RenderView_ViewLogEdit(void); 

/// @brief Выполнить отрисовку экрана "Очистка журнала событий".
extern void Menu_RenderView_ClearingLog(void);

/// @brief Выполнить отрисовку экрана "Очистка журнала событий: Очистка".
extern void Menu_RenderView_ClearingLogEdit(void);

/// @brief Выполнить отрисовку экрана "Сигнализация".
extern void Menu_RenderView_Signal(void);

/// @brief Выполнить отрисовку экрана "Диагностика".
extern void Menu_RenderView_Diag(void);

/// @brief Выполнить отрисовку экрана "HART".
extern void Menu_RenderView_Hart(void);

/// @brief Выполнить отрисовку экрана "Дисплей".
extern void Menu_RenderView_Display(void);

/// @brief Выполнить отрисовку экрана "Ручной режим".
extern void Menu_RenderView_Hand(void);

/// @brief Выполнить отрисовку основного экрана в режиме ручного управления.
extern void Menu_RenderView_HandEnable(void);

/// @brief Выполнить отрисовку экрана "Калибровка".
extern void Menu_RenderView_Calib(void);

/// @brief Выполнить отрисовку экрана "Автонастройка".
extern void Menu_RenderView_Autotune(void);

/// @brief Выполнить отрисовку экрана "Шкала".
extern void Menu_RenderView_Scale(void);

/// @brief Выполнить отрисовку экрана "Шкала - Редактирование".
extern void Menu_RenderView_ScaleEdit(void);

/// @brief Выполнить отрисовку экрана "Линеаризация".
extern void Menu_RenderView_Linearization(void);

/// @brief Выполнить отрисовку экрана "Линеаризация - Редактирование".
extern void Menu_RenderView_LinearizationEdit(void);

/// @brief Выполнить отрисовку экрана "Диапазон входа".
extern void Menu_RenderView_InputRange(void);

/// @brief Выполнить отрисовку экрана "Плотное закрытие".
extern void Menu_RenderView_TightClosing(void);

/// @brief Выполнить отрисовку экрана "Передаточная характеристика".
extern void Menu_RenderView_TransferCharacteristic(void);

/// @brief Выполнить отрисовку экрана "Передаточная характеристика - Редактирование".
extern void Menu_RenderView_TransferCharacteristicEdit(void);

/// @brief Выполнить отрисовку экрана "Скорость хода".
extern void Menu_RenderView_StrokeSpeed(void);

/// @brief Выполнить отрисовку экрана "ПИД регулятор".
extern void Menu_RenderView_PID(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки".
extern void Menu_RenderView_ExpertSetup(void);

/// @brief Выполнить отрисовку экрана "Cброс настроек пользователя".
extern void Menu_RenderView_Reset(void);

/// @brief Выполнить отрисовку экрана "Cброс настроек пользователя - Редактирование".
extern void Menu_RenderView_ResetEdit(void);

/// @brief Выполнить отрисовку экрана "Просмотр журнала событий".

/// @brief Выполнить отрисовку экрана "Аналоговый выход".
extern void Menu_RenderView_AO(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 1".
extern void Menu_RenderView_DO1(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 2".
extern void Menu_RenderView_DO2(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 3".
extern void Menu_RenderView_DO3(void);

/// @brief Выполнить отрисовку экрана "Очистка журнала событий".
extern void Menu_RenderView_ClearingLog(void);

/// @brief Выполнить отрисовку экрана "PST-тест".
extern void Menu_RenderView_PST(void);

/// @brief Выполнить отрисовку экрана "Трение страгивания".
extern void Menu_RenderView_BreakawayFriction(void);

/// @brief Выполнить отрисовку экрана "Гистограммы".
extern void Menu_RenderView_Histograms(void);

/// @brief Выполнить отрисовку экрана "Высокое трение".
extern void Menu_RenderView_HighFriction(void);

/// @brief Выполнить отрисовку экрана "HART: Идентификатор".
extern void Menu_RenderView_HartIdentifier(void);

/// @brief Выполнить отрисовку экрана "HART: Приоритет".
extern void Menu_RenderView_HartPriority(void);

/// @brief Выполнить отрисовку экрана "HART: Отключение".
extern void Menu_RenderView_HartShutdown(void);

/// @brief Выполнить отрисовку экрана "Единицы ИМ".
extern void Menu_RenderView_UnitsIM(void);

/// @brief Выполнить отрисовку экрана "Единицы входа".
extern void Menu_RenderView_UnitsInput(void);

/// @brief Выполнить отрисовку экрана "Контрастность".
extern void Menu_RenderView_Contrast(void);

/// @brief Выполнить отрисовку экрана "Поворот экрана".
extern void Menu_RenderView_ScreenRotation(void);

/// @brief Выполнить отрисовку экрана "Сглаживание ПК".
extern void Menu_RenderView_PcSmoothing(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Тест".
extern void Menu_RenderView_CalibTest(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Тест - Редактирование".
extern void Menu_RenderView_CalibTestEdit(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Минимум".
extern void Menu_RenderView_CalibMin(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Минимум - Редактирование".
extern void Menu_RenderView_CalibMinEdit(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Максимум".
extern void Menu_RenderView_CalibMax(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Максимум - Редактирование".
extern void Menu_RenderView_CalibMaxEdit(void);

/// @brief Выполнить отрисовку экрана "Диапазон входа: Минимум".
extern void Menu_RenderView_InputRangeMin(void);

/// @brief Выполнить отрисовку экрана "Диапазон входа: Минимум - Редактирование".
extern void Menu_RenderView_InputRangeMinEdit(void);

/// @brief Выполнить отрисовку экрана "Диапазон входа: Максимум".
extern void Menu_RenderView_InputRangeMax(void);

/// @brief Выполнить отрисовку экрана "Диапазон входа: Максимум - Редактирование".
extern void Menu_RenderView_InputRangeMaxEdit(void);

/// @brief Выполнить отрисовку экрана "Плотное закрытие: Минимум".
extern void Menu_RenderView_TightClosingMin(void);

/// @brief Выполнить отрисовку экрана "Плотное закрытие: Минимум - Редактирование".
extern void Menu_RenderView_TightClosingMinEdit(void);

/// @brief Выполнить отрисовку экрана "Плотное закрытие: Максимум".
extern void Menu_RenderView_TightClosingMax(void);

/// @brief Выполнить отрисовку экрана "Плотное закрытие: Максимум - Редактирование".
extern void Menu_RenderView_TightClosingMaxEdit(void);

/// @brief Выполнить отрисовку экрана "Скорость хода: Скорость накачки".
extern void Menu_RenderView_StrokeSpeedPump(void);

/// @brief Выполнить отрисовку экрана "Скорость хода: Скорость накачки - Редактирование".
extern void Menu_RenderView_StrokeSpeedPumpEdit(void);

/// @brief Выполнить отрисовку экрана "Скорость хода: Скорость сброса".
extern void Menu_RenderView_StrokeSpeedBleed(void);

/// @brief Выполнить отрисовку экрана "Скорость хода: Скорость сброса - Редактирование".
extern void Menu_RenderView_StrokeSpeedBleedEdit(void);

/// @brief Выполнить отрисовку экрана "Скорость хода: Ограничение расхода".
extern void Menu_RenderView_StrokeSpeedLimit(void);

/// @brief Выполнить отрисовку экрана "Скорость хода: Ограничение расхода - Редактирование".
extern void Menu_RenderView_StrokeSpeedLimitEdit(void);

/// @brief Выполнить отрисовку экрана "ПИД: Кп".
extern void Menu_RenderView_PidKp(void);

/// @brief Выполнить отрисовку экрана "ПИД: Кп - Редактирование".
extern void Menu_RenderView_PidKpEdit(void);

/// @brief Выполнить отрисовку экрана "ПИД: Кд".
extern void Menu_RenderView_PidKd(void);

/// @brief Выполнить отрисовку экрана "ПИД: Кд - Редактирование".
extern void Menu_RenderView_PidKdEdit(void);

/// @brief Выполнить отрисовку экрана "ПИД: Ки".
extern void Menu_RenderView_PidKi(void);

/// @brief Выполнить отрисовку экрана "ПИД: Ки - Редактирование".
extern void Menu_RenderView_PidKiEdit(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Калибровка".
extern void Menu_RenderView_ExpertCalib(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Тест".
extern void Menu_RenderView_ExpertTest(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: ФНЧ".
extern void Menu_RenderView_ExpertLpf(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Линеаризация".
extern void Menu_RenderView_ExpertLinearization(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Дисплей".
extern void Menu_RenderView_ExpertDisplay(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Дисплей - Редактирование".
extern void Menu_RenderView_ExpertDisplayEdit(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Температурное исполнение".
extern void Menu_RenderView_ExpertTempVersion(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Температурное исполнение - Редактирование".
extern void Menu_RenderView_ExpertTempVersionEdit(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Зона накопления И".
extern void Menu_RenderView_ExpertAccumZone(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Зона накопления И - Редактирование".
extern void Menu_RenderView_ExpertAccumZoneEdit(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Полный сброс насроек".
extern void Menu_RenderView_ExpertFactoryReset(void);

/// @brief Выполнить отрисовку экрана "Экспертные настройки: Полный сброс насроек - Редактирование".
extern void Menu_RenderView_ExpertFactoryResetEdit(void);

/// @brief Выполнить отрисовку экрана "Аналоговый выход: Сигнал".
extern void Menu_RenderView_AoSignal(void);

/// @brief Выполнить отрисовку экрана "Аналоговый выход: Минимальный ток".
extern void Menu_RenderView_AoMin(void);

/// @brief Выполнить отрисовку экрана "Аналоговый выход: Максимальный ток".
extern void Menu_RenderView_AoMax(void);

/// @brief Выполнить отрисовку экрана "Аналоговый выход: Нижний диапазон".
extern void Menu_RenderView_AoLower(void);

/// @brief Выполнить отрисовку экрана "Аналоговый выход: Верхний диапазон".
extern void Menu_RenderView_AoUpper(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 1: Сигнал".
extern void Menu_RenderView_Do1Signal(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 2: Сигнал".
extern void Menu_RenderView_Do2Signal(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 2: Гистерезис".
extern void Menu_RenderView_Do2Hysteresis(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 2: Направление".
extern void Menu_RenderView_Do2Direction(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 2: Положение".
extern void Menu_RenderView_Do2Position(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 3: Сигнал".
extern void Menu_RenderView_Do3Signal(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 3: Гистерезис".
extern void Menu_RenderView_Do3Hysteresis(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 3: Направление".
extern void Menu_RenderView_Do3Direction(void);

/// @brief Выполнить отрисовку экрана "Дискретный выход 3: Положение".
extern void Menu_RenderView_Do3Position(void);

/// @brief Выполнить отрисовку экрана "PST-тест: Процент хода".
extern void Menu_RenderView_PsStroke(void);

/// @brief Выполнить отрисовку экрана "PST-тест: Запуск теста".
extern void Menu_RenderView_PstRun(void);

/// @brief Выполнить отрисовку экрана "PST-тест: Автозапуск, час".
extern void Menu_RenderView_PstAutorun(void);

/// @brief Выполнить отрисовку экрана "PST-тест: Задержка ошибки, с".
extern void Menu_RenderView_PstErrorDelay(void);

/// @brief Выполнить отрисовку экрана "PST-тест: Время теста, с".
extern void Menu_RenderView_PstTime(void);

/// @brief Выполнить отрисовку экрана "Трение страгивания: Диапазон теста".
extern void Menu_RenderView_BreakawayFrictionRange(void);

/// @brief Выполнить отрисовку экрана "Трение страгивания: Количество отсчетов".
extern void Menu_RenderView_BreakawayFrictionCount(void);

/// @brief Выполнить отрисовку экрана "Трение страгивания: Запуск теста".
extern void Menu_RenderView_BreakawayFrictionRun(void);

/// @brief Выполнить отрисовку экрана "Трение страгивания: Отображаемые данные".
extern void Menu_RenderView_BreakawayFrictionData(void);

/// @brief Выполнить отрисовку экрана "Высокое трение: Рассогласование".
extern void Menu_RenderView_HighFrictionError(void);

/// @brief Выполнить отрисовку экрана "Высокое трение: Время".
extern void Menu_RenderView_HighFrictionTime(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Датчик положения".
extern void Menu_RenderView_PositionSensor(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Токовый вход".
extern void Menu_RenderView_CalibCurrentInput(void);

/// @brief Выполнить отрисовку экрана "Калибровка: Токовый выход".
extern void Menu_RenderView_CalibCurrentOutput(void);

/// @brief Выполнить отрисовку экрана "Тест: Аналоговый выход".
extern void Menu_RenderView_TestAO(void);

/// @brief Выполнить отрисовку экрана "Тест: Аналоговый выход - Редактирование".
extern void Menu_RenderView_TestAOEdit(void);

/// @brief Выполнить отрисовку экрана "Тест: Дискретный выход 1".
extern void Menu_RenderView_TestDO1(void);

/// @brief Выполнить отрисовку экрана "Тест: Дискретный выход 1 - Редактирование".
extern void Menu_RenderView_TestDO1Edit(void);

/// @brief Выполнить отрисовку экрана "Тест: Дискретный выход 2".
extern void Menu_RenderView_TestDO2(void);

/// @brief Выполнить отрисовку экрана "Тест: Дискретный выход 2 - Редактирование".
extern void Menu_RenderView_TestDO2Edit(void);

/// @brief Выполнить отрисовку экрана "Тест: Дискретный выход 3".
extern void Menu_RenderView_TestDO3(void);

/// @brief Выполнить отрисовку экрана "Тест: Дискретный выход 3 - Редактирование".
extern void Menu_RenderView_TestDO3Edit(void);

/// @brief Выполнить отрисовку экрана "ФНЧ: Вход".
extern void Menu_RenderView_LpfInput(void);

/// @brief Выполнить отрисовку экрана "ФНЧ: Вход - Редактирование".
extern void Menu_RenderView_LpfInputEdit(void);

/// @brief Выполнить отрисовку экрана "ФНЧ: Выход".
extern void Menu_RenderView_LpfOutput(void);

/// @brief Выполнить отрисовку экрана "ФНЧ: Выход - Редактирование".
extern void Menu_RenderView_LpfOutputEdit(void);

/// @brief Выполнить отрисовку экрана "Линеаризация: Коэффициент".
extern void Menu_RenderView_LinearizationCoef(void);

/// @brief Выполнить отрисовку экрана "Линеаризация: Коэффициент - Редактирование".
extern void Menu_RenderView_LinearizationCoefEdit(void);

/// @brief Выполнить отрисовку экрана "Линеаризация: Сдвиг нуля".
extern void Menu_RenderView_LinearizationZeroShift(void);

/// @brief Выполнить отрисовку экрана "Линеаризация: Сдвиг нуля - Редактирование".
extern void Menu_RenderView_LinearizationZeroShiftEdit(void);

/// @brief Выполнить отрисовку экрана "Датчик положения: Тип датчика".
extern void Menu_RenderView_PositionSensorType(void);

/// @brief Выполнить отрисовку экрана "Датчик положения: Тип датчика - Редактирование".
extern void Menu_RenderView_PositionSensorTypeEdit(void);

/// @brief Выполнить отрисовку экрана "Датчик положения: Записать минимум".
extern void Menu_RenderView_PositionSensorMin(void);

/// @brief Выполнить отрисовку экрана "Датчик положения - Минимум: Автоматически".
extern void Menu_RenderView_PositionSensorMin_Auto(void);

/// @brief Выполнить отрисовку экрана "Датчик положения - Минимум: Автоматически - Редактирование".
extern void Menu_RenderView_PositionSensorMin_AutoEdit(void);

/// @brief Выполнить отрисовку экрана "Датчик положения - Минимум: Вручную".
extern void Menu_RenderView_PositionSensorMin_Manual(void);

/// @brief Выполнить отрисовку экрана "Датчик положения - Минимум: Вручную - Редактирование".
extern void Menu_RenderView_PositionSensorMin_ManualEdit(void);

/// @brief Выполнить отрисовку экрана "Датчик положения: Записать максимум".
extern void Menu_RenderView_PositionSensorMax(void);

/// @brief Выполнить отрисовку экрана "Датчик положения - Максимум: Автоматически".
extern void Menu_RenderView_PositionSensorMax_Auto(void);

/// @brief Выполнить отрисовку экрана "Датчик положения - Максимум: Автоматически - Редактирование".
extern void Menu_RenderView_PositionSensorMax_AutoEdit(void);

/// @brief Выполнить отрисовку экрана "Датчик положения - Максимум: Вручную".
extern void Menu_RenderView_PositionSensorMax_Manual(void);

/// @brief Выполнить отрисовку экрана "Датчик положения - Максимум: Вручную - Редактирование".
extern void Menu_RenderView_PositionSensorMax_ManualEdit(void);

/// @brief Выполнить отрисовку экрана "Токовый вход: 4мА".
extern void Menu_RenderView_CurrentInput4mA(void);

/// @brief Выполнить отрисовку экрана "Токовый вход: 20мА".
extern void Menu_RenderView_CurrentInput20mA(void);

/// @brief Выполнить отрисовку экрана "Токовый выход: 4мА".
extern void Menu_RenderView_CurrentOutput4mA(void);

/// @brief Выполнить отрисовку экрана "Токовый выход: 4мА - Редактирование".
extern void Menu_RenderView_CurrentOutput4mAEdit(void);

/// @brief Выполнить отрисовку экрана "Токовый выход: 20мА".
extern void Menu_RenderView_CurrentOutput20mA(void);

/// @brief Выполнить отрисовку экрана "Токовый выход: 20мА - Редактирование".
extern void Menu_RenderView_CurrentOutput20mAEdit(void);

/// @brief Выполнить отрисовку экрана "Токовый вход 4мА: Задать".
extern void Menu_RenderView_CurrentInput4mA_Auto(void);

/// @brief Выполнить отрисовку экрана "Токовый вход 4мА: Задать - Редактирование".
extern void Menu_RenderView_CurrentInput4mA_AutoEdit(void);

/// @brief Выполнить отрисовку экрана "Токовый вход 4мА: Указать".
extern void Menu_RenderView_CurrentInput4mA_Manual(void);

/// @brief Выполнить отрисовку экрана "Токовый вход 4мА: Указать - Редактирование".
extern void Menu_RenderView_CurrentInput4mA_ManualEdit(void);

/// @brief Выполнить отрисовку экрана "Токовый вход 20мА: Задать".
extern void Menu_RenderView_CurrentInput20mA_Auto(void);

/// @brief Выполнить отрисовку экрана "Токовый вход 20мА: Задать - Редактирование".
extern void Menu_RenderView_CurrentInput20mA_AutoEdit(void);

/// @brief Выполнить отрисовку экрана "Токовый вход 20мА: Указать".
extern void Menu_RenderView_CurrentInput20mA_Manual(void);

/// @brief Выполнить отрисовку экрана "Токовый вход 20мА: Указать - Редактирование".
extern void Menu_RenderView_CurrentInput20mA_ManualEdit(void);

#endif // _MENU_RENDER_H_