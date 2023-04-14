/**
 * @file pcf8531.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Драйвер дисплея PCF8531.
 * @version 0.1
 * @date 20.05.2016
 *
 * @copyright (C) Ярославль, 2016
 *
 */
#include "pcf8531.h"
#include "font.h"
#include <string.h>

BYTE pcf8531_video[PCF8531_SIZEX][5];

//-----------------------------------------------------------------------------
//	void pcf8531_delay(void)
//-----------------------------------------------------------------------------
void pcf8531_delay(void)
{
	volatile int i;
	for (i = 0; i < 1; i++)
		;
}

//-----------------------------------------------------------------------------
//	void pcf8531_write(const BYTE x)
//-----------------------------------------------------------------------------
void pcf8531_write(const BYTE x)
{
	int i;
	DISP_PORT->MODER =
		(DISP_PORT->MODER & ~((3 << (DISP_SDA * 2)) | (3 << (DISP_SCL * 2)))) | (GPIO_MODE_OUTPUT << (DISP_SDA * 2)) | (GPIO_MODE_OUTPUT << (DISP_SCL * 2));
#ifdef PCF8531_DEBUG
	con_str("[WR=");
	con_byte(x);
	con_str("]");
	con_start();
#endif // PCF8531_DEBUG
	for (i = 0; i < 8; i++)
	{
		if ((x >> (7 - i)) & 1)
			DISP_SDA_1;
		else
			DISP_SDA_0;
		pcf8531_delay();
		DISP_SCL_1;
		pcf8531_delay();
		DISP_SCL_0;
	}
	DISP_PORT->MODER =
		(DISP_PORT->MODER & ~((3 << (DISP_SDA * 2)) | (3 << (DISP_SCL * 2)))) | (GPIO_MODE_INPUT << (DISP_SDA * 2)) | (GPIO_MODE_OUTPUT << (DISP_SCL * 2));
	pcf8531_delay();
	DISP_SCL_1;
	pcf8531_delay();
#ifdef PCF8531_DEBUG
	if (DISP_SDA_d)
		con_str("[!A]");
	else
		con_str("[A]");
	con_start();
#endif // PCF8531_DEBUG
	DISP_SCL_0;
	pcf8531_delay();
}

//-----------------------------------------------------------------------------
//	BYTE pcf8531_read(const BYTE ack)
//-----------------------------------------------------------------------------
BYTE pcf8531_read(const BYTE ack)
{
	int i;
	BYTE x = 0;
	DISP_PORT->MODER =
		(DISP_PORT->MODER & ~((3 << (DISP_SDA * 2)) | (3 << (DISP_SCL * 2)))) | (GPIO_MODE_INPUT << (DISP_SDA * 2)) | (GPIO_MODE_OUTPUT << (DISP_SCL * 2));
	DISP_SDA_1;
	for (i = 0; i < 8; i++)
	{
		pcf8531_delay();
		x |= DISP_SDA_d << (7 - i);
		DISP_SCL_1;
		pcf8531_delay();
		DISP_SCL_0;
	}
	DISP_PORT->MODER =
		(DISP_PORT->MODER & ~((3 << (DISP_SDA * 2)) | (3 << (DISP_SCL * 2)))) | (GPIO_MODE_OUTPUT << (DISP_SDA * 2)) | (GPIO_MODE_OUTPUT << (DISP_SCL * 2));
#ifdef PCF8531_DEBUG
	con_str("[RD=");
	con_byte(x);
	con_str("]");
	con_start();
#endif // PCF8531_DEBUG
	pcf8531_delay();
	if (ack)
	{
#ifdef PCF8531_DEBUG
		con_str("[!A]");
		con_start();
#endif // PCF8531_DEBUG
		DISP_SDA_1;
	}
	else
	{
#ifdef PCF8531_DEBUG
		con_str("[A]");
		con_start();
#endif // PCF8531_DEBUG
		DISP_SDA_0;
	}
	pcf8531_delay();
	DISP_SCL_1;
	pcf8531_delay();
	DISP_SCL_0;
	pcf8531_delay();
	return x;
}

//-----------------------------------------------------------------------------
//	void pcf8531_start(void)
//-----------------------------------------------------------------------------
void pcf8531_start(void)
{
	DISP_PORT->MODER =
		(DISP_PORT->MODER & ~((3 << (DISP_SDA * 2)) | (3 << (DISP_SCL * 2)))) | (GPIO_MODE_OUTPUT << (DISP_SDA * 2)) | (GPIO_MODE_OUTPUT << (DISP_SCL * 2));
	// start
	pcf8531_delay();
	DISP_SDA_1;
	pcf8531_delay();
	DISP_SCL_1;
	pcf8531_delay();
	DISP_SDA_0;
	pcf8531_delay();
	DISP_SCL_0;
	pcf8531_delay();
#ifdef PCF8531_DEBUG
	con_str("[START]");
	con_start();
#endif // PCF8531_DEBUG
}

