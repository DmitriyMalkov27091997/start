/**
 * @file spi_sensor.h
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @author Цыплёнков Виктор Юрьевич (victs76@mail.ru)
 * @brief Датчик положения по SPI
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

BYTE vbByteCount = 0;					// счетчик полученных битов

volatile BYTE vbSpiPwrOn = 0;			// флаг окончания времени ожидания после подачи питания
volatile BYTE vbSpiPwrRelease = 0;		// флаг завершения инициализации микросхемы после включения
volatile BYTE vbSpiReady = 0;			// флаг готовности к работе

volatile WORD vwSpiPwrReleaseTimer = 0; // время ожидания инициализации микросхемы
volatile WORD vwDataAs5055a = 0;		// 16 бит данных принятых с микросхемы
volatile WORD vwSensorPosition = 0;		// 14 бит данных положения

volatile DWORD vdSpiPwrOnTimer = 0;		// время ожидания включения микросхемы после подачи питания

WORD get_data_via_spi(BYTE is_disable)
{
	if (is_disable)
		return vwSensorPosition;

	if (!vbSpiPwrOn)
	{
		vdSpiPwrOnTimer++;

		if (vdSpiPwrOnTimer > 15) // ждем пока все конденсаторы в изделии зарядатся
			vbSpiPwrOn = 1;
	}

	if (vbSpiPwrOn && (!vbSpiPwrRelease))
	{
		GPIOB->ODR |= (1 << SPI_PWR); // подаем питание на микросхему
		GPIOA->ODR |= (1 << SNSS);	  // и высталяем правильные сигналы для инициализации
		GPIOB->ODR |= (1 << SMOSI);
		vbSpiPwrRelease = 1;
	}

	if (vbSpiPwrRelease && (!vbSpiReady))
	{
		vwSpiPwrReleaseTimer++;
		if (vwSpiPwrReleaseTimer > 10) // ждем пока микросхема проинициализируется
			vbSpiReady = 1;
	}

	if (vbSpiReady)
	{
		GPIOA->ODR &= ~(1 << SNSS);
		vwDataAs5055a = 0;
		for (vbByteCount = 0; vbByteCount < 16; vbByteCount++) // в цикле считываем данные
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
		vwSensorPosition = vwDataAs5055a >> 2; // и сохраняем их в переменной
	}

	return vwSensorPosition;
}