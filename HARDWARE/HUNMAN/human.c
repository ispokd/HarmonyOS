#include "human.h"
#include "stm32f10x.h"                  // Device header


  /*
    人体红外传感器
    in->PC0
  */

void human_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
}





