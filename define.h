/**
 * \file	define.h
 * \brief 	������������ ���� �������� �������.
 * @brief ���� �������� �������.
 * @version 0.1
 * @date 20.05.2016
 *
 * @copyright (C) ���������, 2016
 *
 */
#ifndef __DEFINE_H__
#define __DEFINE_H__

/******************************************************************************
 *	README
 ******************************************************************************
ain_adc_min=55
ain_ma_min=400
ain_adc_max=1832
ain_ma_max=1836
	#hmc_adc_min=397
hmc_adc_0=397
	#hmc_pos_min=0
	#hmc_adc_max=1448
hmc_adc_100=1448
	#hmc_pos_max=1000
ain_ma_t_min=400
hmc_pos_t_min=0
ain_ma_t_max=2000
hmc_pos_t_max=1000
kp=25
ki=0.15
kd=7
lpf_pos=1
aout_pwm_min=166
aout_pwm_max=820
pid_int_min=-3500
pid_int_max=3500
******************************************************************************/

/******************************************************************************
 *	INCLUDE
 *****************************************************************************/
#include "stm32l1xx.h"
#include "serial_port.h"
#include "type.h"

/******************************************************************************
 *	Positioner model
 *****************************************************************************/
// #define AM800W

/******************************************************************************
 *	DEFINE
 *****************************************************************************/
#define NO_AMP
#define ADC_OVER
#define ADC_HMC_N (8)
#define ADC_HMC_X (4)

//-----------------------------------------------------------------------------
//	Clock & Time
//-----------------------------------------------------------------------------
#define FPLL (24000000UL)			//!< ������� ���� ����� PLL, ��
#define FMSI (2097000UL)			//!< ������� MSI, ��
#define FHSI (16000000UL)			//!< ������� ����������� RC-����������, ��
#define FLSI (40000UL)				//!< ������� ����������� �� RC-����������, ��
#define FHSE (12000000UL)			//!< ������� �������� ����������, ��
#define FLSE (32768UL)				//!< ������� �������� �������� ����������, ��
#define FADC (32UL)					//!< ������� ������� ���, ��
#define FSYSTICK (10UL)				//!< ������� ���������� �������, ��
#define TO_SEC (FSYSTICK)			//!< ������� �������
#define TO_TICK (1000UL / FSYSTICK) //!< ��������� ���, ��
#define TO_PLL_ON (1000UL)			//!< ������� ��������� PLL, ��
#define TO_HSE_ON (10000UL)			//!< ������� ��������� HSE, ��
#define TO_WORKLED (1000UL)			//!< ������ ���������� "������", ��
#define TO_DISP_INIT (200UL)		//!< ������� ������������� �������, ��
#define TO_POWER_FAIL (500UL)		//!< ������� ����� ���� ����� �������, ��
#define TO_PST_EVENT (FADC * 1)		//!< ��������� ������ PST, ���
#define HMC_PATH_DELTA (50UL)		//!< ����������� �������� ��� ����� ���� �������, 0.1%.���
#define PRESENT_IWDG				//!< ������� ����������� �������
#define FAIL_DEBUG					//!< ������� ����������� �������
#define FPWM_AOUT (1000)			//!< ������� ��� ��� ����������� ������, ��
#define MAX_PWM_AOUT (1000)			//!< ������� ��� ��� ����������� ������
#define FPWM_COIL (100)				//!< ������� ��� ��� ��������������, ��
#define MAX_PWM_COIL (1000)			//!< ������� ��� ��� ��������������
#define PWM_COIL TIM3->CCR1			//!< ��� ��������������
#define PWM_AOUT TIM9->CCR1			//!< ��� ����������� ������
#define AIN_MA_POWER_FAIL (380)		//!< ������� ������� "���������� ������� (��� ���� ������������)"
#define AIN_MA_MIN (390)			//!< ������� ������� "��� ������ ���������� �����������"
#define AIN_MA_MAX (2080)			//!< ������� ������� "��� ������ ����������� �����������"
#define TEMP_C_MIN (-40)			//!< ������� ������� "����������� ���� -40�"
#define TEMP_C_MAX (85)				//!< ������� ������� "����������� ���� +85�"
#define LIMIT_DELTA (5)
#define TIME_MIN (FADC * 0.5)
#define TIME_MAX (FADC * 35.0)
#define KP_MIN (35.0)			 //!< ����������� �������� �-������������ ��� �������������
#define KP_MAX (200.0)			 //!< ������������ �������� �-������������ ��� �������������
#define LP_TIME_ADD (FADC * 0.8) //!< �������������� ����� � ����������� �� ����� ������������� ��� ������������

//-----------------------------------------------------------------------------
// ����� ��������
//-----------------------------------------------------------------------------
// #define HART_DEBUG					//!< ������� HART
// #define AIN_AVG							//!< ���������� ������ ���� �� �����

extern DWORD _app_start;					   //!< ����� ������ ��������
extern DWORD _app_size;						   //!< ������ ��������
#define SW_CRC_START ((DWORD)&_app_start)	   //!< ����� ������ ��������
#define SW_CRC_BLOCK_SIZE (256)				   //!< ������ ����� ��� ���������� CRC
#define SW_CRC_BLOCK_END (sizeof(sIMAGE_INFO)) //!< ������ ����� � ����������������� �������
#define SW_CRC_APP_SIZE ((DWORD)&_app_size)	   //!< ������ ��������

//-----------------------------------------------------------------------------
// Connections
//-----------------------------------------------------------------------------
// #define	WORKLED_PIN					(GPIOC_PORT + 2)			//!< PC2  - ��������� "������"
#define HART_RTS_PIN (GPIOB_PORT + 14) //!< PB14 - HART_RTS

