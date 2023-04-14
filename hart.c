/**
 * @file hart.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief HART-протокол.
 * @version 0.1
 * @date 20.05.2016
 * 
 * @copyright (C) Ярославль, 2016
 * 
 */
#include "hart.h"
#include "type.h"
#include <string.h>
#include <math.h>

BYTE	hart_tx_buf[HART_TX_BUF];
volatile int	hart_tx_size = 0;
volatile int	hart_tx_pos = 0;
volatile int	hart_need_reboot = 0;
//volatile BYTE	hart_short_addr = 0;

volatile float hart_current_lower = 3.6f;
volatile float hart_current_upper = 20.4f;
volatile float hart_demph = 0.8f;

BYTE	hart_message[24];
BYTE	hart_tag[6];
BYTE	hart_desc[12];

volatile int	hart_log_pos = 0;

#define HART_MANUF_ID     (254)
#define HART_DEVICE_TYPE  (150)
#define HART_PRE_COUNT    (8)
//#define HART_ID        		(54321)

//#define HART_SENSOR_ID 		(123456)
#define HART_SENSOR_UPPER	(20.2f)
#define HART_SENSOR_LOWER	(3.8f)
#define HART_SENSOR_SPAN	(0.3f)

enum eHART_STATE
{
  HART_STATE_IDLE = 0,
  HART_STATE_ADDR_SHORT,
  HART_STATE_ADDR_LONG,
  HART_STATE_CMD,
  HART_STATE_CMDEX,
  HART_STATE_DATA,
  HART_STATE_DATAIN,
};

volatile int hart_ff_count = 0;
volatile int hart_state = HART_STATE_IDLE;
volatile BYTE hart_cs = 0;
volatile int  hart_cmd = 0;
volatile BYTE	hart_cmd_data[32];
volatile int	hart_data_pos = 0;
volatile int  hart_data_size = 0;
volatile BYTE hart_addr_count = 0;
volatile BYTE hart_addr[6];
volatile float	hart_ext_current = 0.0f;
tTIMER to_hart = TO_HART;
tTIMER to_hart_act = 0;
volatile BYTE	hart_master_act = 0;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD0
//! \brief Ответ на команду 0 "Считать уникальный идентификатор"
//! \struct sHART_ANS_CMD0
//! \brief Ответ на команду 0 "Считать уникальный идентификатор"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD0
{
  BYTE  ext;									//!< Поле "расширение"
  BYTE  code_manuf;						//!< Поле "ID изготовителя"
  BYTE  code_type;						//!< Поле "код типа устройства"
  BYTE  pre_count;						//!< Поле "число преамбул"
  BYTE  ver_uni;							//!< Поле "версия универсальных команд"
  BYTE  ver_spec;							//!< Поле "версия специализированных команд"
  BYTE  ver_sw;								//!< Поле "версия программного обеспечения"
  BYTE  ver_hw;								//!< Поле "версия аппаратного обеспечения"
  BYTE  flags;								//!< Поле "флаги функций устройства"
  BYTE  id[3];								//!< Поле "Идентификатор устройтва"
} sHART_ANS_CMD0;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD1
//! \brief Ответ на команду 1 "Считать первичную переменную"
//! \struct sHART_ANS_CMD1
//! \brief Ответ на команду 1 "Считать первичную переменную"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD1
{
  BYTE 	type;									//!< Поле "Единицы измерения первичной переменной"
  DWORD pv;										//!< Поле "Первичная Переменная"
} sHART_ANS_CMD1;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD2
//! \brief Ответ на команду 2 "Считать ток и процент диапазона"
//! \struct sHART_ANS_CMD2
//! \brief Ответ на команду 2 "Считать ток и процент диапазона"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD2
{
  DWORD current;							//!< Поле "Ток PV, мА"
  DWORD percent;							//!< Поле "Процент диапазона PV, %"
} sHART_ANS_CMD2;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD3
//! \brief Ответ на команду "Считать значения четырех переменных"
//! \struct sHART_ANS_CMD3
//! \brief Ответ на команду "Считать значения четырех переменных"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD3
{
  DWORD current;							//!< Поле "Ток первичной переменной, мА."
  BYTE  type_1;								//!< Поле "Единицы измерения Первичной Переменной"
  DWORD var_1;								//!< Поле "Первичная переменная"
  BYTE  type_2;								//!< Поле "Единицы измерения Второй Переменной"
  DWORD var_2;								//!< Поле "Вторая переменная"
  BYTE  type_3;								//!< Поле "Единицы измерения Третьей Переменной"
  DWORD var_3;								//!< Поле "Третья переменная"
  BYTE  type_4;								//!< Поле "Единицы измерения Четвертой Переменной"
  DWORD var_4;								//!< Поле "Четвертая переменная"
} sHART_ANS_CMD3;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD12
//! \brief Ответ на команду 12 "Считать сообщение"
//! \struct sHART_ANS_CMD12
//! \brief Ответ на команду 12 "Считать сообщение"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD12
{
  BYTE  message[24];					//!< Поле "Сообщение "
} sHART_ANS_CMD12;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD13
//! \brief Ответ на команду 13 "Считать тэг, дескриптор, дату"
//! \struct sHART_ANS_CMD13
//! \brief Ответ на команду 13 "Считать тэг, дескриптор, дату"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD13
{
  BYTE  tag[6];								//!< Поле "Тег"
  BYTE  desc[12];							//!< Поле "Дескриптор"
  BYTE  data[3];							//!< Поле "Дата, соответственно день, месяц, год-1900"
} sHART_ANS_CMD13;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD14
//! \brief Ответ на команду 14 "Считать информацию о сенсоре первичной переменной"
//! \struct sHART_ANS_CMD14
//! \brief Ответ на команду 14 "Считать информацию о сенсоре первичной переменной"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD14
{
  BYTE  serial[3];						//!< Поле "Серийный номер сенсора"
  BYTE  units;								//!< Поле "Код единиц измерения пределов и минимального интервала сенсора"
  DWORD upper;								//!< Поле "Верхний предел сенсора"
  DWORD lower;								//!< Поле "Нижний предел сенсора"
  DWORD span;									//!< Поле "Минимальный интервал"
} sHART_ANS_CMD14;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD15
//! \brief Ответ на команду 15 "Считать информацию о выходе первичной переменной"
//! \struct sHART_ANS_CMD15
//! \brief Ответ на команду 15 "Считать информацию о выходе первичной переменной"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD15
{
  BYTE  alarm;								//!< Поле "Код выбора аларма"
  BYTE  trans;								//!< Поле "Код функции преобразования"
  BYTE  units;								//!< Поле "Код единиц измерения диапазона первичной переменной"
  DWORD upper;								//!< Поле "Верхнее значение диапазона"
  DWORD lower;								//!< Поле "Нижнее значение диапазона"
  DWORD damping;							//!< Поле "Величина демпфирования, сек"
  BYTE  write_protect;				//!< Поле "Код защиты от записи"
  BYTE  priv_code;						//!< Поле "Код метки дистрибьютера "
} sHART_ANS_CMD15;

