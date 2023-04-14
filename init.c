/**
 * @file init.c
 * @author Шанин А.М. (dnega@mail.ru)
 * @brief Инициализация периферии.
 * @version 0.1
 * @date 20.05.2016
 * 
 * @copyright (C) Ярославль, 2016
 * 
 */
#include "init.h"

#define SMISO 4

//-----------------------------------------------------------------------------
//	void init_SYSTEM(void)
//-----------------------------------------------------------------------------
void init_SYSTEM(void)
{
#ifndef NO_AMP
	volatile int i;
	int trim_tr_high;
	int trim_tr_low;
	int trim_tr_lp_high;
	int trim_tr_lp_low;
#endif // NO_AMP

	RCC->AHBENR = 0							  //
				  | (1 << RCC_AHBENR_GPIOAEN) //
				  | (1 << RCC_AHBENR_GPIOBEN) //
				  | (1 << RCC_AHBENR_GPIOCEN) //
				  | (1 << RCC_AHBENR_GPIODEN) //
				  | (1 << RCC_AHBENR_GPIOHEN) //
				  | (0 << RCC_AHBENR_CRCEN)	  //
				  | (1 << RCC_AHBENR_FLITFEN) //
#ifdef ADC_OVER
				  | (1 << RCC_AHBENR_DMA1EN) //
#endif
				  | (0 << RCC_AHBENR_DMA2EN); //

	RCC->APB2ENR = 0							  //
				   | (1 << RCC_APB2ENR_SYSCFGEN)  //
				   | (1 << RCC_APB2ENR_TIM9EN)	  //
				   | (0 << RCC_APB2ENR_TIM10EN)	  //
				   | (0 << RCC_APB2ENR_TIM11EN)	  //
				   | (1 << RCC_APB2ENR_ADC1EN)	  //
				   | (0 << RCC_APB2ENR_SPI1EN)	  //
				   | (1 << RCC_APB2ENR_USART1EN); //

	RCC->APB1ENR = 0							 //
				   | (0 << RCC_APB1ENR_TIM2EN)	 //
				   | (1 << RCC_APB1ENR_TIM3EN)	 //
				   | (0 << RCC_APB1ENR_TIM4EN)	 //
				   | (0 << RCC_APB1ENR_TIM5EN)	 //
				   | (1 << RCC_APB1ENR_TIM6EN)	 //
				   | (0 << RCC_APB1ENR_TIM7EN)	 //
				   | (0 << RCC_APB1ENR_LCDEN)	 //
				   | (0 << RCC_APB1ENR_WWDGEN)	 //
				   | (0 << RCC_APB1ENR_SPI2EN)	 //
				   | (0 << RCC_APB1ENR_SPI3EN)	 //
				   | (0 << RCC_APB1ENR_USART2EN) //
				   | (1 << RCC_APB1ENR_USART3EN) //
				   | (0 << RCC_APB1ENR_I2C1EN)	 //
				   | (0 << RCC_APB1ENR_I2C2EN)	 //
				   | (0 << RCC_APB1ENR_USBEN)	 //
				   | (1 << RCC_APB1ENR_PWREN)	 //
				   | (0 << RCC_APB1ENR_DACEN)	 //
#ifndef NO_AMP
				   | (1 << RCC_APB1ENR_COMPEN) //
#endif
		;

	// SCB->SCR = (1 << SCB_SCR_SLEEPONEXIT) | (0 << SCB_SCR_SLEEPDEEP);//

	RCC->ICSCR = (5 << RCC_ICSCR_MSIRANGE)	   //
				 | (0x00 << RCC_ICSCR_MSICAL); //

	PWR->CR = (2 << 11); //

	RCC->CIR = (1 << RCC_CIR_HSIRDYC); //

	RCC->CR = 0						 //
			  | (1 << RCC_CR_MSION)	 //
			  | (1 << RCC_CR_HSION); //

	while (!(RCC->CR & (1 << RCC_CR_HSIRDY)))
		;

	// RCC->CFGR = (1 << RCC_CFGR_SW);//

	ADC->CCR = 0						//
			   | (1 << ADC_CCR_TSVREFE) //
			   | (2 << ADC_CCR_ADCPRE); //

#ifdef ADC_OVER
	ADC1->SQR1 = 0							 //
				 | ((16 - 1) << ADC_SQR1_L); //

	ADC1->SQR5 = 0							  //
				 | (ADC_HMC << ADC_SQR5_SQ1)  //
				 | (ADC_AIN << ADC_SQR5_SQ2)  //
				 | (ADC_HMC << ADC_SQR5_SQ3)  //
				 | (ADC_BTN << ADC_SQR5_SQ4)  //
				 | (ADC_HMC << ADC_SQR5_SQ5)  //
				 | (ADC_AIN << ADC_SQR5_SQ6); //

	ADC1->SQR4 = 0							   //
				 | (ADC_HMC << ADC_SQR4_SQ7)   //
				 | (ADC_TMP << ADC_SQR4_SQ8)   //
				 | (ADC_HMC << ADC_SQR4_SQ9)   //
				 | (ADC_AIN << ADC_SQR4_SQ10)  //
				 | (ADC_HMC << ADC_SQR4_SQ11)  //
				 | (ADC_BTN << ADC_SQR4_SQ12); //

	ADC1->SQR3 = 0							   //
				 | (ADC_HMC << ADC_SQR3_SQ13)  //
				 | (ADC_AIN << ADC_SQR3_SQ14)  //
				 | (ADC_HMC << ADC_SQR3_SQ15)  //
				 | (ADC_TMP << ADC_SQR3_SQ16); //

	ADC1->SR = ~(1 << ADC_SR_EOC); //

	ADC1->CR1 = 0					  //
				| (1 << ADC_CR1_SCAN) //
				| (1 << ADC_CR1_PDD)  //
				| (1 << ADC_CR1_PDI); //
#else
	ADC1->JSQR = 0							//
				 | ((4 - 1) << ADC_JSQR_JL) //
#ifdef NEW_PCB
				 | (3 << ADC_JSQR_JSQ1)		//
				 | (8 << ADC_JSQR_JSQ2)		//
				 | (16 << ADC_JSQR_JSQ3)	//
				 | (5 << ADC_JSQR_JSQ4)		//
#else
#ifndef NO_AMP
				 | (3 << ADC_JSQR_JSQ1)	 // hmc_adc
#else									   // NO_AMP
				 | (1 << ADC_JSQR_JSQ1) // hmc_adc
#endif									   // NO_AMP
				 | (0 << ADC_JSQR_JSQ2)	 // ain_adc
				 | (16 << ADC_JSQR_JSQ3) // temp_adc
				 | (5 << ADC_JSQR_JSQ4)	 // btn_adc
#endif									   // NEW_PCB
		;
	ADC1->SR = ~(1 << ADC_SR_JEOC);						   //
	ADC1->CR1 = 0										   //
				| (1 << ADC_CR1_JEOCIE)					   //
				| (1 << ADC_CR1_SCAN)					   //
				| (1 << ADC_CR1_PDD)					   //
				| (1 << ADC_CR1_PDI);					   //
#endif									   // ADC_OVER
	ADC1->SMPR3 = 0						   //
				  | (7 << ADC_SMPR3_SMP0)  // ain_adc
				  | (7 << ADC_SMPR3_SMP1)  // hmc_adc (resistor)
				  | (7 << ADC_SMPR3_SMP3)  // hmc_adc (magnetic)
				  | (7 << ADC_SMPR3_SMP5); // btn_adc

	ADC1->SMPR2 = 0							//
				  | (7 << ADC_SMPR2_SMP16); // temp_adc

	ADC1->CR2 = 0 //
#ifdef ADC_OVER
				| (1 << ADC_CR2_DMA)  //
				| (1 << ADC_CR2_DDS)  //
				| (0 << ADC_CR2_EOCS) //
				| (7 << ADC_CR2_DELS) //
#endif
				| (1 << ADC_CR2_ADON);			  //
				
	while ((ADC1->SR & (1 << ADC_SR_ADONS)) == 0) //
		;

#ifndef NO_AMP
	OPAMP->CSR = 0							   //
				 | (0 << OPAMP_CSR_OPA1PD)	   //
				 | (1 << OPAMP_CSR_OPA2PD)	   //
				 | (1 << OPAMP_CSR_OPA3PD)	   //
				 | (0 << OPAMP_CSR_S4SEL1)	   //
				 | (0 << OPAMP_CSR_S5SEL1)	   //
				 | (1 << OPAMP_CSR_OPA_RANGE); //

	con_str("\n\rOPAMP1: ");
	con_str("OTR = ");
	con_dword(OPAMP->OTR);
	con_str(", LPOTR = ");
	con_dword(OPAMP->LPOTR);
	con_str("\n\r");
	con_start();

	OPAMP->OTR = 0 | (1 << OPAMP_OTR_OT_USER);

	con_str("trim_tr_high");
	con_start();

	OPAMP->CSR = 0							   //
				 | (0 << OPAMP_CSR_OPA1PD)	   //
				 | (1 << OPAMP_CSR_OPA2PD)	   //
				 | (1 << OPAMP_CSR_OPA3PD)	   //
				 | (0 << OPAMP_CSR_S4SEL1)	   //
				 | (0 << OPAMP_CSR_S5SEL1)	   //
				 | (1 << OPAMP_CSR_OPA1CAL_H)  //
				 | (0 << OPAMP_CSR_OPA1CAL_L)  //
				 | (1 << OPAMP_CSR_OPA_RANGE); //

	trim_tr_high = 0;
	OPAMP->OTR = 0														 //
				 | (1 << OPAMP_OTR_OT_USER)								 //
				 | (trim_tr_high << OPAMP_OTR_AO1_OPT_OFFSET_TRIM_HIGH); //
	for (i = 0; i < 1000; i++)
		;

	do
	{
		trim_tr_high++;
		OPAMP->OTR = 0 | (1 << OPAMP_OTR_OT_USER) | (trim_tr_high << OPAMP_OTR_AO1_OPT_OFFSET_TRIM_HIGH);
		for (i = 0; i < 1000; i++)
			;
		con_push('.');
		con_start();
	} while (((OPAMP->CSR & (1 << OPAMP_CSR_OPA1CALOUT)) == 0) && (trim_tr_high < 0x1F));

	con_str(" ");
	con_dec(trim_tr_high);
	con_str("\n\r");
	con_start();

	con_str("trim_tr_low");
	con_start();

	OPAMP->CSR = 0							   //
				 | (0 << OPAMP_CSR_OPA1PD)	   //
				 | (1 << OPAMP_CSR_OPA2PD)	   //
				 | (1 << OPAMP_CSR_OPA3PD)	   //
				 | (0 << OPAMP_CSR_S4SEL1)	   //
				 | (0 << OPAMP_CSR_S5SEL1)	   //
				 | (0 << OPAMP_CSR_OPA1CAL_H)  //
				 | (1 << OPAMP_CSR_OPA1CAL_L)  //
				 | (1 << OPAMP_CSR_OPA_RANGE); //

	trim_tr_low = 0;
	OPAMP->OTR = 0														//
				 | (1 << OPAMP_OTR_OT_USER)								//
				 | (trim_tr_high << OPAMP_OTR_AO1_OPT_OFFSET_TRIM_HIGH) //
				 | (trim_tr_low << OPAMP_OTR_AO1_OPT_OFFSET_TRIM_LOW);	//
	for (i = 0; i < 1000; i++)
		;

	do
	{
		trim_tr_low++;
		OPAMP->OTR = 0														//
					 | (1 << OPAMP_OTR_OT_USER)								//
					 | (trim_tr_high << OPAMP_OTR_AO1_OPT_OFFSET_TRIM_HIGH) //
					 | (trim_tr_low << OPAMP_OTR_AO1_OPT_OFFSET_TRIM_LOW);	//
		for (i = 0; i < 1000; i++)
			;
		con_push('.');
		con_start();
	} while (((OPAMP->CSR & (1 << OPAMP_CSR_OPA1CALOUT)) == 0) && (trim_tr_low < 0x1F));

	con_str(" ");
	con_dec(trim_tr_low);
	con_str("\n\r");
	con_start();

	con_str("trim_tr_lp_high");
	con_start();

	OPAMP->CSR = 0							   //
				 | (0 << OPAMP_CSR_OPA1PD)	   //
				 | (1 << OPAMP_CSR_OPA2PD)	   //
				 | (1 << OPAMP_CSR_OPA3PD)	   //
				 | (0 << OPAMP_CSR_S4SEL1)	   //
				 | (0 << OPAMP_CSR_S5SEL1)	   //
				 | (1 << OPAMP_CSR_OPA1LPM)	   //
				 | (1 << OPAMP_CSR_OPA1CAL_H)  //
				 | (0 << OPAMP_CSR_OPA1CAL_L)  //
				 | (1 << OPAMP_CSR_OPA_RANGE); //

	trim_tr_high = 0;
	OPAMP->LPOTR = 0 | (trim_tr_high << OPAMP_LP_OTR_AO1_OPT_OFFSET_TRIM_LP_HIGH);
	for (i = 0; i < 1000; i++)
		;

	do
	{
		trim_tr_high++;
		OPAMP->LPOTR = 0 | (trim_tr_high << OPAMP_LP_OTR_AO1_OPT_OFFSET_TRIM_LP_HIGH);
		for (i = 0; i < 1000; i++)
			;
		con_push('.');
		con_start();
	} while (((OPAMP->CSR & (1 << OPAMP_CSR_OPA1CALOUT)) == 0) && (trim_tr_high < 0x1F));

	con_str(" ");
	con_dec(trim_tr_high);
	con_str("\n\r");
	con_start();

	con_str("trim_tr_lp_low");
	con_start();

	OPAMP->CSR = 0							   //
				 | (0 << OPAMP_CSR_OPA1PD)	   //
				 | (1 << OPAMP_CSR_OPA2PD)	   //
				 | (1 << OPAMP_CSR_OPA3PD)	   //
				 | (0 << OPAMP_CSR_S4SEL1)	   //
				 | (0 << OPAMP_CSR_S5SEL1)	   //
				 | (1 << OPAMP_CSR_OPA1LPM)	   //
				 | (0 << OPAMP_CSR_OPA1CAL_H)  //
				 | (1 << OPAMP_CSR_OPA1CAL_L)  //
				 | (1 << OPAMP_CSR_OPA_RANGE); //

	trim_tr_low = 0;
	OPAMP->LPOTR = 0															//
				   | (trim_tr_high << OPAMP_LP_OTR_AO1_OPT_OFFSET_TRIM_LP_HIGH) //
				   | (trim_tr_low << OPAMP_LP_OTR_AO1_OPT_OFFSET_TRIM_LP_LOW);	//
	for (i = 0; i < 1000; i++)
		;

	do
	{
		trim_tr_low++;
		OPAMP->LPOTR = 0															//
					   | (trim_tr_high << OPAMP_LP_OTR_AO1_OPT_OFFSET_TRIM_LP_HIGH) //
					   | (trim_tr_low << OPAMP_LP_OTR_AO1_OPT_OFFSET_TRIM_LP_LOW);	//
		for (i = 0; i < 1000; i++)
			;
		con_push('.');
		con_start();
	} while (((OPAMP->CSR & (1 << OPAMP_CSR_OPA1CALOUT)) == 0) && (trim_tr_low < 0x1F));

	con_str(" ");
	con_dec(trim_tr_low);
	con_str("\n\r");
	con_start();

	con_str("\n\raOTR = ");
	con_dword(OPAMP->OTR);
	con_str(", aLPOTR = ");
	con_dword(OPAMP->LPOTR);
	con_str("\n\r");
	con_start();

	OPAMP->CSR = 0 | (0 << OPAMP_CSR_OPA1PD) //
#ifdef NEW_PCB
				 | (0 << OPAMP_CSR_OPA2PD) //
#else
				 | (1 << OPAMP_CSR_OPA2PD) //
#endif										// NEW_PCB
				 | (1 << OPAMP_CSR_OPA3PD)	//
				 | (1 << OPAMP_CSR_OPA1LPM) //
				 | (1 << OPAMP_CSR_S4SEL1)	//
				 | (1 << OPAMP_CSR_S5SEL1)	//
#ifdef NEW_PCB
				 | (1 << OPAMP_CSR_S4SEL2)	   //
				 | (1 << OPAMP_CSR_S5SEL2)	   //
#endif										   // NEW_PCB
				 | (1 << OPAMP_CSR_OPA_RANGE); //
#endif										   // NO_AMP
}

