/**
 * @file menu_navigate.h
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Заголовочный файл методов навигации по пунктам меню пользователя.
 * @version 1.0
 * @date 2023-03-15
 *
 * @copyright (C) Ярославль, 2023
 */

#ifndef _MENU_NAVIGATE_H_
#define _MENU_NAVIGATE_H_

#include "menu_helper.h"

/// @brief Базовая реализация навигации по пункту меню.
extern void Menu_NavigateBase(void);

/// @brief Очистить текущее состояние кнопок.
extern void Menu_ClearButtonStates(void);

/// @brief Реализует навигацию по экрану "Стартовый".
extern void Menu_NavigateByView_Start(void);

/**
 * @brief Получить доступ к экрану "Сервисное меню".
 * @remark Для получения доступа к экрану "Сервисное меню" необходимо выполнить следующие действия:
 *      \n \b 1. Нажать и удерживать кнопки "Стрелка вниз" + "Круг".
 *      \n \b 2. Дополнительно нажать и удерживать кнопку "Стрелка вверх"
 *      \n \b 3. Отпустить и повторно нажать кнопку "Стрелка вниз"
 * @param serviceMenu Указатель на экран "Севисное меню".
 */
extern void Menu_ServiceMenuEntry(Menu_Item_t *serviceMenu);

/// @brief Получить состояние кнопок.
extern void Menu_GetButtonState(void);

/// @brief Реализует управление в инженерном режиме.
extern void Menu_NavigateByView_Service(void);

/// @brief Реализует управление основным экраном в ручном режиме.
extern void Menu_NavigateByView_HandEnable(void);

/// @brief Реализует управление на экране "Калибровка: Тест - Редактирование".
extern void Menu_NavigateByView_CalibTest(void);

/// @brief Реализует управление на экране "Калибровка: Минимум - Редактирование".
extern void Menu_NavigateByView_CalibMin(void);

/// @brief Реализует управление на экране "Калибровка: Максимум - Редактирование".
extern void Menu_NavigateByView_CalibMax(void);

/// @brief Реализует управление на экране "Автонастройка".
extern void Menu_NavigateByView_Autotune(void);

/// @brief Реализует управление на экране "Шкала".
extern void Menu_NavigateByView_Scale(void);

/// @brief Реализует управление на экране "Линеаризация".
extern void Menu_NavigateByView_Linearization(void);

/// @brief Реализует управление на экране "Диапазон входа: Минимум".
extern void Menu_NavigateByView_InputRangeMin(void);

/// @brief Реализует управление на экране "Диапазон входа: Максимум".
extern void Menu_NavigateByView_InputRangeMax(void);

/// @brief Реализует управление на экране "Плотное закрытие: Минимум".
extern void Menu_NavigateByView_TightClosingMin(void);

/// @brief Реализует управление на экране "Плотное закрытие: Максимум".
extern void Menu_NavigateByView_TightClosingMax(void);

/// @brief Реализует управление на экране "Передаточная характеристика".
extern void Menu_NavigateByView_TransferCharacteristic(void);

/// @brief Реализует управление на экране "Скорость накачки".
extern void Menu_NavigateByView_StrokeSpeedPump(void);

/// @brief Реализует управление на экране "Скорость сброса".
extern void Menu_NavigateByView_StrokeSpeedBleed(void);

/// @brief Реализует управление на экране "Ограничение расхода".
extern void Menu_NavigateByView_StrokeSpeedLimit(void);

/// @brief Реализует управление на экране "ПИД: Кп".
extern void Menu_NavigateByView_PidKp(void);

/// @brief Реализует управление на экране "ПИД: Кд".
extern void Menu_NavigateByView_PidKd(void);

/// @brief Реализует управление на экране "ПИД: Ки".
extern void Menu_NavigateByView_PidKi(void);

/// @brief Реализует управление на экране "Датчик положения: Тип датчика".
extern void Menu_NavigateByView_SensorType(void);

/// @brief Реализует управление на экране "ПИД: Кп".
extern void Menu_NavigateByView_PositionSensorMin_Auto(void);

/// @brief Реализует управление на экране "ПИД: Кд".
extern void Menu_NavigateByView_PositionSensorMin_Manual(void);

/// @brief Реализует управление на экране "ПИД: Ки".
extern void Menu_NavigateByView_PositionSensorMax_Auto(void);

/// @brief Реализует управление на экране "Датчик положения: Тип датчика".
extern void Menu_NavigateByView_PositionSensorMax_Manual(void);

/// @brief Реализует управление на экране "Токовый вход 4мА: Задать".
extern void Menu_NavigateByView_CurrentInput4mA_Auto(void);

/// @brief Реализует управление на экране "Токовый вход 4мА: Указать".
extern void Menu_NavigateByView_CurrentInput4mA_Manual(void);

/// @brief Реализует управление на экране "Токовый вход 20мА: Задать".
extern void Menu_NavigateByView_CurrentInput20mA_Auto(void);

/// @brief Реализует управление на экране "Токовый вход 20мА: Указать".
extern void Menu_NavigateByView_CurrentInput20mA_Manual(void);

/// @brief Реализует управление на экране "Токовый выход 4мА: Указать".
extern void Menu_NavigateByView_CurrentOutput4mA(void);

/// @brief Реализует управление на экране "Токовый выход 20мА: Указать".
extern void Menu_NavigateByView_CurrentOutput20mA(void);

/// @brief Реализует управление на экране "Тест: Аналоговый выход".
extern void Menu_NavigateByView_TestAO(void);

/// @brief Реализует управление на экране "Тест: Дискретный выход 1".
extern void Menu_NavigateByView_TestDO1(void);

/// @brief Реализует управление на экране "Тест: Дискретный выход 2".
extern void Menu_NavigateByView_TestDO2(void);

/// @brief Реализует управление на экране "Тест: Дискретный выход 3".
extern void Menu_NavigateByView_TestDO3(void);

/// @brief Реализует управление на экране "ФНЧ: Вход".
extern void Menu_NavigateByView_LpfInput(void);

/// @brief Реализует управление на экране "ФНЧ: Выход".
extern void Menu_NavigateByView_LpfOutput(void);

/// @brief Реализует управление на экране "Линеаризация: Коэффициент".
extern void Menu_NavigateByView_LinearizationCoef(void);

/// @brief Реализует управление на экране "Линеаризация: Сдвиг нуля".
extern void Menu_NavigateByView_LinearizationZeroShift(void);

/// @brief Реализует управление на экране "Дисплей".
extern void Menu_NavigateByView_ExpertDisplay(void);

/// @brief Реализует управление на экране "Температурное исполнение".
extern void Menu_NavigateByView_ExpertTempVersion(void);

/// @brief Реализует управление на экране "Зона накопления И".
extern void Menu_NavigateByView_ExpertAccumZone(void);

/// @brief Реализует управление на экране "Полный сброс насроек".
extern void Menu_NavigateByView_ExpertFactoryReset(void);

/// @brief Реализует управление на экране "Cброс настроек пользователя".
extern void Menu_NavigateByView_Reset(void);

/// @brief Реализует управление на экране "Журнал событий: Просмотр". 
extern void Menu_NavigateByView_ViewLogEdit(void);

/// @brief Реализует управление на экране "Журнал событий: Очистить" 
extern void Menu_NavigateByView_ClearingLogEdit(void);

#endif // _MENU_NAVIGATE_H_