//-----------------------------------------------------------------------------
//! \typedef struct sHART_ANS_CMD48
//! \brief Ответ на команду 48 "Считать дополнительный статус устройства"
//! \struct sHART_ANS_CMD48
//! \brief Ответ на команду 48 "Считать дополнительный статус устройства"
//-----------------------------------------------------------------------------
typedef struct sHART_ANS_CMD48
{
  BYTE  fix_L;								//!< Поле "Символ L"
  DWORD datetime;							//!< Поле "Дата события"
  WORD	pos;									//!< Поле "Номер записи"
  WORD	num;									//!< Поле "Число записей"
  DWORD	zero;									//!< Поле "0"
  BYTE	message[11];					//!< Поле "Сообщение"
} sHART_ANS_CMD48;

/// @brief Ответ на команду 170
typedef struct sHART_ANS_CMD170
{
	WORD		data[16];
} sHART_ANS_CMD170;

/// @brief Ответ на команду 174
typedef struct sHART_ANS_CMD174
{
	float		kp;
	float		ki;
	float		kd;
	float		lp_up;
	float		lp_down;
} sHART_ANS_CMD174;

/// @brief Структура комманды 175
typedef struct sHART_CMD175
{
	float		kp;
	float		ki;
	float		kd;
	float		lp_up;
	float		lp_down;
	union
	{
		DWORD	flags;
		struct
		{
			DWORD	f_kp:1;
			DWORD	f_ki:1;
			DWORD	f_kd:1;
			DWORD	f_lp_up:1;
			DWORD	f_lp_down:1;
			DWORD	f_none:27;
		};
	};
} sHART_CMD175;

sHART_ANS_CMD0 ans0;
sHART_ANS_CMD1 ans1;
sHART_ANS_CMD2 ans2;
sHART_ANS_CMD3 ans3;
sHART_ANS_CMD12 ans12;
sHART_ANS_CMD13 ans13;
sHART_ANS_CMD14 ans14;
sHART_ANS_CMD15 ans15;
sHART_ANS_CMD48 ans48;

//-----------------------------------------------------------------------------
//	DWORD float_to_dword(const float x)
//-----------------------------------------------------------------------------
DWORD float_to_dword(const float x)
{
	int	i;

  union
  {
    float f;
    BYTE  b[4];
  } tx;
  union
  {
    DWORD d;
    BYTE b[4];
  } ty;

  tx.f = x;
  for(i = 0; i < 4; i++) ty.b[i] = tx.b[3 - i];
  return ty.d;
}

