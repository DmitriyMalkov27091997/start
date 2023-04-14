/**
 * @file menu_service.c
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @brief Методы навигации и отрисовки инженерного меню.
 * @version 1.0
 * @date 2023-03-21
 *
 * @copyright (C) Ярославль, 2023
 */

#include <math.h>
#include "menu_v2.h"
#include "menu_service.h"
#include "define.h"
#include "pcf8531.h"
#include "picto.h"
#include "font.h"

volatile int service_fsm = 0;
volatile int service_data_pos = -1;
volatile int service_item_pos = 0;
volatile BYTE service_data[6];
const BYTE service_buttons[] = {6, 7, 3, 7, 0};

void Service_Command_SetPosition_0(void)
{
    con_str("[SERVICE_CMD_HMC_SET_0]");
    con_start();
    eeprom_unlock();
    data_eeprom.hmc_adc_0 = pos_value;
    eeprom_lock();
}

void Service_Command_SetPosition_100(void)
{
    con_str("[SERVICE_CMD_HMC_SET_100]");
    con_start();
    eeprom_unlock();
    data_eeprom.hmc_adc_100 = pos_value;
    eeprom_lock();
}

void Service_Command_SetAIn_4(void)
{
    con_str("[SERVICE_CMD_AIN_SET_4]");
    con_start();
    eeprom_unlock();
    data_eeprom.ain_adc_min = ain_adc_s;
    eeprom_lock();
}

void Service_Command_SetAIn_20(void)
{
    con_str("[SERVICE_CMD_AIN_SET_20]");
    con_start();
    eeprom_unlock();
    data_eeprom.ain_adc_max = ain_adc_s;
    eeprom_lock();
}

void Service_Command_Reboot(void)
{
    con_str("[SERVICE_CMD_REBOOT]");
    con_start();
    NVIC_SystemReset();
}

void Service_Command_FactoryReset(void)
{
    con_str("[SERVICE_CMD_FACTRESET]");
    con_start();
    factory_reset();
}

void Service_Command_FullReset(void)
{
    con_str("[SERVICE_CMD_FULLRESET]");
    con_start();
    im_reset();
}

void Service_Command_ChangeTempVersion(int newVersion)
{
    con_str("[SERVICE_CMD_ChangeTempVersion]");
    con_start();

    eeprom_unlock();

    switch (newVersion)
    {
    case N:
        data_eeprom.temp_version = menu_data.i;
        data_eeprom.slow_LT_edge = -30;
        data_eeprom.slow_LT_val = 0.5;
        data_eeprom.midpoint = 350;
        break;
    case C:
        data_eeprom.temp_version = menu_data.i;
        data_eeprom.slow_LT_edge = -30;
        data_eeprom.slow_LT_val = 0.5;
        data_eeprom.midpoint = 350;
        break;
    case E:
        data_eeprom.temp_version = menu_data.i;
        data_eeprom.slow_LT_edge = -30;
        data_eeprom.slow_LT_val = 0.5;
        data_eeprom.midpoint = 300;
        break;
    }

    eeprom_lock();
}

