/**
 * @file hart.h
 * @author ����� �.�. (dnega@mail.ru)
 * @brief ������������ ���� HART-���������.
 * @version 0.1
 * @date 20.05.2016
 * 
 * @copyright (C) ���������, 2016
 * 
 */
#ifndef __HART_H__
#define __HART_H__

/******************************************************************************
 * INCLUDE
 *****************************************************************************/
#include "define.h"
#include "type.h"

/******************************************************************************
 * DEFINE
 *****************************************************************************/
#define HART_TX_BUF				(128)						//!< ������ ������ ����������� HART
#define TO_HART						(1000UL)				//!< ������� ���������� HART, ��
#define HART_ID						(image.serial)	//!< �������� ����� ����������
#define HART_SENSOR_ID		(image.serial)	//!< �������� ����� �������
#define TO_HART_ACT				(3)							//!< ������� ��������� ������, ���

/******************************************************************************
 * FUNCTION
 *****************************************************************************/
//-----------------------------------------------------------------------------
//! \fn extern void hart_init(void)
//! \brief ������������� HART
//-----------------------------------------------------------------------------
extern void hart_init(void);

//-----------------------------------------------------------------------------
//! \fn extern void hart_on_rx(const BYTE x)
//! \brief ����� ������ ������ �� HART
//! \param x �������� ������
//-----------------------------------------------------------------------------
extern void hart_on_rx(const BYTE x);

//-----------------------------------------------------------------------------
//! \fn extern int hart_on_tx(BYTE *x)
//! \brief �������� ������ �� ������ ����������� HART
//! \param x ��������� �� ���������� ���������� �������
//! \return ������ ����������:
//! - 0 - ����� ����������� ����
//! - 1 - ������ ��������
//-----------------------------------------------------------------------------
extern int hart_on_tx(BYTE *x);

//-----------------------------------------------------------------------------
//! \fn extern void hart_start_tx(const int size)
//! \brief ������ �������� ������ HART
//! \param size ������ ������
//-----------------------------------------------------------------------------
extern void hart_start_tx(const int size);

//-----------------------------------------------------------------------------
//! \fn extern void hart_timer(void)
//! \brief ������� ������� HART
//-----------------------------------------------------------------------------
extern void hart_timer(void);

//-----------------------------------------------------------------------------
//! \fn extern void hart_str(BYTE *dst, BYTE *src, int size)
//! \brief ������������� ������ � HART-������� ����� (��������)
//! \param dst ����� ����������
//! \param src �������� ������
//! \param size ������ ������ ����������
//-----------------------------------------------------------------------------
extern void hart_str(BYTE *dst, BYTE *src, int size);

extern void hart_send_act(void);

/******************************************************************************
 * VARIABLE
 *****************************************************************************/
extern tTIMER to_hart;										//!< ������ ���������� HART
extern tTIMER to_hart_act;								//!< ������� ��������� ������, ���
extern volatile float	hart_ext_current;		//!< �������� ���� HART, ��

#endif // __HART_H__
/******************************************************************************
 * ����� �����
 *****************************************************************************/