#define NAMUR1_PIN (GPIOC_PORT + 12) //!< PC12 - NAMUR1
#define NAMUR2_PIN (GPIOC_PORT + 7)	 //!< PC7  - NAMUR2
#define NAMUR3_PIN (GPIOC_PORT + 10) //!< PC10 - NAMUR3

#define NAMUR1D_PIN (GPIOD_PORT + 2)  //!< PD2  - NAMUR1_DIAG
#define NAMUR2D_PIN (GPIOC_PORT + 8)  //!< PC8  - NAMUR2_DIAG
#define NAMUR3D_PIN (GPIOC_PORT + 11) //!< PC11 - NAMUR3_DIAG

#define FAIL_PIN (GPIOA_PORT + 4)		//!< PA4  - FAIL
#define AOUT_DIAG_PIN (GPIOB_PORT + 15) //!< PB15 - AOUT_DIAG

#ifdef NO_AMP
#define TEST_PIN (GPIOA_PORT + 2) //!< PA2 - TEST_PIN
#endif

#ifdef AM800W
#define AOUT_SW_PIN (GPIOC_PORT + 0)   //!< PC0 - AOUT detection switch pin
#define NAMUR_SW1_PIN (GPIOC_PORT + 1) //!< PC1 - Namur detection switch pin 1
#define NAMUR_SW2_PIN (GPIOC_PORT + 2) //!< PC2 - Namur detection switch pin 2
#define HART_SW_PIN (GPIOC_PORT + 3)   //!< PC3 - HART modem detection switch pin
#endif

#define CON_RX (10) //!< PA10 - ���� RX UART1

#define DISP_SDA (7)	//!< PB7  - DISP I2C_SDA
#define DISP_SCL (6)	//!< PB6  - DISP I2C_SCL
#define DISP_RST (8)	//!< PB8  - DISP RESET
#define DISP_PORT GPIOB //!< GPIOB - ���� �������

#define DISP_RST_0                          \
	do                                      \
	{                                       \
		DISP_PORT->BSRRH = (1 << DISP_RST); \
	} while (0) // < Disp.rst = 0
#define DISP_RST_1                          \
	do                                      \
	{                                       \
		DISP_PORT->BSRRL = (1 << DISP_RST); \
	} while (0) // < Disp.rst = 1
#define DISP_SDA_0                          \
	do                                      \
	{                                       \
		DISP_PORT->BSRRH = (1 << DISP_SDA); \
	} while (0) // < Disp.sda = 0
#define DISP_SDA_1                          \
	do                                      \
	{                                       \
		DISP_PORT->BSRRL = (1 << DISP_SDA); \
	} while (0) //!< Disp.sda = 1
#define DISP_SCL_0                          \
	do                                      \
	{                                       \
		DISP_PORT->BSRRH = (1 << DISP_SCL); \
	} while (0) //!< Disp.scl = 0
#define DISP_SCL_1                          \
	do                                      \
	{                                       \
		DISP_PORT->BSRRL = (1 << DISP_SCL); \
	} while (0)										  //!< Disp.scl = 1
#define DISP_SDA_d ((DISP_PORT->IDR >> DISP_SDA) & 1) //!< ������� Disp.sda

//-----------------------------------------------------------------------------
//	Console
//-----------------------------------------------------------------------------
#define con_push(x) sp_tx_push(&con, (x))	 //!< �������� ������ � ����� ����������� �������
#define con_tx_pop(x) sp_tx_pop(&con)		 //!< ��������� ������ �� ������ �������� �������
#define con_str(x) sp_str(&con, (x))		 //!< �������� ������ � ����� ����������� �������
#define con_start() sp_start(&con)			 //!< �������� ��������
#define con_rx_push(x) sp_rx_push(&con, (x)) //!< �������� ������ � ����� ��������� �������
#define con_rx_pop() sp_rx_pop(&con)		 //!< ��������� ������ �� ������ ��������� �������
#define con_byte(x) sp_byte(&con, (x))		 //!< �������� ���� � ����� ����������� �������
#define con_word(x) sp_word(&con, (x))		 //!< �������� ����� � ����� ����������� �������
#define con_dword(x) sp_dword(&con, (x))	 //!< �������� ������� ����� � ����� ����������� �������
#define con_dec(x) sp_dec(&con, (x))		 //!< �������� ���������� ����� �� ������ � ����� ����������� �������
#define con_udec(x) sp_udec(&con, (x))		 //!< �������� ���������� ����� ��� ����� � ����� ����������� �������
#define con_float(x) sp_float(&con, (x))	 //!< �������� ����� � ��������� ������ � ����� ����������� �������

#define CONSOLE_BAUD (115200)				   //!< �������� �������, ���
#define CONSOLE_TX_SIZE (1024)				   //!< ������ ������ ��������, ����
#define CONSOLE_RX_SIZE (128)				   //!< ������ ������ ������, ����
#define CONSOLE_COMMAND_SIZE (CONSOLE_RX_SIZE) //!< ������ ������ �������
#define COM_END (0x0D)						   //!< ������ ����� �������
#define COM_BEGIN ('>')						   //!< ������ ������ �������
#define COM_BS (0x08)						   //!< ������ �������� �������
#define COM_ESC (0x1B)						   //!< ������ �������� �������
#define TO_COM_CLEAR (10000)				   //!< ������� ����� �������, ��

#define HART_BAUD (1200) //!< �������� HART, ���

#define EVENT_LOG_NUM (128) //!< ������������ ����� ������� � �������
#define EVENT_FLAG (7)		//!< ����� ���� �������� ��������� �������

