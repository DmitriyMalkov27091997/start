/**
 * @file spi_sensor.h
 * @author Мазлов Евгений Александрович (mazlov1989@gmail.com)
 * @author Цыплёнков Виктор Юрьевич (victs76@mail.ru)
 * @brief Заголовочный файл датчика положения по SPI
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

/// @brief Получить данные с SPI датчика.
/// @param is_disable Запретить запрос данных, например в случае сбоя блока питания: \n 1 - запрещен, \n 0 - разрешен.
/// @return Полученные данные.
extern WORD get_data_via_spi (BYTE is_disable);

#endif /* spi_sensor_H_ */