//-----------------------------------------------------------------------------
//	void pcf8531_stop(void)
//-----------------------------------------------------------------------------
void pcf8531_stop(void)
{
	DISP_PORT->MODER =
		(DISP_PORT->MODER & ~((3 << (DISP_SDA * 2)) | (3 << (DISP_SCL * 2)))) | (GPIO_MODE_OUTPUT << (DISP_SDA * 2)) | (GPIO_MODE_OUTPUT << (DISP_SCL * 2));
	// stop
	pcf8531_delay();
	DISP_SDA_0;
	pcf8531_delay();
	DISP_SCL_1;
	pcf8531_delay();
	DISP_SDA_1;
#ifdef PCF8531_DEBUG
	con_str("[STOP]\n\r");
	con_start();
#endif // PCF8531_DEBUG
	pcf8531_delay();
	pcf8531_delay();
}

//-----------------------------------------------------------------------------
//	void pcf8531_restart(void)
//-----------------------------------------------------------------------------
void pcf8531_restart(void)
{
	volatile int delay;
	DISP_RST_0;
	for (delay = 0; delay < 10000; delay++)
		;
	DISP_RST_1;
	for (delay = 0; delay < 10000; delay++)
		;
}

//-----------------------------------------------------------------------------
//	void pcf8531_init(void)
//-----------------------------------------------------------------------------
void pcf8531_init(void)
{
	int i;

	PCF8531_START;
	PCF8531_WRITE_CONTROL(PCF8531_PA_SET_PAGE_0);
	PCF8531_WRITE_CONTROL(PCF8531_P0_PD_V(0, 1));

	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_PAGE(1));
	PCF8531_WRITE_CONTROL(PCF8531_P1_DISP(PCF8531_DISP_NORMAL));
	PCF8531_WRITE_CONTROL(PCF8531_P1_MUX(PCF8531_MUX_34));
	PCF8531_WRITE_CONTROL(PCF8531_P1_BIAS(PCF8531_BIAS_7));

	PCF8531_WRITE_CONTROL(PCF8531_PA_SET_PAGE_0);
	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_PAGE(2));
	PCF8531_WRITE_CONTROL(PCF8531_P2_MULT(PCF8531_MULT_2X));
	PCF8531_WRITE_CONTROL(PCF8531_P2_TC(0));
	PCF8531_WRITE_CONTROL(PCF8531_P2_HV_LOW);
	// PCF8531_WRITE_CONTROL(PCF8531_P2_VOP(88));	// (Vlcd - a) / b = (5.2V - 2.94V) / 0.03V = 75

	PCF8531_WRITE_CONTROL(PCF8531_PA_SET_PAGE_0);
	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_Y(0));
	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_X(0));

	PCF8531_WRITE_DATA;

	for (i = 0; i < 128 * 5; i++)
		pcf8531_write(pcf8531_video[i / 5][i % 5]);

	pcf8531_stop();
}

//-----------------------------------------------------------------------------
//	void pcf8531_init_after(void)
//-----------------------------------------------------------------------------
void pcf8531_init_after(void)
{
	int i;

	PCF8531_START;
	PCF8531_WRITE_CONTROL(PCF8531_PA_SET_PAGE_0);
	PCF8531_WRITE_CONTROL(PCF8531_P0_PD_V(0, 1));

	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_PAGE(1));
	PCF8531_WRITE_CONTROL(PCF8531_P1_DISP(PCF8531_DISP_NORMAL));
	PCF8531_WRITE_CONTROL(PCF8531_P1_MUX(PCF8531_MUX_34));
	PCF8531_WRITE_CONTROL(PCF8531_P1_BIAS(PCF8531_BIAS_7));

	PCF8531_WRITE_CONTROL(PCF8531_PA_SET_PAGE_0);
	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_PAGE(2));
	PCF8531_WRITE_CONTROL(PCF8531_P2_MULT(PCF8531_MULT_2X));
	PCF8531_WRITE_CONTROL(PCF8531_P2_TC(0));
	PCF8531_WRITE_CONTROL(PCF8531_P2_HV_LOW);

	pcf8531_stop();
}

