/**
 * @file menu_render.�
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @brief ������ ������������ ������� ���� ������������.
 * @version 1.0
 * @date 2023-03-15
 *
 * @copyright (C) ���������, 2023
 */

#include "pcf8531.h"
#include "menu_v2.h"
#include "menu_navigate.h"
#include "menu_service.h"
#include "picto.h"
#include "define.h"
#include "font.h"
#include <math.h>
#include "hart.h"

/// @brief �������� ��������� ������ �������.
/// @return ��������� ������ �������.
eDISPLAY_FORMAT GetDisplayFormat(void)
{
    switch (data_eeprom.display_format)
    {
    case 1:
        return FROM_W;

    default:
        return FROM_D;
    }
}

/// @brief �������� ������ �� ������ ������� ���������.
/// @param str ��������� �� �������� ������.
/// @param value ������ � ������� ���������.
void FromPosition(BYTE *str, const int value)
{
    int h = value;
    str[0] = 0;

    if (h < 0)
    {
        h = -h;
        mstr_add(str, "-");
    }

    mstr_add_int(str, h / 10);
    mstr_add(str, ".");
    mstr_add_int(str, h % 10);
    mstr_add(str, "%");
}

/// @brief �������� ������ �� �������� ������ � ��������� ���������.
/// @param str ��������� �� �������� ������.
/// @param value �������� ������.
/// @param decimals ���������� ������ ����� �������.
void FromFloat(BYTE *str, const int value, const int decimals)
{
    int i;
    int x = value;
    str[0] = 0;

    if (x < 0)
    {
        x = -x;
        mstr_add(str, "-");
    }

    switch (decimals)
    {
    case 1:
        mstr_add_int(str, x / 10);
        mstr_add(str, ".");
        mstr_add_int(str, x % 10);
        break;
    case 3:
        mstr_add_int(str, x / 1000);
        mstr_add(str, ".");
        mstr_add_int(str, (x / 100) % 10);
        mstr_add_int(str, (x / 10) % 10);
        mstr_add_int(str, (x / 1) % 10);
        break;
    default:
        break;
    }
}

/// @brief ��������� ��������� ������� ��������.
/// @param text ������������ ������ ���� ��������.
void Render_SettingsScreens(char *text)
{
    pcf8531_FullClear();
    pcf8531_pic(0, 0, picto_menu, 128, 32);
    pcf8531_str_to((128 - pcf8531_str_len(text, font9)) / 2, 16, text, 128, font9);
}

/// @brief ��������� �������� �������� ������ ����.
/// @param text ��������� ������ ����.
void Render_MenuTitle(char *text)
{
    int i = pcf8531_str_len(text, font9);
    pcf8531_pic(0, -1, picto_submenu, 128, 32);
    pcf8531_clear((128 - i) / 2 - 2, 0, i + 4, 10);
    pcf8531_str((128 - i) / 2, 1, text, font9);
}

/// @brief ��������� ��������� ������ ����
void Render_MenuButtons()
{
    pcf8531_clear(0, 24, 128, 8);
    pcf8531_pic(0, 24, picto_buttons, 128, 8);
}

/// @brief ��������� ��������� ����� "�����"
void Render_MenuExitButton()
{
    pcf8531_clear(0, 24, 128, 8);
    pcf8531_pic(0, 24, picto_buttons, 128, 8);
    pcf8531_clear(0, 24, 64, 8);
    pcf8531_clear(90, 24, 64, 8);

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "�����");
    pcf8531_str(91, 23, str, font9);
}

/// @brief ��������� ��������� ������ �� ���� ��������.
/// @param selector �������� �������������.
/// @param y ������ �� ������� ���������� ���������� ��������.
/// @param first ������������ ������� ��������.
/// @param second  ������������ ������� ��������.
void RenderChoiceOfTwo(int selector, int y, char *first, char *second)
{
    int coordX, lenght;
    int coordY = y;
    int height = ((sFONT *)font9)->height;
    BYTE str[32];

    switch (selector)
    {
    case 0:
    {
        lenght = pcf8531_str_len(first, font9);
        coordX = (64 - lenght) / 2;

        pcf8531_fill(coordX - 1, coordY - 1, lenght + 1, height + 1);
        pcf8531_str_inverse(coordX, coordY, first, font9);
        pcf8531_str(64 + (64 - pcf8531_str_len(second, font9)) / 2, coordY, second, font9);
    }
    break;

    case 1:
    {
        pcf8531_str((64 - pcf8531_str_len(first, font9)) / 2, coordY, first, font9);

        lenght = pcf8531_str_len(second, font9);
        coordX = 64 + (64 - lenght) / 2;

        pcf8531_fill(coordX - 1, coordY - 1, lenght + 1, height + 1);
        pcf8531_str_inverse(coordX, coordY, second, font9);
    }
    break;
    }
}

/// @brief ��������� ��������� ���������� ������ ��� ���������� �����.
void RenderTestStartView()
{
    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

/// @brief ��������� ��������� ������ ����� ����������� ������ � ��������� �������.
/// @param index ����� ���������� ������.
void RenderTestDO(int index)
{
    pcf8531_FullClear();

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "���������� ����� ");
    mstr_add_int(str, index);
    Render_MenuTitle(str);

    if (menu_switch == -1)
        menu_data.i = 0;

    RenderChoiceOfTwo(menu_data.i, 14, "����.", "���.");
    Render_MenuExitButton();
}

/// @brief ��������� ��������� �������� ������� � �������� �� ��������� ������.
/// @param value �������� �������.
/// @param y �������� ������, �� ������� ����� ��������� ���������.
void RenderTimeInSecondValue(int value, int y)
{
    BYTE str[32];
    str[0] = 0;

    FromFloat(str, value * 5, 1);
    mstr_add(str, "c");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, y, str, font9);
}

/// @brief ��������� ��������� ������ ������� ���.
void RenderLpf(void)
{
    BYTE str[32];
    str[0] = 0;

    switch (menu_data.i)
    {
    case LPF_OFF:
        mstr_cpy(str, "��������j");
        break;

    case LPF_MIN:
        mstr_cpy(str, "�������");
        break;

    case LPF_MID:
        mstr_cpy(str, "�������");
        break;

    case LPF_MAX:
        mstr_cpy(str, "��������");
        break;
    }

    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

/// @brief ��������� ��������� ���������.
void RenderQuestion(void)
{    
    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "�� �������?");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
    Render_MenuButtons();
}