//-----------------------------------------------------------------------------
//	float dword_to_float(const DWORD x)
//-----------------------------------------------------------------------------
float dword_to_float(const DWORD x)
{
	int	i;

  union
  {
    DWORD d;
    BYTE b[4];
  } tx;
  union
  {
    float f;
    BYTE  b[4];
  } ty;

  tx.d = x;
  for(i = 0; i < 4; i++) ty.b[i] = tx.b[3 - i];
  return ty.f;
}

//-----------------------------------------------------------------------------
//	void hart_init(void)
//-----------------------------------------------------------------------------
void hart_init(void)
{
  memset(hart_message, 0xFF, sizeof(hart_message));
  hart_str(hart_message, "MESSAGE", sizeof(hart_message));
  memset(hart_tag, 0xFF, sizeof(hart_tag));
  hart_str(hart_tag, "AM800", sizeof(hart_tag));
  memset(hart_desc, 0xFF, sizeof(hart_desc));
  hart_str(hart_desc, "POSITIONER", sizeof(hart_desc));
}

//-----------------------------------------------------------------------------
//	void hart_send(BYTE *ans, const int ans_size)
//-----------------------------------------------------------------------------
void hart_send(BYTE *ans, const int ans_size)
{
	int	i;

	to_hart = 0;

  hart_tx_size = 0;

  for(i = 0; i < HART_PRE_COUNT; i++)
  {
    hart_tx_buf[hart_tx_size] = 0xFF;
    hart_tx_size++;
  }
  if(hart_addr_count == 1)
  {
    // start
    hart_tx_buf[hart_tx_size] = 0x06;
    hart_cs = hart_tx_buf[hart_tx_size];
    hart_tx_size++;
    // addr
    hart_tx_buf[hart_tx_size] = data_eeprom.flags.hart_id | 0x80;
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
  }
  else
  {
    // start
    hart_tx_buf[hart_tx_size] = 0x86;
    hart_cs = hart_tx_buf[hart_tx_size];
    hart_tx_size++;
    // addr
    hart_tx_buf[hart_tx_size] = (HART_MANUF_ID & 0x3F) | 0x80;
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
    hart_tx_buf[hart_tx_size] = HART_DEVICE_TYPE;
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
    hart_tx_buf[hart_tx_size] = (HART_ID >> 16) & 0xFF;
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
    hart_tx_buf[hart_tx_size] = (HART_ID >> 8) & 0xFF;
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
    hart_tx_buf[hart_tx_size] = (HART_ID >> 0) & 0xFF;
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
  }
  // cmd
  hart_tx_buf[hart_tx_size] = hart_cmd;
  hart_cs ^= hart_tx_buf[hart_tx_size];
  hart_tx_size++;
  // size
  hart_tx_buf[hart_tx_size] = ans_size + 2;
  hart_cs ^= hart_tx_buf[hart_tx_size];
  hart_tx_size++;
  // status
  hart_tx_buf[hart_tx_size] = 0;
  hart_cs ^= hart_tx_buf[hart_tx_size];
  hart_tx_size++;
  hart_tx_buf[hart_tx_size] = 0;
  hart_cs ^= hart_tx_buf[hart_tx_size];
  hart_tx_size++;
  // data
  for(i = 0; i < ans_size; i++)
  {
    hart_tx_buf[hart_tx_size] = ans[i];
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
  }
  // status
  hart_tx_buf[hart_tx_size] = hart_cs;
  hart_tx_size++;

  hart_start_tx(hart_tx_size);
}

