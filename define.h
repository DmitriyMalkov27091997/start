/**
 * \file	define.h
 * \brief 	Заголовочный файл настроек проекта.
 * @brief Файл настроек проекта.
 * @version 0.1
 * @date 20.05.2016
 *
 * @copyright (C) Ярославль, 2016
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
#define FPLL (24000000UL)			//!< Частота ядра после PLL, Гц
#define FMSI (2097000UL)			//!< Частота MSI, Гц
#define FHSI (16000000UL)			//!< Частота внутреннего RC-генератора, Гц
#define FLSI (40000UL)				//!< Частота внутреннего НЧ RC-генератора, Гц
#define FHSE (12000000UL)			//!< Частота внешнего генератора, Гц
#define FLSE (32768UL)				//!< Частота внешнего часового генератора, Гц
#define FADC (32UL)					//!< Частота выборок АЦП, Гц
#define FSYSTICK (10UL)				//!< Частота системного таймера, Гц
#define TO_SEC (FSYSTICK)			//!< Таймаут секунды
#define TO_TICK (1000UL / FSYSTICK) //!< Системный тик, мс
#define TO_PLL_ON (1000UL)			//!< Таймаут включания PLL, мс
#define TO_HSE_ON (10000UL)			//!< Таймаут включания HSE, мс
#define TO_WORKLED (1000UL)			//!< Период светодиода "РАБОТА", мс
#define TO_DISP_INIT (200UL)		//!< Таймаут инициализации дисплея, мс
#define TO_POWER_FAIL (500UL)		//!< Таймаут после сбоя блока питания, мс
#define TO_PST_EVENT (FADC * 1)		//!< Удержание ошибки PST, тик
#define HMC_PATH_DELTA (50UL)		//!< Минимальное смежение для учета хода клапана, 0.1%.раб
#define PRESENT_IWDG				//!< Наличие сторожевого таймера
#define FAIL_DEBUG					//!< Отладка сторожевого таймера
#define FPWM_AOUT (1000)			//!< Частота ШИМ для аналогового выхода, Гц
#define MAX_PWM_AOUT (1000)			//!< Уровень ШИМ для аналогового выхода
#define FPWM_COIL (100)				//!< Частота ШИМ для электромагнита, Гц
#define MAX_PWM_COIL (1000)			//!< Уровень ШИМ для электромагнита
#define PWM_COIL TIM3->CCR1			//!< ШИМ электромагнита
#define PWM_AOUT TIM9->CCR1			//!< ШИМ аналогового выхода
#define AIN_MA_POWER_FAIL (380)		//!< Уровень тревоги "Пропадание питания (ток ниже критического)"
#define AIN_MA_MIN (390)			//!< Уровень тревоги "Ток меньше минимально допустимого"
#define AIN_MA_MAX (2080)			//!< Уровень тревоги "Ток больше максимально допустимого"
#define TEMP_C_MIN (-40)			//!< Уровень тревоги "Температура ниже -40°"
#define TEMP_C_MAX (85)				//!< Уровень тревоги "Температура выше +85°"
#define LIMIT_DELTA (5)
#define TIME_MIN (FADC * 0.5)
#define TIME_MAX (FADC * 35.0)
#define KP_MIN (35.0)			 //!< Минимальное значение П-составляющей для автонастройки
#define KP_MAX (200.0)			 //!< Максимальное значение П-составляющей для автонастройки
#define LP_TIME_ADD (FADC * 0.8) //!< Дополнительное время к измеренному во время автонастройки для стабильности

//-----------------------------------------------------------------------------
// Опции прошивки
//-----------------------------------------------------------------------------
// #define HART_DEBUG					//!< Отладка HART
// #define AIN_AVG							//!< Вычисление уровня шума на входе

extern DWORD _app_start;					   //!< Адрес начала прошивки
extern DWORD _app_size;						   //!< Размер прошивки
#define SW_CRC_START ((DWORD)&_app_start)	   //!< Адрес начала прошивки
#define SW_CRC_BLOCK_SIZE (256)				   //!< Размер блока для вычисления CRC
#define SW_CRC_BLOCK_END (sizeof(sIMAGE_INFO)) //!< Размер блока с пользовательскими данными
#define SW_CRC_APP_SIZE ((DWORD)&_app_size)	   //!< Размер прошивки

//-----------------------------------------------------------------------------
// Connections
//-----------------------------------------------------------------------------
// #define	WORKLED_PIN					(GPIOC_PORT + 2)			//!< PC2  - Светодиод "РАБОТА"
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

#define CON_RX (10) //!< PA10 - Вход RX UART1

#define DISP_SDA (7)	//!< PB7  - DISP I2C_SDA
#define DISP_SCL (6)	//!< PB6  - DISP I2C_SCL
#define DISP_RST (8)	//!< PB8  - DISP RESET
#define DISP_PORT GPIOB //!< GPIOB - Порт дисплея

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
#define DISP_SDA_d ((DISP_PORT->IDR >> DISP_SDA) & 1) //!< Считать Disp.sda

//-----------------------------------------------------------------------------
//	Console
//-----------------------------------------------------------------------------
#define con_push(x) sp_tx_push(&con, (x))	 //!< Помещает символ в буфер передатчика консоли
#define con_tx_pop(x) sp_tx_pop(&con)		 //!< Извлекает символ из буфера передачи консоли
#define con_str(x) sp_str(&con, (x))		 //!< Помещает строку в буфер передатчика консоли
#define con_start() sp_start(&con)			 //!< Начинает передачу
#define con_rx_push(x) sp_rx_push(&con, (x)) //!< Помещает символ в буфер приемника консоли
#define con_rx_pop() sp_rx_pop(&con)		 //!< Извлекает символ из буфера приемника консоли
#define con_byte(x) sp_byte(&con, (x))		 //!< Помещает байт в буфер передатчика консоли
#define con_word(x) sp_word(&con, (x))		 //!< Помещает слово в буфер передатчика консоли
#define con_dword(x) sp_dword(&con, (x))	 //!< Помещает двойное слово в буфер передатчика консоли
#define con_dec(x) sp_dec(&con, (x))		 //!< Помещает десятичное число со знаком в буфер передатчика консоли
#define con_udec(x) sp_udec(&con, (x))		 //!< Помещает десятичное число без знака в буфер передатчика консоли
#define con_float(x) sp_float(&con, (x))	 //!< Помещает число с плавающей точкой в буфер передатчика консоли

#define CONSOLE_BAUD (115200)				   //!< Скорость консоли, бод
#define CONSOLE_TX_SIZE (1024)				   //!< Размер буфера передачи, байт
#define CONSOLE_RX_SIZE (128)				   //!< Размер буфера приема, байт
#define CONSOLE_COMMAND_SIZE (CONSOLE_RX_SIZE) //!< Размер буфера команды
#define COM_END (0x0D)						   //!< Символ конца команды
#define COM_BEGIN ('>')						   //!< Символ начала команды
#define COM_BS (0x08)						   //!< Символ стирания символа
#define COM_ESC (0x1B)						   //!< Символ стирания команды
#define TO_COM_CLEAR (10000)				   //!< Таймаут ввода команды, мс

#define HART_BAUD (1200) //!< Скорость HART, бод

#define EVENT_LOG_NUM (128) //!< Маскимальное число событий в журнале
#define EVENT_FLAG (7)		//!< Номер бита признака окончания тревоги

//-----------------------------------------------------------------------------
//!	\enum eEVENT
//! \brief Привязка битов к событиям
//-----------------------------------------------------------------------------
enum eEVENT
{
	EVENT_FREE = 0,		//!< 00 - Пусто
	EVENT_SW_FAIL,		//!< 01 - Событие "Прошивка неисправна"
	EVENT_AIN_MIN,		//!< 02 - Событие "Ток меньше минимально допустимого" \ref AIN_MA_MIN
	EVENT_AIN_MAX,		//!< 03 - Событие "Ток больше максимально допустимого" \ref AIN_MA_MAX
	EVENT_POWER_FAIL,	//!< 04 - Событие "Пропадание питания (ток ниже критического)" \ref AIN_MA_POWER_FAIL
	EVENT_ANGLE_MAX,	//!< 05 - Событие "Угол поворота больше 25°"
	EVENT_ANGLE_MIN,	//!< 06 - Событие "Угол поворота меньше -25°"
	EVENT_TA_MIN,		//!< 07 - Событие "Температура ниже -40°" \ref TEMP_C_MIN
	EVENT_TA_MAX,		//!< 08 - Событие "Температура выше +85°" \ref TEMP_C_MAX
	EVENT_AOUT_FAIL,	//!< 09 - Событие "Обрыв цепи аналогового выхода"
	EVENT_DOUT1_FAIL,	//!< 10 - Событие "Обрыв цепи аварийного дискретного выхода"
	EVENT_DOUT2_FAIL,	//!< 11 - Событие "Обрыв цепи дискретного выхода 2"
	EVENT_DOUT3_FAIL,	//!< 12 - Событие "Обрыв цепи дискретного выхода 3"
	EVENT_IM_FAIL,		//!< 13 - Событие "Отсутствие реакции ИМ на управляющее воздействие (в настоящий момент)"
	EVENT_IM_FAIL_ONCE, //!< 14 - Событие "Отсутствие реакции ИМ на управляющее воздействие (в прошлом) – один раз"
	EVENT_HMC_MAX,		//!< 15 - Событие "Угол поворота больше откалиброванного значения"
	EVENT_HMC_MIN,		//!< 16 - Событие "Угол поворота меньше откалиброванного значения"
	EVENT_WDT_RESET,	//!< 17 - Событие "Сброс процессора по WDT – один раз"
	EVENT_TAMPER,		//!< 18 - Событие "Открыта крышка"
	EVENT_BP_FAIL,		//!< 19 - Событие "Сбой блока питания"
	EVENT_FULL_RST,		//!< 20 - Событие "Сброс"
	EVENT_PST_FAIL,		//!< 21 - Событие "Ошибка PST"
	EVENT_HIF_FAIL,		//!< 22 - Событие "Высокое трение"

	EVENT_NUM //!< Общее число событий
};

#define EVENT_MASK_ONCE (0 | (1 << EVENT_PST_FAIL))

//-----------------------------------------------------------------------------
//!	\typedef struct sEVENT
//! \brief Элемент события
//!	\struct sEVENT
//! \brief Элемент события
//-----------------------------------------------------------------------------
typedef struct sEVENT
{
	DWORD time;	 //!< Поле "Время" в сек
	DWORD event; //!< Поле "Событие" \sa eEVENT
} sEVENT;

//-----------------------------------------------------------------------------
//!	\typedef struct sEEPROM_FLAGS
//! \brief Флаги в энергонезависимой памяти
//!	\struct sEEPROM_FLAGS
//! \brief Флаги в энергонезависимой памяти
//-----------------------------------------------------------------------------
typedef struct sEEPROM_FLAGS
{
	union
	{
		DWORD dw; //!< Формат беззнаковый 32 бита
		int i;	  //!< Формат знаковый 32 бита
		struct
		{
			DWORD display_im : 1;		//!< Единицы ИМ \sa eDISPLAY_IM
			DWORD display_ain : 1;		//!< Единицы входа \sa eDISPLAY_AIN
			DWORD display_contrast : 5; //!< Контрастность экрана
			DWORD display_rotate : 1;	//!< Поворот экрана \sa eDISPLAY_ROTATE

			DWORD hart_id : 4;		//!< Адрес HART
			DWORD hart_enable : 1;	//!< Отключение HART \sa eHART_ENABLE
			DWORD hart_prio : 1;	//!< Приоритет входа \sa eHART_PRIO
			DWORD hart_none : 1;	//!< Резервный бит
			DWORD coil_inverse : 1; //!< Инверсия шкалы \sa eCOIL_INVERSE

			DWORD aout_sig : 2;	 //!< Источник для аналогового выхода \sa eOUT_SIG
			DWORD dout2_sig : 2; //!< Источник для дискретного выхода 2 \sa eOUT_SIG
			DWORD dout2_dir : 1; //!< Направление дискретного выхода 2 \sa eDOUT_DIR
			DWORD dout3_sig : 2; //!< Источник для дискретного выхода 3\sa eOUT_SIG
			DWORD dout3_dir : 1; //!< Направление дискретного выхода 3 \sa eDOUT_DIR

			DWORD display_filter : 3; //!< Параметр ФНЧ для положения на дисплее \sa menu_disp_filter_item
			DWORD im_ph : 4;		  //!< Переходная характеристика \sa menu_im_ph_item

			DWORD dout1_disable : 1; //!< Отключение дискретного выхода 1 \sa eDOUT1_ENABLE
		};
	};
} sEEPROM_FLAGS;

//-----------------------------------------------------------------------------
//!	\enum eDISPLAY_IM
//! \brief Отображение положения на дисплее
//-----------------------------------------------------------------------------
enum eDISPLAY_IM
{
	DISPLAY_IM_PROC = 0, //!< Положение "Проценты"
	DISPLAY_IM_GRAD,	 //!< Положение "Градусы"
};

//-----------------------------------------------------------------------------
//!	\enum eDISPLAY_AIN
//! \brief Отображение входного тока на дисплее
//-----------------------------------------------------------------------------
enum eDISPLAY_AIN
{
	DISPLAY_AIN_MA = 0, //!< Входной ток "мА"
	DISPLAY_AIN_PROC,	//!< Входной ток "Проценты"
};

//-----------------------------------------------------------------------------
//!	\enum eDISPLAY_ROTATE
//! \brief Поворот экрана
//-----------------------------------------------------------------------------
enum eDISPLAY_ROTATE
{
	DISPLAY_ROTATE_OFF = 0, //!< Экран "Нормал"
	DISPLAY_ROTATE_ON,		//!< Экран "Поворот"
};

//-----------------------------------------------------------------------------
//!	\enum eHART_ENABLE
//! \brief Отключение HART
//-----------------------------------------------------------------------------
enum eHART_ENABLE
{
	HART_ENABLE_ON = 0, //!< HART-модем включен
	HART_ENABLE_OFF,	//!< HART-модем выключен
};

//-----------------------------------------------------------------------------
//!	\enum eHART_PRIO
//! \brief Приоритет входа
//-----------------------------------------------------------------------------
enum eHART_PRIO
{
	HART_PRIO_AIN = 0, //!< Приоритет аналогового входа
	HART_PRIO_HART,	   //!< Приоритет HART-модема
};

//-----------------------------------------------------------------------------
//!	\enum eCOIL_INVERSE
//! \brief Инверсия шкалы
//-----------------------------------------------------------------------------
enum eCOIL_INVERSE
{
	COIL_INVERSE_NONE = 0, //!< Прямая шкала
	COIL_INVERSE_INV,	   //!< Обратная шкала
};

//-----------------------------------------------------------------------------
//!	\enum eAOUT_SIG
//! \brief Источник сигнала
//-----------------------------------------------------------------------------
enum eOUT_SIG
{
	OUT_SIG_NONE = 0, //!< Источник "Нет"
	OUT_SIG_POS,	  //!< Источник "Положение"
	OUT_SIG_AIN,	  //!< Источник "Вход"
	OUT_SIG_DELTA,	  //!< Источник "Рассогласование"
};

//-----------------------------------------------------------------------------
//!	\enum eDOUT_DIR
//! \brief Направление дискретного выхода
//-----------------------------------------------------------------------------
enum eDOUT_DIR
{
	DOUT_DIR_UP = 0, //!< Направление "Увеличение"
	DOUT_DIR_DOWN,	 //!< Направление "Уменьшение"
};

//-----------------------------------------------------------------------------
//!	\enum eDOUT1_ENABLE
//! \brief Отключение дискретного выхода аварии
//-----------------------------------------------------------------------------
enum eDOUT1_ENABLE
{
	DOUT1_ENABLE = 0, //!< Выход аварии разрешен
	DOUT1_DISABLE,	  //!< Выход аварии отключен
};

/// @brief Перечисление доступных форматов отображения данных на дисплее.
typedef enum eDISPLAY_FORMAT
{
	/// @brief Отображение для исполнения AM800.D.
	FROM_D = 0,

	/// @brief Отображение для исполнения AM800.W.
	FROM_W
} eDISPLAY_FORMAT;

/// @brief Перечисление доступных температурных исполнений.
typedef enum eTEMP_VERSION
{
	/// @brief -20..+60.
	N = 0,

	/// @brief -40..+75.
	C,

	/// @brief -60..+60.
	E
} eTEMP_VERSION;

/// @brief Перечисление состояний вкл/выкл.
typedef enum eON_OFF
{
	/// @brief Выключено.
	OFF = 0,

	/// @brief Включено.
	ON
} eON_OFF;

/// @brief Перечисление типов датчик.
enum eSENSOR_TYPES
{
	/// @brief АЦП.
	ADC_SENSOR = 0,

	/// @brief SPI.
	SPI_SENSOR
};

/// @brief Перечисление режимов ФНЧ.
enum eLPF_MODE
{
	/// @brief Выключен.
	LPF_OFF = 0,

	/// @brief Минимальная фильтрация.
	LPF_MIN,

	/// @brief Средняя фильтрация.
	LPF_MID,

	/// @brief Максимальная фильтрация.
	LPF_MAX
};

//-----------------------------------------------------------------------------
//!	\typedef struct sDATA_EEPROM
//! \brief Энергонезависимая память переменных и журнала событий
//!	\struct sDATA_EEPROM
//! \brief Энергонезависимая память переменных и журнала событий
//! \note Обозначения типа:
//! \note - \b "ЗК" - заводская калибровка
//! \note - \b "ПЗ" - пользовательское значение
//! \note - \b "РП" - рабочая переменная
//! \note - \b "НИ" - не используется
//-----------------------------------------------------------------------------
typedef struct sDATA_EEPROM
{
	volatile int ain_adc_min; //!< \b ЗК. Значение АЦП для нижней точки в ед. АЦП
	volatile int ain_ma_min;  //!< \b ЗК. Значение тока в линии для нижней точки в 0.01 мА
	volatile int ain_adc_max; //!< \b ЗК. Значение АЦП для верхней точки в ед. АЦП
	volatile int ain_ma_max;  //!< \b ЗК. Значение тока в линии для верхней точки в 0.01 мА

	volatile int linear_en;	   //!< \b ПЗ. Включение линеаризации положения [0 или 1]
	volatile int linear_coef;  //!< \b ПЗ. Коэффициент линеаризации.
	volatile int linear_shift; //!< \b НИ. Сдвиг нуля линеаризации.

	volatile int display_format; //!< \b ПЗ. Формат дисплея: 0 - D; 1 - W
	volatile int sensor_type;	 //!< \b ПЗ. Тип датчика: 0 - АЦП; 1 - SPI
	volatile int temp_version; //!< \b ПЗ. Температурное исполнение: 0 - -20..+60; 1 - -40..+75; 2 - -60..+60;

	volatile int ain_ma_t_min;	//!< \b ПЗ. Значение тока в линии для нижней точки в 0.01 мА
	volatile int hmc_pos_t_min; //!< \b ЗК. Целевое положение для нижней точки в 0.1%.раб [0%.раб]
	volatile int ain_ma_t_max;	//!< \b ПЗ. Значение тока в линии для верхней точки в 0.01 мА
	volatile int hmc_pos_t_max; //!< \b ЗК. Целевое положение для верхней точки в 0.1%.раб [100%.раб]

	volatile float kp; //!< \b ПЗ. Коэффициент ПИД Кп
	volatile float ki; //!< \b ПЗ. Коэффициент ПИД Ки
	volatile float kd; //!< \b ПЗ. Коэффициент ПИД Кд

	volatile float pid_int_min; //!< \b ЗК. Нижний порог И-составляющей
	volatile float pid_int_max; //!< \b ЗК. Верхний порог И-составляющей

	volatile int lpf_max_samples; //!< \b ЗК. Максимальное количество отсчетов для ФНЧ.
	volatile float lpf_pos;		  //!< \b ПЗ. Параметр ФНЧ для датчика положения [0..1]
	volatile int lpf_a_in;		  //!< \b ПЗ. Степень усреднения ФНЧ для ain_adc [0-выкл; 1-мин; 2-сред; 3-макс]
	volatile int lpf_a_out;		  //!< \b ПЗ. Степень усреднения ФНЧ для aout_adc [0-выкл; 1-мин; 2-сред; 3-макс]

	volatile sEEPROM_FLAGS flags; //!< \b Флаги \sa sEEPROM_FLAGS

	volatile int aout_pos_min; //!< \b ПЗ. Нижняя точка аналогового выхода в 0.1%раб
	volatile int aout_pos_max; //!< \b ПЗ. Верхняя точка аналогового выхода в 0.1%раб
	volatile int aout_ma_min;  //!< \b ПЗ. Нижняя точка аналогового выхода в 0.1мА
	volatile int aout_ma_max;  //!< \b ПЗ. Верхняя точка аналогового выхода в 0.1мА

	volatile int dout2_pos; //!< \b ПЗ. Выход NAMUR2 положение в 0.1%раб
	volatile int dout2_gis; //!< \b ПЗ. Выход NAMUR2 гистерезис в 0.1%раб
	volatile int dout3_pos; //!< \b ПЗ. Выход NAMUR3 положение в 0.1%раб
	volatile int dout3_gis; //!< \b ПЗ. Выход NAMUR3 гистерезис в 0.1%раб

	volatile int aout_pwm_min; //!< \b ЗК. Значение ШИМ для 4.00 мА
	volatile int aout_pwm_max; //!< \b ЗК. Значение ШИМ для 20.00 мА

	volatile int hmc_path_nv[3]; //!< \b РП. Переменная "Кц" в 0.1%.раб
	volatile int worktime_nv[3]; //!< \b РП. Переменная "Тк" в сек

	volatile int hard_pos_min; //!< \b ПЗ. Плотное закрытие в сторону минимума в 0.1%.раб от начала
	volatile int hard_pos_max; //!< \b ПЗ. Плотное закрытие в сторону максимума в 0.1%.раб с конца

	volatile int hmc_adc_0;		 //!< \b ЗК. Значение hmc_adc для точки 0%.абс
	volatile int hmc_adc_100;	 //!< \b ЗК. Значение hmc_adc для точки 100%.абс
	volatile int hmc_lim_0;		 //!< \b ПЗ. Значение hmc_pos_s для точки 0%.раб в 0.1%.абс
	volatile int hmc_lim_100;	 //!< \b ПЗ. Значение hmc_pos_s для точки 100%.раб в 0.1%.абс
	volatile int hmc_g_0;		 //!< \b ЗК. Значение угла для hmc_adc_0 в 0.1 градуса
	volatile int hmc_g_100;		 //!< \b ЗК. Значение угла для hmc_adc_100 в 0.1 градуса
	volatile int coil_pwm_limit; //!< \b ПЗ. Ограничение диапазона для coil_pwm в 1% полудиапазона
	// 48

	volatile float t_lp_up;	  //!< \b ПЗ. Скорость псевдоцели вверх [0..1]
	volatile float t_lp_down; //!< \b ПЗ. Скорость псевдоцели вниз [0..1]
	// 50

	volatile int pst_dx;	  //!< \b ПЗ. PST. Процент хода, %раб [4..15%]
	volatile int pst_t;		  //!< \b ПЗ. PST. Время теста, с [2..60с]
	volatile int pst_te;	  //!< \b ПЗ. PST. Задержка ошибки, с [2..60с]
	volatile int pst_autorun; //!< \b ПЗ. PST. Автозапуск, ч [0..4096ч]
	// 54

	volatile int hif_dx; //!< \b ПЗ. Высокое трение. Рассогласование, %раб [3..25%]
	volatile int hif_t;	 //!< \b ПЗ. Высокое трение. Время, с [3..60]
	// 56

	volatile float si_off; //!< \b ЗК. Область накопления И-составляющей, %раб [0..100%]
	// 57

	volatile int slow_LT_edge;	//!< \b ЗК. Граница замедления хода привода из-за температуры
	volatile float slow_LT_val; //!< \b ЗК. Коэффициент деградации скорости
	volatile int midpoint;		//!< \b ЗК. Средняя точка для настройки пневмоблока
	// 60

	int none[64 - 61]; //!< \b НИ. Не используется

	volatile sEVENT events[EVENT_LOG_NUM]; //!< \b РП. Журнал событий \sa sEVENT

} sDATA_EEPROM;

#define TEMPERATURE_BAD (0x55AA) //!< Магическое число для некорректной температуры

//-----------------------------------------------------------------------------
//!	\enum eBUTTON
//! \brief Номер кнопки
//-----------------------------------------------------------------------------
enum eBUTTON
{
	BUTTON_UP = 1,	 //!< Кнопка "Вверх"
	BUTTON_OK = 2,	 //!< Кнопка "Ок"
	BUTTON_DOWN = 4, //!< Кнопка "Вниз"
};

/// @brief Источники цели для ПИД-регулятора.
enum ePID_SOURCE
{
	/// @brief  По внешенему сигналу.
	PID_SOURCE_SIGNAL = 0,

	/// @brief По ручному указанию
	PID_SOURCE_HAND,
};

//-----------------------------------------------------------------------------
//!	\enum ePUPD
//! \brief Подтяжка вывода
//-----------------------------------------------------------------------------
enum ePUPD
{
	PUPD_OFF = 0, //!< Подтяжка выключена
	PUPD_UP,	  //!< Подтяжка вверх
	PUPD_DOWN,	  //!< Подтяжка вниз
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

/// @brief Тип шкалы
enum eTYPE_SCALE
{
	/// @brief Прямая
	TYPE_DIRECT = 0,

	/// @brief Обратная
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
//! \brief Функция "Сброс времени"
//-----------------------------------------------------------------------------
extern void calib_reset(void);

//-----------------------------------------------------------------------------
//! \fn extern void im_reset(void)
//! \brief Функция "Полный сброс"
//-----------------------------------------------------------------------------
extern void im_reset(void);

//-----------------------------------------------------------------------------
//! \fn extern void factory_reset(void)
//! \brief Функция "Сброс к заводским настройкам"
//-----------------------------------------------------------------------------
extern void factory_reset(void);

//-----------------------------------------------------------------------------
//! \fn extern void log_reset(void)
//! @brief Очистка журнала событий 
//-----------------------------------------------------------------------------
extern void log_reset(void);

//-----------------------------------------------------------------------------
//! \fn extern void hart_tx_enable(void)
//! \brief Включить передатчик HART
//-----------------------------------------------------------------------------
extern void hart_tx_enable(void);

//-----------------------------------------------------------------------------
//! \fn extern void tick(tTIMER *t)
//! \brief Функция инкремента времени
//! \param t Указатель на переменную времени
//! \sa tTIMER
//-----------------------------------------------------------------------------
extern void tick(tTIMER *t);

//-----------------------------------------------------------------------------
//! \fn extern void print_fault(sFAULT *fault)
//! \brief Печать отладочной информации при HardFault
//! \param fault Указатель на кадр регистров \ref sFAULT
//-----------------------------------------------------------------------------
extern void print_fault(sFAULT *fault);

//-----------------------------------------------------------------------------
//! \fn extern void set_pin(const WORD pin, const BYTE value)
//! \brief Установить вывод
//! \param pin Номер вывода \ref eGPIO_PORT
//! \param value Значение вывода
//-----------------------------------------------------------------------------
extern void set_pin(const WORD pin, const BYTE value);

//-----------------------------------------------------------------------------
//! \fn extern int get_pin(const WORD pin)
//! \brief Получить значение вывода
//! \param pin Номер вывода \ref eGPIO_PORT
//! \return Значение вывода
//-----------------------------------------------------------------------------
extern int get_pin(const WORD pin);

//-----------------------------------------------------------------------------
//! \fn extern void set_pupd(const WORD pin, const BYTE value)
//! \brief Установить подтяжку
//! \param pin Номер вывода \ref eGPIO_PORT
//! \param value Значение подтяжки \ref ePUPD
//-----------------------------------------------------------------------------
extern void set_pupd(const WORD pin, const BYTE value);

//-----------------------------------------------------------------------------
//! \fn extern void eeprom_unlock(void)
//! \brief Разблокировать EEPROM
//-----------------------------------------------------------------------------
extern void eeprom_unlock(void);

//-----------------------------------------------------------------------------
//! \fn extern void eeprom_lock(void)
//! \brief Заблокировать EEPROM
//-----------------------------------------------------------------------------
extern void eeprom_lock(void);

//-----------------------------------------------------------------------------
//! \fn extern int load_maj(volatile int x[3], volatile int *v)
//! \brief Загрузить мажоритарную переменную
//! \param x Мажоритарная переменная
//! \param v Указатель на переменую
//! \return Статус выполнения:
//! - 0 - ошибка выполнения
//! - 1 - успешное выполнение
//-----------------------------------------------------------------------------
extern int load_maj(volatile int x[3], volatile int *v);

//-----------------------------------------------------------------------------
//! \fn extern int save_maj(volatile int x[3], volatile int v)
//! \brief Сохранить мажоритарную переменную
//! \param x Мажоритарная переменная
//! \param v Значение переменой
//! \return Статус выполнения:
//! - 0 - ошибка выполнения
//! - 1 - успешное выполнение
//-----------------------------------------------------------------------------
extern int save_maj(volatile int x[3], volatile int v);

//-----------------------------------------------------------------------------
//! \fn extern void load_events(void)
//! \brief Загрузить журнал событий
//! \sa event_pos, event_num
//-----------------------------------------------------------------------------
extern void load_events(void);

//-----------------------------------------------------------------------------
//! \fn extern void event_save(const DWORD event)
//! \brief Добавить событие в журнал
//! \param event Событие
//! \sa eEVENT, EVENT_FLAG
//-----------------------------------------------------------------------------
extern void event_save(const DWORD event);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_cpy(char *dst, const char *src)
//! \brief Записать строку
//! \param dst Строка назначения
//! \param src Строка источника
//-----------------------------------------------------------------------------
extern void mstr_cpy(char *dst, const char *src);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_add(char *dst, const char *src)
//! \brief Добавить строку
//! \param dst Строка назначения
//! \param src Строка источника
//-----------------------------------------------------------------------------
extern void mstr_add(char *dst, const char *src);

/// @brief Записать целое число
//! \param dst Строка назначения
//! \param x Целое число
extern void mstr_copy_int(char *dst, int x);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_add_int(char *dst, int x)
//! \brief Добавить целое число
//! \param dst Строка назначения
//! \param x Целое число
//-----------------------------------------------------------------------------
extern void mstr_add_int(char *dst, int x);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_add_int2(char *dst, int x);
//! \brief Добавить целое двузначное число
//! \param dst Строка назначения
//! \param x Целое число
//-----------------------------------------------------------------------------
extern void mstr_add_int2(char *dst, int x);

//-----------------------------------------------------------------------------
//! \fn extern void mstr_add_dword(char *dst, DWORD x)
//! \brief Добавить 32-битное беззнаковое число в шестнадцатеричном виде
//! \param dst Строка назначения
//! \param x Целое число
//-----------------------------------------------------------------------------
extern void mstr_add_dword(char *dst, DWORD x);

/// @brief Изменить значение ШИМ аналогового выхода.
/// @param value Записываемое значение.
extern void Change_PWM_Aout(int value);

/// @brief Получить значение ФНЧ.
/// @param filtrationDegree Степень фильтрации.
extern float Get_LpfValue(int filtrationDegree);

/******************************************************************************
 *	VARIABLE
 *****************************************************************************/
