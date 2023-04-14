/**
 * @file serial_port.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Драйвер последовательного порта.
 * @version 0.1
 * @date 20.05.2016
 * 
 * @copyright (C) Ярославль, 2016
 * 
 */
#include "serial_port.h"
#include <math.h>

//-----------------------------------------------------------------------------
//	const BYTE Hex[]
//-----------------------------------------------------------------------------
const BYTE Hex[] = "0123456789ABCDEF";

//-----------------------------------------------------------------------------
//	void sp_init(sSERIAL_PORT *port,
//		BYTE *tx_buf, int tx_size,
//		BYTE *rx_buf, int rx_size,
//		usart_cr1_type *usart_cr1)
//-----------------------------------------------------------------------------
void sp_init(sSERIAL_PORT *port,
			 BYTE *tx_buf, int tx_size,
			 BYTE *rx_buf, int rx_size,
			 usart_cr1_type *usart_cr1)
{
	port->tx_t = 0;
	port->tx_b = 0;
	port->tx_buf = tx_buf;
	port->tx_size = tx_size;
	port->rx_t = 0;
	port->rx_b = 0;
	port->rx_tm = 0;
	port->rx_buf = rx_buf;
	port->rx_size = rx_size;
	port->usart_cr1 = usart_cr1;
	port->pin = 0;
}

//-----------------------------------------------------------------------------
//	void sp_tx_push(sSERIAL_PORT *port, BYTE x)
//-----------------------------------------------------------------------------
void sp_tx_push(sSERIAL_PORT *port, BYTE x)
{
	int tmp_t;
	port->tx_buf[port->tx_t] = x;
	tmp_t = port->tx_t + 1;
	if (tmp_t >= port->tx_size)
		tmp_t -= port->tx_size;
	if (tmp_t != port->tx_b)
		port->tx_t = tmp_t;
}

//-----------------------------------------------------------------------------
//	BYTE sp_tx_pop(sSERIAL_PORT *port)
//-----------------------------------------------------------------------------
BYTE sp_tx_pop(sSERIAL_PORT *port)
{
	BYTE x;
	if (port->tx_b == port->tx_t)
		return 0;
	x = port->tx_buf[port->tx_b];
	port->tx_buf[port->tx_b++] = 0;
	if (port->tx_b >= port->tx_size)
		port->tx_b -= port->tx_size;
	return x;
}

//-----------------------------------------------------------------------------
//	void sp_str(sSERIAL_PORT *port, BYTE *str)
//-----------------------------------------------------------------------------
void sp_str(sSERIAL_PORT *port, BYTE *str)
{
	while (*str)
		sp_tx_push(port, *str++);
}

//-----------------------------------------------------------------------------
//	void sp_start(sSERIAL_PORT *port)
//-----------------------------------------------------------------------------
void sp_start(sSERIAL_PORT *port)
{
	if (port->usart_cr1)
		*port->usart_cr1 |= (1 << USART_CR1_TXEIE);
}

//-----------------------------------------------------------------------------
//	void sp_rx_push(sSERIAL_PORT *port, BYTE x)
//-----------------------------------------------------------------------------
void sp_rx_push(sSERIAL_PORT *port, BYTE x)
{
	int tmp_t;
	port->rx_buf[port->rx_t] = x;
	tmp_t = port->rx_t + 1;
	if (tmp_t >= port->rx_size)
		tmp_t -= port->rx_size;
	if (tmp_t != port->rx_b)
		port->rx_t = tmp_t;
}

//-----------------------------------------------------------------------------
//	BYTE sp_rx_pop(sSERIAL_PORT *port)
//-----------------------------------------------------------------------------
BYTE sp_rx_pop(sSERIAL_PORT *port)
{
	BYTE x;
	if (port->rx_b == port->rx_t)
		return 0;
	x = port->rx_buf[port->rx_b];
	port->rx_buf[port->rx_b++] = 0;
	if (port->rx_b >= port->rx_size)
		port->rx_b -= port->rx_size;
	return x;
}

//-----------------------------------------------------------------------------
//	void sp_hex(sSERIAL_PORT *port, const DWORD x, const int n)
//-----------------------------------------------------------------------------
void sp_hex(sSERIAL_PORT *port, const DWORD x, const int n)
{
	int i;
	for (i = n; i; --i)
		sp_tx_push(port, Hex[(x >> (4 * (i - 1))) & 15]);
}

//-----------------------------------------------------------------------------
//	void sp_dec(sSERIAL_PORT *port, DWORD x)
//-----------------------------------------------------------------------------
void sp_dec(sSERIAL_PORT *port, long x)
{
	DWORD h;
	DWORD t;
	BYTE b[10];
	BYTE c;

	if (x == 0)
		sp_tx_push(port, '0');
	else
	{
		if (x < 0)
		{
			sp_tx_push(port, '-');
			t = -x;
		}
		else
			t = x;
		h = 10;
		c = 0;
		while (t > 0)
		{
			b[c++] = t % h;
			t /= 10;
		}
		while (c > 0)
			sp_tx_push(port, b[--c] | '0');
	}
}

//-----------------------------------------------------------------------------
//	void sp_udec(sSERIAL_PORT *port, DWORD x)
//-----------------------------------------------------------------------------
void sp_udec(sSERIAL_PORT *port, DWORD x)
{
	DWORD h;
	DWORD t;
	BYTE b[10];
	BYTE c;

	if (x == 0)
		sp_tx_push(port, '0');
	else
	{
		t = x;
		h = 10;
		c = 0;
		while (t > 0)
		{
			b[c++] = t % h;
			t /= 10;
		}
		while (c > 0)
			sp_tx_push(port, b[--c] | '0');
	}
}

//-----------------------------------------------------------------------------
//	void sp_float(sSERIAL_PORT *port, const float f)
//-----------------------------------------------------------------------------
void sp_float(sSERIAL_PORT *port, const float f)
{
	float x = f;
	int i;
	if (x < 0.0f)
	{
		x = -x;
		sp_tx_push(port, '-');
	}
	sp_dec(port, truncf(x));
	sp_tx_push(port, '.');
	for (i = 0; i < 6; i++)
	{
		x = (x - trunc(x)) * 10.0f;
		sp_dec(port, truncf(x));
	}
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
