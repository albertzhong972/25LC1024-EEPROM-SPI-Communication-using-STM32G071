/*
 * spi.h
 *
 *  Created on: Mar 24, 2021
 *      Author: Albert Zhong
 */

#ifndef SPI_H_
#define SPI_H_



#endif /* SPI_H_ */
#include <../Inc/stm32g071xx.h>


void SPI1_Config(void);
void SPI1_SendChar (char c);
void SPI1_SendString (char * s);
char SPI1_ReadChar(void);
void SPI1_Enable(void);
void SPI1_Disable(void);
void SPI1_DummyRead(void);