extern volatile tTIMER t_workled; //!< Таймаут светодиода "РАБОТА".
// extern volatile int		blink_var;						//!< Переменная мигания
extern const BYTE s_who[];							//!< Название контроллера и прошивки
extern BYTE console_tx_buf[CONSOLE_TX_SIZE];		//!< Буфер передачи UART1.
extern BYTE console_rx_buf[CONSOLE_RX_SIZE];		//!< Буфер приема UART1.
extern sSERIAL_PORT con;							//!< Последовательный порт UART1 - консоль.
extern BYTE console_command[CONSOLE_COMMAND_SIZE];	//!< Буфер команды
extern BYTE console_exec_buf[CONSOLE_COMMAND_SIZE]; //!< Буфер выполнения
extern volatile int console_command_pos;			//!< Положение в буфере команды

extern volatile int disp_flag;		   //!< Флаг обновления экрана (период #TO_WORKLED / 2)
extern volatile int disp_init_flag;	   //!< Флаг инициализации экрана
extern tTIMER to_disp_init;			   //!< Таймер задержки инициализации экрана
extern volatile int disp_update_count; //!< Счетчик обновлений экрана
extern volatile int display_contrast;  //!< Контрастность дисплея

extern volatile DWORD uptime;	  //!< Время работы позиционера после перезагрузки в сек
extern volatile int worktime;	  //!< Время работы клапана в сек
extern volatile int get_adc_flag; //!< Флаг вывода значений АЦП
extern volatile int get_pos_flag; //!< Флаг вывода значений координаты и цели