//-----------------------------------------------------------------------------
//!	\enum eEVENT
//! \brief �������� ����� � ��������
//-----------------------------------------------------------------------------
enum eEVENT
{
	EVENT_FREE = 0,		//!< 00 - �����
	EVENT_SW_FAIL,		//!< 01 - ������� "�������� ����������"
	EVENT_AIN_MIN,		//!< 02 - ������� "��� ������ ���������� �����������" \ref AIN_MA_MIN
	EVENT_AIN_MAX,		//!< 03 - ������� "��� ������ ����������� �����������" \ref AIN_MA_MAX
	EVENT_POWER_FAIL,	//!< 04 - ������� "���������� ������� (��� ���� ������������)" \ref AIN_MA_POWER_FAIL
	EVENT_ANGLE_MAX,	//!< 05 - ������� "���� �������� ������ 25�"
	EVENT_ANGLE_MIN,	//!< 06 - ������� "���� �������� ������ -25�"
	EVENT_TA_MIN,		//!< 07 - ������� "����������� ���� -40�" \ref TEMP_C_MIN
	EVENT_TA_MAX,		//!< 08 - ������� "����������� ���� +85�" \ref TEMP_C_MAX
	EVENT_AOUT_FAIL,	//!< 09 - ������� "����� ���� ����������� ������"
	EVENT_DOUT1_FAIL,	//!< 10 - ������� "����� ���� ���������� ����������� ������"
	EVENT_DOUT2_FAIL,	//!< 11 - ������� "����� ���� ����������� ������ 2"
	EVENT_DOUT3_FAIL,	//!< 12 - ������� "����� ���� ����������� ������ 3"
	EVENT_IM_FAIL,		//!< 13 - ������� "���������� ������� �� �� ����������� ����������� (� ��������� ������)"
	EVENT_IM_FAIL_ONCE, //!< 14 - ������� "���������� ������� �� �� ����������� ����������� (� �������) � ���� ���"
	EVENT_HMC_MAX,		//!< 15 - ������� "���� �������� ������ ���������������� ��������"
	EVENT_HMC_MIN,		//!< 16 - ������� "���� �������� ������ ���������������� ��������"
	EVENT_WDT_RESET,	//!< 17 - ������� "����� ���������� �� WDT � ���� ���"
	EVENT_TAMPER,		//!< 18 - ������� "������� ������"
	EVENT_BP_FAIL,		//!< 19 - ������� "���� ����� �������"
	EVENT_FULL_RST,		//!< 20 - ������� "�����"
	EVENT_PST_FAIL,		//!< 21 - ������� "������ PST"
	EVENT_HIF_FAIL,		//!< 22 - ������� "������� ������"

	EVENT_NUM //!< ����� ����� �������
};

#define EVENT_MASK_ONCE (0 | (1 << EVENT_PST_FAIL))

//-----------------------------------------------------------------------------
//!	\typedef struct sEVENT
//! \brief ������� �������
//!	\struct sEVENT
//! \brief ������� �������
//-----------------------------------------------------------------------------
typedef struct sEVENT
{
	DWORD time;	 //!< ���� "�����" � ���
	DWORD event; //!< ���� "�������" \sa eEVENT
} sEVENT;

//-----------------------------------------------------------------------------
//!	\typedef struct sEEPROM_FLAGS
//! \brief ����� � ����������������� ������
//!	\struct sEEPROM_FLAGS
//! \brief ����� � ����������������� ������
//-----------------------------------------------------------------------------
typedef struct sEEPROM_FLAGS
{
	union
	{
		DWORD dw; //!< ������ ����������� 32 ����
		int i;	  //!< ������ �������� 32 ����
		struct
		{
			DWORD display_im : 1;		//!< ������� �� \sa eDISPLAY_IM
			DWORD display_ain : 1;		//!< ������� ����� \sa eDISPLAY_AIN
			DWORD display_contrast : 5; //!< ������������� ������
			DWORD display_rotate : 1;	//!< ������� ������ \sa eDISPLAY_ROTATE

			DWORD hart_id : 4;		//!< ����� HART
			DWORD hart_enable : 1;	//!< ���������� HART \sa eHART_ENABLE
			DWORD hart_prio : 1;	//!< ��������� ����� \sa eHART_PRIO
			DWORD hart_none : 1;	//!< ��������� ���
			DWORD coil_inverse : 1; //!< �������� ����� \sa eCOIL_INVERSE

			DWORD aout_sig : 2;	 //!< �������� ��� ����������� ������ \sa eOUT_SIG
			DWORD dout2_sig : 2; //!< �������� ��� ����������� ������ 2 \sa eOUT_SIG
			DWORD dout2_dir : 1; //!< ����������� ����������� ������ 2 \sa eDOUT_DIR
			DWORD dout3_sig : 2; //!< �������� ��� ����������� ������ 3\sa eOUT_SIG
			DWORD dout3_dir : 1; //!< ����������� ����������� ������ 3 \sa eDOUT_DIR

			DWORD display_filter : 3; //!< �������� ��� ��� ��������� �� ������� \sa menu_disp_filter_item
			DWORD im_ph : 4;		  //!< ���������� �������������� \sa menu_im_ph_item

			DWORD dout1_disable : 1; //!< ���������� ����������� ������ 1 \sa eDOUT1_ENABLE
		};
	};
} sEEPROM_FLAGS;

//-----------------------------------------------------------------------------
//!	\enum eDISPLAY_IM
//! \brief ����������� ��������� �� �������
//-----------------------------------------------------------------------------
enum eDISPLAY_IM
{
	DISPLAY_IM_PROC = 0, //!< ��������� "��������"
	DISPLAY_IM_GRAD,	 //!< ��������� "�������"
};

//-----------------------------------------------------------------------------
//!	\enum eDISPLAY_AIN
//! \brief ����������� �������� ���� �� �������
//-----------------------------------------------------------------------------
enum eDISPLAY_AIN
{
	DISPLAY_AIN_MA = 0, //!< ������� ��� "��"
	DISPLAY_AIN_PROC,	//!< ������� ��� "��������"
};

//-----------------------------------------------------------------------------
//!	\enum eDISPLAY_ROTATE
//! \brief ������� ������
//-----------------------------------------------------------------------------
enum eDISPLAY_ROTATE
{
	DISPLAY_ROTATE_OFF = 0, //!< ����� "������"
	DISPLAY_ROTATE_ON,		//!< ����� "�������"
};

