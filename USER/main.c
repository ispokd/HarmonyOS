#include "stm32f10x.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "oled.h"
#include "main.h"
#include "RFID.h"
#include "human.h"
#include "wifi.h"
#include "RFID.h"
#include "buzzer.h"
#include "usart.h"
unsigned char status;		//׵ܘػ
unsigned int temp,i;
unsigned char buf[20];  //զ؅ߨѲۅ
//unsigned char buf[20];  // ��ſ����
//extern unsigned int rec_data[4];

uint8_t RFID_Index=0;

DeviceTypeDef DeviceState;

uint8_t value;

int main(void)
{ 	    
     SystemInit();
	 delay_init(72);
 	 LED_Init();
	 OLED_Init();
   
	MFRC522_Init();//RFID初始化
    DoorLockAndLED_Init(); //继电器初始化	
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//  delay_ms(1000);
    key_Init();
    human_Init();    
	buzzer_Init();
	camera_Init();
	door_Init();
    usart_init();
    usart_init2();
    
    OLED_ShowString(1,1,"hello");
    HuaweiIot_init();    //???????????
    LED_Off();
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    
    
while (1)
    {   

 

	    delay_ms(100);
//      HuaweiIot_DevDate_publish("temperature",DeviceState.temperature);
//      delay_ms(100);
			 
//	    HuaweiIot_DevDate_publish("door_state",DeviceState.door_state=0);
	    delay_ms(500);
       
        if(check_password()==1)
	    {
            GPIO_ResetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
	        HuaweiIot_DevDate_publish("door_state",DeviceState.door_state=1);          
            delay_ms(3000); // 显示2秒           
            GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);          
            HuaweiIot_DevDate_publish("door_state",DeviceState.door_state=0);				 
		}
   
        
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_9); // 开锁          
			HuaweiIot_DevDate_publish("door_state",DeviceState.door_state=1);
		    delay_ms(3000); // 2秒
		    delay_ms(3000); // 2秒  
           
		    GPIO_SetBits(GPIOB, GPIO_Pin_9); // 关闭开锁   
            delay_ms(3000);
            HuaweiIot_DevDate_publish("door_state",DeviceState.door_state=0);		 		
		}
     
        
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
        {
            delay_ms(50);
            if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
		    {
                GPIO_ResetBits(GPIOC, GPIO_Pin_0); // 开启蜂鸣器
                delay_ms(2000); // 2秒
                GPIO_SetBits(GPIOC, GPIO_Pin_0); // 开启蜂鸣器
            }                
		}		
    //GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    //GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // ϨİۏרߨƬ͡ʾ LED
    //GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // Ϩİ؇רߨƬ͡ʾ LED
  //  printf("hello:");
    	
        
	status = MFRC522_Request(PICC_REQALL, buf);//Ѱߨ
	if (status != MI_OK)
	{    
        MFRC522_Reset();      //ט׃MFRC522ģ�?
		MFRC522_AntennaOff(); //ژҕͬП
		MFRC522_AntennaOn();  //ղߪͬП
		continue;
	}

	printf("");
	for(i=0;i<2;i++)
	{
	temp=buf[i];
	printf("%X",temp);
	}	
		status = MFRC522_Anticoll(buf);//؀ԥͻ
	if (status != MI_OK)
	{    
        continue;    
	}
			
	////////ӔЂΪӬܶו׋ղӡԶքŚɝ////////////////////////
        printf("\r\n");	//Ӭܶו׋Дʾ,
	for(i=0;i<4;i++)
	{
        //ܱȡbufةք˽ߝ
		temp=buf[i];
        //ݫbuf˽ߝд16޸׆ղӡ
		printf("%X",temp);
	}			
        printf("\r\n");
             // ҩ֤ UID
    if (IsValidUID(buf)) 
    {
        HuaweiIot_DevDate_publish("door_state",DeviceState.door_state=1);
        OpenDoor();
        HuaweiIot_DevDate_publish("door_state",DeviceState.door_state=0);
    //    GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN);
    } 
    else 
    {
        IllegalCardAlert();
        
    }
        delay_ms(100); // ˊձғʱìҜĢƵױݬ       
    }  

	}