//-----------------------------------------------------------------------------
//	void pcf8531_redraw(void)
//-----------------------------------------------------------------------------
void pcf8531_redraw(const int page)
{
	int i;

	BYTE *video;

	PCF8531_START;
	PCF8531_WRITE_CONTROL(PCF8531_PA_SET_PAGE_0);
	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_Y(0));
	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_X(page * PCF8531_PAGE_SIZE));
	PCF8531_WRITE_DATA;

	if (data_eeprom.flags.display_rotate == DISPLAY_ROTATE_ON)
	{
#ifdef AM800W
		video = (BYTE *)&pcf8531_video[page * PCF8531_PAGE_SIZE][0];
		for (i = 0; i < PCF8531_PAGE_SIZE * PCF8531_SIZEY; i++)
			pcf8531_write(video[i]);
#else
		// flipped
		video = (BYTE *)&pcf8531_video[(PCF8531_PAGE_NUM - 1 - page) * PCF8531_PAGE_SIZE][4];
		for (i = 0; i < PCF8531_PAGE_SIZE * PCF8531_SIZEY; i++)
			pcf8531_write(rbit8(video[PCF8531_PAGE_SIZE * PCF8531_SIZEY - 1 - i]));
#endif
	}
	else
	{
#ifdef AM800W
		video = (BYTE *)&pcf8531_video[(PCF8531_PAGE_NUM - 1 - page) * PCF8531_PAGE_SIZE][4];
		for (i = 0; i < PCF8531_PAGE_SIZE * PCF8531_SIZEY; i++)
			pcf8531_write(rbit8(video[PCF8531_PAGE_SIZE * PCF8531_SIZEY - 1 - i]));
#else
		// normal
		video = (BYTE *)&pcf8531_video[page * PCF8531_PAGE_SIZE][0];
		for (i = 0; i < PCF8531_PAGE_SIZE * PCF8531_SIZEY; i++)
			pcf8531_write(video[i]);
#endif
	}

	pcf8531_stop();
}

//-----------------------------------------------------------------------------
//	void pcf8531_contrast(const int contrast)
//-----------------------------------------------------------------------------
void pcf8531_contrast(const int contrast)
{
	int value;

	value = contrast - 15;
	if (value < -10)
		value = 0;
	if (value > 10)
		value = 0;

	PCF8531_START;
	PCF8531_WRITE_CONTROL(PCF8531_PA_SET_PAGE_0);
	PCF8531_WRITE_CONTROL(PCF8531_P0_SET_PAGE(2));
	PCF8531_WRITE_CONTROL(PCF8531_P2_MULT(PCF8531_MULT_2X));
	PCF8531_WRITE_CONTROL(PCF8531_P2_TC(0));
	PCF8531_WRITE_CONTROL(PCF8531_P2_HV_LOW);
	PCF8531_WRITE_CONTROL(PCF8531_P2_VOP(107 + value * 2)); // (Vlcd - a) / b = (5.2V - 2.94V) / 0.03V = 75
	pcf8531_stop();
}

//-----------------------------------------------------------------------------
//	void pcf8531_str(const int x, const int y, char *s, const BYTE *font)
//-----------------------------------------------------------------------------
void pcf8531_str(const int x, const int y, char *s, const BYTE *font)
{
	int i, j;
	// const BYTE	*font;
	int font_data;
	int index, length, height;
	int lcd_pos_x, lcd_pos_y;
	DWORD font_temp;

	DWORD *video_bit;

	lcd_pos_x = x;
	lcd_pos_y = y;

	video_bit = (DWORD *)&RAM_BIT_ADDR(&pcf8531_video[lcd_pos_x][lcd_pos_y >> 3], lcd_pos_y & 7);

	// font = Font_Single;
	font_data = 16 + (((sFONT *)font)->size) * 4;
	height = ((sFONT *)font)->height;
	while (*s)
	{
		index = ((sFONT *)font)->item[*s].position + font_data;
		length = ((sFONT *)font)->item[*s].width;
		for (i = 0; i < length; i++)
			if (lcd_pos_x < 128)
			{
				if (height <= 8)
					font_temp = 0 | (font[index + i * 1 + 0] << 0);
				else if (height <= 16)
					font_temp = 0 | (font[index + i * 2 + 0] << 0) | (font[index + i * 2 + 1] << 8);
				else if (height <= 24)
					font_temp = 0 | (font[index + i * 3 + 0] << 0) | (font[index + i * 3 + 1] << 8) | (font[index + i * 3 + 2] << 16);

				for (j = 0; j < height; j++)
					video_bit[j] = (font_temp >> (height - 1 - j)) & 1;
				video_bit += PCF8531_SIZEY * 8;
				lcd_pos_x++;
			}
		if (!((sFONT *)font)->item[*s].flag)
			if (lcd_pos_x < 128)
			{
				for (j = 0; j < height; j++)
					video_bit[j] = 0;
				video_bit += PCF8531_SIZEY * 8;
				lcd_pos_x++;
			}
		s++;
	}
}