//-----------------------------------------------------------------------------
//!	\enum eHART_ENABLE
//! \brief ���������� HART
//-----------------------------------------------------------------------------
enum eHART_ENABLE
{
	HART_ENABLE_ON = 0, //!< HART-����� �������
	HART_ENABLE_OFF,	//!< HART-����� ��������
};

//-----------------------------------------------------------------------------
//!	\enum eHART_PRIO
//! \brief ��������� �����
//-----------------------------------------------------------------------------
enum eHART_PRIO
{
	HART_PRIO_AIN = 0, //!< ��������� ����������� �����
	HART_PRIO_HART,	   //!< ��������� HART-������
};

//-----------------------------------------------------------------------------
//!	\enum eCOIL_INVERSE
//! \brief �������� �����
//-----------------------------------------------------------------------------
enum eCOIL_INVERSE
{
	COIL_INVERSE_NONE = 0, //!< ������ �����
	COIL_INVERSE_INV,	   //!< �������� �����
};

//-----------------------------------------------------------------------------
//!	\enum eAOUT_SIG
//! \brief �������� �������
//-----------------------------------------------------------------------------
enum eOUT_SIG
{
	OUT_SIG_NONE = 0, //!< �������� "���"
	OUT_SIG_POS,	  //!< �������� "���������"
	OUT_SIG_AIN,	  //!< �������� "����"
	OUT_SIG_DELTA,	  //!< �������� "���������������"
};

//-----------------------------------------------------------------------------
//!	\enum eDOUT_DIR
//! \brief ����������� ����������� ������
//-----------------------------------------------------------------------------
enum eDOUT_DIR
{
	DOUT_DIR_UP = 0, //!< ����������� "����������"
	DOUT_DIR_DOWN,	 //!< ����������� "����������"
};

//-----------------------------------------------------------------------------
//!	\enum eDOUT1_ENABLE
//! \brief ���������� ����������� ������ ������
//-----------------------------------------------------------------------------
enum eDOUT1_ENABLE
{
	DOUT1_ENABLE = 0, //!< ����� ������ ��������
	DOUT1_DISABLE,	  //!< ����� ������ ��������
};

/// @brief ������������ ��������� �������� ����������� ������ �� �������.
typedef enum eDISPLAY_FORMAT
{
	/// @brief ����������� ��� ���������� AM800.D.
	FROM_D = 0,

	/// @brief ����������� ��� ���������� AM800.W.
	FROM_W
} eDISPLAY_FORMAT;

/// @brief ������������ ��������� ������������� ����������.
typedef enum eTEMP_VERSION
{
	/// @brief -20..+60.
	N = 0,

	/// @brief -40..+75.
	C,

	/// @brief -60..+60.
	E
} eTEMP_VERSION;

/// @brief ������������ ��������� ���/����.
typedef enum eON_OFF
{
	/// @brief ���������.
	OFF = 0,

	/// @brief ��������.
	ON
} eON_OFF;

/// @brief ������������ ����� ������.
enum eSENSOR_TYPES
{
	/// @brief ���.
	ADC_SENSOR = 0,

	/// @brief SPI.
	SPI_SENSOR
};

/// @brief ������������ ������� ���.
enum eLPF_MODE
{
	/// @brief ��������.
	LPF_OFF = 0,

	/// @brief ����������� ����������.
	LPF_MIN,

	/// @brief ������� ����������.
	LPF_MID,

	/// @brief ������������ ����������.
	LPF_MAX
};