const sSERVICE_ITEM service_items[] =
    {
        {"ain_adc_min", ESERVICE_TYPE_INT_P, &data_eeprom.ain_adc_min},
        {"ain_adc_max", ESERVICE_TYPE_INT_P, &data_eeprom.ain_adc_max},
        {"ain_ma_min", ESERVICE_TYPE_INT_P, &data_eeprom.ain_ma_min},
        {"ain_ma_max", ESERVICE_TYPE_INT_P, &data_eeprom.ain_ma_max},
        {"ain_ma_t_min", ESERVICE_TYPE_INT_P, &data_eeprom.ain_ma_t_min},
        {"ain_ma_t_max", ESERVICE_TYPE_INT_P, &data_eeprom.ain_ma_t_max},
        {"hmc_pos_t_min", ESERVICE_TYPE_INT_P, &data_eeprom.hmc_pos_t_min},
        {"hmc_pos_t_max", ESERVICE_TYPE_INT_P, &data_eeprom.hmc_pos_t_max},
        {"kp", ESERVICE_TYPE_FLOAT_U, &data_eeprom.kp},
        {"ki", ESERVICE_TYPE_FLOAT_U, &data_eeprom.ki},
        {"kd", ESERVICE_TYPE_FLOAT_U, &data_eeprom.kd},
        {"pid_int_min", ESERVICE_TYPE_FLOAT_PB, &data_eeprom.pid_int_min},
        {"pid_int_max", ESERVICE_TYPE_FLOAT_PB, &data_eeprom.pid_int_max},
        {"aout_pos_min", ESERVICE_TYPE_INT_U, &data_eeprom.aout_pos_min},
        {"aout_pos_max", ESERVICE_TYPE_INT_U, &data_eeprom.aout_pos_max},
        {"aout_ma_min", ESERVICE_TYPE_INT_U, &data_eeprom.aout_ma_min},
        {"aout_ma_max", ESERVICE_TYPE_INT_U, &data_eeprom.aout_ma_max},
        {"dout2_pos", ESERVICE_TYPE_INT_U, &data_eeprom.dout2_pos},
        {"dout2_gis", ESERVICE_TYPE_INT_U, &data_eeprom.dout2_gis},
        {"dout3_pos", ESERVICE_TYPE_INT_U, &data_eeprom.dout3_pos},
        {"dout3_gis", ESERVICE_TYPE_INT_U, &data_eeprom.dout3_gis},
        {"aout_pwm_min", ESERVICE_TYPE_INT_P, &data_eeprom.aout_pwm_min},
        {"aout_pwm_max", ESERVICE_TYPE_INT_P, &data_eeprom.aout_pwm_max},
        {"hard_pos_min", ESERVICE_TYPE_INT_U, &data_eeprom.hard_pos_min},
        {"hard_pos_max", ESERVICE_TYPE_INT_U, &data_eeprom.hard_pos_max},
        {"hmc_adc_0", ESERVICE_TYPE_INT_P, &data_eeprom.hmc_adc_0},
        {"hmc_adc_100", ESERVICE_TYPE_INT_P, &data_eeprom.hmc_adc_100},
        {"hmc_lim_0", ESERVICE_TYPE_INT_U, &data_eeprom.hmc_lim_0},
        {"hmc_lim_100", ESERVICE_TYPE_INT_U, &data_eeprom.hmc_lim_100},
        {"hmc_g_0", ESERVICE_TYPE_INT_P, &data_eeprom.hmc_g_0},
        {"hmc_g_100", ESERVICE_TYPE_INT_P, &data_eeprom.hmc_g_100},
        {"lpf_max_samples", ESERVICE_TYPE_INT_P, &data_eeprom.lpf_max_samples},
        {"lpf_a_out", ESERVICE_TYPE_INT_P, &data_eeprom.lpf_a_out},
        {"lpf_a_in", ESERVICE_TYPE_INT_P, &data_eeprom.lpf_a_in},
        {"lpf_pos", ESERVICE_TYPE_FLOAT_U, &data_eeprom.lpf_pos},
        {"coil_pwm_limit", ESERVICE_TYPE_INT_U, &data_eeprom.coil_pwm_limit},
        {"t_lp_up", ESERVICE_TYPE_FLOAT_U, &data_eeprom.t_lp_up},
        {"t_lp_down", ESERVICE_TYPE_FLOAT_U, &data_eeprom.t_lp_down},
        {"pst_dx", ESERVICE_TYPE_INT_U, &data_eeprom.pst_dx},
        {"pst_t", ESERVICE_TYPE_INT_U, &data_eeprom.pst_t},
        {"pst_te", ESERVICE_TYPE_INT_U, &data_eeprom.pst_te},
        {"pst_autorun", ESERVICE_TYPE_INT_U, &data_eeprom.pst_autorun},
        {"hif_dx", ESERVICE_TYPE_INT_U, &data_eeprom.hif_dx},
        {"hif_t", ESERVICE_TYPE_INT_U, &data_eeprom.hif_t},
        {"si_off", ESERVICE_TYPE_FLOAT_U, &data_eeprom.si_off},
        {"linear_en", ESERVICE_TYPE_INT_P, &data_eeprom.linear_en},
        {"linear_coef", ESERVICE_TYPE_INT_P, &data_eeprom.linear_coef},
        {"linear_shift", ESERVICE_TYPE_INT_P, &data_eeprom.linear_shift},
        {"sensor_type", ESERVICE_TYPE_INT_P, &data_eeprom.sensor_type},
        {"display_format", ESERVICE_TYPE_INT_P, &data_eeprom.display_format},
        {"temp_version", ESERVICE_TYPE_INT_P, &data_eeprom.temp_version},
        {"display_im", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 0, 1},
        {"display_ain", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 1, 1},
        {"display_contrast", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 2, 5},
        {"display_rotate", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 7, 1},
        {"hart_id", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 8, 4},
        {"hart_enable", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 12, 1},
        {"hart_prio", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 13, 1},
        {"hart_none", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 14, 1},
        {"coil_inverse", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 15, 1},
        {"aout_sig", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 16, 2},
        {"dout2_sig", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 18, 2},
        {"dout2_dir", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 20, 1},
        {"dout3_sig", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 21, 2},
        {"dout3_dir", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 23, 1},
        {"display_filter", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 24, 3},
        {"im_ph", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 27, 4},
        {"dout1_disable", ESERVICE_TYPE_FLAGS, &data_eeprom.flags, 31, 1},
        {"hmc_set_0", ESERVICE_TYPE_COMMAND, &Service_Command_SetPosition_0},
        {"hmc_set_100", ESERVICE_TYPE_COMMAND, &Service_Command_SetPosition_100},
        {"ain_set_4", ESERVICE_TYPE_COMMAND, &Service_Command_SetAIn_4},
        {"ain_set_20", ESERVICE_TYPE_COMMAND, &Service_Command_SetAIn_20},
        {"reboot", ESERVICE_TYPE_COMMAND, &Service_Command_Reboot},
        {"factory_reset", ESERVICE_TYPE_COMMAND, &Service_Command_FactoryReset},

        {"", ESERVICE_TYPE_NONE, 0},
};