/// @brief ��������� ��������� ��������� ������ � ��������� ������ ������.
/// @param mode ����� ������: \n \b 0 - ��������������; \n \b 1 - ������
void Menu_RenderView_Main(const int mode)
{
    int i, s;
    eDISPLAY_FORMAT displayFormat = GetDisplayFormat();

    //-------------------------------------
    //	���������. ������
    //-------------------------------------

    pcf8531_FullClear();
    pcf8531_pic(0, 24, picto_line, 128, 8);

    i = ihmc_pos_d;
    if (i != -1)
        pcf8531_pica(2 + i * 120 / 1000, 25, picto_pointer, 4, 7);

    i = ihmc_pos_t;
    if (i != -1)
        pcf8531_pica(2 + i * 122 / 1000, 25, picto_pointer_t, 2, 7);

    if (mode == 0)
    {
        if (hart_ma == -1)
        {
            // AIN
            if (data_eeprom.flags.display_ain == DISPLAY_AIN_MA)
            {
                //-------------------------------------
                //	������� ������ ��
                //-------------------------------------
                pcf8531_pic(81, 16, picto_ma_small_place, 35, 8);
                i = ain_ma_s;
                if (i < 0)
                    i = 0;
                if (i >= 1000)
                    pcf8531_pic(83, 16, picto_digit_small[(i / 1000) % 10], 5, 8);
                pcf8531_pic(89, 16, picto_digit_small[(i / 100) % 10], 5, 8);
                pcf8531_pic(97, 16, picto_digit_small[(i / 10) % 10], 5, 8);
            }
            else
            {
                //-------------------------------------
                //	������� ������ ��������
                //-------------------------------------
                pcf8531_pic(81, 16, picto_proc_small_place, 35, 8);
                i = ain_ma_s;
                if (0)
                {
                }
                else if (i < data_eeprom.ain_ma_t_min)
                    i = data_eeprom.hmc_pos_t_min;
                else if (i > data_eeprom.ain_ma_t_max)
                    i = data_eeprom.hmc_pos_t_max;
                else
                {
                    i = data_eeprom.hmc_pos_t_min + (i - data_eeprom.ain_ma_t_min) * (data_eeprom.hmc_pos_t_max - data_eeprom.hmc_pos_t_min) / (data_eeprom.ain_ma_t_max - data_eeprom.ain_ma_t_min);
                }
                if (i < 0)
                    i = 0;
                if (i > 1000)
                    i = 1000;
                if (i >= 1000)
                    pcf8531_pic(82, 16, picto_digit_small[(i / 1000) % 10], 5, 8);
                if (i >= 100)
                    pcf8531_pic(88, 16, picto_digit_small[(i / 100) % 10], 5, 8);
                pcf8531_pic(94, 16, picto_digit_small[(i / 10) % 10], 5, 8);
                pcf8531_pic(102, 16, picto_digit_small[(i / 1) % 10], 5, 8);
            }
        }
        else
        {
            // HART
            if (data_eeprom.flags.display_ain == DISPLAY_AIN_MA)
            {
                //-------------------------------------
                //	������� ������ ��
                //-------------------------------------
                pcf8531_pic(81, 16, picto_ma_small_place, 35, 8);
                i = hart_ma;
                if (i < 0)
                    i = 0;
                if (i >= 1000)
                    pcf8531_pic(83, 16, picto_digit_small[(i / 1000) % 10], 5, 8);
                pcf8531_pic(89, 16, picto_digit_small[(i / 100) % 10], 5, 8);
                pcf8531_pic(97, 16, picto_digit_small[(i / 10) % 10], 5, 8);
            }
            else
            {
                //-------------------------------------
                //	������� ������ ��������
                //-------------------------------------
                pcf8531_pic(81, 16, picto_proc_small_place, 35, 8);
                if (0)
                {
                }
                else if (hart_ma < data_eeprom.ain_ma_t_min)
                    i = data_eeprom.hmc_pos_t_min;
                else if (hart_ma > data_eeprom.ain_ma_t_max)
                    i = data_eeprom.hmc_pos_t_max;
                else
                {
                    i = data_eeprom.hmc_pos_t_min + (hart_ma - data_eeprom.ain_ma_t_min) * (data_eeprom.hmc_pos_t_max - data_eeprom.hmc_pos_t_min) / (data_eeprom.ain_ma_t_max - data_eeprom.ain_ma_t_min);
                }
                if (i < 0)
                    i = 0;
                if (i > 1000)
                    i = 1000;
                if (i >= 1000)
                    pcf8531_pic(82, 16, picto_digit_small[(i / 1000) % 10], 5, 8);
                if (i >= 100)
                    pcf8531_pic(88, 16, picto_digit_small[(i / 100) % 10], 5, 8);
                pcf8531_pic(94, 16, picto_digit_small[(i / 10) % 10], 5, 8);
                pcf8531_pic(102, 16, picto_digit_small[(i / 1) % 10], 5, 8);
            }
        }
    }
    else
    {
        //-------------------------------------
        //	������ ������� ��������
        //-------------------------------------
        i = ihmc_pos_t;
        pcf8531_pic(77, 16, picto_hand_pos, 39, 8);

        if (i >= 1000)
            pcf8531_pic(77, 16, picto_digit_small[(i / 1000) % 10], 5, 8);

        if (i >= 100)
            pcf8531_pic(83, 16, picto_digit_small[(i / 100) % 10], 5, 8);

        pcf8531_pic(89, 16, picto_digit_small[(i / 10) % 10], 5, 8);
        pcf8531_pic(97, 16, picto_digit_small[(i / 1) % 10], 5, 8);
    }

    //-------------------------------------
    //	���������. �����
    //-------------------------------------

    switch (displayFormat)
    {
    case FROM_D:
        pcf8531_pic(12, 5, picto_pos_large, 63, 17);
        break;

    case FROM_W:
        pcf8531_clear(12, 5, 63, 17);
        break;
    }

    if (data_eeprom.flags.display_im == DISPLAY_IM_GRAD)
    {
        i = ihmc_g;

        if (displayFormat == FROM_W)
            pcf8531_pic(66, 4, picto_main_grad, 9, 18);
    }
    else
    {
        i = ihmc_pos_d;
    }

    switch (displayFormat)
    {
    case FROM_D:
    {
        if (i < 0)
        {
            i = -i;
            pcf8531_fill(11, 4, 55, 19);
            pcf8531_pic_invert(43, 17, picto_dot_large, 5, 5);
            if (i < 1000)
            {
                if (i >= 100)
                    pcf8531_pic_invert(12, 5, picto_digit_large[(i / 100) % 10], 14, 17);
                pcf8531_pic_invert(27, 5, picto_digit_large[(i / 10) % 10], 14, 17);
                pcf8531_pic_invert(51, 5, picto_digit_large[(i / 1) % 10], 14, 17);
            }
            else
            {
                pcf8531_pic_invert(21, 5, picto_digit_large[(i / 1000) % 10], 14, 17);
                pcf8531_pic_invert(36, 5, picto_digit_large[(i / 100) % 10], 14, 17);
                pcf8531_pic_invert(51, 5, picto_digit_large[(i / 10) % 10], 14, 17);
            }
        }
        else
        {
            pcf8531_clear(11, 4, 55, 19);
            pcf8531_pic(43, 17, picto_dot_large, 5, 5);
            if (i < 1000)
            {
                if (i >= 100)
                    pcf8531_pic(12, 5, picto_digit_large[(i / 100) % 10], 14, 17);
                pcf8531_pic(27, 5, picto_digit_large[(i / 10) % 10], 14, 17);
                pcf8531_pic(51, 5, picto_digit_large[(i / 1) % 10], 14, 17);
            }
            else
            {
                pcf8531_pic(21, 5, picto_digit_large[(i / 1000) % 10], 14, 17);
                pcf8531_pic(36, 5, picto_digit_large[(i / 100) % 10], 14, 17);
                pcf8531_pic(51, 5, picto_digit_large[(i / 10) % 10], 14, 17);
            }
        }
    }
    break;

    case FROM_W:
    {
        if (i < 0)
        {
            i = -i;
            pcf8531_fill(11, 4, 64, 19);
            pcf8531_pic_invert(43 + 9, 17, picto_dot_large, 5, 5);
            if (i < 1000)
            {
                if (i >= 100)
                    pcf8531_pic_invert(12 + 9, 5, picto_digit_large[(i / 100) % 10], 14, 17);
                pcf8531_pic_invert(27 + 9, 5, picto_digit_large[(i / 10) % 10], 14, 17);
                pcf8531_pic_invert(51 + 9, 5, picto_digit_large[(i / 1) % 10], 14, 17);
            }
            else
            {
                pcf8531_pic_invert(21 + 9, 5, picto_digit_large[(i / 1000) % 10], 14, 17);
                pcf8531_pic_invert(36 + 9, 5, picto_digit_large[(i / 100) % 10], 14, 17);
                pcf8531_pic_invert(51 + 9, 5, picto_digit_large[(i / 10) % 10], 14, 17);
            }
        }
        else
        {
            pcf8531_clear(11, 4, 55 + 9, 19);
            pcf8531_pic(43 + 9, 17, picto_dot_large, 5, 5);
            if (i < 1000)
            {
                if (i >= 100)
                    pcf8531_pic(12 + 9, 5, picto_digit_large[(i / 100) % 10], 14, 17);
                pcf8531_pic(27 + 9, 5, picto_digit_large[(i / 10) % 10], 14, 17);
                pcf8531_pic(51 + 9, 5, picto_digit_large[(i / 1) % 10], 14, 17);
            }
            else
            {
                pcf8531_pic(21 + 9, 5, picto_digit_large[(i / 1000) % 10], 14, 17);
                pcf8531_pic(36 + 9, 5, picto_digit_large[(i / 100) % 10], 14, 17);
                pcf8531_pic(51 + 9, 5, picto_digit_large[(i / 10) % 10], 14, 17);
            }
        }
    }
    break;
    }

    //-------------------------------------
    //	�����������
    //-------------------------------------
    if (mode == 0)
    {
        if (!show_ph)
        {
            if (hart_ma == -1)
                pcf8531_pic(77, 3, picto_auto, 39, 10);
            else
                pcf8531_pic(77, 3, picto_hart, 39, 10);
        }
        else
        {
            // ain_ma
            pcf8531_clear(77, 3, 39, 10);
            pcf8531_pic(81, 5, picto_ma_small_place, 35, 8);
            i = ain_ma;
            if (i < 0)
                i = 0;
            if (i >= 1000)
                pcf8531_pic(83, 5, picto_digit_small[(i / 1000) % 10], 5, 8);
            pcf8531_pic(89, 5, picto_digit_small[(i / 100) % 10], 5, 8);
            pcf8531_pic(97, 5, picto_digit_small[(i / 10) % 10], 5, 8);
        }
    }
    else
        pcf8531_pic(77, 3, picto_hand, 39, 10);

    if ((data_eeprom.flags.hart_enable == HART_ENABLE_ON) && ((to_hart >= TO_HART) || (disp_update_count & 1)))
        pcf8531_pic(1, 2, picto_hart_flag, 9, 9);
    else
        pcf8531_pic(1, 2, picto_flag, 9, 9);

    if (0)
        pcf8531_pic(1, 12, picto_limits_flag, 9, 9);
    else
        pcf8531_pic(1, 12, picto_flag, 9, 9);

    if ((data_eeprom.hard_pos_min != 0) || (data_eeprom.hard_pos_max != 0))
    {
        pcf8531_pic(118, 2, picto_press_flag, 9, 9);

        i = data_eeprom.hard_pos_min;
        if ((0 < i) && (i < 1000))
            pcf8531_fill(2, 28, i * 124 / 1000, 2);

        if (ihmc_pos_t < (0 + data_eeprom.hard_pos_min))
            pcf8531_pic(0, 23, picto_hard_min, 3, 5);

        i = data_eeprom.hard_pos_max;
        if ((0 < i) && (i < 1000))
            pcf8531_fill(126 - i * 124 / 1000, 28, i * 124 / 1000, 2);

        if (ihmc_pos_t > (1000 - data_eeprom.hard_pos_max))
            pcf8531_pic(125, 23, picto_hard_max, 3, 5);
    }

    if (event_flag)
        pcf8531_pic(118, 12, picto_status_err, 9, 9);
    else if (event_flag_last)
        pcf8531_pic(118, 12, picto_status_err_last, 9, 9);
    else
        pcf8531_pic(118, 12, picto_status_ok, 9, 9);
}