//-----------------------------------------------------------------------------
//!	\typedef struct sDATA_EEPROM
//! \brief ����������������� ������ ���������� � ������� �������
//!	\struct sDATA_EEPROM
//! \brief ����������������� ������ ���������� � ������� �������
//! \note ����������� ����:
//! \note - \b "��" - ��������� ����������
//! \note - \b "��" - ���������������� ��������
//! \note - \b "��" - ������� ����������
//! \note - \b "��" - �� ������������
//-----------------------------------------------------------------------------
typedef struct sDATA_EEPROM
{
	volatile int ain_adc_min; //!< \b ��. �������� ��� ��� ������ ����� � ��. ���
	volatile int ain_ma_min;  //!< \b ��. �������� ���� � ����� ��� ������ ����� � 0.01 ��
	volatile int ain_adc_max; //!< \b ��. �������� ��� ��� ������� ����� � ��. ���
	volatile int ain_ma_max;  //!< \b ��. �������� ���� � ����� ��� ������� ����� � 0.01 ��

	volatile int linear_en;	   //!< \b ��. ��������� ������������ ��������� [0 ��� 1]
	volatile int linear_coef;  //!< \b ��. ����������� ������������.
	volatile int linear_shift; //!< \b ��. ����� ���� ������������.

	volatile int display_format; //!< \b ��. ������ �������: 0 - D; 1 - W
	volatile int sensor_type;	 //!< \b ��. ��� �������: 0 - ���; 1 - SPI
	volatile int temp_version; //!< \b ��. ������������� ����������: 0 - -20..+60; 1 - -40..+75; 2 - -60..+60;

	volatile int ain_ma_t_min;	//!< \b ��. �������� ���� � ����� ��� ������ ����� � 0.01 ��
	volatile int hmc_pos_t_min; //!< \b ��. ������� ��������� ��� ������ ����� � 0.1%.��� [0%.���]
	volatile int ain_ma_t_max;	//!< \b ��. �������� ���� � ����� ��� ������� ����� � 0.01 ��
	volatile int hmc_pos_t_max; //!< \b ��. ������� ��������� ��� ������� ����� � 0.1%.��� [100%.���]

	volatile float kp; //!< \b ��. ����������� ��� ��
	volatile float ki; //!< \b ��. ����������� ��� ��
	volatile float kd; //!< \b ��. ����������� ��� ��

	volatile float pid_int_min; //!< \b ��. ������ ����� �-������������
	volatile float pid_int_max; //!< \b ��. ������� ����� �-������������

	volatile int lpf_max_samples; //!< \b ��. ������������ ���������� �������� ��� ���.
	volatile float lpf_pos;		  //!< \b ��. �������� ��� ��� ������� ��������� [0..1]
	volatile int lpf_a_in;		  //!< \b ��. ������� ���������� ��� ��� ain_adc [0-����; 1-���; 2-����; 3-����]
	volatile int lpf_a_out;		  //!< \b ��. ������� ���������� ��� ��� aout_adc [0-����; 1-���; 2-����; 3-����]

	volatile sEEPROM_FLAGS flags; //!< \b ����� \sa sEEPROM_FLAGS

	volatile int aout_pos_min; //!< \b ��. ������ ����� ����������� ������ � 0.1%���
	volatile int aout_pos_max; //!< \b ��. ������� ����� ����������� ������ � 0.1%���
	volatile int aout_ma_min;  //!< \b ��. ������ ����� ����������� ������ � 0.1��
	volatile int aout_ma_max;  //!< \b ��. ������� ����� ����������� ������ � 0.1��

	volatile int dout2_pos; //!< \b ��. ����� NAMUR2 ��������� � 0.1%���
	volatile int dout2_gis; //!< \b ��. ����� NAMUR2 ���������� � 0.1%���
	volatile int dout3_pos; //!< \b ��. ����� NAMUR3 ��������� � 0.1%���
	volatile int dout3_gis; //!< \b ��. ����� NAMUR3 ���������� � 0.1%���

	volatile int aout_pwm_min; //!< \b ��. �������� ��� ��� 4.00 ��
	volatile int aout_pwm_max; //!< \b ��. �������� ��� ��� 20.00 ��

	volatile int hmc_path_nv[3]; //!< \b ��. ���������� "��" � 0.1%.���
	volatile int worktime_nv[3]; //!< \b ��. ���������� "��" � ���

	volatile int hard_pos_min; //!< \b ��. ������� �������� � ������� �������� � 0.1%.��� �� ������
	volatile int hard_pos_max; //!< \b ��. ������� �������� � ������� ��������� � 0.1%.��� � �����

	volatile int hmc_adc_0;		 //!< \b ��. �������� hmc_adc ��� ����� 0%.���
	volatile int hmc_adc_100;	 //!< \b ��. �������� hmc_adc ��� ����� 100%.���
	volatile int hmc_lim_0;		 //!< \b ��. �������� hmc_pos_s ��� ����� 0%.��� � 0.1%.���
	volatile int hmc_lim_100;	 //!< \b ��. �������� hmc_pos_s ��� ����� 100%.��� � 0.1%.���
	volatile int hmc_g_0;		 //!< \b ��. �������� ���� ��� hmc_adc_0 � 0.1 �������
	volatile int hmc_g_100;		 //!< \b ��. �������� ���� ��� hmc_adc_100 � 0.1 �������
	volatile int coil_pwm_limit; //!< \b ��. ����������� ��������� ��� coil_pwm � 1% �������������
	// 48

	volatile float t_lp_up;	  //!< \b ��. �������� ���������� ����� [0..1]
	volatile float t_lp_down; //!< \b ��. �������� ���������� ���� [0..1]
	// 50

	volatile int pst_dx;	  //!< \b ��. PST. ������� ����, %��� [4..15%]
	volatile int pst_t;		  //!< \b ��. PST. ����� �����, � [2..60�]
	volatile int pst_te;	  //!< \b ��. PST. �������� ������, � [2..60�]
	volatile int pst_autorun; //!< \b ��. PST. ����������, � [0..4096�]
	// 54

	volatile int hif_dx; //!< \b ��. ������� ������. ���������������, %��� [3..25%]
	volatile int hif_t;	 //!< \b ��. ������� ������. �����, � [3..60]
	// 56

	volatile float si_off; //!< \b ��. ������� ���������� �-������������, %��� [0..100%]
	// 57

	volatile int slow_LT_edge;	//!< \b ��. ������� ���������� ���� ������� ��-�� �����������
	volatile float slow_LT_val; //!< \b ��. ����������� ���������� ��������
	volatile int midpoint;		//!< \b ��. ������� ����� ��� ��������� �����������
	// 60

	int none[64 - 61]; //!< \b ��. �� ������������

	volatile sEVENT events[EVENT_LOG_NUM]; //!< \b ��. ������ ������� \sa sEVENT

} sDATA_EEPROM;

#define TEMPERATURE_BAD (0x55AA) //!< ���������� ����� ��� ������������ �����������

//-----------------------------------------------------------------------------
//!	\enum eBUTTON
//! \brief ����� ������
//-----------------------------------------------------------------------------
enum eBUTTON
{
	BUTTON_UP = 1,	 //!< ������ "�����"
	BUTTON_OK = 2,	 //!< ������ "��"
	BUTTON_DOWN = 4, //!< ������ "����"
};

/// @brief ��������� ���� ��� ���-����������.
enum ePID_SOURCE
{
	/// @brief  �� ��������� �������.
	PID_SOURCE_SIGNAL = 0,

	/// @brief �� ������� ��������
	PID_SOURCE_HAND,
};

//-----------------------------------------------------------------------------
//!	\enum ePUPD
//! \brief �������� ������
//-----------------------------------------------------------------------------
enum ePUPD
{
	PUPD_OFF = 0, //!< �������� ���������
	PUPD_UP,	  //!< �������� �����
	PUPD_DOWN,	  //!< �������� ����
};

// #define ADC_HMC	(1)
// #define ADC_AIN	(1)
// #define ADC_BTN	(1)
// #define ADC_TMP	(1)