extern volatile int st_auto;
extern volatile int auto_type;
extern volatile int hart_autoset_flag;		//!< Флаг запуска автонастройки через HART
extern volatile int hart_autoset_done_flag; //!< Флаг окончания автонастройки через HART
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

extern volatile int btn_adc;		//!< Значение АЦП для кнопок в ед. АЦП
extern tTIMER to_button_pressed;	//!< Таймер удержания кнопки
extern volatile int button_pressed; //!< Нажатая кнопка
extern volatile int tamper;			//!< Вскрытие крышки (0 - закрыто, 1 - вскрытие)

extern volatile int tempeature;		//!< Значение температуры в градусах Цельсия
extern volatile int tempeature_adc; //!< Значение АЦП для температуры в ед. АЦП

extern volatile int ain_adc_s;	//!< Сырые данные с АЦП входа [0-4095]
extern volatile float fain_adc; //!< Фильтрованные данные с АЦП входа [0-4095]
extern volatile int ain_ma_s;	//!< Реальный ток задания в 0.01 мА
extern volatile int ain_ma;		//!< Рабочий ток задания в 0.01 мА
extern volatile int ain_pos_t;	//!< Целевое значение для ПИД в 0.1%.раб

extern volatile int hart_ma;	   //!< Задание тока по HART в 0.01 мА
extern volatile int aout_ma;	   //!< Ток аналогового выхода в 0.1 мА
extern volatile int aout_diag;	   //!< Диагностика аналогового выхода (0 - норм, иначе - сбой)
extern volatile int namur_out[3];  //!< Значение дискретного выхода
extern volatile int namur_diag[3]; //!< Диагностика дискретного выхода (0 - норм, иначе - сбой)