//-----------------------------------------------------------------------------
//	void pcf8531_str_inverse(const int x, const int y, char *s, const BYTE *font)
//-----------------------------------------------------------------------------
void pcf8531_str_inverse(const int x, const int y, char *s, const BYTE *font)
{
	int i, j;
	int font_data;
	int index, length, height;
	int lcd_pos_x = x;
	int lcd_pos_y = y;

	DWORD font_temp;
	DWORD *video_bit;

	video_bit = (DWORD *)&RAM_BIT_ADDR(&pcf8531_video[lcd_pos_x][lcd_pos_y >> 3], lcd_pos_y & 7);
	font_data = 16 + (((sFONT *)font)->size) * 4;
	height = ((sFONT *)font)->height;

	while (*s)
	{
		index = ((sFONT *)font)->item[*s].position + font_data;
		length = ((sFONT *)font)->item[*s].width;
		for (i = 0; i < length; i++)
			if (lcd_pos_x < 128)
			{
				if (height <= 8)
					font_temp = 0 | (font[index + i * 1 + 0] << 0);
				else if (height <= 16)
					font_temp = 0 | (font[index + i * 2 + 0] << 0) | (font[index + i * 2 + 1] << 8);
				else if (height <= 24)
					font_temp = 0 | (font[index + i * 3 + 0] << 0) | (font[index + i * 3 + 1] << 8) | (font[index + i * 3 + 2] << 16);

				for (j = 0; j < height; j++)
					video_bit[j] = ((font_temp >> (height - 1 - j)) & 1) ^ 1;

				video_bit += PCF8531_SIZEY * 8;
				lcd_pos_x++;
			}

		if (!((sFONT *)font)->item[*s].flag)
			if (lcd_pos_x < 128)
			{
				for (j = 0; j < height; j++)
					video_bit[j] = 0 ^ 1;

				video_bit += PCF8531_SIZEY * 8;
				lcd_pos_x++;
			}
		s++;
	}
}

//-----------------------------------------------------------------------------
//	void pcf8531_str_to(const int x, const int y, char *s, const int p, const BYTE *font)
//-----------------------------------------------------------------------------
void pcf8531_str_to(const int x, const int y, char *s, const int p, const BYTE *font)
{
	int i, j;
	// const BYTE	*font;
	int font_data;
	int index, length, height;
	int lcd_pos_x, lcd_pos_y;
	DWORD font_temp;

	DWORD *video_bit;

	lcd_pos_x = x;
	lcd_pos_y = y;

	video_bit = (DWORD *)&RAM_BIT_ADDR(&pcf8531_video[lcd_pos_x][lcd_pos_y >> 3], lcd_pos_y & 7);

	// font = Font_Single;
	font_data = 16 + (((sFONT *)font)->size) * 4;
	height = ((sFONT *)font)->height;
	while (*s)
	{
		index = ((sFONT *)font)->item[*s].position + font_data;
		length = ((sFONT *)font)->item[*s].width;
		for (i = 0; i < length; i++)
			if (lcd_pos_x < 128)
			{
				if (height <= 8)
					font_temp = 0 | (font[index + i * 1 + 0] << 0);
				else if (height <= 16)
					font_temp = 0 | (font[index + i * 2 + 0] << 0) | (font[index + i * 2 + 1] << 8);
				else if (height <= 24)
					font_temp = 0 | (font[index + i * 3 + 0] << 0) | (font[index + i * 3 + 1] << 8) | (font[index + i * 3 + 2] << 16);

				for (j = 0; j < height; j++)
					video_bit[j] = (font_temp >> (height - 1 - j)) & 1;
				video_bit += PCF8531_SIZEY * 8;
				lcd_pos_x++;
			}
		if (!((sFONT *)font)->item[*s].flag)
			if (lcd_pos_x < 128)
			{
				for (j = 0; j < height; j++)
					video_bit[j] = 0;
				video_bit += PCF8531_SIZEY * 8;
				lcd_pos_x++;
			}
		s++;
	}
	while (lcd_pos_x < p)
	{
		for (j = 0; j < height; j++)
			video_bit[j] = 0;
		video_bit += PCF8531_SIZEY * 8;
		lcd_pos_x++;
	}
}