void Menu_RenderView_Start(void)
{
    BYTE str[32];

    pcf8531_pic(0, 0, picto_start, 128, 32);

    mstr_cpy(str, "AM800, ���. �� ");
    mstr_add_int(str, SW_VER_MAJ);
    mstr_add(str, ".");
    mstr_add_int(str, SW_VER_MIN);
    mstr_add(str, ".");
    mstr_add_int(str, SW_VER_BUILD);
    pcf8531_str_to(0, 23, str, 127, font9);
}

void Menu_RenderView_Service(void)
{
    Service_Render();
}

void Menu_RenderView_Home(void)
{
    Menu_RenderView_Main(0);
}

void Menu_RenderView_Info1(void)
{
    int i;
    BYTE str[32];

    pcf8531_pic(0, 0, picto_info_1, 128, 32);

    str[0] = 0;
    i = uptime;
    if (i < 60)
    {
        mstr_add_int(str, i);
        mstr_add(str, "�");
    }
    else if (i < (60 * 60))
    {
        mstr_add_int(str, i / 60);
        mstr_add(str, "�");
        mstr_add_int(str, i % 60);
        mstr_add(str, "�");
    }
    else if (i < (24 * 60 * 60))
    {
        i /= 60;
        mstr_add_int(str, i / 60);
        mstr_add(str, "�");
        mstr_add_int(str, i % 60);
        mstr_add(str, "�");
    }
    else
    {
        i /= 60 * 60;
        mstr_add_int(str, i / 24);
        mstr_add(str, "�");
        mstr_add_int(str, i % 24);
        mstr_add(str, "�");
    }
    pcf8531_str_to(18, 2, str, 88, font9);

    str[0] = 0;
    i = worktime;
    if (i < 60)
    {
        mstr_add_int(str, i);
        mstr_add(str, "�");
    }
    else if (i < (60 * 60))
    {
        mstr_add_int(str, i / 60);
        mstr_add(str, "�");
        mstr_add_int(str, i % 60);
        mstr_add(str, "�");
    }
    else if (i < (24 * 60 * 60))
    {
        i /= 60;
        mstr_add_int(str, i / 60);
        mstr_add(str, "�");
        mstr_add_int(str, i % 60);
        mstr_add(str, "�");
    }
    else
    {
        i /= 60 * 60;
        mstr_add_int(str, i / 24);
        mstr_add(str, "�");
        mstr_add_int(str, i % 24);
        mstr_add(str, "�");
    }
    pcf8531_str_to(18, 12, str, 81, font9);

    str[0] = 0;
    i = ihmc_path / 200;
    mstr_add_int(str, i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, i % 10);
    mstr_add(str, "�");
    pcf8531_str_to(21, 22, str, 74, font9);

    i = tempeature;
    if (i < 0)
    {
        pcf8531_pic(104, 2, picto_minus_small, 3, 8);
        i = -i;
    }
    if (i >= 10)
        pcf8531_pic(108, 2, picto_digit_small[(i / 10) % 10], 5, 8);
    pcf8531_pic(114, 2, picto_digit_small[(i / 1) % 10], 5, 8);

    mstr_cpy(str, "�� v");
    mstr_add_int(str, SW_VER_MAJ);
    mstr_add(str, ".");
    mstr_add_int(str, SW_VER_MIN);
    mstr_add(str, ".");
    mstr_add_int(str, SW_VER_BUILD);
    i = pcf8531_str_len(str, font9);
    pcf8531_str(128 - 1 - i, 12, str, font9);

    mstr_cpy(str, "sn:");

    if (!image.serial)
        mstr_add(str, " �� �����");
    else
        mstr_add_int(str, image.serial);

    pcf8531_clear(64, 22, 63, 9);
    i = pcf8531_str_len(str, font9);
    pcf8531_str(128 - 1 - i, 22, str, font9);
}