void Service_Entry(Menu_Item_t *serviceMenu)
{
    if (button_pressed == service_buttons[service_fsm])
    {
        if (service_buttons[service_fsm])
            service_fsm++;
        else
        {
            Menu_GoTo(serviceMenu);
            service_fsm = 0;
        }
    }

    if (button_pressed == 0)
        service_fsm = 0;
}

void Service_Navigate(void)
{
    int i;
    DWORD t1, t2;

    if (menu_btn.btn_up)
    {
        menu_btn.clr_up = 1;
        if (service_data_pos == -1)
        {
            if (service_items[service_item_pos + 1].type != ESERVICE_TYPE_NONE)
                service_item_pos++;
            else
                service_item_pos = 0;
        }
        else
        {
            if (service_data[service_data_pos] > 0)
                service_data[service_data_pos]--;
            else
                service_data[service_data_pos] = 9;

            switch (service_items[service_item_pos].type)
            {
            case ESERVICE_TYPE_INT_U:
            case ESERVICE_TYPE_INT_P:
                if (service_data_pos == 4)
                    service_data[service_data_pos] = 0;
                break;

            case ESERVICE_TYPE_FLOAT_U:
            case ESERVICE_TYPE_FLOAT_P:
                if (service_data_pos == 6)
                    service_data[service_data_pos] = 0;
                break;

            case ESERVICE_TYPE_FLOAT_PB:
                if (service_data_pos == 0)
                    service_data[service_data_pos] = '-';
                if (service_data_pos == 7)
                    service_data[service_data_pos] = 0;
                break;

            case ESERVICE_TYPE_FLAGS:
                if (service_data_pos == 2)
                    service_data[service_data_pos] = 0;
                break;
            }
        }
    }

    if (menu_btn.btn_down)
    {
        menu_btn.clr_down = 1;

        if (service_data_pos == -1)
        {
            if (service_item_pos > 0)
                service_item_pos--;
            else
            {
                service_item_pos = 0;
                if (service_items[0].type != ESERVICE_TYPE_NONE)
                {
                    while (service_items[service_item_pos + 1].type != ESERVICE_TYPE_NONE)
                        service_item_pos++;
                }
            }
        }
        else
        {
            if (service_data[service_data_pos] < 9)
                service_data[service_data_pos]++;
            else
                service_data[service_data_pos] = 0;

            switch (service_items[service_item_pos].type)
            {
            case ESERVICE_TYPE_INT_U:
            case ESERVICE_TYPE_INT_P:
                if (service_data_pos == 4)
                    service_data[service_data_pos] = 1;
                break;

            case ESERVICE_TYPE_FLOAT_U:
            case ESERVICE_TYPE_FLOAT_P:
                if (service_data_pos == 6)
                    service_data[service_data_pos] = 1;
                break;

            case ESERVICE_TYPE_FLOAT_PB:
                if (service_data_pos == 0)
                    service_data[service_data_pos] = '+';
                if (service_data_pos == 7)
                    service_data[service_data_pos] = 1;
                break;

            case ESERVICE_TYPE_FLAGS:
                if (service_data_pos == 2)
                    service_data[service_data_pos] = 1;
                break;
            }
        }
    }

    if (menu_btn.btn_ok)
    {
        menu_btn.clr_ok = 1;
        if ((service_items[service_item_pos].type == ESERVICE_TYPE_COMMAND) && (service_items[service_item_pos].command))
            service_items[service_item_pos].command();

        if (service_data_pos == -1)
        {
            switch (service_items[service_item_pos].type)
            {
            case ESERVICE_TYPE_INT_U:
            case ESERVICE_TYPE_INT_P:
                t1 = *(int *)service_items[service_item_pos].addr;
                for (i = 0; i < 4; i++)
                {
                    service_data[3 - i] = t1 % 10;
                    t1 /= 10;
                }
                service_data[4] = 1;
                service_data_pos = 0;
                break;

            case ESERVICE_TYPE_FLOAT_U:
            case ESERVICE_TYPE_FLOAT_P:
                t1 = roundf((*(float *)service_items[service_item_pos].addr) * 1000.0f);
                for (i = 0; i < 6; i++)
                {
                    service_data[5 - i] = t1 % 10;
                    t1 /= 10;
                }
                service_data[6] = 1;
                service_data_pos = 0;
                break;

            case ESERVICE_TYPE_FLOAT_PB:
                i = roundf(*(float *)service_items[service_item_pos].addr);
                if (i < 0)
                {
                    i = -i;
                    service_data[0] = '-';
                }
                else
                    service_data[0] = '+';
                t1 = i;
                for (i = 0; i < 6; i++)
                {
                    service_data[6 - i] = t1 % 10;
                    t1 /= 10;
                }
                service_data[7] = 1;
                service_data_pos = 0;
                break;

            case ESERVICE_TYPE_FLAGS:
                t1 = ((*(DWORD *)service_items[service_item_pos].addr) >> service_items[service_item_pos].pos) & ((1 << service_items[service_item_pos].size) - 1);
                for (i = 0; i < 2; i++)
                {
                    service_data[1 - i] = t1 % 10;
                    t1 /= 10;
                }
                service_data[2] = 1;
                service_data_pos = 0;
                break;
            }
        }
        else
        {
            switch (service_items[service_item_pos].type)
            {
            case ESERVICE_TYPE_INT_U:
            case ESERVICE_TYPE_INT_P:
                if (service_data_pos < 4)
                    service_data_pos++;
                else
                {
                    if (service_data[4])
                    {
                        eeprom_unlock();
                        *(int *)service_items[service_item_pos].addr = ((service_data[0] * 10 + service_data[1]) * 10 + service_data[2]) * 10 + service_data[3];
                        eeprom_lock();
                    }
                    service_data_pos = -1;
                }
                break;

            case ESERVICE_TYPE_FLOAT_U:
            case ESERVICE_TYPE_FLOAT_P:
                if (service_data_pos < 6)
                    service_data_pos++;
                else
                {
                    if (service_data[6])
                    {
                        eeprom_unlock();
                        *(float *)service_items[service_item_pos].addr = 0.001f * (((((service_data[0] * 10 + service_data[1]) * 10 + service_data[2]) * 10 + service_data[3]) * 10 + service_data[4]) * 10 + service_data[5]);
                        eeprom_lock();
                    }
                    service_data_pos = -1;
                }
                break;

            case ESERVICE_TYPE_FLOAT_PB:
                if (service_data_pos < 7)
                    service_data_pos++;
                else
                {
                    if (service_data[7])
                    {
                        eeprom_unlock();
                        *(float *)service_items[service_item_pos].addr = ((service_data[0] == '+') ? 1.0f : -1.0f) * (((((service_data[1] * 10 + service_data[2]) * 10 + service_data[3]) * 10 + service_data[4]) * 10 + service_data[5]) * 10 + service_data[6]);
                        eeprom_lock();
                    }
                    service_data_pos = -1;
                }
                break;

            case ESERVICE_TYPE_FLAGS:
                if (service_data_pos < 2)
                    service_data_pos++;
                else
                {
                    if (service_data[2])
                    {
                        t1 = (*(DWORD *)service_items[service_item_pos].addr) & ~(((1 << service_items[service_item_pos].size) - 1) << service_items[service_item_pos].pos);
                        t2 = ((service_data[0] * 10 + service_data[1]) & ((1 << service_items[service_item_pos].size) - 1)) << service_items[service_item_pos].pos;

                        eeprom_unlock();
                        *(int *)service_items[service_item_pos].addr = t1 | t2;
                        eeprom_lock();
                    }
                    service_data_pos = -1;
                }
                break;
            }
        }
    }

    if (menu_btn.btn_cancel)
    {
        menu_btn.clr_cancel = 1;
        service_data_pos = -1;
    }
}