#ifdef NO_AMP
#define ADC_HMC (1)
#else
#define ADC_HMC (3)
#endif // NO_AMP
#define ADC_AIN (0)
#define ADC_BTN (5)
#define ADC_TMP (16)

#define STAT_POS_SIZE (128)
#define STAT_POS_DEF (0xFFFF)

#define GIST_POS_SIZE (21)
#define GIST_TGT_SIZE (21)
#define GIST_TMP_SIZE (14)

#define T_LP_M (4.3f)

//-----------------------------------------------------------------------------
//	enum eST_AUTO
//-----------------------------------------------------------------------------
enum eST_AUTO
{
	ST_AUTO_IDLE = 0,
	ST_AUTO_SELECT,
	ST_AUTO_START,
	ST_AUTO_DOWN_LIMIT,
	ST_AUTO_UP_LIMIT,
	ST_AUTO_LIMIT_DONE,
	ST_AUTO_DOWN_TIME,
	ST_AUTO_UP_TIME,
	ST_AUTO_TIME_DONE,
};

/// @brief ��� �����
enum eTYPE_SCALE
{
	/// @brief ������
	TYPE_DIRECT = 0,

	/// @brief ��������
	TYPE_REVERSE,
};

//-----------------------------------------------------------------------------
//	enum eHART_ACT_DATA_TYPE
//-----------------------------------------------------------------------------
enum eHART_ACT_DATA_TYPE
{
	HART_ACT_DATA_POS = 0,
	HART_ACT_DATA_TGT,
	HART_ACT_DATA_POS_TGT,
};

#define LOF_SIZE (960)
#define LOF_GIST_SIZE (32)

/******************************************************************************
 *	FUNCTION
 *****************************************************************************/
//-----------------------------------------------------------------------------
//! \fn extern void calib_reset(void)
//! \brief ������� "����� �������"
//-----------------------------------------------------------------------------
extern void calib_reset(void);

//-----------------------------------------------------------------------------
//! \fn extern void im_reset(void)
//! \brief ������� "������ �����"
//-----------------------------------------------------------------------------
extern void im_reset(void);

//-----------------------------------------------------------------------------
//! \fn extern void factory_reset(void)
//! \brief ������� "����� � ��������� ����������"
//-----------------------------------------------------------------------------
extern void factory_reset(void);

//-----------------------------------------------------------------------------
//! \fn extern void log_reset(void)
//! @brief ������� ������� ������� 
//-----------------------------------------------------------------------------
extern void log_reset(void);

//-----------------------------------------------------------------------------
//! \fn extern void hart_tx_enable(void)
//! \brief �������� ���������� HART
//-----------------------------------------------------------------------------
extern void hart_tx_enable(void);

//-----------------------------------------------------------------------------
//! \fn extern void tick(tTIMER *t)
//! \brief ������� ���������� �������
//! \param t ��������� �� ���������� �������
//! \sa tTIMER
//-----------------------------------------------------------------------------
extern void tick(tTIMER *t);

//-----------------------------------------------------------------------------
//! \fn extern void print_fault(sFAULT *fault)
//! \brief ������ ���������� ���������� ��� HardFault
//! \param fault ��������� �� ���� ��������� \ref sFAULT
//-----------------------------------------------------------------------------
extern void print_fault(sFAULT *fault);

//-----------------------------------------------------------------------------
//! \fn extern void set_pin(const WORD pin, const BYTE value)
//! \brief ���������� �����
//! \param pin ����� ������ \ref eGPIO_PORT
//! \param value �������� ������
//-----------------------------------------------------------------------------
extern void set_pin(const WORD pin, const BYTE value);

//-----------------------------------------------------------------------------
//! \fn extern int get_pin(const WORD pin)
//! \brief �������� �������� ������
//! \param pin ����� ������ \ref eGPIO_PORT
//! \return �������� ������
//-----------------------------------------------------------------------------
extern int get_pin(const WORD pin);

//-----------------------------------------------------------------------------
//! \fn extern void set_pupd(const WORD pin, const BYTE value)
//! \brief ���������� ��������
//! \param pin ����� ������ \ref eGPIO_PORT
//! \param value �������� �������� \ref ePUPD
//-----------------------------------------------------------------------------
extern void set_pupd(const WORD pin, const BYTE value);

//-----------------------------------------------------------------------------
//! \fn extern void eeprom_unlock(void)
//! \brief �������������� EEPROM
//-----------------------------------------------------------------------------
extern void eeprom_unlock(void);

//-----------------------------------------------------------------------------
//! \fn extern void eeprom_lock(void)
//! \brief ������������� EEPROM
//-----------------------------------------------------------------------------
extern void eeprom_lock(void);

//-----------------------------------------------------------------------------
//! \fn extern int load_maj(volatile int x[3], volatile int *v)
//! \brief ��������� ������������ ����������
//! \param x ������������ ����������
//! \param v ��������� �� ���������
//! \return ������ ����������:
//! - 0 - ������ ����������
//! - 1 - �������� ����������
//-----------------------------------------------------------------------------
extern int load_maj(volatile int x[3], volatile int *v);

//-----------------------------------------------------------------------------
//! \fn extern int save_maj(volatile int x[3], volatile int v)
//! \brief ��������� ������������ ����������
//! \param x ������������ ����������
//! \param v �������� ���������
//! \return ������ ����������:
//! - 0 - ������ ����������
//! - 1 - �������� ����������
//-----------------------------------------------------------------------------
extern int save_maj(volatile int x[3], volatile int v);

//-----------------------------------------------------------------------------
//! \fn extern void load_events(void)
//! \brief ��������� ������ �������
//! \sa event_pos, event_num
//-----------------------------------------------------------------------------
extern void load_events(void);