void Menu_RenderView_Info2(void)
{
    int i;
    BYTE str[32];

    pcf8531_pic(0, 0, picto_info_2, 128, 32);

    //-------------------------------------
    //	��������� �� ��������
    //-------------------------------------
    i = ihmc_pos_d;
    if (i < 1000)
    {
        if (i >= 100)
            pcf8531_pic(2, 2, picto_digit_small[(i / 100) % 10], 5, 8);

        pcf8531_pic(8, 2, picto_digit_small[(i / 10) % 10], 5, 8);
        pcf8531_pic(14, 2, picto_digit_small_dot, 1, 8);
        pcf8531_pic(16, 2, picto_digit_small[(i / 1) % 10], 5, 8);
    }
    else
    {
        pcf8531_pic(4, 2, picto_digit_small[(i / 1000) % 10], 5, 8);
        pcf8531_pic(10, 2, picto_digit_small[(i / 100) % 10], 5, 8);
        pcf8531_pic(16, 2, picto_digit_small[(i / 10) % 10], 5, 8);
    }

    //-------------------------------------
    //	��������� �� �������
    //-------------------------------------
    i = ihmc_g;

    if (i < 0)
    {
        i = -i;
        pcf8531_fill(1, 12, 21, 10);
        if (i < 1000)
        {
            if (i >= 100)
                pcf8531_pic_invert(2, 13, picto_digit_small[(i / 100) % 10], 5, 8);

            pcf8531_pic_invert(8, 13, picto_digit_small[(i / 10) % 10], 5, 8);
            pcf8531_pic_invert(14, 13, picto_digit_small_dot, 1, 8);
            pcf8531_pic_invert(16, 13, picto_digit_small[(i / 1) % 10], 5, 8);
        }
        else
        {
            pcf8531_pic_invert(4, 13, picto_digit_small[(i / 1000) % 10], 5, 8);
            pcf8531_pic_invert(10, 13, picto_digit_small[(i / 100) % 10], 5, 8);
            pcf8531_pic_invert(16, 13, picto_digit_small[(i / 10) % 10], 5, 8);
        }
    }
    else
    {
        pcf8531_clear(1, 12, 21, 10);
        if (i < 1000)
        {
            if (i >= 100)
                pcf8531_pic(2, 13, picto_digit_small[(i / 100) % 10], 5, 8);

            pcf8531_pic(8, 13, picto_digit_small[(i / 10) % 10], 5, 8);
            pcf8531_pic(14, 13, picto_digit_small_dot, 1, 8);
            pcf8531_pic(16, 13, picto_digit_small[(i / 1) % 10], 5, 8);
        }
        else
        {
            pcf8531_pic(4, 13, picto_digit_small[(i / 1000) % 10], 5, 8);
            pcf8531_pic(10, 13, picto_digit_small[(i / 100) % 10], 5, 8);
            pcf8531_pic(16, 13, picto_digit_small[(i / 10) % 10], 5, 8);
        }
    }

    //-------------------------------------
    //	������� ������ ��
    //-------------------------------------
    i = ain_ma_s;

    if (i < 0)
        i = 0;

    if (i >= 1000)
        pcf8531_pic(51, 13, picto_digit_small[(i / 1000) % 10], 5, 8);

    pcf8531_pic(57, 13, picto_digit_small[(i / 100) % 10], 5, 8);
    pcf8531_pic(63, 13, picto_digit_small_dot, 1, 8);
    pcf8531_pic(65, 13, picto_digit_small[(i / 10) % 10], 5, 8);

    //-------------------------------------
    //	��������� ��-���� ��������
    //-------------------------------------
    i = ihmc_pos_t;

    if (i < 1000)
    {
        if (i >= 100)
            pcf8531_pic(57, 2, picto_digit_small[(i / 100) % 10], 5, 8);

        pcf8531_pic(63, 2, picto_digit_small[(i / 10) % 10], 5, 8);
        pcf8531_pic(69, 2, picto_digit_small_dot, 1, 8);
        pcf8531_pic(71, 2, picto_digit_small[(i / 1) % 10], 5, 8);
    }
    else
    {
        pcf8531_pic(59, 2, picto_digit_small[(i / 1000) % 10], 5, 8);
        pcf8531_pic(65, 2, picto_digit_small[(i / 100) % 10], 5, 8);
        pcf8531_pic(71, 2, picto_digit_small[(i / 10) % 10], 5, 8);
    }

    //-------------------------------------
    //	�������� ������ ��
    //-------------------------------------
    i = aout_ma;
    if (i < 0)
        i = 0;

    i = (i + 5) / 10;

    str[0] = 0;
    mstr_add(str, "��:");

    if (data_eeprom.flags.aout_sig == OUT_SIG_NONE)
    {
        pcf8531_clear(87, 1, 40, 10);
        pcf8531_str(88, 2, str, font9);
        pcf8531_str(105, 2, "���", font9);
    }
    else
    {
        if (aout_diag)
        {
            pcf8531_fill(87, 1, 40, 10);
            pcf8531_str_inverse(88, 2, str, font9);
            str[0] = 0;
            mstr_add_int(str, i);
            mstr_add(str, "��");
            pcf8531_str_inverse(127 - pcf8531_str_len(str, font9), 2, str, font9);
        }
        else
        {
            pcf8531_clear(87, 1, 40, 10);
            pcf8531_str(88, 2, str, font9);
            str[0] = 0;
            mstr_add_int(str, i);
            mstr_add(str, "��");
            pcf8531_str(127 - pcf8531_str_len(str, font9), 2, str, font9);
        }
    }

    //-------------------------------------
    //	������� NAMUR
    //-------------------------------------
    for (i = 0; i < 3; i++)
    {
        if (((i == 0) && (data_eeprom.flags.dout1_disable == DOUT1_DISABLE)) || ((i == 1) && (data_eeprom.flags.dout2_sig == OUT_SIG_NONE)) || ((i == 2) && (data_eeprom.flags.dout3_sig == OUT_SIG_NONE)))
        {
            // none
            pcf8531_pic(104 + i * 8 + 1, 13, picto_minus_small, 3, 8);
        }
        else
        {
            if (namur_out[i])
            {
                pcf8531_pic(103 + i * 8, 12, picto_namur_invert, 7, 10);
                if (namur_diag[i])
                    pcf8531_pic_invert(104 + i * 8, 13, picto_digit_small[10], 5, 8);
                else
                    pcf8531_pic_invert(104 + i * 8, 13, picto_digit_small[i + 1], 5, 8);
            }
            else
            {
                if (namur_diag[i])
                    pcf8531_pic(104 + i * 8, 13, picto_digit_small[10], 5, 8);
                else
                    pcf8531_pic(104 + i * 8, 13, picto_digit_small[i + 1], 5, 8);
            }
        }
    }

    //-------------------------------------
    //	��������� �� ������
    //-------------------------------------
    i = ihmc_pos;
    if (i != -1)
        pcf8531_pica(2 + i * 120 / 1000, 25, picto_pointer, 4, 7);

    //-------------------------------------
    //	��������� ��-���� ������
    //-------------------------------------
    i = ihmc_pos_t;
    if (i != -1)
        pcf8531_pica(2 + i * 122 / 1000, 25, picto_pointer_t, 2, 7);
}

void Menu_RenderView_Info3(void)
{
    BYTE str[32];

    pcf8531_FullClear();

    FromPosition(str, ihmc_pos);
    pcf8531_str(0, 0, str, Font_Single);

    FromPosition(str, ihmc_pos_t);
    pcf8531_str(40, 0, str, Font_Single);

    str[0] = 0;
    mstr_add_int(str, PWM_COIL);
    mstr_add(str, "�/");
    mstr_add_int(str, info_coil_ofs);
    pcf8531_str(80, 0, str, Font_Single);

    FromFloat(str, roundf(info_hmc_d * 1000.0f), 3);
    mstr_add(str, "�");
    pcf8531_str(0, 8, str, Font_Single);

    if (to_power_fail < TO_POWER_FAIL)
        mstr_cpy(str, "�� ����");
    else
        mstr_cpy(str, "�� ����");

    pcf8531_str(0, 16, str, Font_Single);

    FromFloat(str, roundf(info_coil * 1000.0f), 3);
    mstr_add(str, "�");
    pcf8531_str(0, 24, str, Font_Single);

    FromFloat(str, roundf(info_hmc_d_pr * 1000.0f), 3);
    mstr_add(str, "�");
    pcf8531_str(64, 8, str, Font_Single);

    FromFloat(str, roundf(info_hmc_d_int * 1000.0f), 3);
    mstr_add(str, "�");
    pcf8531_str(64, 16, str, Font_Single);

    FromFloat(str, roundf(info_hmc_d_dif * 1000.0f), 3);
    mstr_add(str, "�");
    pcf8531_str(64, 24, str, Font_Single);
}

void Menu_RenderView_Setup(void)
{
    Render_SettingsScreens("��������� ��");
}

void Menu_RenderView_Log(void)
{
    Render_SettingsScreens("������ �������");
}

void Menu_RenderView_Signal(void)
{
    Render_SettingsScreens("������������");
}

void Menu_RenderView_Diag(void)
{
    Render_SettingsScreens("�����������");
}

void Menu_RenderView_Hart(void)
{
    Render_SettingsScreens("HART");
}

void Menu_RenderView_Display(void)
{
    Render_SettingsScreens("�������");
}

void Menu_RenderView_Hand(void)
{
    Render_SettingsScreens("������ �����");
}

void Menu_RenderView_HandEnable(void)
{
    Menu_RenderView_Main(1);
}

void Menu_RenderView_Calib(void)
{
    int i;
    BYTE str[32];

    pcf8531_FullClear();
    Render_MenuTitle("����������");

    str[0] = 0;

    i = data_eeprom.hmc_lim_0;
    mstr_add_int(str, i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, i % 10);
    mstr_add(str, "% .. ");

    i = data_eeprom.hmc_lim_100;
    mstr_add_int(str, i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, i % 10);
    mstr_add(str, "%");

    i = pcf8531_str_len(str, font9);
    pcf8531_str((128 - i) / 2, 14, str, font9);
}

