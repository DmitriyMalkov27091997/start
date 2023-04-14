_Ram_Base = 0x20000000;

_Ram_Size = 32K ;
_Minimum_Stack_Size = 2K ;

_estack = _Ram_Base + _Ram_Size ; 

PROVIDE ( _Stack_Init = _estack ) ;

PROVIDE (   NMIException = 0 ) ;
PROVIDE (   HardFaultException = 0 ) ;
PROVIDE (   MemManageException = 0 ) ;
PROVIDE (   BusFaultException = 0 ) ;
PROVIDE (   UsageFaultException = 0 ) ;
PROVIDE (   SVCHandler = 0 ) ;
PROVIDE (   DebugMonitor = 0 ) ;
PROVIDE (   PendSVC = 0 ) ;
PROVIDE (   SysTickHandler = 0 ) ;

PROVIDE (   WWDG_IRQHandler = 0 ) ;
PROVIDE (   PVD_IRQHandler = 0 ) ;
PROVIDE (   TAMPER_STAMP_IRQHandler = 0 ) ;
PROVIDE (   RTC_WKUP_IRQHandler = 0 ) ;
PROVIDE (   FLASH_IRQHandler = 0 ) ;
PROVIDE (   RCC_IRQHandler = 0 ) ;
PROVIDE (   EXTI0_IRQHandler = 0 ) ;
PROVIDE (   EXTI1_IRQHandler = 0 ) ;
PROVIDE (   EXTI2_IRQHandler = 0 ) ;
PROVIDE (   EXTI3_IRQHandler = 0 ) ;
PROVIDE (   EXTI4_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Channel1_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Channel2_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Channel3_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Channel4_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Channel5_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Channel6_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Channel7_IRQHandler = 0 ) ;
PROVIDE (   ADC1_IRQHandler = 0 ) ;
PROVIDE (   USB_HP_IRQHandler = 0 ) ;
PROVIDE (   USB_LP_IRQHandler = 0 ) ;
PROVIDE (   DAC_IRQHandler = 0 ) ;
PROVIDE (   COMP_IRQHandler = 0 ) ;
PROVIDE (   EXTI9_5_IRQHandler = 0 ) ;
PROVIDE (   LCD_IRQHandler = 0 ) ;  
PROVIDE (   TIM9_IRQHandler = 0 ) ;
PROVIDE (   TIM10_IRQHandler = 0 ) ;
PROVIDE (   TIM11_IRQHandler = 0 ) ;
PROVIDE (   TIM2_IRQHandler = 0 ) ;
PROVIDE (   TIM3_IRQHandler = 0 ) ;
PROVIDE (   TIM4_IRQHandler = 0 ) ;
PROVIDE (   I2C1_EV_IRQHandler = 0 ) ;
PROVIDE (   I2C1_ER_IRQHandler = 0 ) ;
PROVIDE (   I2C2_EV_IRQHandler = 0 ) ;
PROVIDE (   I2C2_ER_IRQHandler = 0 ) ;
PROVIDE (   SPI1_IRQHandler = 0 ) ;
PROVIDE (   SPI2_IRQHandler = 0 ) ;
PROVIDE (   USART1_IRQHandler = 0 ) ;
PROVIDE (   USART2_IRQHandler = 0 ) ;
PROVIDE (   USART3_IRQHandler = 0 ) ;
PROVIDE (   EXTI15_10_IRQHandler = 0 ) ;
PROVIDE (   RTC_Alarm_IRQHandler = 0 ) ;
PROVIDE (   USB_FS_WKUP_IRQHandler = 0 ) ;
PROVIDE (   TIM6_IRQHandler = 0 ) ;
PROVIDE (   TIM7_IRQHandler = 0 ) ;
PROVIDE (   TIM5_IRQHandler = 0 ) ;
PROVIDE (   SPI3_IRQHandler = 0 ) ;
PROVIDE (   DMA2_Channel1_IRQHandler = 0 ) ;
PROVIDE (   DMA2_Channel2_IRQHandler = 0 ) ;
PROVIDE (   DMA2_Channel3_IRQHandler = 0 ) ;
PROVIDE (   DMA2_Channel4_IRQHandler = 0 ) ;
PROVIDE (   DMA2_Channel5_IRQHandler = 0 ) ;
PROVIDE (   AES_IRQHandler = 0 ) ;
PROVIDE (   COMP_ACQ_IRQHandler = 0 ) ;  