extern volatile int test_pwm;	//!< Управление электромагнитом с консоли
extern volatile int test_a_out; //!< Управление аналоговым выходом с консоли
extern volatile int test_namur; //!< Управление дискретными выходами с консоли (0 - запрещено, иначе - разрешено)

extern volatile int pos_value; //!< Сырые данные с АЦП датчика положения в ед. АЦП [0-4095]

extern volatile int ihmc_adc_s;	  //!< Сырые данные с АЦП датчика положения в ед. АЦП [0-4095]
extern volatile float fhmc_adc;	  //!< Фильтрованные данные с АЦП датчика положения в ед. АЦП [0-4095] \sa sDATA_EEPROM параметр lpf_pos
extern volatile int ihmc_adc;	  //!< Фильтрованные данные с АЦП датчика положения в ед. АЦП [0-4095] \sa sDATA_EEPROM параметр lpf_pos
extern volatile float fhmc_pos_a; //!< Абсолютное положение в 1%.абс [0-100]
extern volatile int ihmc_pos_a;	  //!< Абсолютное положение в 0.1%.абс [0-1000]
extern volatile float fhmc_pos;	  //!< Рабочее положение в 1%.раб [0-100]
extern volatile int ihmc_pos;	  //!< Рабочее положение в 0.1%.раб [0-1000]
extern volatile int ihmc_adc_d;	  //!< Фильтрованные данные с АЦП датчика положения для дисплея в ед. АЦП [0-4095] \sa sEEPROM_FLAGS параметр display_filter
extern volatile int ihmc_pos_da;  //!< Абсолютное положение в 0.1%.абс для дисплея [0-1000]
extern volatile int ihmc_pos_d;	  //!< Рабочее положение в 0.1%.абс для дисплея [0-1000]
extern volatile int ihmc_g;		  //!< Абсолютное положение в 0.1 градуса
extern volatile int ihmc_path;	  //!< Счетчик циклов клапана в 0.1%.раб
extern volatile int ihmc_pos_t;	  //!< Целевое значение для ПИД в 0.1%.раб

