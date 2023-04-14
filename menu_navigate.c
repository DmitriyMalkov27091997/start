/**
 * @file menu_navigate.c
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Методы навигации по пунктам меню пользователя.
 * @version 1.0
 * @date 2023-03-15
 *
 * @copyright (C) Ярославль, 2023
 */

#include "type.h"
#include "define.h"
#include "menu_v2.h"
#include "menu_service.h"
#include "menu_helper.h"
#include "menu_navigate.h"
#include <math.h>

tTIMER to_menu = 0;

/// @brief Изменить значение PWM согласно нажатию кнопки.
void ChangePWM(void)
{
    switch (button_pressed)
    {
    case BUTTON_UP:
        menu_calib_pwm = MENU_CALIB_PWM_100;
        break;
    case BUTTON_DOWN:
        menu_calib_pwm = MENU_CALIB_PWM_0;
        break;
    default:
        menu_calib_pwm = MENU_CALIB_PWM_50;
        break;
    }
}

/// @brief Изменить состояние дискретного выхода.
/// @param index Номер дискретного выхода: \n \b 1 - DO1; \n \b 2 - DO2; \n \b 3 - DO3;
/// @param newState Новое состояние дискретного выхода.
void ChangeStateDO(int index, int newState)
{
    if (index < 1 || index > 3)
        return;

    int state = 0;

    if (newState > 0)
        state = 1;

    namur_out[index - 1] = state;
}

/// @brief Изменить значение временной переменной.
/// @param min Минимальное значение.
/// @param max Максимальное значение.
/// @param maxSpeed Максимальное значение ускорения приращения значения при удержании клнопки.
void ChangeVariable(int min, int max, int maxSpeed)
{
    int i;

    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        menu_data.i++;

        if (menu_data.i > max)
            menu_data.i = max;
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;
        menu_data.i--;

        if (menu_data.i < min)
            menu_data.i = min;
    }

    if (maxSpeed <= 5)
        return;

    if ((to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
    {
        if (to_button_pressed < 1000UL)
            i = 1;
        else if (to_button_pressed < 3000UL)
            i = maxSpeed / 5;
        else
            i = maxSpeed;

        switch (button_pressed)
        {
        case BUTTON_UP:
            menu_data.i += i;
            break;

        case BUTTON_DOWN:
            menu_data.i -= i;
            break;
        }

        if (menu_data.i > max)
            menu_data.i = max;
        else if (menu_data.i < min)
            menu_data.i = min;
    }
}

/// @brief Выйти из режима автонастройки.
void AutotuneExit(void)
{
    to_auto = 0;
    st_auto = ST_AUTO_IDLE;
    hart_autoset_flag = 0;

    Menu_GoTo(MENU_PARENT);

    con_str("[MENU_AUTO_EXIT]");
    con_start();
}

/// @brief Очистить временные переменные.
void ClearVariable(void)
{
    menu_switch = -1;
    menu_data.i = -1;
    menu_data.f = data_eeprom.flags;
}

/// @brief Реализует общую часть навигации по экрану меню "Тест: Дискретный выход"
/// @param index Номер дискретного выхода: \n \b 1 - DO1; \n \b 2 - DO2; \n \b 3 - DO3;
void NavigateByTestDO(int index)
{
    if (menu_switch == -1)
    {
        test_namur = 1;
        menu_switch = 1;
        menu_data.i = 0;
    }

    ChangeVariable(0, 1, 0);

    if (menu_btn.btn_cancel)
    {
        test_namur = 0;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        test_namur = 0;
        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    ChangeStateDO(index, menu_data.i);
}

void Menu_ClearButtonStates(void)
{
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

void Menu_NavigateBase(void)
{
    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        ClearVariable();
        Menu_GoTo(MENU_NEXT);
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;
        ClearVariable();
        Menu_GoTo(MENU_PREVIOUS);
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_CHILD);
    }
}

void Menu_NavigateByView_Start(void)
{
    tick(&to_menu);

    if (to_menu >= TO_MENU_V2_START)
        Menu_GoTo(MENU_CHILD);

    return;
}
#pragma region  прочее 
void Menu_ServiceMenuEntry(Menu_Item_t *serviceMenu)
{
    Service_Entry(serviceMenu);
}

void Menu_NavigateByView_Service(void)
{
    Service_Navigate();
}

void Menu_GetButtonState(void)
{
    if (button_pressed)
        tick(&to_button_pressed);
}

void Menu_NavigateByView_HandEnable(void)
{
    int i;

    if (pid_source == PID_SOURCE_SIGNAL)
    {
        menu_hand_pos_t = ihmc_pos_t;
        pid_source = PID_SOURCE_HAND;
    }

    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        if (menu_hand_pos_t > 0)
            menu_hand_pos_t--;
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;
        if (menu_hand_pos_t < 1000)
            menu_hand_pos_t++;
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        pid_source = PID_SOURCE_SIGNAL;
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;
        pid_source = PID_SOURCE_SIGNAL;
        Menu_GoTo(MENU_PARENT);
    }

    if ((to_button_pressed >= 400UL) && (!(to_button_pressed % 200)))
    {
        if (to_button_pressed < 1000UL)
            i = 1;
        else if (to_button_pressed < 3000UL)
            i = 5;
        else
            i = 50;

        switch (button_pressed)
        {
        case BUTTON_UP:
            if (menu_hand_pos_t <= (1000 - i))
                menu_hand_pos_t += i;
            else
                menu_hand_pos_t = 1000;
            break;

        case BUTTON_DOWN:
            if (menu_hand_pos_t >= i)
                menu_hand_pos_t -= i;
            else
                menu_hand_pos_t = 0;
            break;
        }
    }
}