MEMORY
{
  RAM (xrw)			: ORIGIN = 0x20000000, LENGTH = 32K
  FLASH (rx)		: ORIGIN = 0x08000000, LENGTH = 128K - 16
  IMAGE_INFO(r)	: ORIGIN = 0x0801FFF0, LENGTH = 16
  EEPROM (rx)		: ORIGIN = 0x08080000, LENGTH = 8K
}

SECTIONS
{
	_app_start = ORIGIN(FLASH);
	_app_size = LENGTH(FLASH);
	
	.isr_vector :
	{
		. = ALIGN(4) ;
		KEEP(*(.isr_vector))
		. = ALIGN(4) ;
	} >FLASH
 
	.flashtext :
	{
		. = ALIGN(4) ;
		*(.flashtext)
		. = ALIGN(4) ;
	} >FLASH
    
	.text :
	{
		. = ALIGN(4) ;
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata*)
		*(.glue_7)
		*(.glue_7t)
		. = ALIGN(4) ;
		 _etext = . ;
   	 _sidata = _etext ;
	} >FLASH
	
	.image_info (NOLOAD) :
	{
		. = ALIGN(4) ;
		*(.image_info)
	} >IMAGE_INFO
    
	.data : AT ( _sidata )
	{
		. = ALIGN(4) ;
		_sdata = . ;
		*(.data)
		*(.data.*)
		*(.ramfunc)
		. = ALIGN(4) ;
		_edata = . ;
	} >RAM

	.eeprom (NOLOAD) :
	{
		. = ALIGN(4) ;
		*(.eeprom)
		. = ALIGN(4) ;
	} >EEPROM
    
	.bss :
	{
		. = ALIGN(4 );
		_sbss = . ;
		*(.bss)
		*(COMMON)
		. = ALIGN(4) ;
		_ebss = . ;
	} >RAM

	PROVIDE ( end = _ebss ) ;
	PROVIDE ( _end = _ebss ) ;

	._usrstack :
	{
		. = ALIGN(4) ;
		_susrstack = . ;
		. = . + _Minimum_Stack_Size ;
		. = ALIGN(4) ;
		_eusrstack = . ;
	} >RAM

	__exidx_start = . ;
	__exidx_end = . ;

	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}

	.stab							0 : { *(.stab) }
	.stabstr					0 : { *(.stabstr) }
	.stab.excl				0 : { *(.stab.excl) }
	.stab.exclstr			0 : { *(.stab.exclstr) }
	.stab.index				0 : { *(.stab.index) }
	.stab.indexstr		0 : { *(.stab.indexstr) }
	.comment					0 : { *(.comment) }
	.debug						0 : { *(.debug) }
	.line							0 : { *(.line) }
	.debug_srcinfo		0 : { *(.debug_srcinfo) }
	.debug_sfnames		0 : { *(.debug_sfnames) }
	.debug_aranges		0 : { *(.debug_aranges) }
	.debug_pubnames		0 : { *(.debug_pubnames) }
	.debug_info				0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev			0 : { *(.debug_abbrev) }
	.debug_line				0 : { *(.debug_line) }
	.debug_frame			0 : { *(.debug_frame) }
	.debug_str				0 : { *(.debug_str) }
	.debug_loc				0 : { *(.debug_loc) }
	.debug_macinfo		0 : { *(.debug_macinfo) }
	.debug_weaknames	0 : { *(.debug_weaknames) }
	.debug_funcnames	0 : { *(.debug_funcnames) }
	.debug_typenames	0 : { *(.debug_typenames) }
	.debug_varnames		0 : { *(.debug_varnames) }
}
