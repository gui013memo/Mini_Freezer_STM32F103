/*
 * ds18b20.h
 *
 *  Created on: Feb 6, 2022
 *      Author: GUI013
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

/* ----- FUNCTIONS GUILHERME ----- */
float DS18B20_Run(uint8_t *Presence);
/* ----- FUNCTIONS GUILHERME END ----- */

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

uint8_t DS18B20_Start(void);

void DS18B20_Write(uint8_t data);

uint8_t DS18B20_Read(void);


#endif /* INC_DS18B20_H_ */
