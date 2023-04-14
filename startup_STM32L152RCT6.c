/**
 * @file startup_STM32L152RCT6.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Startup - файл.
 *
 * Включает таблицу векторов прерываний и обработчик события "сброс".
 * @version 0.1
 * @date 20.05.2016
 * 
 * @copyright (C) Ярославль, 2016
 * 
 */

//-----------------------------------------------------------------------------
//	Объявления обработчиков прерываний (векторов)
//-----------------------------------------------------------------------------
void NMIException(void);
void HardFaultException(void);
void MemManageException(void);
void BusFaultException(void);
void UsageFaultException(void);
void SVCHandler(void);
void DebugMonitor(void);
void PendSVC(void);
void SysTickHandler(void);

void WWDG_IRQHandler(void);
void PVD_IRQHandler(void);
void TAMPER_STAMP_IRQHandler(void);
void RTC_WKUP_IRQHandler(void);
void FLASH_IRQHandler(void);
void RCC_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void ADC1_IRQHandler(void);
void USB_HP_IRQHandler(void);
void USB_LP_IRQHandler(void);
void DAC_IRQHandler(void);
void COMP_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void LCD_IRQHandler(void);
void TIM9_IRQHandler(void);
void TIM10_IRQHandler(void);
void TIM11_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void RTC_Alarm_IRQHandler(void);
void USB_FS_WKUP_IRQHandler(void);
void TIM6_IRQHandler(void);
void TIM7_IRQHandler(void);
void TIM5_IRQHandler(void);
void SPI3_IRQHandler(void);
void DMA2_Channel1_IRQHandler(void);
void DMA2_Channel2_IRQHandler(void);
void DMA2_Channel3_IRQHandler(void);
void DMA2_Channel4_IRQHandler(void);
void DMA2_Channel5_IRQHandler(void);
void AES_IRQHandler(void);
void COMP_ACQ_IRQHandler(void);

//-----------------------------------------------------------------------------
//	Указатели секций линкера
//-----------------------------------------------------------------------------
extern unsigned long _sidata;		//!< Начало инициализируемых данных.
extern unsigned long _sdata;		//!< Начало секции .data.
extern unsigned long _edata;		//!< Конец секции .data.
extern unsigned long _sbss;			//!< Начало секции .bss.
extern unsigned long _ebss;			//!< Конец секции .bss.
extern unsigned long _estack;		//!< Начало стека.
extern unsigned long _susrstack;

//-----------------------------------------------------------------------------
//	Определение функций
//-----------------------------------------------------------------------------
void Reset_Handler(void);
extern int main(void);

//-----------------------------------------------------------------------------
//	Таблица векторов прерываний
//-----------------------------------------------------------------------------
typedef void (*t_vector)(void);

__attribute__ ((section(".isr_vector")))
const t_vector vectors[] =
{
  (t_vector)&_estack,  				// Начальное значение указателя стека.
  Reset_Handler,             	// Вектор сброса.
  NMIException,
  HardFaultException,
  MemManageException,
  BusFaultException,
  UsageFaultException,
  0,
  0,
  0,
  0,
  SVCHandler,
  DebugMonitor,
  0,
  PendSVC,
  SysTickHandler,

  WWDG_IRQHandler,
  PVD_IRQHandler,
  TAMPER_STAMP_IRQHandler,
  RTC_WKUP_IRQHandler,
  FLASH_IRQHandler,
  RCC_IRQHandler,
  EXTI0_IRQHandler,
  EXTI1_IRQHandler,
  EXTI2_IRQHandler,
  EXTI3_IRQHandler,
  EXTI4_IRQHandler,
  DMA1_Channel1_IRQHandler,
  DMA1_Channel2_IRQHandler,
  DMA1_Channel3_IRQHandler,
  DMA1_Channel4_IRQHandler,
  DMA1_Channel5_IRQHandler,
  DMA1_Channel6_IRQHandler,
  DMA1_Channel7_IRQHandler,
  ADC1_IRQHandler,
  USB_HP_IRQHandler,
  USB_LP_IRQHandler,
  DAC_IRQHandler,
  COMP_IRQHandler,
  EXTI9_5_IRQHandler,
  LCD_IRQHandler,
  TIM9_IRQHandler,
  TIM10_IRQHandler,
  TIM11_IRQHandler,
  TIM2_IRQHandler,
  TIM3_IRQHandler,
  TIM4_IRQHandler,
  I2C1_EV_IRQHandler,
  I2C1_ER_IRQHandler,
  I2C2_EV_IRQHandler,
  I2C2_ER_IRQHandler,
  SPI1_IRQHandler,
  SPI2_IRQHandler,
  USART1_IRQHandler,
  USART2_IRQHandler,
  USART3_IRQHandler,
  EXTI15_10_IRQHandler,
  RTC_Alarm_IRQHandler,
  USB_FS_WKUP_IRQHandler,
  TIM6_IRQHandler,
  TIM7_IRQHandler,
  0,
  TIM5_IRQHandler,
  SPI3_IRQHandler,
  0,
  0,
  DMA2_Channel1_IRQHandler,
  DMA2_Channel2_IRQHandler,
  DMA2_Channel3_IRQHandler,
  DMA2_Channel4_IRQHandler,
  DMA2_Channel5_IRQHandler,
  AES_IRQHandler,
  COMP_ACQ_IRQHandler,
};

//-----------------------------------------------------------------------------
//! \fn void Reset_Handler(void)
//! \brief Вектор сброса.
//-----------------------------------------------------------------------------
void Reset_Handler(void)
{
	unsigned long *pulSrc, *pulDest;

	// Инициализация стека
	for(pulDest = &_susrstack; pulDest < &_estack; )
	{
		*(pulDest++) = 0xCCCCCCCC;
	}

	// Инициализация данных в секции .data
	pulSrc = &_sidata;
	for(pulDest = &_sdata; pulDest < &_edata; )
	{
		*(pulDest++) = *(pulSrc++);
	}

	// Обнуление данных в секции .bss
	for(pulDest = &_sbss; pulDest < &_ebss; )
	{
		*(pulDest++) = 0;
	}

	// Вызов функции main
	main();
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
