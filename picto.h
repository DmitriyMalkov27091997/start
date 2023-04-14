/**
 * @file picto.h
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Заголовочный файл пиктограмм.
 * @version 0.1
 * @date 20.05.2016
 * 
 * @copyright (C) Ярославль, 2016
 * 
 */
#ifndef __PICTO_H__
#define __PICTO_H__

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include "type.h"

/******************************************************************************
 * DEFINE
 *****************************************************************************/

/******************************************************************************
 * FUNCTION
 *****************************************************************************/

/******************************************************************************
 * VARIABLE
 *****************************************************************************/
extern const BYTE picto_warn[2 * 9];
extern const BYTE picto_start[4 * 128];
extern const BYTE picto_normal[4 * 128];
extern const BYTE picto_pointer[1 * 4];
extern const BYTE picto_pointer_t[1 * 2];
extern const BYTE picto_line[1 * 128];
extern const BYTE picto_digit_small[11][1 * 5];
extern const BYTE picto_digit_small_dot[1];
extern const BYTE picto_dot_small[1 * 1];
extern const BYTE picto_minus_small[1 * 3];
extern const BYTE picto_ma_small[1 * 13];
extern const BYTE picto_ma_small_place[1 * 35];
extern const BYTE picto_proc_small_place[1 * 35];
extern const BYTE picto_digit_large[10][3 * 14];
extern const BYTE picto_pos_large[3 * 63];
extern const BYTE picto_auto[2 * 39];
extern const BYTE picto_hand[2 * 39];
extern const BYTE picto_hart[2 * 39];
extern const BYTE picto_flag[2 * 9];
extern const BYTE picto_hart_flag[2 * 9];
extern const BYTE picto_limits_flag[2 * 9];
extern const BYTE picto_press_flag[2 * 9];
extern const BYTE picto_status_ok[2 * 9];
extern const BYTE picto_status_err[2 * 9];
extern const BYTE picto_status_err_last[2 * 9];
extern const BYTE picto_info_1[4 * 128];
extern const BYTE picto_info_2[4 * 128];
extern const BYTE picto_namur_invert[2 * 7];
extern const BYTE picto_menu[4 * 128];
extern const BYTE picto_hand_pos[1 * 39];
extern const BYTE picto_submenu[4 * 128];
extern const BYTE picto_submenu_edit[4 * 128];
extern const BYTE picto_buttons[1 * 128];
extern const BYTE picto_main_grad[3 * 9];
extern const BYTE picto_dot_large[1 * 5];
extern const BYTE picto_ph[11][3 * 27];
extern const BYTE picto_ph_edit[1 * 71];
extern const BYTE picto_hard_im[1 * 100];
extern const BYTE picto_hard_min[1 * 3];
extern const BYTE picto_hard_max[1 * 3];
extern const BYTE picto_calib_pwm[1 * 128];
extern const BYTE picto_gist[4 * 128];
extern const BYTE picto_gist_tmp[4 * 128];
extern const BYTE picto_gist_col[3 * 8];
extern const BYTE picto_gist_col_max[1 * 3];
extern const BYTE picto_gist_col_max6[1 * 6];
extern const BYTE picto_gist_col_pos[1 * 1];

#endif // __PICTO_H__
/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/