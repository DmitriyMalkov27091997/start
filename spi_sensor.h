/**
 * @file spi_sensor.h
 * @author ������ ������� ������������� (mazlov1989@gmail.com)
 * @author �������� ������ ������� (victs76@mail.ru)
 * @brief ������������ ���� ������� ��������� �� SPI
 * @version 1.1
 * @date 22.02.2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef spi_sensor_H_
#define spi_sensor_H_

#include "stm32l1xx.h"
#include "type.h"

/// @brief �������� ������ � SPI �������.
/// @param is_disable ��������� ������ ������, �������� � ������ ���� ����� �������: \n 1 - ��������, \n 0 - ��������.
/// @return ���������� ������.
extern WORD get_data_via_spi (BYTE is_disable);

#endif /* spi_sensor_H_ */