//-----------------------------------------------------------------------------
//! \fn extern void event_save(const DWORD event)
//! \brief �������� ������� � ������
//! \param event �������
//! \sa eEVENT, EVENT_FLAG
//-----------------------------------------------------------------------------
extern void event_save(const DWORD event);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_cpy(char *dst, const char *src)
//! \brief �������� ������
//! \param dst ������ ����������
//! \param src ������ ���������
//-----------------------------------------------------------------------------
extern void mstr_cpy(char *dst, const char *src);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_add(char *dst, const char *src)
//! \brief �������� ������
//! \param dst ������ ����������
//! \param src ������ ���������
//-----------------------------------------------------------------------------
extern void mstr_add(char *dst, const char *src);

/// @brief �������� ����� �����
//! \param dst ������ ����������
//! \param x ����� �����
extern void mstr_copy_int(char *dst, int x);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_add_int(char *dst, int x)
//! \brief �������� ����� �����
//! \param dst ������ ����������
//! \param x ����� �����
//-----------------------------------------------------------------------------
extern void mstr_add_int(char *dst, int x);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_add_int2(char *dst, int x);
//! \brief �������� ����� ���������� �����
//! \param dst ������ ����������
//! \param x ����� �����
//-----------------------------------------------------------------------------
extern void mstr_add_int2(char *dst, int x);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_add_dword(char *dst, DWORD x)
//! \brief �������� 32-������ ����������� ����� � ����������������� ����
//! \param dst ������ ����������
//! \param x ����� �����
//-----------------------------------------------------------------------------
extern void mstr_add_dword(char *dst, DWORD x);

/// @brief �������� �������� ��� ����������� ������.
/// @param value ������������ ��������.
extern void Change_PWM_Aout(int value);

/// @brief �������� �������� ���.
/// @param filtrationDegree ������� ����������.
extern float Get_LpfValue(int filtrationDegree);

/******************************************************************************
 *	VARIABLE
 *****************************************************************************/
extern volatile tTIMER t_workled; //!< ������� ���������� "������".
// extern volatile int		blink_var;						//!< ���������� �������
extern const BYTE s_who[];							//!< �������� ����������� � ��������
extern BYTE console_tx_buf[CONSOLE_TX_SIZE];		//!< ����� �������� UART1.
extern BYTE console_rx_buf[CONSOLE_RX_SIZE];		//!< ����� ������ UART1.
extern sSERIAL_PORT con;							//!< ���������������� ���� UART1 - �������.
extern BYTE console_command[CONSOLE_COMMAND_SIZE];	//!< ����� �������
extern BYTE console_exec_buf[CONSOLE_COMMAND_SIZE]; //!< ����� ����������
extern volatile int console_command_pos;			//!< ��������� � ������ �������

extern volatile int disp_flag;		   //!< ���� ���������� ������ (������ #TO_WORKLED / 2)
extern volatile int disp_init_flag;	   //!< ���� ������������� ������
extern tTIMER to_disp_init;			   //!< ������ �������� ������������� ������
extern volatile int disp_update_count; //!< ������� ���������� ������
extern volatile int display_contrast;  //!< ������������� �������

extern volatile DWORD uptime;	  //!< ����� ������ ����������� ����� ������������ � ���
extern volatile int worktime;	  //!< ����� ������ ������� � ���
extern volatile int get_adc_flag; //!< ���� ������ �������� ���
extern volatile int get_pos_flag; //!< ���� ������ �������� ���������� � ����

extern volatile int st_auto;
extern volatile int auto_type;
extern volatile int hart_autoset_flag;		//!< ���� ������� ������������� ����� HART
extern volatile int hart_autoset_done_flag; //!< ���� ��������� ������������� ����� HART
extern tTIMER to_auto;

extern WORD stat_pos_arr[STAT_POS_SIZE];
extern volatile int stat_pos;
extern volatile int stat_pos_arr_valid;
extern volatile int stat_pos_min;
extern volatile int stat_pos_max;
extern volatile int stat_pos_delta;
extern volatile int stat_pos_avg;
extern volatile int stat_pos_cur;
extern volatile int stat_pos_adc_min;
extern volatile int stat_pos_adc_max;
extern volatile int stat_time_down;
extern volatile int stat_time_up;
extern volatile float stat_kp;
extern volatile float stat_ki;
extern volatile float stat_coil_pwm;

#ifdef AIN_AVG
extern volatile int ain_avg_min;
extern volatile int ain_avg_max;
extern volatile int ain_avg_avg;
extern volatile int ain_avg_done;
extern volatile int ain_avg_tmin;
extern volatile int ain_avg_tmax;
extern volatile int ain_avg_sum;
extern volatile int ain_avg_cnt;
extern tTIMER to_ain_avg;
#endif

extern volatile int btn_adc;		//!< �������� ��� ��� ������ � ��. ���
extern tTIMER to_button_pressed;	//!< ������ ��������� ������
extern volatile int button_pressed; //!< ������� ������
extern volatile int tamper;			//!< �������� ������ (0 - �������, 1 - ��������)

extern volatile int tempeature;		//!< �������� ����������� � �������� �������
extern volatile int tempeature_adc; //!< �������� ��� ��� ����������� � ��. ���

extern volatile int ain_adc_s;	//!< ����� ������ � ��� ����� [0-4095]
extern volatile float fain_adc; //!< ������������� ������ � ��� ����� [0-4095]
extern volatile int ain_ma_s;	//!< �������� ��� ������� � 0.01 ��
extern volatile int ain_ma;		//!< ������� ��� ������� � 0.01 ��
extern volatile int ain_pos_t;	//!< ������� �������� ��� ��� � 0.1%.���

extern volatile int hart_ma;	   //!< ������� ���� �� HART � 0.01 ��
extern volatile int aout_ma;	   //!< ��� ����������� ������ � 0.1 ��
extern volatile int aout_diag;	   //!< ����������� ����������� ������ (0 - ����, ����� - ����)
extern volatile int namur_out[3];  //!< �������� ����������� ������
extern volatile int namur_diag[3]; //!< ����������� ����������� ������ (0 - ����, ����� - ����)