void Service_Render(void)
{
    int i;
    BYTE str[32];
    BYTE strh[32];

    pcf8531_pic(0, 0, picto_submenu_edit, 128, 32);
    pcf8531_clear(0, 32 - 7, 128, 7);
    str[0] = 0;
    mstr_add(str, "Инженерный режим");
    i = pcf8531_str_len(str, font9);
    pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
    pcf8531_str((128 - i) / 2, 2, str, font9);

    str[0] = 0;
    mstr_add(str, service_items[service_item_pos].name);
    pcf8531_str(0, 14, str, font9);

    str[0] = 0;
    mstr_add_int(str, service_item_pos + 1);
    i = pcf8531_str_len(str, font9);
    pcf8531_str(128 - i, 24, str, font9);

    switch (service_items[service_item_pos].type)
    {
    case ESERVICE_TYPE_NONE:
        break;

    case ESERVICE_TYPE_COMMAND:
        mstr_cpy(str, "CMD");
        i = pcf8531_str_len(str, font9);
        pcf8531_str(128 - i, 14, str, font9);
        mstr_cpy(str, "!выполнить");
        pcf8531_str(0, 24, str, font9);
        break;

    case ESERVICE_TYPE_INT_U:
    case ESERVICE_TYPE_INT_P:
        if (service_items[service_item_pos].type == ESERVICE_TYPE_INT_U)
            mstr_cpy(str, "iПЗ");
        else
            mstr_cpy(str, "iЗК");
        i = pcf8531_str_len(str, font9);
        pcf8531_str(128 - i, 14, str, font9);
        if (service_data_pos == -1)
        {
            mstr_cpy(str, "=");
            mstr_add_int(str, *(int *)service_items[service_item_pos].addr);
            pcf8531_str(0, 24, str, font9);
        }
        else
        {
            str[0] = 0;
            for (i = 0; i < 4; i++)
            {
                if (i == service_data_pos)
                    mstr_add(str, ">");
                mstr_add_int(str, service_data[i]);
            }
            if (service_data_pos == 4)
            {
                if (service_data[4])
                    mstr_add(str, " >записать");
                else
                    mstr_add(str, " >отмена");
            }
            pcf8531_str(0, 24, str, font9);
        }
        break;

    case ESERVICE_TYPE_FLOAT_U:
    case ESERVICE_TYPE_FLOAT_P:
        if (service_items[service_item_pos].type == ESERVICE_TYPE_FLOAT_U)
            mstr_cpy(str, "fПЗ");
        else
            mstr_cpy(str, "fЗК");

        i = pcf8531_str_len(str, font9);
        pcf8531_str(128 - i, 14, str, font9);
        if (service_data_pos == -1)
        {
            mstr_cpy(str, "=");
            FromFloat(strh, (*(float *)service_items[service_item_pos].addr) * 1000.0f, 3);
            mstr_add(str, strh);
            pcf8531_str(0, 24, str, font9);
        }
        else
        {
            str[0] = 0;
            for (i = 0; i < 6; i++)
            {
                if (i == 3)
                    mstr_add(str, ".");
                if (i == service_data_pos)
                    mstr_add(str, ">");
                mstr_add_int(str, service_data[i]);
            }
            if (service_data_pos == 6)
            {
                if (service_data[6])
                    mstr_add(str, " >записать");
                else
                    mstr_add(str, " >отмена");
            }
            pcf8531_str(0, 24, str, font9);
        }
        break;

    case ESERVICE_TYPE_FLOAT_PB:
        mstr_cpy(str, "fЗК");
        i = pcf8531_str_len(str, font9);
        pcf8531_str(128 - i, 14, str, font9);
        if (service_data_pos == -1)
        {
            mstr_cpy(str, "=");
            FromFloat(strh, (*(float *)service_items[service_item_pos].addr) * 10.0f, 1);
            mstr_add(str, strh);
            pcf8531_str(0, 24, str, font9);
        }
        else
        {
            str[0] = 0;
            for (i = 0; i < 7; i++)
            {
                if (i == service_data_pos)
                    mstr_add(str, ">");
                if (i > 0)
                    mstr_add_int(str, service_data[i]);
                else
                {
                    if (service_data[i] == '-')
                        mstr_add(str, "-");
                    if (service_data[i] == '+')
                        mstr_add(str, "+");
                }
            }
            if (service_data_pos == 7)
            {
                if (service_data[7])
                    mstr_add(str, " >записать");
                else
                    mstr_add(str, " >отмена");
            }
            pcf8531_str(0, 24, str, font9);
        }
        break;

    case ESERVICE_TYPE_FLAGS:
        mstr_cpy(str, "bПЗ");
        i = pcf8531_str_len(str, font9);
        pcf8531_str(128 - i, 14, str, font9);
        if (service_data_pos == -1)
        {
            mstr_cpy(str, "=");
            mstr_add_int(str,
                         ((*(DWORD *)service_items[service_item_pos].addr) >> service_items[service_item_pos].pos) & ((1 << service_items[service_item_pos].size) - 1));
            pcf8531_str(0, 24, str, font9);
        }
        else
        {
            str[0] = 0;
            for (i = 0; i < 2; i++)
            {
                if (i == service_data_pos)
                    mstr_add(str, ">");
                mstr_add_int(str, service_data[i]);
            }
            if (service_data_pos == 2)
            {
                if (service_data[2])
                    mstr_add(str, " >записать");
                else
                    mstr_add(str, " >отмена");
            }
            pcf8531_str(0, 24, str, font9);
        }
        break;
    }
}