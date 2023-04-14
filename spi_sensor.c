/**
 * @file spi_sensor.h
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @author �������� ������ ������� (victs76@mail.ru)
 * @brief ������ ��������� �� SPI
 * @version 1.1
 * @date 22.02.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "spi_sensor.h"

#define SPI_PWR 9
#define SNSS 15
#define SSCK 3
#define SMISO 4
#define SMOSI 5

BYTE vbByteCount = 0;					// ������� ���������� �����

volatile BYTE vbSpiPwrOn = 0;			// ���� ��������� ������� �������� ����� ������ �������
volatile BYTE vbSpiPwrRelease = 0;		// ���� ���������� ������������� ���������� ����� ���������
volatile BYTE vbSpiReady = 0;			// ���� ���������� � ������

volatile WORD vwSpiPwrReleaseTimer = 0; // ����� �������� ������������� ����������
volatile WORD vwDataAs5055a = 0;		// 16 ��� ������ �������� � ����������
volatile WORD vwSensorPosition = 0;		// 14 ��� ������ ���������

volatile DWORD vdSpiPwrOnTimer = 0;		// ����� �������� ��������� ���������� ����� ������ �������

WORD get_data_via_spi(BYTE is_disable)
{
	if (is_disable)
		return vwSensorPosition;

	if (!vbSpiPwrOn)
	{
		vdSpiPwrOnTimer++;

		if (vdSpiPwrOnTimer > 15) // ���� ���� ��� ������������ � ������� ���������
			vbSpiPwrOn = 1;
	}

	if (vbSpiPwrOn && (!vbSpiPwrRelease))
	{
		GPIOB->ODR |= (1 << SPI_PWR); // ������ ������� �� ����������
		GPIOA->ODR |= (1 << SNSS);	  // � ��������� ���������� ������� ��� �������������
		GPIOB->ODR |= (1 << SMOSI);
		vbSpiPwrRelease = 1;
	}

	if (vbSpiPwrRelease && (!vbSpiReady))
	{
		vwSpiPwrReleaseTimer++;
		if (vwSpiPwrReleaseTimer > 10) // ���� ���� ���������� �������������������
			vbSpiReady = 1;
	}

	if (vbSpiReady)
	{
		GPIOA->ODR &= ~(1 << SNSS);
		vwDataAs5055a = 0;
		for (vbByteCount = 0; vbByteCount < 16; vbByteCount++) // � ����� ��������� ������
		{
			GPIOB->ODR |= (1 << SSCK);
			__NOP();
			vwDataAs5055a <<= 1;
			GPIOB->ODR &= ~(1 << SSCK);
			if (GPIOB->IDR & (1 << SMISO))
				vwDataAs5055a |= 1;
		}
		GPIOB->ODR &= ~(1 << SSCK);
		GPIOA->ODR |= (1 << SNSS);
		vwSensorPosition = vwDataAs5055a >> 2; // � ��������� �� � ����������
	}

	return vwSensorPosition;
}