void Menu_RenderView_CalibTest(void)
{
    BYTE str[32];
    str[0] = 0;

    pcf8531_FullClear();
    Render_MenuTitle("����");

    mstr_add(str, "���");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CalibTestEdit(void)
{
    BYTE str[32];
    str[0] = 0;

    pcf8531_FullClear();
    Render_MenuTitle("����");

    switch (menu_calib_pwm)
    {
    case MENU_CALIB_PWM_0:
        mstr_add(str, "�������");
        break;
    case MENU_CALIB_PWM_50:
        mstr_add(str, "��������");
        break;
    case MENU_CALIB_PWM_100:
        mstr_add(str, "��������");
        break;
    }

    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
    pcf8531_pic(0, 24, picto_calib_pwm, 128, 8);
}

void Menu_RenderView_CalibMin(void)
{
    int i;
    BYTE str[32];
    str[0] = 0;

    pcf8531_FullClear();
    Render_MenuTitle("�������");

    i = get_hmc_pos();
    mstr_add_int(str, i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, i % 10);
    mstr_add(str, "% -> ");

    i = data_eeprom.hmc_lim_0;
    mstr_add_int(str, i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, i % 10);
    mstr_add(str, "%");

    i = pcf8531_str_len(str, font9);
    pcf8531_str((128 - i) / 2, 14, str, font9);
}

void Menu_RenderView_CalibMinEdit(void)
{
    Menu_RenderView_CalibMin();
    Render_MenuButtons();
}

void Menu_RenderView_CalibMax(void)
{
    int i;
    BYTE str[32];
    str[0] = 0;

    pcf8531_FullClear();
    Render_MenuTitle("��������");

    i = get_hmc_pos();
    mstr_add_int(str, i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, i % 10);
    mstr_add(str, "% -> ");

    i = data_eeprom.hmc_lim_100;
    mstr_add_int(str, i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, i % 10);
    mstr_add(str, "%");

    i = pcf8531_str_len(str, font9);
    pcf8531_str((128 - i) / 2, 14, str, font9);
}

void Menu_RenderView_CalibMaxEdit(void)
{
    Menu_RenderView_CalibMax();
    Render_MenuButtons();
}

void Menu_RenderView_Autotune(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������������");

    int avg, delta;
    BYTE str[32];
    str[0] = 0;

    switch (st_auto)
    {
    case ST_AUTO_IDLE:
    {
        mstr_cpy(str, "��������!!! ������");
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

        mstr_cpy(str, "����� ���������");
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
    }
    break;

    case ST_AUTO_SELECT:
    {
        mstr_cpy(str, "�������� ��� �����");
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
        RenderChoiceOfTwo(auto_type, 23, "������", "��������");
    }
    break;

    case ST_AUTO_START:
    {
    }
    break;

    case ST_AUTO_DOWN_LIMIT:
    {
        mstr_cpy(str, "����������� ��������");
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

        if (stat_pos_arr_valid)
        {
            avg = stat_pos_avg;
            delta = stat_pos_delta;

            str[0] = 0;
            mstr_add_int(str, avg);
            mstr_add(str, ", ");
            mstr_add_int(str, delta);
            pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
        }
        else
        {
            mstr_cpy(str, "���� ���� ����������");
            pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
        }
    }
    break;

    case ST_AUTO_UP_LIMIT:
    {
        mstr_cpy(str, "����������� ���������");
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

        if (stat_pos_arr_valid)
        {
            avg = stat_pos_avg;
            delta = stat_pos_delta;

            str[0] = 0;
            mstr_add_int(str, avg);
            mstr_add(str, ", ");
            mstr_add_int(str, delta);
            pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
        }
        else
        {
            mstr_cpy(str, "���� ���� ����������");
            pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
        }
    }
    break;

    case ST_AUTO_LIMIT_DONE:
    {
        mstr_cpy(str, "�������� �������");
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

        str[0] = 0;
        mstr_add_int(str, stat_pos_adc_min);
        mstr_add(str, " .. ");
        mstr_add_int(str, stat_pos_adc_max);
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
    }
    break;

    case ST_AUTO_DOWN_TIME:
    {
        mstr_cpy(str, "����� ���� ����");
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

        str[0] = 0;
        FromPosition(str, ihmc_pos);
        mstr_add(str, ", ");
        mstr_add_int(str, stat_time_down);
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
    }
    break;

    case ST_AUTO_UP_TIME:
    {
        mstr_cpy(str, "����� ���� �����");
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

        str[0] = 0;
        FromPosition(str, ihmc_pos);
        mstr_add(str, ", ");
        mstr_add_int(str, stat_time_up);
        pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
    }
    break;

    case ST_AUTO_TIME_DONE:
    {
        if (stat_pos_arr_valid)
        {
            if (to_auto < 10000)
            {
                mstr_cpy(str, "��������� 50%");
                pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
            }
            else
            {
                mstr_cpy(str, "������� ������");
                pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
            }

            str[0] = 0;
            delta = stat_pos_delta;
            FromPosition(str, ihmc_pos);
            mstr_add(str, ", ");
            mstr_add_int(str, delta / 10);
            mstr_add(str, ".");
            mstr_add_int(str, delta % 10);
            mstr_add(str, "%, ");
            mstr_add_int(str, stat_coil_pwm);
            pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
        }
        else
        {
            mstr_cpy(str, "����� ���� ����");
            pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

            str[0] = 0;
            mstr_add_int(str, stat_time_down);
            mstr_add(str, ", ");
            mstr_add_int(str, stat_time_up);
            pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
        }
    }
    break;
    }
}

void Menu_RenderView_Scale(void)
{
    if (menu_switch == -1)
        menu_data.f.coil_inverse = data_eeprom.flags.coil_inverse;

    pcf8531_FullClear();
    Render_MenuTitle("�����");
    RenderChoiceOfTwo(menu_data.f.coil_inverse, 14, "������", "��������");
}

void Menu_RenderView_ScaleEdit(void)
{
    Menu_RenderView_Scale();
    Render_MenuButtons();
}

void Menu_RenderView_Linearization(void)
{
    if (menu_switch == -1)
        menu_data.i = data_eeprom.linear_en;

    pcf8531_FullClear();
    Render_MenuTitle("������������");
    RenderChoiceOfTwo(menu_data.i, 14, "����.", "���.");
}

void Menu_RenderView_LinearizationEdit(void)
{
    Menu_RenderView_Linearization();
    Render_MenuButtons();
}

void Menu_RenderView_InputRange(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������� �����");

    BYTE str[32];

    str[0] = 0;
    mstr_add_int(str, data_eeprom.ain_ma_t_min / 100);
    mstr_add(str, ".");
    mstr_add_int2(str, data_eeprom.ain_ma_t_min % 100);
    mstr_add(str, " ��");
    pcf8531_str((64 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);

    mstr_cpy(str, "���");
    pcf8531_str((64 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

    str[0] = 0;
    mstr_add_int(str, data_eeprom.ain_ma_t_max / 100);
    mstr_add(str, ".");
    mstr_add_int2(str, data_eeprom.ain_ma_t_max % 100);
    mstr_add(str, " ��");
    pcf8531_str(64 + (64 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);

    mstr_cpy(str, "����");
    pcf8531_str(64 + (64 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_InputRangeMin(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������");

    if (menu_data.i <= 0)
        menu_data.i = data_eeprom.ain_ma_t_min;

    BYTE str[32];

    str[0] = 0;
    mstr_add_int(str, menu_data.i / 100);
    mstr_add(str, ".");
    mstr_add_int2(str, menu_data.i % 100);
    mstr_add(str, " ��");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_InputRangeMinEdit(void)
{
    Menu_RenderView_InputRangeMin();
    Render_MenuButtons();
}

void Menu_RenderView_InputRangeMax(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("��������");

    if (menu_data.i <= 0)
        menu_data.i = data_eeprom.ain_ma_t_max;

    BYTE str[32];

    str[0] = 0;
    mstr_add_int(str, menu_data.i / 100);
    mstr_add(str, ".");
    mstr_add_int2(str, menu_data.i % 100);
    mstr_add(str, " ��");

    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_InputRangeMaxEdit(void)
{
    Menu_RenderView_InputRangeMax();
    Render_MenuButtons();
}

void Menu_RenderView_TightClosing(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� ��������");

    BYTE str[32];
    int min = data_eeprom.hard_pos_min;
    int max = data_eeprom.hard_pos_max;

    pcf8531_pic(14, 14, picto_hard_im, 100, 8);

    if (min)
    {
        str[0] = 0;
        mstr_add_int(str, min / 10);
        mstr_add(str, ".");
        mstr_add_int(str, min % 10);
        mstr_add(str, "%");
    }
    else
    {
        mstr_cpy(str, "����.");
    }

    pcf8531_str(14, 24, str, font9);

    if (max)
    {
        str[0] = 0;
        mstr_add_int(str, max / 10);
        mstr_add(str, ".");
        mstr_add_int(str, max % 10);
        mstr_add(str, "%");
    }
    else
    {
        mstr_cpy(str, "����.");
    }

    pcf8531_str(113 - pcf8531_str_len(str, font9), 24, str, font9);

    pcf8531_fill(14, 14, min / 10, 7);
    pcf8531_fill(113 - max / 10, 14, max / 10, 7);
}

void Menu_RenderView_TightClosingMin(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������");

    if (menu_data.i <= 0)
        menu_data.i = data_eeprom.hard_pos_min;

    BYTE str[32];
    int offset = data_eeprom.hard_pos_max;

    pcf8531_pic(27, 14, picto_hard_im, 100, 8);

    str[0] = 0;
    mstr_add_int(str, menu_data.i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, menu_data.i % 10);
    pcf8531_str(0, 14, str, font9);

    pcf8531_fill(27, 14, menu_data.i / 10, 7);
    pcf8531_fill(127 - offset / 10, 14, offset / 10, 7);
}

void Menu_RenderView_TightClosingMinEdit(void)
{
    Menu_RenderView_TightClosingMin();
    Render_MenuButtons();
}

void Menu_RenderView_TightClosingMax(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("��������");

    if (menu_data.i <= 0)
        menu_data.i = data_eeprom.hard_pos_max;

    BYTE str[32];
    int offset = data_eeprom.hard_pos_min;

    pcf8531_pic(0, 14, picto_hard_im, 100, 8);

    str[0] = 0;
    mstr_add_int(str, menu_data.i / 10);
    mstr_add(str, ".");
    mstr_add_int(str, menu_data.i % 10);
    pcf8531_str(127 - pcf8531_str_len(str, font9), 14, str, font9);

    pcf8531_fill(0, 14, offset / 10, 7);
    pcf8531_fill(99 - menu_data.i / 10, 14, menu_data.i / 10, 7);
}

void Menu_RenderView_TightClosingMaxEdit(void)
{
    Menu_RenderView_TightClosingMax();
    Render_MenuButtons();
}

void Menu_RenderView_TransferCharacteristic(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������������ ���");

    BYTE *ph_item[11] =
        {
            [0] = "��������� +50%",
            [1] = "��������� +40%",
            [2] = "��������� +30%",
            [3] = "��������� +20%",
            [4] = "��������� +10%",
            [5] = "��������",
            [6] = "����������� -10%",
            [7] = "����������� -20%",
            [8] = "����������� -30%",
            [9] = "����������� -40%",
            [10] = "����������� -50%",
        };

    pcf8531_pic(2, 12, picto_ph[menu_data.f.im_ph], 27, 19);
    pcf8531_clear(30, 12, 128 - 30, 9);
    pcf8531_str(30 + (128 - 30 - pcf8531_str_len(ph_item[menu_data.f.im_ph], font9)) / 2, 12, ph_item[menu_data.f.im_ph], font9);
}

void Menu_RenderView_TransferCharacteristicEdit(void)
{
    Menu_RenderView_TransferCharacteristic();
    pcf8531_pic(43, 32 - 7, picto_ph_edit, 71, 7);
}

void Menu_RenderView_StrokeSpeed(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������� ����");

    BYTE str[32];
    int coordX = 44;

    mstr_cpy(str, "�������");
    pcf8531_str((coordX - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
    FromFloat(str, roundf(T_LP_M * 10.0f / (data_eeprom.t_lp_up * FADC)), 1);
    mstr_add(str, "c");
    pcf8531_str((coordX - pcf8531_str_len(str, font9)) / 2, 24, str, font9);

    pcf8531_fill(coordX, 12, 1, 20);

    coordX += 3;
    mstr_cpy(str, "�����");
    pcf8531_str(coordX + (35 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
    FromFloat(str, roundf(T_LP_M * 10.0f / (data_eeprom.t_lp_down * FADC)), 1);
    mstr_add(str, "c");
    pcf8531_str(coordX + (35 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);

    coordX = 82;
    pcf8531_fill(coordX, 12, 1, 20);

    coordX += 3;
    mstr_cpy(str, "�������.");
    pcf8531_str(coordX, 14, str, font9);
    str[0] = 0;
    mstr_add_int(str, data_eeprom.coil_pwm_limit);
    mstr_add(str, "%");
    pcf8531_str((128 + coordX - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
}

void Menu_RenderView_StrokeSpeedPump(void)
{
    if (menu_switch == -1)
        menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_up * FADC));

    pcf8531_FullClear();
    Render_MenuTitle("�������� �������");
    RenderTimeInSecondValue(menu_data.i, 14);
}

void Menu_RenderView_StrokeSpeedPumpEdit(void)
{
    Menu_RenderView_StrokeSpeedPump();
    Render_MenuButtons();
}

void Menu_RenderView_StrokeSpeedBleed(void)
{
    if (menu_switch == -1)
        menu_data.i = roundf(T_LP_M * 2.0f / (data_eeprom.t_lp_down * FADC));

    pcf8531_FullClear();
    Render_MenuTitle("�������� ������");
    RenderTimeInSecondValue(menu_data.i, 14);
}

void Menu_RenderView_StrokeSpeedBleedEdit(void)
{
    Menu_RenderView_StrokeSpeedBleed();
    Render_MenuButtons();
}

void Menu_RenderView_StrokeSpeedLimit(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������� �������");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.coil_pwm_limit;

    BYTE str[32];
    str[0] = 0;

    mstr_add_int(str, menu_data.i);
    mstr_add(str, "%");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
}

void Menu_RenderView_StrokeSpeedLimitEdit(void)
{
    Menu_RenderView_StrokeSpeedLimit();
    Render_MenuButtons();
}

void Menu_RenderView_PID(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("��� ���������");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "��");
    pcf8531_str(0 + (42 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
    FromFloat(str, roundf(data_eeprom.kp * 1000.0f), 3);
    pcf8531_str(0 + (42 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);

    mstr_cpy(str, "��");
    pcf8531_str(43 + (42 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
    FromFloat(str, roundf(data_eeprom.ki * 1000.0f), 3);
    pcf8531_str(43 + (42 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);

    mstr_cpy(str, "��");
    pcf8531_str(86 + (42 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
    FromFloat(str, roundf(data_eeprom.kd * 1000.0f), 3);
    pcf8531_str(86 + (42 - pcf8531_str_len(str, font9)) / 2, 24, str, font9);
}

void Menu_RenderView_PidKp(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("��");

    if (menu_switch == -1)
        menu_data.i = roundf(data_eeprom.kp * 1000.0f);

    BYTE str[32];
    str[0] = 0;

    FromFloat(str, menu_data.i, 3);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PidKpEdit(void)
{
    Menu_RenderView_PidKp();
    Render_MenuButtons();
}

void Menu_RenderView_PidKd(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("��");

    if (menu_switch == -1)
        menu_data.i = roundf(data_eeprom.kd * 1000.0f);

    BYTE str[32];
    str[0] = 0;

    FromFloat(str, menu_data.i, 3);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PidKdEdit(void)
{
    Menu_RenderView_PidKd();
    Render_MenuButtons();
}

void Menu_RenderView_PidKi(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("��");

    if (menu_switch == -1)
        menu_data.i = roundf(data_eeprom.ki * 1000.0f);

    BYTE str[32];
    str[0] = 0;

    FromFloat(str, menu_data.i, 3);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PidKiEdit(void)
{
    Menu_RenderView_PidKi();
    Render_MenuButtons();
}

void Menu_RenderView_ExpertSetup(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "��������!!! ������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);

    mstr_cpy(str, "��� ������. ���������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);
}

void Menu_RenderView_ExpertCalib(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "����������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
}

void Menu_RenderView_PositionSensor(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������ ���������");

    BYTE str[32];
    str[0] = 0;

    int center = 64;
    int firstLine = 12;
    int secondLine = 23;

    switch (data_eeprom.sensor_type)
    {
    case ADC_SENSOR:
        mstr_cpy(str, "���");
        break;

    case SPI_SENSOR:
        mstr_cpy(str, "SPI");
        break;
    }

    pcf8531_str(0 + (center - pcf8531_str_len(str, font9)) / 2, firstLine, str, font9);
    mstr_copy_int(str, pos_value);
    pcf8531_str(0 + (center - pcf8531_str_len(str, font9)) / 2, secondLine, str, font9);

    mstr_cpy(str, "���.:");
    pcf8531_str(center + 33 - pcf8531_str_len(str, font9), firstLine, str, font9);
    mstr_copy_int(str, data_eeprom.hmc_adc_0);
    pcf8531_str(center + 58 - pcf8531_str_len(str, font9), firstLine, str, font9);

    mstr_cpy(str, "����.:");
    pcf8531_str(center + 33 - pcf8531_str_len(str, font9), secondLine, str, font9);
    mstr_copy_int(str, data_eeprom.hmc_adc_100);
    pcf8531_str(center + 58 - pcf8531_str_len(str, font9), secondLine, str, font9);
}

void Menu_RenderView_PositionSensorType(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("��� �������");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.sensor_type;

    BYTE str[32];
    str[0] = 0;

    switch (menu_data.i)
    {
    case ADC_SENSOR:
        mstr_cpy(str, "���");
        break;

    case SPI_SENSOR:
        mstr_cpy(str, "SPI");
        break;
    }

    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PositionSensorTypeEdit(void)
{
    Menu_RenderView_PositionSensorType();
    Render_MenuButtons();
}

void Menu_RenderView_PositionSensorMin(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������");

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, data_eeprom.hmc_adc_0);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PositionSensorMin_Auto(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������������");

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, pos_value);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PositionSensorMin_AutoEdit(void)
{
    Menu_RenderView_PositionSensorMin_Auto();
    Render_MenuButtons();
}

void Menu_RenderView_PositionSensorMin_Manual(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.hmc_adc_0;

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, menu_data.i);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PositionSensorMin_ManualEdit(void)
{
    Menu_RenderView_PositionSensorMin_Manual();
    Render_MenuButtons();
}

void Menu_RenderView_PositionSensorMax(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("��������");

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, data_eeprom.hmc_adc_100);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PositionSensorMax_Auto(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������������");

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, pos_value);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PositionSensorMax_AutoEdit(void)
{
    Menu_RenderView_PositionSensorMax_Auto();
    Render_MenuButtons();
}

void Menu_RenderView_PositionSensorMax_Manual(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.hmc_adc_100;

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, menu_data.i);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_PositionSensorMax_ManualEdit(void)
{
    Menu_RenderView_PositionSensorMax_Manual();
    Render_MenuButtons();
}

void Menu_RenderView_CalibCurrentInput(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� ����");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "4 ��");
    pcf8531_str((64 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
    mstr_copy_int(str, data_eeprom.ain_adc_min);
    pcf8531_str((64 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);

    mstr_cpy(str, "20 ��");
    pcf8531_str(64 + (64 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
    mstr_copy_int(str, data_eeprom.ain_adc_max);
    pcf8531_str(64 + (64 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);
}

void Menu_RenderView_CurrentInput4mA(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("4��");

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, data_eeprom.ain_adc_min);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CurrentInput4mA_Auto(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������");

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, ain_adc_s);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CurrentInput4mA_AutoEdit(void)
{
    Menu_RenderView_CurrentInput4mA_Auto();
    Render_MenuButtons();
}

void Menu_RenderView_CurrentInput4mA_Manual(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.ain_adc_min;

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, menu_data.i);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CurrentInput4mA_ManualEdit(void)
{
    Menu_RenderView_CurrentInput4mA_Manual();
    Render_MenuButtons();
}

void Menu_RenderView_CurrentInput20mA(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("20��");

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, data_eeprom.ain_adc_max);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CurrentInput20mA_Auto(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������");

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, ain_adc_s);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CurrentInput20mA_AutoEdit(void)
{
    Menu_RenderView_CurrentInput20mA_Auto();
    Render_MenuButtons();
}

void Menu_RenderView_CurrentInput20mA_Manual(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.ain_adc_max;

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, menu_data.i);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CurrentInput20mA_ManualEdit(void)
{
    Menu_RenderView_CurrentInput20mA_Manual();
    Render_MenuButtons();
}

void Menu_RenderView_CalibCurrentOutput(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� �����");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "4 ��");
    pcf8531_str((64 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
    mstr_copy_int(str, data_eeprom.aout_pwm_min);
    pcf8531_str((64 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);

    mstr_cpy(str, "20 ��");
    pcf8531_str(64 + (64 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
    mstr_copy_int(str, data_eeprom.aout_pwm_max);
    pcf8531_str(64 + (64 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);
}

void Menu_RenderView_CurrentOutput4mA(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("4��");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.aout_pwm_min;

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, menu_data.i);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CurrentOutput4mAEdit(void)
{
    Menu_RenderView_CurrentOutput4mA();
    Render_MenuButtons();
}

void Menu_RenderView_CurrentOutput20mA(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("20��");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.aout_pwm_max;

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, menu_data.i);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_CurrentOutput20mAEdit(void)
{
    Menu_RenderView_CurrentOutput20mA();
    Render_MenuButtons();
}

void Menu_RenderView_ExpertTest(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "����");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
}

void Menu_RenderView_TestAO(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������� �����");
    RenderTestStartView();
}

void Menu_RenderView_TestAOEdit(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������� �����");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.aout_pwm_min;

    BYTE str[32];
    str[0] = 0;

    FromFloat(str, (16.0f / (data_eeprom.aout_pwm_max - data_eeprom.aout_pwm_min) * menu_data.i) * 1000.0f, 3);
    mstr_add(str, " ��");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);

    Render_MenuExitButton();
}

void Menu_RenderView_TestDO1(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������� ����� 1");
    RenderTestStartView();
}

void Menu_RenderView_TestDO1Edit(void)
{
    RenderTestDO(1);
}

void Menu_RenderView_TestDO2(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������� ����� 2");
    RenderTestStartView();
}

void Menu_RenderView_TestDO2Edit(void)
{
    RenderTestDO(2);
}

void Menu_RenderView_TestDO3(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������� ����� 3");
    RenderTestStartView();
}

void Menu_RenderView_TestDO3Edit(void)
{
    RenderTestDO(3);
}

void Menu_RenderView_ExpertLpf(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "���");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
}

void Menu_RenderView_LpfInput(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.lpf_a_in;

    RenderLpf();
}

void Menu_RenderView_LpfInputEdit(void)
{
    Menu_RenderView_LpfInput();
    Render_MenuButtons();
}

void Menu_RenderView_LpfOutput(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�����");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.lpf_a_out;

    RenderLpf();
}

void Menu_RenderView_LpfOutputEdit(void)
{
    Menu_RenderView_LpfOutput();
    Render_MenuButtons();
}

void Menu_RenderView_ExpertLinearization(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "������������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 14, str, font9);
}

void Menu_RenderView_LinearizationCoef(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�����������");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.linear_coef;

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, menu_data.i);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
}

void Menu_RenderView_LinearizationCoefEdit(void)
{
    Menu_RenderView_LinearizationCoef();
    Render_MenuButtons();
}

void Menu_RenderView_LinearizationZeroShift(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����� ����");

    if (menu_switch == -1)
        menu_data.i = data_eeprom.linear_shift;

    BYTE str[32];
    str[0] = 0;

    mstr_copy_int(str, menu_data.i);
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
}

void Menu_RenderView_LinearizationZeroShiftEdit(void)
{
    Menu_RenderView_LinearizationZeroShift();
    Render_MenuButtons();
}

void Menu_RenderView_ExpertDisplay(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.display_format;
    }

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "������ �������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);

    switch (menu_data.i)
    {

    case FROM_D:
        mstr_cpy(str, "��� AM800.D");
        break;

    case FROM_W:
        mstr_cpy(str, "��� AM800.W");
        break;
    }

    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);
}

void Menu_RenderView_ExpertDisplayEdit(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������ �������");

    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.display_format;
    }

    BYTE str[32];
    str[0] = 0;

    switch (menu_data.i)
    {

    case FROM_D:
        mstr_cpy(str, "��� AM800.D");
        break;

    case FROM_W:
        mstr_cpy(str, "��� AM800.W");
        break;
    }

    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
    Render_MenuButtons();
}

void Menu_RenderView_ExpertTempVersion(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.temp_version;
    }

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "����.����������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);

    switch (menu_data.i)
    {

    case N:
        mstr_cpy(str, "-20..+60�C");
        break;

    case C:
        mstr_cpy(str, "-40..+75�C");
        break;

    case E:
        mstr_cpy(str, "-60..+60�C");
        break;
    }
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);
}

void Menu_RenderView_ExpertTempVersionEdit(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����.����������");

    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = data_eeprom.temp_version;
    }

    BYTE str[32];
    str[0] = 0;

    switch (menu_data.i)
    {

    case N:
        mstr_cpy(str, "-20..+60�C");
        break;

    case C:
        mstr_cpy(str, "-40..+75�C");
        break;

    case E:
        mstr_cpy(str, "-60..+60�C");
        break;
    }

    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
    Render_MenuButtons();
}

void Menu_RenderView_ExpertAccumZone(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = roundf(data_eeprom.si_off * 10.0f);
    }

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "���� ���������� �");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);

    FromFloat(str, menu_data.i, 1);
    mstr_add(str, "%");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);
}

void Menu_RenderView_ExpertAccumZoneEdit(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���� ���������� �");

    if (menu_switch == -1)
    {
        menu_switch = 1;
        menu_data.i = roundf(data_eeprom.si_off * 10.0f);
    }

    BYTE str[32];
    str[0] = 0;

    FromFloat(str, menu_data.i, 1);
    mstr_add(str, "%");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
    Render_MenuButtons();
}

void Menu_RenderView_ExpertFactoryReset(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "������ �����");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);

    mstr_cpy(str, "��������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);
}

void Menu_RenderView_ExpertFactoryResetEdit(void)
{
    pcf8531_FullClear();
    RenderQuestion();
}

void Menu_RenderView_Reset(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("C����");

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "����� ��������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);

    mstr_cpy(str, "������������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 22, str, font9);
}

void Menu_RenderView_ResetEdit(void)
{
    pcf8531_FullClear();
    RenderQuestion();
}

void Menu_RenderView_PcSmoothing(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������� ��");
}

void Title_RenderView_LogItems(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������ �������");
}

void Menu_RenderView_ViewLog(void)
{
    Title_RenderView_LogItems();

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "��������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
}
/// @brief ������ �������� ������
const BYTE *menu_event_message[EVENT_NUM] =
	{
		[EVENT_FREE] = "�����",
		[EVENT_SW_FAIL] = "���� ��",
		[EVENT_AIN_MIN] = "���� ���",
		[EVENT_AIN_MAX] = "���� ����",
		[EVENT_POWER_FAIL] = "�������",
		[EVENT_ANGLE_MAX] = "���� ����",
		[EVENT_ANGLE_MIN] = "���� ���",
		[EVENT_TA_MIN] = "���� -40",
		[EVENT_TA_MAX] = "���� +85",
		[EVENT_AOUT_FAIL] = "������. �����",
		[EVENT_DOUT1_FAIL] = "����. �����1",
		[EVENT_DOUT2_FAIL] = "����. �����2",
		[EVENT_DOUT3_FAIL] = "����. �����3",
		[EVENT_IM_FAIL] = "�� ����",
		[EVENT_IM_FAIL_ONCE] = "�� ���� �",
		[EVENT_HMC_MAX] = "�� ����",
		[EVENT_HMC_MIN] = "�� ���",
		[EVENT_WDT_RESET] = "�������",
		[EVENT_TAMPER] = "������",
		[EVENT_BP_FAIL] = "���� �������",
		[EVENT_FULL_RST] = "�����",
		[EVENT_PST_FAIL] = "������ PST",
		[EVENT_HIF_FAIL] = "������� ������",
};

void Menu_RenderView_ViewLogEdit(void)
{
    pcf8531_FullClear();
    
    unsigned int i;
    int res;

    DWORD t1, t2;
    BYTE str[32];
    str[0] = 0;

    if (event_num)
	{
		str[0] = 0;
		mstr_add(str, "������ ");
		mstr_add_int(str, menu_data.i + 1);
		mstr_add(str, "/");
		mstr_add_int(str, event_num);
        t1 = data_eeprom.events[i_old].time;
        t2 = data_eeprom.events[menu_data.i].time;
        res = i_old;
        res -= menu_data.i;
        con_str("EV: ");
        con_dword(t1);
        con_str(" - ");
        con_dword(t2);

		if (((res > 0 || res < -1) && t1 < t2) || ((res < 0 || res > 1) && t2 < t1))
        {
            con_str(" *");
            mstr_add(str, " (�)");
        }
        con_str("\n\r");
        con_start();
		i = pcf8531_str_len(str, font9);
		pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
		pcf8531_str((128 - i) / 2, 2, str, font9);

		str[0] = 0;
		if (data_eeprom.events[menu_data.i].event & (1 << EVENT_FLAG))
			mstr_add(str, "�");
		else
			mstr_add(str, "�");
		mstr_add_int(str, data_eeprom.events[menu_data.i].event & (~(1 << EVENT_FLAG)));
		mstr_add(str, ": ");
		mstr_add(str, menu_event_message[data_eeprom.events[menu_data.i].event & (~(1 << EVENT_FLAG))]);
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);

		str[0] = 0;
		mstr_add(str, "�: ");
		i = t2;
		if (i < 60)
		{
			mstr_add_int(str, i);
			mstr_add(str, "�");
		}
		else if (i < (60 * 60))
		{
			mstr_add_int(str, i / 60);
			mstr_add(str, "�");
			mstr_add_int2(str, i % 60);
			mstr_add(str, "�");
		}
		else if (i < (24 * 60 * 60))
		{
			mstr_add_int(str, i / (60 * 60));
			mstr_add(str, "�");
			mstr_add_int2(str, (i / 60) % 60);
			mstr_add(str, "�");
			mstr_add_int2(str, i % 60);
			mstr_add(str, "�");
		}
		else
		{
			mstr_add_int(str, i / (24 * 60 * 60));
			mstr_add(str, "�");
			mstr_add_int2(str, (i / (60 * 60)) % 24);
			mstr_add(str, "�");
			mstr_add_int2(str, (i / 60) % 60);
			mstr_add(str, "�");
			mstr_add_int2(str, i % 60);
			mstr_add(str, "�");
		}
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 24, str, font9);
	}
	else
	{
		str[0] = 0;
		mstr_add(str, "������ 0 / 0");
		i = pcf8531_str_len(str, font9);
		pcf8531_clear((128 - i) / 2 - 2, 1, i + 4, 10);
		pcf8531_str((128 - i) / 2, 2, str, font9);

		str[0] = 0;
		mstr_add(str, "��� �������");
		i = pcf8531_str_len(str, font9);
		pcf8531_str((128 - i) / 2, 14, str, font9);
	}
}


void Menu_RenderView_ClearingLog(void)
{
    Title_RenderView_LogItems();

    BYTE str[32];
    str[0] = 0;

    mstr_cpy(str, "��������");
    pcf8531_str((128 - pcf8531_str_len(str, font9)) / 2, 12, str, font9);
}

void Menu_RenderView_ClearingLogEdit(void)
{
    Title_RenderView_LogItems();

    RenderQuestion();
}

void Menu_RenderView_AO(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������. �����");
}

void Menu_RenderView_DO1(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����. ����� 1");
}

void Menu_RenderView_DO2(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����. ����� 2");
}

void Menu_RenderView_DO3(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����. ����� 3");
}

void Menu_RenderView_PST(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("PST-����");
}

void Menu_RenderView_BreakawayFriction(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������ �����������");
}

void Menu_RenderView_Histograms(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�����������");
}

void Menu_RenderView_HighFriction(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� ������");
}

void Menu_RenderView_HartIdentifier(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������������");
}

void Menu_RenderView_HartPriority(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������");
}

void Menu_RenderView_HartShutdown(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");
}

void Menu_RenderView_UnitsIM(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� ��");
}

void Menu_RenderView_UnitsInput(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� �����");
}

void Menu_RenderView_Contrast(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������������");
}

void Menu_RenderView_ScreenRotation(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� ������");
}

void Menu_RenderView_AoSignal(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������");
}

void Menu_RenderView_AoMin(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������� ���");
}

void Menu_RenderView_AoMax(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������������ ���");
}

void Menu_RenderView_AoLower(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������ ��������");
}

void Menu_RenderView_AoUpper(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� ��������");
}

void Menu_RenderView_Do1Signal(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������");
}

void Menu_RenderView_Do2Signal(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������");
}

void Menu_RenderView_Do2Hysteresis(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");
}

void Menu_RenderView_Do2Direction(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�����������");
}

void Menu_RenderView_Do2Position(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������");
}

void Menu_RenderView_Do3Signal(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������");
}

void Menu_RenderView_Do3Hysteresis(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������");
}

void Menu_RenderView_Do3Direction(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�����������");
}

void Menu_RenderView_Do3Position(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������");
}

void Menu_RenderView_PsStroke(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������� ����");
}

void Menu_RenderView_PstRun(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������ �����");
}

void Menu_RenderView_PstAutorun(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����������, ���");
}

void Menu_RenderView_PstErrorDelay(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������� ������, �");
}

void Menu_RenderView_PstTime(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("����� �����, �");
}

void Menu_RenderView_BreakawayFrictionRange(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�������� �����");
}

void Menu_RenderView_BreakawayFrictionCount(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������� ��������");
}

void Menu_RenderView_BreakawayFrictionRun(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������");
}

void Menu_RenderView_BreakawayFrictionData(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("������������ ������");
}

void Menu_RenderView_HighFrictionError(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("���������������, %");
}

void Menu_RenderView_HighFrictionTime(void)
{
    pcf8531_FullClear();
    Render_MenuTitle("�����, �");
}