//-----------------------------------------------------------------------------
//	void hart_send_act(void)
//-----------------------------------------------------------------------------
void hart_send_act(void)
{
	int	i;

	to_hart = 0;

  hart_tx_size = 0;

  for(i = 0; i < HART_PRE_COUNT; i++)
  {
    hart_tx_buf[hart_tx_size] = 0xFF;
    hart_tx_size++;
  }
	// start
	hart_tx_buf[hart_tx_size] = 0x01;
	hart_cs = hart_tx_buf[hart_tx_size];
	hart_tx_size++;
	// addr
	hart_tx_buf[hart_tx_size] = data_eeprom.flags.hart_id | ((hart_master_act == 0)? 0xC0: 0x40);
	hart_master_act = (hart_master_act == 0)? 1: 0;
	hart_cs ^= hart_tx_buf[hart_tx_size];
	hart_tx_size++;
  // cmd
  hart_tx_buf[hart_tx_size] = 170;
  hart_cs ^= hart_tx_buf[hart_tx_size];
  hart_tx_size++;
  // size
  hart_tx_buf[hart_tx_size] = 48 + 2;
  hart_cs ^= hart_tx_buf[hart_tx_size];
  hart_tx_size++;
  // status
  hart_tx_buf[hart_tx_size] = 0;
  hart_cs ^= hart_tx_buf[hart_tx_size];
  hart_tx_size++;
  hart_tx_buf[hart_tx_size] = 0;
  hart_cs ^= hart_tx_buf[hart_tx_size];
  hart_tx_size++;
  // data
  for(i = 0; i < 16; i++)
  {
    hart_tx_buf[hart_tx_size] = (hart_act_data[hart_act_data_page_send * 32 + i * 2 + 0] >> 0) & 0xFF;
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
    hart_tx_buf[hart_tx_size] = ((hart_act_data[hart_act_data_page_send * 32 + i * 2 + 0] >> 8) & 0x0F)
    	| ((hart_act_data[hart_act_data_page_send * 32 + i * 2 + 1] << 4) & 0xF0);
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
    hart_tx_buf[hart_tx_size] = (hart_act_data[hart_act_data_page_send * 32 + i * 2 + 1] >> 4) & 0xFF;
    hart_cs ^= hart_tx_buf[hart_tx_size];
    hart_tx_size++;
  }
  // status
  hart_tx_buf[hart_tx_size] = hart_cs;
  hart_tx_size++;

  hart_start_tx(hart_tx_size);
}

//-----------------------------------------------------------------------------
//	void hart_str(BYTE *dst, BYTE *src, int size)
//-----------------------------------------------------------------------------
void hart_str(BYTE *dst, BYTE *src, int size)
{
  WORD data = 0;
  int count = 0;

  while(*src)
  {
    data = (data << 6) | ((*src) & 0x3F);
    count += 6;
    if(count >= 8)
    {
      *dst = (data >> (count - 8)) & 0xFF;
      count -= 8;
      dst++;
      size--;
    }
    src++;
  }

  while(size > 0)
  {
    data = (data << 6) | ((' ') & 0x3F);
    count += 6;
    if(count >= 8)
    {
      *dst = (data >> (count - 8)) & 0xFF;
      count -= 8;
      dst++;
      size--;
    }
  }

  if(count) *dst = data & 0xFF;
}