//-----------------------------------------------------------------------------
//	void init_GPIO(void)
//-----------------------------------------------------------------------------
void init_GPIO(void)
{
	GPIOA->MODER = 0									   //
				   | (GPIO_MODE_ANALOG << GPIO_MODER_PIN0) //(14) AIN
				   | (GPIO_MODE_ANALOG << GPIO_MODER_PIN1) //(15) HMC_P (OAMP1_P)
#if 1
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN2) //(16) TEST_PIN
#else
				   | (GPIO_MODE_ANALOG << GPIO_MODER_PIN2) //(16) HMC_M (OAMP1_M)
#endif
				   | (GPIO_MODE_ANALOG << GPIO_MODER_PIN3) //(17) HMC_OUT (OAMP1_OUT)
				   | (GPIO_MODE_INPUT << GPIO_MODER_PIN4)  //(20) FAIL
				   | (GPIO_MODE_ANALOG << GPIO_MODER_PIN5) //(21) KEY
#ifdef NEW_PCB
				   | (GPIO_MODE_ANALOG << GPIO_MODER_PIN6) //(22) AIN_P (OAMP2_P)
				   | (GPIO_MODE_ANALOG << GPIO_MODER_PIN7) //(23) AIN_M (OAMP2_M)
#else
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN6) //(22) free
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN7) //(23) free
#endif														   // NEW_PCB
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN8)	   //(41) free
				   | (GPIO_MODE_ALTERNATE << GPIO_MODER_PIN9)  //(42) USART1_TX
				   | (GPIO_MODE_ALTERNATE << GPIO_MODER_PIN10) //(43) USART1_RX
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN11)	   //(44) free (USB_DM)
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN12)	   //(45) free (USB_DP) - PULLUP
				   | (GPIO_MODE_ALTERNATE << GPIO_MODER_PIN13) //(46) SWDIO
				   | (GPIO_MODE_ALTERNATE << GPIO_MODER_PIN14) //(49) SWCLK
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN15);   //(50) SNSS spi_sensor

	GPIOA->AFR[0] = 0;

	GPIOA->AFR[1] = 0										 //
					| (AF_PA9_USART1_TX << GPIO_AFR1_PIN9)	 //
					| (AF_PA10_USART1_RX << GPIO_AFR1_PIN10) //
					| (AF_PA13_SWDIO << GPIO_AFR1_PIN13)	 //
					| (AF_PA14_SWCLK << GPIO_AFR1_PIN14);	 //

	GPIOB->MODER = 0 //