void Menu_NavigateByView_CalibTest(void)
{
    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        menu_calib_pwm = MENU_CALIB_PWM_100;
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;
        menu_calib_pwm = MENU_CALIB_PWM_0;
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;
        menu_calib_pwm = MENU_CALIB_PWM_50;
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_CalibMin(void)
{
    ChangePWM();

    if (menu_btn.btn_up)
        menu_btn.clr_up = 1;

    if (menu_btn.btn_down)
        menu_btn.clr_down = 1;

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.hmc_lim_0 = get_hmc_pos();
        eeprom_lock();
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_CalibMax(void)
{
    ChangePWM();

    if (menu_btn.btn_up)
        menu_btn.clr_up = 1;

    if (menu_btn.btn_down)
        menu_btn.clr_down = 1;

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.hmc_lim_100 = get_hmc_pos();
        eeprom_lock();
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_Autotune(void)
{
    if (st_auto == ST_AUTO_IDLE)
        st_auto = ST_AUTO_SELECT;

    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;

        switch (st_auto)
        {
        case ST_AUTO_SELECT:
            auto_type = TYPE_REVERSE;
            break;

        case ST_AUTO_TIME_DONE:
            AutotuneExit();
            break;
        }
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;

        switch (st_auto)
        {
        case ST_AUTO_SELECT:
            auto_type = TYPE_DIRECT;
            break;

        case ST_AUTO_TIME_DONE:
            AutotuneExit();
            break;
        }
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        AutotuneExit();
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        switch (st_auto)
        {
        case ST_AUTO_SELECT:
        {
            to_auto = 0;

            if (data_eeprom.linear_en == 1)
            {
                data_eeprom.linear_en = 0;
                linear_temp_disable = 1;
            }

            st_auto = ST_AUTO_START;
        }
        break;

        case ST_AUTO_TIME_DONE:
            AutotuneExit();
            break;
        }
    }
}

void Menu_NavigateByView_Scale(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.f.coil_inverse = data_eeprom.flags.coil_inverse;
    }

    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;

        if (menu_data.f.coil_inverse < 1)
            menu_data.f.coil_inverse++;
        else
            menu_data.f.coil_inverse = 0;
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;

        if (menu_data.f.coil_inverse > 0)
            menu_data.f.coil_inverse--;
        else
            menu_data.f.coil_inverse = 1;
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();

        if (menu_data.f.coil_inverse < 0 || menu_data.f.coil_inverse > 1)
            menu_data.f.coil_inverse = 0;

        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        // TODO: действия по инвертированию шкалы.
        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_Linearization(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.linear_en;
    }

    ChangeVariable(0, 1, 0);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.linear_en = menu_data.i;
        eeprom_lock();

        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_InputRangeMin(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.ain_ma_t_min;
    }

    ChangeVariable(400, 2000, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.ain_ma_t_min = menu_data.i;
        eeprom_lock();

        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_InputRangeMax(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.ain_ma_t_max;
    }

    ChangeVariable(400, 2000, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.ain_ma_t_max = menu_data.i;
        eeprom_lock();

        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_TightClosingMin(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.hard_pos_min;
    }

    ChangeVariable(0, 200, 10);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.hard_pos_min = menu_data.i;
        eeprom_lock();

        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_TightClosingMax(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.hard_pos_max;
    }

    ChangeVariable(0, 200, 10);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.hard_pos_max = menu_data.i;
        eeprom_lock();

        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_TransferCharacteristic(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.f.im_ph = data_eeprom.flags.im_ph;
    }

    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;

        if (menu_data.f.im_ph > 0)
            menu_data.f.im_ph--;
        else
            menu_data.f.im_ph = 0;
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;

        if (menu_data.f.im_ph < 10)
            menu_data.f.im_ph++;
        else
            menu_data.f.im_ph = 10;
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();

        if (menu_data.f.im_ph < 0 || menu_data.f.im_ph > 10)
            menu_data.f.im_ph = 5;

        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.flags.im_ph = menu_data.f.im_ph;
        eeprom_lock();

        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_StrokeSpeedPump(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_up * FADC));
    }

    ChangeVariable(0, 1000, 10);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.t_lp_up = T_LP_M * 2.0f / ((float)(menu_data.i * FADC));
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_StrokeSpeedBleed(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_down * FADC));
    }

    ChangeVariable(0, 1000, 10);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.t_lp_down = T_LP_M * 2.0f / ((float)(menu_data.i * FADC));
        eeprom_lock();

        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_StrokeSpeedLimit(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.coil_pwm_limit;
    }

    ChangeVariable(0, 90, 10);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.coil_pwm_limit = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_PidKp(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = roundf(data_eeprom.kp * 1000.0f);
    }

    ChangeVariable(0, 9999999, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.kp = 0.001f * menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_PidKd(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = roundf(data_eeprom.kd * 1000.0f);
    }

    ChangeVariable(0, 9999999, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.kd = 0.001f * menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_PidKi(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = roundf(data_eeprom.ki * 1000.0f);
    }

    ChangeVariable(0, 9999999, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.ki = 0.001f * menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_SensorType(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.sensor_type;
    }

    ChangeVariable(0, 1, 0);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;

        eeprom_unlock();
        data_eeprom.sensor_type = menu_data.i;
        eeprom_lock();

        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_PositionSensorMin_Auto(void)
{
    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        ClearVariable();
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;
        ClearVariable();
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;
        ClearVariable();
        Service_Command_SetPosition_0();

        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_PositionSensorMin_Manual(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.hmc_adc_0;
    }

    ChangeVariable(0, 4095, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.hmc_adc_0 = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_PositionSensorMax_Auto(void)
{
    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        ClearVariable();
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;
        ClearVariable();
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;
        ClearVariable();
        Service_Command_SetPosition_100();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_PositionSensorMax_Manual(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.hmc_adc_100;
    }

    ChangeVariable(0, 4095, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.hmc_adc_100 = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_CurrentInput4mA_Auto(void)
{
    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        ClearVariable();
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;
        ClearVariable();
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;
        ClearVariable();
        Service_Command_SetAIn_4();

        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_CurrentInput4mA_Manual(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.ain_adc_min;
    }

    ChangeVariable(0, 4095, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.ain_adc_min = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_CurrentInput20mA_Auto(void)
{
    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        ClearVariable();
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;
        ClearVariable();
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;
        ClearVariable();
        Service_Command_SetAIn_20();

        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_CurrentInput20mA_Manual(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.ain_adc_max;
    }

    ChangeVariable(0, 4095, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.ain_adc_max = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_CurrentOutput4mA(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.aout_pwm_min;
    }

    ChangeVariable(0, 1000, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.aout_pwm_min = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    Change_PWM_Aout(menu_data.i);
}

void Menu_NavigateByView_CurrentOutput20mA(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.aout_pwm_max;
    }

    ChangeVariable(0, 1000, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.aout_pwm_max = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    Change_PWM_Aout(menu_data.i);
}

void Menu_NavigateByView_TestAO(void)
{
    if (menu_switch == -1)
    {
        test_a_out = 1;
        menu_switch = 1;
        menu_data.i = data_eeprom.aout_pwm_min;
    }

    ChangeVariable(data_eeprom.aout_pwm_min, data_eeprom.aout_pwm_max, 25);

    if (menu_btn.btn_cancel)
    {
        test_a_out = 0;
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        test_a_out = 0;
        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    Change_PWM_Aout(menu_data.i);
}

void Menu_NavigateByView_TestDO1(void)
{
    NavigateByTestDO(1);
}

void Menu_NavigateByView_TestDO2(void)
{
    NavigateByTestDO(2);
}

void Menu_NavigateByView_TestDO3(void)
{
    NavigateByTestDO(3);
}

void Menu_NavigateByView_LpfInput(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.lpf_a_in;
    }

    ChangeVariable(0, 3, 0);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.lpf_a_in = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_LpfOutput(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.lpf_a_out;
    }

    ChangeVariable(0, 3, 0);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.lpf_a_out = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}
#pragma endregion
  
void Menu_NavigateByView_LinearizationCoef(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.linear_coef;
    }

    ChangeVariable(0, 100, 25);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.linear_coef = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_LinearizationZeroShift(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.linear_shift;
    }

    ChangeVariable(0, 100, 25);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.linear_shift = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_ExpertDisplay(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.display_format;
    }

    ChangeVariable(0, 1, 0);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.display_format = menu_data.i;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_ExpertTempVersion(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.temp_version;
    }

    ChangeVariable(0, 2, 0);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        Service_Command_ChangeTempVersion(menu_data.i);

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_ExpertAccumZone(void)
{
    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = roundf(data_eeprom.si_off * 10.0f);
    }

    ChangeVariable(3, 1000, 50);

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        eeprom_unlock();
        data_eeprom.si_off = ((float)(menu_data.i)) * 0.1f;
        eeprom_lock();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_ExpertFactoryReset(void)
{
    if (menu_switch == -1)
        menu_switch = 1;

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        Service_Command_FactoryReset();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_Reset(void)
{
    if (menu_switch == -1)
        menu_switch = 1;

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        Service_Command_FullReset();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}

void Menu_NavigateByView_ViewLogEdit(void)
{
    if (menu_switch == -1)
    {
        if (event_num == (EVENT_LOG_NUM - 1)) menu_data.i = event_pos;
        else menu_data.i = event_num - 1;
        menu_switch = 1;
    }
    if (menu_btn.btn_down)
	{
		menu_btn.clr_down = 1;
        if (event_num)
        {
            i_old = menu_data.i;
            if (menu_data.i > 0) menu_data.i--;
            else menu_data.i = event_num - 1;
	    }
    }
	if (menu_btn.btn_up)
	{
		menu_btn.clr_up = 1;
		if (event_num)
        {
            i_old = menu_data.i;
            if (menu_data.i < (event_num - 1)) menu_data.i++;
            else menu_data.i = 0;
	    }
    }
	if (menu_btn.btn_ok)
	{
		menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
	}
	if (menu_btn.btn_cancel)
	{
		menu_btn.clr_cancel = 1;
		ClearVariable();
        Menu_GoTo(MENU_PARENT);
	}
}

void Menu_NavigateByView_ClearingLogEdit(void)
{
    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }

    if (menu_btn.btn_ok)
    {
        log_reset();

        menu_btn.clr_ok = 1;
        ClearVariable();
        Menu_GoTo(MENU_PARENT);
    }
}