//-----------------------------------------------------------------------------
//	void hart_on_rx(const BYTE x)
//-----------------------------------------------------------------------------
void hart_on_rx(const BYTE x)
{
	float f;
	int		i;
	char	str[16];
	sEEPROM_FLAGS eeprom_flags;

	#if 0
	con_str("[");
	con_byte(x);
	con_str("]");
	con_start();
	#endif

	if(to_power_fail < TO_POWER_FAIL) return;
	if(data_eeprom.flags.hart_enable == HART_ENABLE_OFF) return;

	if(x == 0xFF)
  {
    hart_ff_count++;
    //if(hart_ff_count > 2) hart_state = HART_STATE_IDLE;
  }

  if((hart_state == HART_STATE_IDLE) && (hart_ff_count > 2))
  {
    switch(x)
    {
      case 0x02: // от главного подчиненному короткий
        hart_cs = x;
        hart_addr_count = 0;
        hart_state = HART_STATE_ADDR_SHORT;
        break;

      case 0x82: // от главного подчиненному длинный
        hart_cs = x;
        hart_addr_count = 0;
        hart_state = HART_STATE_ADDR_LONG;
        break;
    }
  }
  else if(hart_state == HART_STATE_ADDR_SHORT)
  {
    hart_cs ^= x;

    if(hart_addr_count == 0) hart_addr[hart_addr_count] = x & 0x0F;
    else hart_addr[hart_addr_count] = x;

    con_str("{addr_short:");
    con_byte(hart_addr[hart_addr_count]);
    con_str("}");
    con_start();

    hart_addr_count++;
    if(hart_addr_count == 1)
    {
      if(hart_addr[0] == data_eeprom.flags.hart_id)
      {
        hart_cmd = 0;
        hart_state = HART_STATE_CMD;
      }
      else
      {
        con_str("{addr drop}");
        con_start();
        hart_state = HART_STATE_IDLE;
      }
    }
  }
  else if(hart_state == HART_STATE_ADDR_LONG)
  {
    hart_cs ^= x;

    if(hart_addr_count == 0) hart_addr[hart_addr_count] = x & 0x3F;
    else hart_addr[hart_addr_count] = x;
    hart_addr_count++;
    if(hart_addr_count == 5)
    {
      con_str("{addr_long:");
      con_byte(hart_addr[0]); con_push('-');
      con_byte(hart_addr[1]); con_push('-');
      con_byte(hart_addr[2]); con_push('-');
      con_byte(hart_addr[3]); con_push('-');
      con_byte(hart_addr[4]); con_push('}');
      con_start();

      if((hart_addr[0] == (HART_MANUF_ID & 0x3F))
        && (hart_addr[1] == HART_DEVICE_TYPE)
        && (hart_addr[2] == ((HART_ID >> 16) & 0xFF))
        && (hart_addr[3] == ((HART_ID >> 8) & 0xFF))
        && (hart_addr[4] == ((HART_ID >> 0) & 0xFF)))
      {
        hart_cmd = 0;
        hart_state = HART_STATE_CMD;
      }
      else
      {
        con_str("{addr drop}");
        con_start();
        hart_state = HART_STATE_IDLE;
      }
    }
  }
  else if(hart_state == HART_STATE_CMD)
  {
    hart_cs ^= x;
    hart_cmd = (hart_cmd << 8) + x;
    switch(hart_cmd)
    {
      case 0:
      case 2:
      case 3:
      case 6:
      case 12:
      case 13:
      case 14:
      case 15:
      case 17:
      case 18:
      case 34:
      case 35:
      case 40:
      case 42:
      case 43:
      case 45:
      case 46:
      case 48:
      case 171:
      case 172:
      case 173:
      case 174:
      case 175:
      case 176:
      case 177:
      	con_str("{cmd:");
      	con_dec(hart_cmd);
      	con_str("}");
      	con_start();
        hart_state = HART_STATE_DATA;
        break;

      case 254:
        hart_state = HART_STATE_CMD;
        break;

      default:
        con_str("{cmd drop ");
        con_dec(hart_cmd);
        con_str("}");
        con_start();

        hart_state = HART_STATE_IDLE;
        break;
    }

  }
  else if(hart_state == HART_STATE_DATA)
  {
    hart_cs ^= x;

    hart_data_size = x;
    hart_data_pos = 0;
  	con_str("{s:");
  	con_dec(x);
  	con_str("}");
  	con_start();
    hart_state = HART_STATE_DATAIN;
  }
  else if(hart_state == HART_STATE_DATAIN)
  {
    hart_cs ^= x;
    if(hart_data_pos < hart_data_size)
    {
    	hart_cmd_data[hart_data_pos] = x;
    	con_str("{d:");
    	con_byte(x);
    	con_str("}");
    	con_start();
    	hart_data_pos++;
    }
    else
    {
      con_str("{cs:");
      con_byte(hart_cs);
      con_str("}");
      con_start();

      if(hart_cs == 0)
      {
        // CS valid
        switch(hart_cmd)
        {
          case 0:
            ans0.ext = 254;
            ans0.code_manuf = HART_MANUF_ID;
            ans0.code_type = HART_DEVICE_TYPE;
            ans0.pre_count = HART_PRE_COUNT;
            ans0.ver_uni = 5;
            ans0.ver_spec = SW_VER_BUILD;
            ans0.ver_sw = SW_VER_MIN;
            ans0.ver_hw = SW_VER_MAJ;
            ans0.flags = 0;
            ans0.id[0] = (HART_ID >> 16) & 0xFF;
            ans0.id[1] = (HART_ID >> 8) & 0xFF;
            ans0.id[2] = (HART_ID >> 0) & 0xFF;
            hart_send((BYTE *)&ans0, sizeof(ans0));
            break;

          case 1:
            ans1.type = 57;	//HERE единицы измерения и PV
            ans1.pv = float_to_dword(fhmc_pos);
            hart_send((BYTE *)&ans1, sizeof(ans1));
            break;

          case 2:
            ans2.current = float_to_dword(ain_ma_s * 0.01f);
            ans2.percent = float_to_dword(fhmc_pos);
            hart_send((BYTE *)&ans2, sizeof(ans2));
            break;

          case 3:
            ans3.current = float_to_dword(ain_ma_s * 0.01f);
            ans3.type_1 = 0;
            ans3.var_1 = float_to_dword(fhmc_pos);
            //ans3.type_2 = 0;
            //ans3.var_2 = float_to_dword(ain_ma_s * 0.01f);
            ans3.type_2 = 32; //Градусы Цельсия
            ans3.var_2 = float_to_dword((float)tempeature);
            ans3.type_3 = 0;
            ans3.var_3 = float_to_dword(aout_ma * 0.1f);
            ans3.type_4 = 0;
            ans3.var_4 = float_to_dword((float)uptime);
            //ans3.var_4 = float_to_dword((float)tempeature);
            hart_send((BYTE *)&ans3, sizeof(ans3));
            break;

          case 6:
          	eeprom_flags.dw = data_eeprom.flags.dw;
          	eeprom_flags.hart_id = hart_cmd_data[0];
          	eeprom_unlock();
          	data_eeprom.flags.dw = eeprom_flags.dw;
          	eeprom_lock();
            hart_send((BYTE *)hart_cmd_data, sizeof(BYTE));
            break;

          //HERE HART cmd_11

          case 12:
          	i = 0;
          	while(i < 32)
          	{
          		if(event_flag_last & (1 << i))
          		{
          			event_flag_last &= ~(1 << i);
          			break;
          		}
          		else i++;
          	}
          	if(i == 32) i = 0;

          	mstr_cpy(str, "ST-");
          	mstr_add_int(str, i);
            mstr_add(str, " ");

          	memset(hart_message, 0xFF, sizeof(hart_message));
            hart_str(hart_message, str, sizeof(hart_message));

            hart_send(hart_message, sizeof(hart_message));
            break;

          case 13:
          	memcpy(ans13.tag, hart_tag, sizeof(hart_tag));
          	memcpy(ans13.desc, hart_desc, sizeof(hart_desc));
            i = (image.date >> 0) & 0xFF;
            if(i < 1) i = 1;
            if(i > 31) i = 31;
          	ans13.data[0] = i;
          	i = (image.date >> 8) & 0xFF;
            if(i < 1) i = 1;
            if(i > 12) i = 12;
            ans13.data[1] = i;
            i = (image.date >> 16) & 0xFFFF;
            if(i < 1900) i = 1900;
            if(i > 2099) i = 2099;
            ans13.data[2] = i - 1900;
            hart_send((BYTE *)&ans13, sizeof(ans13));
            break;

          case 14:
            ans14.serial[0] = (HART_SENSOR_ID >> 16) & 0xFF;
            ans14.serial[1] = (HART_SENSOR_ID >> 8) & 0xFF;
            ans14.serial[2] = (HART_SENSOR_ID >> 0) & 0xFF;
            ans14.units = 39;
            ans14.upper = float_to_dword(HART_SENSOR_UPPER);
            ans14.lower = float_to_dword(HART_SENSOR_LOWER);
            ans14.span = float_to_dword(HART_SENSOR_SPAN);
            hart_send((BYTE *)&ans14, sizeof(ans14));
            break;

          case 15:
            ans15.alarm = 10;
            ans15.trans = 0;
            ans15.units = 39;
            ans15.upper = float_to_dword(hart_current_upper);
            ans15.lower = float_to_dword(hart_current_lower);
            ans15.damping = float_to_dword(hart_demph);
            ans15.write_protect = 0;
            ans15.priv_code = 0;
            hart_send((BYTE *)&ans15, sizeof(ans15));
            break;

          case 17:
          	memcpy(hart_message, (void *)hart_cmd_data, sizeof(hart_message));
            hart_send((BYTE *)hart_cmd_data, hart_data_size);
          	break;

          case 18:
          	memcpy(hart_tag, (void *)&hart_cmd_data[0], sizeof(hart_tag));
          	memcpy(hart_desc, (void *)&hart_cmd_data[6], sizeof(hart_desc));
            hart_send((BYTE *)hart_cmd_data, hart_data_size);
          	break;

          case 34:
          	hart_demph = dword_to_float(*(DWORD *)hart_cmd_data);
          	con_str("{demph:");
          	con_float(hart_demph);
          	con_str("}");
          	con_start();
            hart_send((BYTE *)hart_cmd_data, sizeof(float));
          	break;

          case 35:
          	con_str("{units:");
          	con_byte(hart_cmd_data[0]);
          	hart_current_upper = dword_to_float(*(DWORD *)&hart_cmd_data[1]);
          	con_str(",up:");
          	con_float(hart_current_upper);
          	hart_current_lower = dword_to_float(*(DWORD *)&hart_cmd_data[5]);
          	con_str(",low:");
          	con_float(hart_current_lower);
          	con_str("}");
          	con_start();
            hart_send((BYTE *)hart_cmd_data, sizeof(float) * 2 + sizeof(BYTE));
          	break;

          case 40:
          	hart_ext_current = dword_to_float(*(DWORD *)hart_cmd_data);
          	con_str("{ext:");
          	con_float(hart_ext_current);
          	con_str("}");
          	con_start();
            hart_send((BYTE *)hart_cmd_data, sizeof(float));
          	break;

          case 42:
          	hart_need_reboot = 1;
            hart_send(0, 0);
            break;

          case 43:
            hart_send(0, 0);
            break;

          case 45:
          	f = dword_to_float(*(DWORD *)hart_cmd_data);
          	con_str("{zero:");
          	con_float(f);
          	con_str("}");
          	con_start();
            hart_send((BYTE *)hart_cmd_data, sizeof(float));
          	break;

          case 46:
          	f = dword_to_float(*(DWORD *)hart_cmd_data);
          	con_str("{amp:");
          	con_float(f);
          	con_str("}");
          	con_start();
            hart_send((BYTE *)hart_cmd_data, sizeof(float));
          	break;

          case 48:
            ans48.fix_L = 'L';
            ans48.datetime = data_eeprom.events[hart_log_pos].time;
            ans48.pos = hart_log_pos;
						ans48.num = event_num;
            ans48.zero = 0;
            mstr_cpy(str, "ST-");
            mstr_add_int(str, data_eeprom.events[hart_log_pos].event);

          	memset(ans48.message, 0xFF, sizeof(ans48.message));
            hart_str(ans48.message, str, sizeof(ans48.message));

            hart_log_pos++;
            if(hart_log_pos >= event_num) hart_log_pos = 0;

            hart_send((BYTE *)&ans48, sizeof(ans48));
          	break;

          case 171:
          	hart_act_count = (*(BYTE *)hart_cmd_data) & 63;
          	hart_act_data_type = ((*(BYTE *)hart_cmd_data) >> 6) & 3;
          	con_str("{act_cnt:");
          	con_dec(hart_act_count);
          	con_str("@");
          	con_dec(hart_act_data_type);
          	con_str("}");
          	con_start();
            hart_send((BYTE *)hart_cmd_data, sizeof(BYTE));
          	break;

          case 172:
          	str[0] = ((BYTE *)hart_cmd_data)[0];
          	str[1] = ((BYTE *)hart_cmd_data)[1];
          	str[2] = ((BYTE *)hart_cmd_data)[2];
          	str[3] = ((BYTE *)hart_cmd_data)[3];
          	if((1 <= str[0]) && (str[0] <= 20)
          		&& (3 <= str[1]) && (str[1] <= 30)
          		&& (0 <= str[2]) && (str[2] <= 100)
          		&& (0 <= str[3]) && (str[3] <= 100))
          	{
							step_test_min = str[2] * 10;
							step_test_max = str[3] * 10;
							step_test_c = 0;
							step_test_n = str[0];
							step_test_to = 0;
							step_test_t = str[1] * FADC;

							hart_act_data_type = HART_ACT_DATA_POS_TGT;
							hart_act_count = (str[0] + 1) * str[1] + 1;
							hart_act_data_pos = 0;

	          	con_str("{step_test:");
	          	con_dec(str[0]);
	          	con_str("@");
	          	con_dec(str[1]);
	          	con_str("}");
	          	con_start();
          	}
            hart_send((BYTE *)hart_cmd_data, sizeof(BYTE));
          	break;

          case 173:
          	str[0] = ((signed char *)hart_cmd_data)[0];
          	str[1] = ((BYTE *)hart_cmd_data)[1];
          	str[2] = ((BYTE *)hart_cmd_data)[1];
          	if((((4 <= str[0]) && (str[0] <= 15)) || ((-15 <= (signed char)str[0]) && ((signed char)str[0] <= -4)))
          		&& (2 <= str[1]) && (str[1] <= 60) && (2 <= str[2]) && (str[2] <= 60))
          	{
          		pst_to = 0;
          		pst_te = str[2] * FADC;
          		pst_last = ihmc_pos_t;
          		pst_cur = pst_last + (signed char)str[0] * 10;
          		pst_result = 0;
          		pst_t = str[1] * FADC;

							hart_act_data_type = HART_ACT_DATA_POS_TGT;
							hart_act_count = str[1] * 2 + 5;
							hart_act_data_pos = 0;

	          	con_str("{pst:");
	          	con_dec((signed char)str[0]);
	          	con_str("@");
	          	con_dec(str[1]);
	          	con_str("@");
	          	con_dec(str[2]);
	          	con_str("}");
	          	con_start();
          	}
            hart_send((BYTE *)hart_cmd_data, sizeof(BYTE));
          	break;

          case 174:
          	((sHART_ANS_CMD174 *)hart_cmd_data)->kp = data_eeprom.kp;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->ki = data_eeprom.ki;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->kd = data_eeprom.kd;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->lp_up = data_eeprom.t_lp_up;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->lp_down = data_eeprom.t_lp_down;
            hart_send((BYTE *)hart_cmd_data, sizeof(sHART_ANS_CMD174));
            break;

          case 175:
          	if(((sHART_CMD175 *)hart_cmd_data)->f_kp)
          	{
            	eeprom_unlock();
          		data_eeprom.kp = ((sHART_CMD175 *)hart_cmd_data)->kp;
            	eeprom_lock();

            	con_str("{kp=");
            	con_float(((sHART_CMD175 *)hart_cmd_data)->kp);
            	con_str("}");
            	con_start();
          	}

          	if(((sHART_CMD175 *)hart_cmd_data)->f_ki)
          	{
            	eeprom_unlock();
          		data_eeprom.ki = ((sHART_CMD175 *)hart_cmd_data)->ki;
            	eeprom_lock();

            	con_str("{ki=");
            	con_float(((sHART_CMD175 *)hart_cmd_data)->ki);
            	con_str("}");
            	con_start();
          	}

          	if(((sHART_CMD175 *)hart_cmd_data)->f_kd)
          	{
            	eeprom_unlock();
          		data_eeprom.kd = ((sHART_CMD175 *)hart_cmd_data)->kd;
            	eeprom_lock();

            	con_str("{kd=");
            	con_float(((sHART_CMD175 *)hart_cmd_data)->kd);
            	con_str("}");
            	con_start();
          	}

          	if(((sHART_CMD175 *)hart_cmd_data)->f_lp_up)
          	{
            	eeprom_unlock();
          		data_eeprom.t_lp_up = ((sHART_CMD175 *)hart_cmd_data)->lp_up;
            	eeprom_lock();

            	con_str("{lp_up=");
            	con_float(((sHART_CMD175 *)hart_cmd_data)->lp_up);
            	con_str("}");
            	con_start();
          	}

          	if(((sHART_CMD175 *)hart_cmd_data)->f_lp_down)
          	{
            	eeprom_unlock();
          		data_eeprom.t_lp_down = ((sHART_CMD175 *)hart_cmd_data)->lp_down;
            	eeprom_lock();

            	con_str("{lp_dn=");
            	con_float(((sHART_CMD175 *)hart_cmd_data)->lp_down);
            	con_str("}");
            	con_start();
          	}

          	hart_cmd = 174;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->kp = data_eeprom.kp;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->ki = data_eeprom.ki;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->kd = data_eeprom.kd;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->lp_up = data_eeprom.t_lp_up;
          	((sHART_ANS_CMD174 *)hart_cmd_data)->lp_down = data_eeprom.t_lp_down;
            hart_send((BYTE *)hart_cmd_data, sizeof(sHART_ANS_CMD174));
          	break;

          case 176:
                    	flag_factory_reset = 1;
                      hart_send(0, 0);
                      break;
          case 177:
          	if(st_auto == ST_AUTO_IDLE)
          		{
          			if (((BYTE *)hart_cmd_data)[0]) auto_type = TYPE_REVERSE;
          		     else auto_type = TYPE_DIRECT;
          		  to_auto = 0;
          		  st_auto = ST_AUTO_START;
          		  hart_autoset_flag = 1;
          			con_str("[HART autoset]");
          			con_start();
          		  hart_cmd_data[0] = 0x01;
          		  hart_send((BYTE *)hart_cmd_data, 1); // Началась автонастройка
          		} else if (st_auto != ST_AUTO_TIME_DONE)
          			{
          		  	hart_cmd_data[0] = 0x02;
          		  	hart_send((BYTE *)hart_cmd_data, 1); // Автонастройка прямо сейчас идёт
          			}
          		else  {
          				to_auto = 0;
          				st_auto = ST_AUTO_IDLE;
          				hart_autoset_done_flag = 1;
         					con_str("[HART autoset done]\n\r");
         					con_start();
            		  hart_cmd_data[0] = 0x03;
            		  hart_send((BYTE *)hart_cmd_data, 1); // Автонастройка закончена
          			}
            break;
        }
      }
      hart_state = HART_STATE_IDLE;
    }
  }

  if(x != 0xFF) hart_ff_count = 0;

}

//-----------------------------------------------------------------------------
//	int hart_on_tx(BYTE *x)
//-----------------------------------------------------------------------------
int hart_on_tx(BYTE *x)
{
	if(hart_tx_pos < hart_tx_size)
	{
		*x = hart_tx_buf[hart_tx_pos];
		hart_tx_pos++;

		#if 0
		con_push('<');
		con_byte(*x);
		con_push('>');
		con_start();
		#endif

		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//	void hart_start_tx(const int size)
//-----------------------------------------------------------------------------
void hart_start_tx(const int size)
{
	hart_tx_size = size;
	hart_tx_pos = 0;

	hart_tx_enable();
}

//-----------------------------------------------------------------------------
//	void hart_timer(void)
//-----------------------------------------------------------------------------
void hart_timer(void)
{
	static tTIMER to_hart_reboot = 0;

	if(to_hart < TO_HART) tick(&to_hart);

	if(hart_need_reboot == 1)
	{
		tick(&to_hart_reboot);
		if(to_hart_reboot > 1000UL)
		{
			NVIC_SystemReset();
		}
	}
	else to_hart_reboot = 0;
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