#ifdef NEW_PCB
				   | (GPIO_MODE_ANALOG << GPIO_MODER_PIN0) //(26) AIN_OUT (OAMP2_OUT)
#else
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN0) //(26) free (MEM_RESET)
#endif														   // NEW_PCB
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN1)	   //(27) free (MEM_CS)
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN2)	   //(28) free (MEM_WP; BOOT1) - PULLDOWN
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN3)	   //(55) SSCK spi_sensor
				   | (GPIO_MODE_INPUT << GPIO_MODER_PIN4)	   //(56) SMISO spi_sensor
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN5)	   //(57) SMOSI spi_sensor
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN6)	   //(58) DISP_SCL - PULLUP
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN7)	   //(59) DISP_SDA - PULLUP
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN8)	   //(61) free (DISP_POWER)
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN9)	   //(62) SPI_PWR spi_sensor
				   | (GPIO_MODE_ALTERNATE << GPIO_MODER_PIN10) //(29) USART3_TX (HART_TX)
				   | (GPIO_MODE_ALTERNATE << GPIO_MODER_PIN11) //(30) USART3_RX (HART_RX)
				   | (GPIO_MODE_INPUT << GPIO_MODER_PIN12)	   //(33) free (HART_CD)
				   | (GPIO_MODE_ALTERNATE << GPIO_MODER_PIN13) //(34) PWM1 (T9C1)
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN14)	   //(35) HART_RTS
				   | (GPIO_MODE_INPUT << GPIO_MODER_PIN15);	   //(36) PWM1_DIAG

	GPIOB->AFR[0] = 0;

	GPIOB->AFR[1] = 0										 //
					| (AF_PB10_USART3_TX << GPIO_AFR1_PIN10) //
					| (AF_PB11_USART3_RX << GPIO_AFR1_PIN11) //
					| (AF_PB13_TIM9_CH1 << GPIO_AFR1_PIN13); //

	GPIOB->OTYPER = 0 //
					| (1 << DISP_SDA) | (1 << DISP_SCL);

	GPIOC->MODER = 0										  //
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN0)	  //( 8) free
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN1)	  //( 9) free
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN2)	  //(10) free
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN3)	  //(11) free
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN4)	  //(24) free
				   | (GPIO_MODE_INPUT << GPIO_MODER_PIN5)	  //(25) PWM2_DIAG - coil_diag
				   | (GPIO_MODE_ALTERNATE << GPIO_MODER_PIN6) //(37) PWM2 (T3C1) - coil
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN7)	  //(38) NAMUR_OUT1
				   | (GPIO_MODE_INPUT << GPIO_MODER_PIN8)	  //(39) NAMUR_DIAG1
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN9)	  //(40) free
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN10)	  //(51) NAMUR_OUT2
				   | (GPIO_MODE_INPUT << GPIO_MODER_PIN11)	  //(52) NAMUR_DIAG2
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN12)	  //(53) NAMUR_OUT3
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN13)	  //( 2) free
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN14)	  //( 3) free
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN15);  //( 4) free

	GPIOC->AFR[0] = 0									   //
					| (AF_PC6_TIM3_CH1 << GPIO_AFR0_PIN6); //

	GPIOC->AFR[1] = 0; //

	GPIOD->MODER = 0									   //
				   | (GPIO_MODE_INPUT << GPIO_MODER_PIN2); //(54) NAMUR_DIAG3

	GPIOH->MODER = 0										//
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN0)	//
				   | (GPIO_MODE_OUTPUT << GPIO_MODER_PIN1); //

	GPIOA->PUPDR = 0; //

	GPIOB->PUPDR = 0					   //
				   | (2 << (SMISO * 2))	   // подтяжка smiso spi_sensor к "0"
				   | (0 << (DISP_SDA * 2)) //
				   | (0 << (DISP_SCL * 2)) //
				   | (0 << (15 * 2));	   //

	GPIOC->PUPDR = 0;

	GPIOD->PUPDR = 0;

	GPIOH->PUPDR = 0;

	GPIOA->ODR = (1 << 12);

	GPIOB->ODR = (1 << 14);

	GPIOC->ODR = (1 << 7) | (1 << 10) | (1 << 12);

	GPIOD->ODR = 0;

	GPIOH->ODR = 0;
}

