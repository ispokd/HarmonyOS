#include "adcx.h"
#include "stm32f10x.h"                  // Device header
#include "led.h"
/**
  * @brief  ADC��ʼ������
  * @param  
  * @retval 
  */
void ADCx_Init(void)
{
	//����ADC��GPIOʱ��
	RCC_APB2PeriphClockCmd(ADC_CLK|RCC_APB2Periph_GPIOA, ENABLE);
	
	//ADCƵ�ʽ���6��Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
    
    //����GPIO�ṹ��
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
	//����ADC�ṹ��
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//����ת��
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//��ɨ��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//��ͨ����
	ADC_Init(ADCx, &ADC_InitStructure);	//��ʼ��ADC1
	
	//����ADCx
	ADC_Cmd(ADCx, ENABLE);
	
	//����ADCУ׼
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx) == SET);
	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx) == SET);	
}



/**
  * @brief  ��ȡADCת���������
  * @param  ADC_Channel 	ѡ����Ҫ�ɼ���ADCͨ��
  * @param  ADC_SampleTime  ѡ����Ҫ����ʱ��
  * @retval ����ת�����ģ���ź���ֵ
  */
uint16_t ADC_GetValue(uint8_t ADC_Channel,uint8_t ADC_SampleTime)
{
	//����ADCͨ��
	ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime);
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE); //�������ADCת��
	while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET); //��ȡADCת����ɱ�־λ
	return ADC_GetConversionValue(ADCx);
}



void ADC_Test(DeviceTypeDef *device)
{
    if(ADC_GetValue(ADC_Channel_1,50)>=3500)
    {
        device->light_state=1;
        LED_On();
    }
    else
    {
        device->light_state=0;
        LED_Off();
    }
}