extern volatile int test_pwm;	//!< ���������� ��������������� � �������
extern volatile int test_a_out; //!< ���������� ���������� ������� � �������
extern volatile int test_namur; //!< ���������� ����������� �������� � ������� (0 - ���������, ����� - ���������)

extern volatile int pos_value; //!< ����� ������ � ��� ������� ��������� � ��. ��� [0-4095]

extern volatile int ihmc_adc_s;	  //!< ����� ������ � ��� ������� ��������� � ��. ��� [0-4095]
extern volatile float fhmc_adc;	  //!< ������������� ������ � ��� ������� ��������� � ��. ��� [0-4095] \sa sDATA_EEPROM �������� lpf_pos
extern volatile int ihmc_adc;	  //!< ������������� ������ � ��� ������� ��������� � ��. ��� [0-4095] \sa sDATA_EEPROM �������� lpf_pos
extern volatile float fhmc_pos_a; //!< ���������� ��������� � 1%.��� [0-100]
extern volatile int ihmc_pos_a;	  //!< ���������� ��������� � 0.1%.��� [0-1000]
extern volatile float fhmc_pos;	  //!< ������� ��������� � 1%.��� [0-100]
extern volatile int ihmc_pos;	  //!< ������� ��������� � 0.1%.��� [0-1000]
extern volatile int ihmc_adc_d;	  //!< ������������� ������ � ��� ������� ��������� ��� ������� � ��. ��� [0-4095] \sa sEEPROM_FLAGS �������� display_filter
extern volatile int ihmc_pos_da;  //!< ���������� ��������� � 0.1%.��� ��� ������� [0-1000]
extern volatile int ihmc_pos_d;	  //!< ������� ��������� � 0.1%.��� ��� ������� [0-1000]
extern volatile int ihmc_g;		  //!< ���������� ��������� � 0.1 �������
extern volatile int ihmc_path;	  //!< ������� ������ ������� � 0.1%.���
extern volatile int ihmc_pos_t;	  //!< ������� �������� ��� ��� � 0.1%.���

extern volatile int linear_temp_disable; // ��������� �������� ����� ������������.

extern volatile int signal_delta; //!< ������� ��������������� � 0.1%.���
extern volatile float pid_int;	  //!< ������� �������� �-������������
extern volatile int coil_pwm;	  //!< ���������� ������ �� �������������
extern volatile int coil_pwm_def;
extern volatile int pid_debug; //!< ������� ��� ����������
extern volatile int pid_int_clear;
extern volatile int pid_source; //!< �������� �������� ���������

extern tTIMER to_power_fail; //!< ������ ���� ����� �������

extern const float hmc_disp_fs[6]; //!< ������������ ��� ��� �������
extern const float im_ph_k[11];	   //!< ������������ ���������� ��������������
extern volatile int show_ph;	   //!< ������� ���������� ��������������

extern volatile float info_hmc_d;	  //!< "���������-���". ���������������
extern volatile float info_hmc_d_pr;  //!< "���������-���". �-������������
extern volatile float info_hmc_d_int; //!< "���������-���". �-������������
extern volatile float info_hmc_d_dif; //!< "���������-���". �-������������
extern volatile float info_coil;	  //!< "���������-���". ����������� ������ �� �������������
extern volatile int info_coil_ofs;	  //!< "���������-���". ����������� ������ �� ������������� (��������)

extern volatile float lof_arr[2][LOF_SIZE];
extern volatile int lof_gist[2][LOF_GIST_SIZE];
extern volatile int lof_pos;

extern volatile int event_pos;			  //!< ������� ��������� � ������� �������
extern volatile int event_num;			  //!< ������� ����� ������� ������� �������
extern volatile DWORD event_flag_current; //!< ������� ������ �������
extern volatile DWORD event_flag;		  //!< ��������� ������ �������
extern volatile DWORD event_flag_last;	  //!< ������� ������ �������

extern volatile int flag_calib_reset;	//!< ���� "����� �������" \sa calib_reset
extern volatile int flag_im_reset;		//!< ���� "������ �����" \sa im_reset
extern volatile int flag_factory_reset; //!< ���� "��������� �����" \sa factory_reset

extern sDATA_EEPROM data_eeprom __attribute__((section(".eeprom")));	//!< ����������������� ������
extern const sIMAGE_INFO image __attribute__((section(".image_info"))); //!< ��������� ������ \sa sIMAGE_INFO

#ifdef ADC_OVER
extern volatile WORD adc_buf[16];
#endif // ADC_OVER

extern volatile DWORD gist_pos[GIST_POS_SIZE];
extern volatile DWORD gist_tgt[GIST_TGT_SIZE];
extern volatile DWORD gist_tmp[GIST_TMP_SIZE];

extern volatile int step_test_n;
extern volatile int step_test_c;
extern volatile int step_test_t;
extern volatile int step_test_to;
extern volatile int step_test_min;
extern volatile int step_test_max;

extern volatile int pst_to;
extern volatile int pst_t;
extern volatile int pst_te;
extern volatile int pst_last;
extern volatile int pst_cur;
extern volatile int pst_result;
extern volatile int pst_event_fail;
extern volatile int pst_auto_to;

extern volatile int hif_to;

extern volatile int lof_to;
extern volatile int lof_amplitude;
extern volatile int lof_period;
extern volatile int lof_time;
extern volatile int lof_position;

extern volatile int hart_act_count;
extern volatile int hart_act_data_pos;
extern volatile int hart_act_data_page_send;
extern volatile WORD hart_act_data[64];
extern volatile int hart_act_data_type;

extern volatile int get_display;

#endif // __DEFINE_H__
/******************************************************************************
 * ����� �����
 *****************************************************************************/