//-----------------------------------------------------------------------------
//	int pcf8531_str_len(char *s, const BYTE *font)
//-----------------------------------------------------------------------------
int pcf8531_str_len(char *s, const BYTE *font)
{
	int result = 0;
	int flag = 0;

	while (*s)
	{
		result += ((sFONT *)font)->item[*s].width;
		if (!((sFONT *)font)->item[*s].flag)
		{
			result += 1;
			flag = 1;
		}
		else
			flag = 0;
		s++;
	}

	if (flag)
		return result;
	else
		return result - 1;
}

//-----------------------------------------------------------------------------
//	void pcf8531_pic(const int x, const int y, const BYTE *pic, const int sx, const int sy)
//-----------------------------------------------------------------------------
void pcf8531_pic(const int x, const int y, const BYTE *pic, const int sx, const int sy)
{
	DWORD *video_bit;
	int bn = ((sy - 1) / 8) + 1;
	int i, j;

	video_bit = (DWORD *)&RAM_BIT_ADDR(&pcf8531_video[x][y >> 3], y & 7);

	for (i = 0; i < sx; i++)
	{
		for (j = 0; j < sy; j++)
		{
			video_bit[j] = (pic[i * bn + (j >> 3)] >> (j & 7)) & 1;
		}
		video_bit += PCF8531_SIZEY * 8;
	}
}

//-----------------------------------------------------------------------------
//	void pcf8531_pic_invert(const int x, const int y, const BYTE *pic, const int sx, const int sy)
//-----------------------------------------------------------------------------
void pcf8531_pic_invert(const int x, const int y, const BYTE *pic, const int sx, const int sy)
{
	DWORD *video_bit;
	int bn = ((sy - 1) / 8) + 1;
	int i, j;

	video_bit = (DWORD *)&RAM_BIT_ADDR(&pcf8531_video[x][y >> 3], y & 7);

	for (i = 0; i < sx; i++)
	{
		for (j = 0; j < sy; j++)
		{
			video_bit[j] = ((pic[i * bn + (j >> 3)] >> (j & 7)) & 1) ^ 1;
		}
		video_bit += PCF8531_SIZEY * 8;
	}
}

//-----------------------------------------------------------------------------
//	void pcf8531_clear(const int x, const int y, const int sx, const int sy)
//-----------------------------------------------------------------------------
void pcf8531_clear(const int x, const int y, const int sx, const int sy)
{
	DWORD *video_bit;
	int bn = ((sy - 1) / 8) + 1;
	int i, j;

	video_bit = (DWORD *)&RAM_BIT_ADDR(&pcf8531_video[x][y >> 3], y & 7);

	for (i = 0; i < sx; i++)
	{
		for (j = 0; j < sy; j++)
		{
			video_bit[j] = 0;
		}
		video_bit += PCF8531_SIZEY * 8;
	}
}

void pcf8531_FullClear()
{
	pcf8531_clear(0, 0, 128, 32);
}

//-----------------------------------------------------------------------------
//	void pcf8531_fill(const int x, const int y, const int sx, const int sy)
//-----------------------------------------------------------------------------
void pcf8531_fill(const int x, const int y, const int sx, const int sy)
{
	DWORD *video_bit;
	int bn = ((sy - 1) / 8) + 1;
	int i, j;

	video_bit = (DWORD *)&RAM_BIT_ADDR(&pcf8531_video[x][y >> 3], y & 7);

	for (i = 0; i < sx; i++)
	{
		for (j = 0; j < sy; j++)
		{
			video_bit[j] = 1;
		}
		video_bit += PCF8531_SIZEY * 8;
	}
}

//-----------------------------------------------------------------------------
//	void pcf8531_pica(const int x, const int y, const BYTE *pic, const int sx, const int sy)
//-----------------------------------------------------------------------------
void pcf8531_pica(const int x, const int y, const BYTE *pic, const int sx, const int sy)
{
	DWORD *video_bit;
	int bn = ((sy - 1) / 8) + 1;
	int i, j;

	video_bit = (DWORD *)&RAM_BIT_ADDR(&pcf8531_video[x][y >> 3], y & 7);

	for (i = 0; i < sx; i++)
	{
		for (j = 0; j < sy; j++)
		{
			video_bit[j] |= (pic[i * bn + (j >> 3)] >> (j & 7)) & 1;
		}
		video_bit += PCF8531_SIZEY * 8;
	}
}

//-----------------------------------------------------------------------------
//	void pcf8531_pixel(const int x, const int y)
//-----------------------------------------------------------------------------
void pcf8531_pixel(const int x, const int y)
{
	RAM_BIT_ADDR(&pcf8531_video[x][y >> 3], y & 7) = 1;
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
