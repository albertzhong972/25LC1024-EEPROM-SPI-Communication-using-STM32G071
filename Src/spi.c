/*
 * spi.c
 *
 *  Created on: Mar 24, 2021
 *      Author: Albert Zhong
 */
#include "spi.h"


void SPI1_Config(void) {

	//Config GPIO Pins for SPI

	RCC->IOPENR = 0x1; //enable Port A
	RCC->IOPENR |= (1 << 3); //enable Port D
	RCC->IOPENR |= (1 << 2); //enable Port C
	GPIOC->MODER &= ~(3 << 26);	//set PC13 as input
	GPIOC->MODER &= ~(1 << 17);	//set PC8 as output

	RCC->APBENR2 |= (1 << 12); //SPI1 clock enable

	GPIOA->MODER &= ~(1 << 22);	//Set PA11 to use alternate function
	GPIOA->MODER &= ~(1 << 24);	//Set PA12 to use alternate function
	GPIOD->MODER &= ~(1 << 16);	//Set PD8 to use alternate function
	GPIOD->MODER &= ~(1 << 18);	//Set PD9 to use alternate function

	GPIOA->AFR[1] |= (0 << 12);	//Use AF0 (SPI1_MISO) on PA11
	GPIOA->AFR[1] |= (0 << 16);	//Use AF0 (SPI1_MOSI) on PA12
	GPIOD->AFR[1] |= (1 << 0);	//Use AF1 (SPI1_SCK) on PD8
	GPIOD->AFR[1] |= (1 << 4);	//Use AF1 (SPI1_NSS) on PD9


	GPIOC->ODR |= (1 << 8); //CS initially high

	//SPI Config and Settings
	SPI1->CR1 |= (1 << 2);	//Master configuration

	SPI1->CR1 |= (1 << 4);	//set baud rate to fclk / 8
	SPI1->CR1 |= (0 << 0); //CPHA = 0
	SPI1->CR1 |= (0 << 1); //CPOL = 0
	SPI1->CR1 |= (0 << 10); //Full-duplex mode
	SPI1->CR1 |= (0 << 7); //MSB transmitted first

	//CRC disabled

	SPI1->CR2 |= (7 << 8); //Data size = 8 bits
	SPI1->CR2 |= (1 << 12);	//RXNE event generated when fifo level is greater than or equal to 1/4 (8-bit)
	SPI1->CR2 |= (1 << 2); //SS output enabled in master mode
	//NSSP disabled
	//RXNE event is generated if the FIFO level is greater than or equal to 1/2 (16-bit)

	//No CRC
	//No DMA

	SPI1->CR1 |= (1 << 6); //SPI enabled
}

//Set GPIO Chip Select Low
void SPI1_Enable(void) {
	GPIOC->ODR &= ~(1 << 8);
}

//Set GPIO Chip Select High
void SPI1_Disable(void) {
	while((SPI1->SR & (1 << 7))); //wait for the BSY flag to clear
	GPIOC->ODR |= (1 << 8);
}


char dummy;
//read a dummy value everytime SPI transmits through MOSI
void SPI1_DummyRead(void) {

	while(!(SPI1->SR & (1 << 1))); //wait for transmit done (TXE flag)
	while(!(SPI1->SR & (1 << 0))); //wait for RXFIFO not empty (RXNE flag)
	dummy = *((uint8_t*) &SPI1->DR);	//read data

}

//Send a char
void SPI1_SendChar (char c) {
	//uint8_t pointer cast is needed otherwise there will be 16 clk pulses instead of 8

	*((uint8_t*) &SPI1->DR) = c;
	SPI1_DummyRead(); //dummy read
}

//Send a string
void SPI1_SendString (char * s) {

	while(*s != 0)
	{
		SPI1_SendChar(*s);
		while(!(SPI1->SR & (1 << 1))); ////wait for transmit done (TXE flag)
		s++;
	}

}

//Read a char
char SPI1_ReadChar(void) {

	char value;

	*((uint8_t*) &SPI1->DR) = 0; // send dummy data
	while(!(SPI1->SR & (1 << 1))); //wait for transmit done (TXE flag)
	while(!(SPI1->SR & (1 << 0))); //wait for RXFIFO not empty (RXNE flag)
	value = *((uint8_t*) &SPI1->DR);	//read data

	return value;
}


