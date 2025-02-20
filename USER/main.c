#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "DHT11.h"
#include "main.h"
#include "RFID.h"
#include "adcx.h"
#include "human.h"
#include "time.h"
#include "wifi.h"
#include "RFID.h"

unsigned char status;		//׵ܘػ
unsigned int temp,i;
unsigned char buf[20];  //զ؅ߨѲۅ
//unsigned char buf[20];  // ��ſ����
//extern unsigned int rec_data[4];

uint8_t RFID_Index=0;

DeviceTypeDef DeviceState;


int main(void)
{ 	    
    SystemInit();
	delay_init(72);
 	LED_Init();
	OLED_Init();
	
	MFRC522_Init();//RFID初始化
    DoorLockAndLED_Init(); //继电器初始化	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_ms(1000);
    ADCx_Init();//光敏传感器    
    human_Init();    
    DHT11_Start();//温湿度传感器
    LED_Off();    
    Time_Init();
    
   
    usart_init();
	usart_init2();
    OLED_ShowString(1,1,"hello");
//	HuaweiIot_init();    //???????????
    
    
    
while (1)
    {   
        
//    delay_ms(1);
//    DHT11_REC_Data(&DeviceState);
//    HuaweiIot_DevDate_publish("light",ADC_vol());
//	  delay_ms(100);
//    HuaweiIot_DevDate_publish("temperature",DeviceState.temperature);
//    delay_ms(100);
//	  HuaweiIot_DevDate_publish("humidity",DeviceState.humidity);
//	  delay_ms(5000);
//       
        
//       DHT11_REC_Data(&DeviceState);
//       printf("temperature:%d.%d\r\n",DeviceState.temperature,DeviceState.temperature_decimal);       
//       printf("humidity:%d.%d\r\n",   DeviceState.humidity,DeviceState.humidity_decimal);
//       delay_ms(500);
//      
//       OLED_ShowString(1,1,"Light:");
//       OLED_ShowNum(2,1,ADC_GetValue(ADC_Channel_1,50),5);
//       ADC_Test(&DeviceState);
//       printf("Light:%d\r\n",DeviceState.light_state);
         
       
      Door_State(&DeviceState);
        
//    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
//    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // ϨİۏרߨƬ͡ʾ LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // Ϩİ؇רߨƬ͡ʾ LED
//	status = MFRC522_Request(PICC_REQALL, buf);//Ѱߨ
//	if (status != MI_OK)
//	{    
//        MFRC522_Reset();      //ט׃MFRC522ģ�?
//		MFRC522_AntennaOff(); //ژҕͬП
//		MFRC522_AntennaOn();  //ղߪͬП
//		continue;
//	}

//		printf("ߨք`э:");
//	for(i=0;i<2;i++)
//	{
//		temp=buf[i];
//		printf("%X",temp);
//	}	
//		status = MFRC522_Anticoll(buf);//؀ԥͻ
//	if (status != MI_OK)
//	{    
//        continue;    
//	}
//			
//	////////ӔЂΪӬܶו׋ղӡԶքŚɝ////////////////////////
//        printf("ߨѲۅú");	//Ӭܶו׋Дʾ,
//	for(i=0;i<4;i++)
//	{
//        //ܱȡbufةք˽ߝ
//		temp=buf[i];
//        //ݫbuf˽ߝд16޸׆ղӡ
//		printf("%X",temp);
//	}			
//        printf("\r\n");
//             // ҩ֤ UID
//    if (IsValidUID(buf)) 
//    {
//        OpenDoor();
//        GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN);
//    } 
//    else 
//    {
//        IllegalCardAlert();
//    }
//        delay_ms(500); // ˊձғʱìҜĢƵױݬӢ
  
    
        
    }  

}
