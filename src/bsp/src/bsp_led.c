/*
*********************************************************************************************************
*
*	Module Name : LED Indicator Control Module
*	File Name   : bsp_led.c
*	Version     : V1.0
*	Description : Controls LED indicators
*
*	Revision History :
*		Version    Date        Author   Description
*		V1.0       2018-09-05  armfly   Initial release
*
*	Copyright (C), 2015-2030, www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
/*
	The STM32-H7 development board controls LED indicators using the 74HC574 latch chip, 
	rather than directly using the CPU's IO pins. 
	The 74HC574 is an 8-bit latch connected via the FMC interface. 
	The driver for the 74HC574 is implemented in: bsp_fmc_io.c
*/

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	bsp_LedOff(1);
	bsp_LedOff(2);
	bsp_LedOff(3);
	bsp_LedOff(4);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	if (_no == 1)
	{
		HC574_SetPin(LED1, 0);
	}
	else if (_no == 2)
	{
		HC574_SetPin(LED2, 0);
	}
	else if (_no == 3)
	{
		HC574_SetPin(LED3, 0);
	}
	else if (_no == 4)
	{
		HC574_SetPin(LED4, 0);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	if (_no == 1)
	{
		HC574_SetPin(LED1, 1);
	}
	else if (_no == 2)
	{
		HC574_SetPin(LED2, 1);
	}
	else if (_no == 3)
	{
		HC574_SetPin(LED3, 1);
	}
	else if (_no == 4)
	{
		HC574_SetPin(LED4, 1);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	uint32_t pin;
	
	if (_no == 1)
	{
		pin = LED1;
	}
	else if (_no == 2)
	{
		pin = LED2;
	}
	else if (_no == 3)
	{
		pin = LED3;
	}
	else if (_no == 4)
	{
		pin = LED4;
	}
	else
	{
		return;
	}

	if (HC574_GetPin(pin))
	{
		HC574_SetPin(pin, 0);
	}
	else
	{
		HC574_SetPin(pin, 1);
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	uint32_t pin;
	
	if (_no == 1)
	{
		pin = LED1;
	}
	else if (_no == 2)
	{
		pin = LED2;
	}
	else if (_no == 3)
	{
		pin = LED3;
	}
	else if (_no == 4)
	{
		pin = LED4;
	}
	else
	{
		return 0;
	}
	
	if (HC574_GetPin(pin))
	{
		return 0;	/* �� */
	}
	else
	{
		return 1;	/* �� */
	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
