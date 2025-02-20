#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>



//??1
#define USART1_GPIO_PORT      GPIOA
#define USART1_GPIO_CLK       RCC_APB2Periph_GPIOA
#define USART1_TX_GPIO_PIN    GPIO_Pin_9
#define USART1_RX_GPIO_PIN    GPIO_Pin_10

//??2
#define USART2_GPIO_PORT      GPIOA
#define USART2_GPIO_CLK       RCC_APB2Periph_GPIOA
#define USART2_TX_GPIO_PIN    GPIO_Pin_2
#define USART2_RX_GPIO_PIN    GPIO_Pin_3

//??3
#define USART3_GPIO_PORT      GPIOB
#define USART3_GPIO_CLK       RCC_APB2Periph_GPIOB
#define USART3_TX_GPIO_PIN    GPIO_Pin_10
#define USART3_RX_GPIO_PIN    GPIO_Pin_11

//void USART1_Config(void);
//void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);


void usart_init(void);
void usart_init2(void);
void usart_init3(void);
void Init_Usart(void);
void USART_Send_Byte(USART_TypeDef* USARTx, uint16_t Data);
void USART_Send_String(USART_TypeDef* USARTx, char *str);
void printf1(void);

void String_test(void);

#endif /* __USART1_H */
