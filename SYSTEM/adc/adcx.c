#include "adcx.h"
#include "stm32f10x.h"                  // Device header
#include "led.h"
/**
  * @brief  ADC初始化程序
  * @param  
  * @retval 
  */
void ADCx_Init(void)
{
	//开启ADC和GPIO时钟
	RCC_APB2PeriphClockCmd(ADC_CLK|RCC_APB2Periph_GPIOA, ENABLE);
	
	//ADC频率进行6分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
    
    //配置GPIO结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
	//配置ADC结构体
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//单次转换
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//非扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//总通道数
	ADC_Init(ADCx, &ADC_InitStructure);	//初始化ADC1
	
	//开启ADCx
	ADC_Cmd(ADCx, ENABLE);
	
	//进行ADC校准
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx) == SET);
	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx) == SET);	
}



/**
  * @brief  获取ADC转换后的数据
  * @param  ADC_Channel 	选择需要采集的ADC通道
  * @param  ADC_SampleTime  选择需要采样时间
  * @retval 返回转换后的模拟信号数值
  */
uint16_t ADC_GetValue(uint8_t ADC_Channel,uint8_t ADC_SampleTime)
{
	//配置ADC通道
	ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime);
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE); //软件触发ADC转换
	while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET); //读取ADC转换完成标志位
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

