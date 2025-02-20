#include "time.h"
#include "stdio.h"
#include "main.h"


extern uint8_t RFID_Index;
void Time_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=999;
    TIM_TimeBaseInitStructure.TIM_Prescaler=7199;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
    
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
    
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
    
    TIM_Cmd(TIM1,ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    NVIC_Init(&NVIC_InitStructure);
    
}



void TIM1_UP_IRQHandler()
{
    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
    {

         RFID_Index=1;    
         TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
    }
    
}