//-----------------------------------------------------------------------------
//	void init_SYSTICK(void)
//-----------------------------------------------------------------------------
void init_SYSTICK(void)
{
	SysTick->LOAD = ((FMSI >> 3) / FSYSTICK) - 1;
	SysTick->CTRL = 0 | (1 << SYSTICK_CTRL_TICKINT) | (1 << SYSTICK_CTRL_ENABLE);
}

//-----------------------------------------------------------------------------
//	void init_USART1(void)
//-----------------------------------------------------------------------------
void init_USART1(void)
{
	USART1->CR1 = 0;
	USART1->BRR = FMSI / CONSOLE_BAUD;
	USART1->CR1 = 0							 //
				  | (1 << USART_CR1_UE)		 //
				  | (1 << USART_CR1_TE)		 //
				  | (1 << USART_CR1_RE)		 //
				  | (1 << USART_CR1_RXNEIE); //
	USART1->CR2 = 0;
	USART1->CR3 = 0;
}

//-----------------------------------------------------------------------------
//	void init_USART3(void)
//-----------------------------------------------------------------------------
void init_USART3(void)
{
	USART3->CR1 = 0;
	USART3->BRR = FMSI / HART_BAUD;
	USART3->CR1 = 0							 //
				  | (1 << USART_CR1_UE)		 //
				  | (1 << USART_CR1_TE)		 //
				  | (1 << USART_CR1_RE)		 //
				  | (1 << USART_CR1_TCIE)	 //
				  | (1 << USART_CR1_M)		 //
				  | (1 << USART_CR1_PCE)	 //
				  | (1 << USART_CR1_PS)		 //
				  | (1 << USART_CR1_RXNEIE); //
	USART3->CR2 = 0;
	USART3->CR3 = 0;
}

