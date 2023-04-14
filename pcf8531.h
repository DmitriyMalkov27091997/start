/**
 * @file pcf8531.h
 * @author ����� �.�. (dnega@mail.ru)
 * @brief ������������ ���� �������� ������� PCF8531.
 * @version 0.1
 * @date 20.05.2016
 *
 * @copyright (C) ���������, 2016
 *
 */
#ifndef __PCF8531_H__
#define __PCF8531_H__

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include "define.h"

/******************************************************************************
 * DEFINE
 *****************************************************************************/
// #define PCF8531_DEBUG

#define PCF8531_SIZEX (128)										 //!< ������ ������, �����
#define PCF8531_SIZEY (5)										 //!< ������ ������, ����
#define PCF8531_PAGE_NUM (8)									 //!< ����� �������
#define PCF8531_PAGE_SIZE ((PCF8531_SIZEX) / (PCF8531_PAGE_NUM)) //!< ������ ��������

#define PCF8531_ADDR (0x78)	   //!< ����� ����������� �������
#define PCF8531_CONTROL (0x80) // Co = 1, RS = 0
#define PCF8531_DATA (0x40)	   // Co = 0, RS = 1

#define PCF8531_START                \
	do                               \
	{                                \
		pcf8531_start();             \
		pcf8531_write(PCF8531_ADDR); \
	} while (0) //!< ������ ��������

#define PCF8531_WRITE_CONTROL(x)        \
	do                                  \
	{                                   \
		pcf8531_write(PCF8531_CONTROL); \
		pcf8531_write((x));             \
	} while (0) //!< �������� �������

#define PCF8531_WRITE_DATA           \
	do                               \
	{                                \
		pcf8531_write(PCF8531_DATA); \
	} while (0) //!< �������� ������

#define PCF8531_PA_SET_PAGE_0 (0x01)

#define PCF8531_P0_SET_PAGE(n) (0x08 | (n))
#define PCF8531_P0_PD_V(pd, v) (0x20 | ((pd) << 2) | ((v) << 1))
#define PCF8531_P0_SET_Y(y) (0x40 | (y))
#define PCF8531_P0_SET_X(x) (0x80 | (x))

#define PCF8531_P1_MUX(m) (0x04 | (m))
#define PCF8531_MUX_17 (0)
#define PCF8531_MUX_34 (1)
#define PCF8531_MUX_26 (2)
#define PCF8531_P1_DISP(d) (0x08 | (d))
#define PCF8531_DISP_BLANK (0)	 // D = 0, IM = 0, E = 0
#define PCF8531_DISP_ALL_SEG (1) // D = 0, IM = 0, E = 1
#define PCF8531_DISP_NORMAL (4)	 // D = 1, IM = 0, E = 0
#define PCF8531_DISP_INVERSE (5) // D = 1, IM = 0, E = 1

#define PCF8531_P1_BIAS(b) (0x10 | (b))
#define PCF8531_BIAS_11 (0)
#define PCF8531_BIAS_10 (1)
#define PCF8531_BIAS_9 (2)
#define PCF8531_BIAS_8 (3)
#define PCF8531_BIAS_7 (4)
#define PCF8531_BIAS_6 (5)
#define PCF8531_BIAS_5 (6)
#define PCF8531_BIAS_4 (7)

#define PCF8531_P2_MULT(m) (0x08 | (m))
#define PCF8531_MULT_2X (0)
#define PCF8531_MULT_3X (1)
#define PCF8531_MULT_4X (2)
#define PCF8531_MULT_5X (3)

#define PCF8531_P2_TC(t) (0x20 | (t))
#define PCF8531_P2_HV_OFF (0x04 | (0 << 1) | 0)
#define PCF8531_P2_HV_LOW (0x04 | (0 << 1) | 1)
#define PCF8531_P2_HV_HIGH (0x04 | (1 << 1) | 1)
#define PCF8531_P2_VOP(v) (0x80 | (v))

/******************************************************************************
 * FUNCTION
 *****************************************************************************/
