/*
 * HC595.cpp
 *
 *  Created on: 2016年12月24日
 *      Author: Romeli
 */

#include "HC595.h"

#define DS_SET GPIOPort->BSRR = DS_PIN
#define DS_RESET GPIOPort->BRR = DS_PIN
#define OE_SET GPIOPort->BSRR = OE_PIN
#define OE_RESET GPIOPort->BRR = OE_PIN
#define STCP_SET GPIOPort->BSRR = STCP_PIN
#define STCP_RESET GPIOPort->BRR = STCP_PIN
#define SHCP_SET GPIOPort->BSRR = SHCP_PIN
#define SHCP_RESET GPIOPort->BRR = SHCP_PIN

HC595Class::HC595Class(GPIO_TypeDef* GPIOx, uint16_t DS, uint16_t OE,
		uint16_t STCP, uint16_t SHCP) {
	GPIOPort = GPIOx;
	DS_PIN = DS;
	OE_PIN = OE;
	STCP_PIN = STCP;
	SHCP_PIN = SHCP;
	GPIOInit();
	Write(0x0000);
	Disable();
}

void HC595Class::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;
	if (GPIOPort == GPIOA) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	} else if (GPIOPort == GPIOB) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	} else if (GPIOPort == GPIOC) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	} else if (GPIOPort == GPIOD) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	} else if (GPIOPort == GPIOE) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	} else if (GPIOPort == GPIOF) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	}
	GPIO_InitStructure.GPIO_Pin = DS_PIN | OE_PIN | STCP_PIN | SHCP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOPort, &GPIO_InitStructure);
}

void HC595Class::Write(uint16_t data) {
	STCP_RESET;	//先置为低电平 以便产生上升沿更新脉冲
	Delay();
	for (uint16_t mask = 0x8000; mask != 0; mask >>= 1) {
		SHCP_RESET; ////先置为低电平 以便产生上升沿移位脉冲  Qn>>Q(n+1)
		Delay();
		if ((mask & data) != 0) {
			DS_SET;
		} else {
			DS_RESET;
		}
		Delay();
		SHCP_SET;
		Delay();
	}
	STCP_SET;
	Delay();
	Enable();
}

inline void HC595Class::Disable() {
	OE_SET;	//高阻输出
}

inline void HC595Class::Enable() {
	OE_RESET; //使能输出
}

inline void HC595Class::Delay() {
	__NOP();
}