//-----------------------------------------------------------------------------
//	void init_TIM6(void)
//-----------------------------------------------------------------------------
void init_TIM6(void)
{
	TIM6->PSC = 4 - 1;
	TIM6->ARR = FMSI / FADC / 4 - 1;
	TIM6->SR = ~(1 << TIM_SR_UIF);
	TIM6->DIER = (1 << TIM_DIER_UIE);
	TIM6->CR1 = (1 << TIM_CR1_CEN);
}

//-----------------------------------------------------------------------------
//	void init_TIM3(void)
//-----------------------------------------------------------------------------
void init_TIM3(void)
{
	TIM3->PSC = 0; // FMSI / FPWM_COIL / MAX_PWM_COIL - 1;
	TIM3->ARR = MAX_PWM_COIL - 1;
	TIM3->CCMR1 = (6 << TIM_CCMR1_OC1M) | (1 << TIM_CCMR1_OC1PE);
	TIM3->CCR1 = 0;
	TIM3->CCER = (1 << TIM_CCER_CC1E);
	TIM3->CR1 = (1 << TIM_CR1_CEN);
}

//-----------------------------------------------------------------------------
//	void init_TIM9(void)
//-----------------------------------------------------------------------------
void init_TIM9(void)
{
	TIM9->PSC = FMSI / FPWM_AOUT / MAX_PWM_AOUT - 1;
	TIM9->ARR = MAX_PWM_AOUT - 1;
	TIM9->CCMR1 = (6 << TIM_CCMR1_OC1M);
	TIM9->CCR1 = 0;
	TIM9->CCER = (1 << TIM_CCER_CC1E);
	TIM9->CR1 = (1 << TIM_CR1_CEN);
}