//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_delay(void)
//! \brief ������� ��������
//-----------------------------------------------------------------------------
extern void pcf8531_delay(void);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_restart(void)
//! \brief ����� �� �������
//-----------------------------------------------------------------------------
extern void pcf8531_restart(void);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_init(void)
//! \brief ������������� �������
//-----------------------------------------------------------------------------
extern void pcf8531_init(void);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_init_after(void)
//! \brief ���������� ������������� �������
//-----------------------------------------------------------------------------
extern void pcf8531_init_after(void);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_str(const int x, const int y, char *s, const BYTE *font)
//! \brief ������� ������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
//! \param s ��������� �� ������
//! \param font ��������� �� �����
//-----------------------------------------------------------------------------
extern void pcf8531_str(const int x, const int y, char *s, const BYTE *font);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_str_inverse(const int x, const int y, char *s, const BYTE *font)
//! \brief ������� ��������������� ������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
//! \param s ��������� �� ������
//! \param font ��������� �� �����
//-----------------------------------------------------------------------------
extern void pcf8531_str_inverse(const int x, const int y, char *s, const BYTE *font);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_str_to(const int x, const int y, char *s, const int p, const BYTE *font)
//! \brief ������� ������ �� ��������� �������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
//! \param s ��������� �� ������
//! \param p �������� ��������� �� �����������
//! \param font ��������� �� �����
//-----------------------------------------------------------------------------
extern void pcf8531_str_to(const int x, const int y, char *s, const int p, const BYTE *font);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_pic(const int x, const int y, const BYTE *pic, const int sx, const int sy)
//! \brief ������� ��������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
//! \param pic ��������� �� ��������
//! \param sx ������ �� �����������
//! \param sy ������ �� ���������
//-----------------------------------------------------------------------------
extern void pcf8531_pic(const int x, const int y, const BYTE *pic, const int sx, const int sy);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_pic_invert(const int x, const int y, const BYTE *pic, const int sx, const int sy)
//! \brief ������� ��������������� ��������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
//! \param pic ��������� �� ��������
//! \param sx ������ �� �����������
//! \param sy ������ �� ���������
//-----------------------------------------------------------------------------
extern void pcf8531_pic_invert(const int x, const int y, const BYTE *pic, const int sx, const int sy);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_pica(const int x, const int y, const BYTE *pic, const int sx, const int sy)
//! \brief �������� �������� ������ ������������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
//! \param pic ��������� �� ��������
//! \param sx ������ �� �����������
//! \param sy ������ �� ���������
//-----------------------------------------------------------------------------
extern void pcf8531_pica(const int x, const int y, const BYTE *pic, const int sx, const int sy);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_clear(const int x, const int y, const int sx, const int sy)
//! \brief �������� �������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
//! \param sx ������ �� �����������
//! \param sy ������ �� ���������
//-----------------------------------------------------------------------------
extern void pcf8531_clear(const int x, const int y, const int sx, const int sy);

/// @brief �������� ����� ���������.
extern void pcf8531_FullClear();

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_fill(const int x, const int y, const int sx, const int sy)
//! \brief ������ �������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
//! \param sx ������ �� �����������
//! \param sy ������ �� ���������
//-----------------------------------------------------------------------------
extern void pcf8531_fill(const int x, const int y, const int sx, const int sy);

//-----------------------------------------------------------------------------
//! \fn extern int pcf8531_str_len(char *s, const BYTE *font)
//! \brief ��������� ������ ������
//! \param s ��������� �� ������
//! \param font ��������� �� �����
//! \return ������ ������
//-----------------------------------------------------------------------------
extern int pcf8531_str_len(char *s, const BYTE *font);

//-----------------------------------------------------------------------------
//! \fn extern void pcf8531_pixel(const int x, const int y)
//! \brief ��������� �������
//! \param x ��������� �� �����������
//! \param y ��������� �� ���������
extern void pcf8531_pixel(const int x, const int y);

/******************************************************************************
 * VARIABLE
 *****************************************************************************/
extern BYTE pcf8531_video[PCF8531_SIZEX][5]; //!< �����������

#endif // __PCF8531_H__
/******************************************************************************
 * ����� �����
 *****************************************************************************/