extern volatile int linear_temp_disable; // временное хранение флага линеаризации.

extern volatile int signal_delta; //!< Текущее рассогласование в 0.1%.раб
extern volatile float pid_int;	  //!< Текущее значение И-составляющей
extern volatile int coil_pwm;	  //!< Управлющий сигнал на электромагнит
extern volatile int coil_pwm_def;
extern volatile int pid_debug; //!< Отладка ПИД регулятора
extern volatile int pid_int_clear;
extern volatile int pid_source; //!< Источник целевого положения

extern tTIMER to_power_fail; //!< Таймер сбоя блока питания

extern const float hmc_disp_fs[6]; //!< Коэффициенты ФНЧ для дисплея
extern const float im_ph_k[11];	   //!< Коэффициенты переходной характеристики
extern volatile int show_ph;	   //!< Отладка переходной характеристики

extern volatile float info_hmc_d;	  //!< "Инфорежим-ПИД". Рассогласование
extern volatile float info_hmc_d_pr;  //!< "Инфорежим-ПИД". П-составляющая
extern volatile float info_hmc_d_int; //!< "Инфорежим-ПИД". И-составляющая
extern volatile float info_hmc_d_dif; //!< "Инфорежим-ПИД". Д-составляющая
extern volatile float info_coil;	  //!< "Инфорежим-ПИД". Управляющий сигнал на электромагнит
extern volatile int info_coil_ofs;	  //!< "Инфорежим-ПИД". Управляющий сигнал на электромагнит (смещение)

extern volatile float lof_arr[2][LOF_SIZE];
extern volatile int lof_gist[2][LOF_GIST_SIZE];
extern volatile int lof_pos;

extern volatile int event_pos;			  //!< Текущее положение в журнале событий
extern volatile int event_num;			  //!< Текущее число записей журнала событий
extern volatile DWORD event_flag_current; //!< Текущий список событий
extern volatile DWORD event_flag;		  //!< Изменение списка событий
extern volatile DWORD event_flag_last;	  //!< Триггер списка событий

extern volatile int flag_calib_reset;	//!< Флаг "Сброс времени" \sa calib_reset
extern volatile int flag_im_reset;		//!< Флаг "Полный сброс" \sa im_reset
extern volatile int flag_factory_reset; //!< Флаг "Заводской сброс" \sa factory_reset

extern sDATA_EEPROM data_eeprom __attribute__((section(".eeprom")));	//!< Энергонезависимые данные
extern const sIMAGE_INFO image __attribute__((section(".image_info"))); //!< Заводские данные \sa sIMAGE_INFO

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
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