//-----------------------------------------------------------------------------
//	void init_NVIC(void)
//-----------------------------------------------------------------------------
void init_NVIC(void)
{
	SCB->AIRCR = 0 | (PRI2_6 << SCB_AIRCR_PRIGROUP) | (SCB_AIRCR_KEY << SCB_AIRCR_VECTKEY); // 5 - 2.6

	//---------------------------------------------------------------------------
	// SYSTEM PRIO
	//---------------------------------------------------------------------------
	SCB->SHP[0] = PRIO2_6_NORMAL;
	SCB->SHP[1] = PRIO2_6_NORMAL;
	SCB->SHP[2] = PRIO2_6_NORMAL;
	SCB->SHP[3] = PRIO2_6_NORMAL;
	SCB->SHP[4] = PRIO2_6_NORMAL;
	SCB->SHP[5] = PRIO2_6_NORMAL;
	SCB->SHP[6] = PRIO2_6_NORMAL;
	SCB->SHP[7] = PRIO2_6_NORMAL;
	SCB->SHP[8] = PRIO2_6_NORMAL;
	SCB->SHP[9] = PRIO2_6_NORMAL;
	SCB->SHP[10] = PRIO2_6_NORMAL;
	SCB->SHP[11] = PRIO2_6_NORMAL; // SysTick

	NVIC->IP[USART3_IRQn] = PRIO2_6_HIGHEST;
	NVIC->IP[USART1_IRQn] = PRIO2_6_HIGH;
	NVIC->IP[ADC1_IRQn] = PRIO2_6_NORMAL;
	NVIC->IP[TIM6_IRQn] = PRIO2_6_NORMAL;

	NVIC->ISER[0] = 0
#ifdef ADC_OVER
					| (1 << NVIC_ISER0_DMA1_CH1)
#else
					| (1 << NVIC_ISER0_ADC1)
#endif // ADC_OVER
		;

	NVIC->ISER[1] = 0 | (1 << NVIC_ISER1_USART1) | (1 << NVIC_ISER1_USART3) | (1 << NVIC_ISER1_TIM6);
}

/******************************************************************************
 * КОНЕЦ ФАЙЛА
 *****************************************************************************/
