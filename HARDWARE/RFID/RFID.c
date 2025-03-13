#include "RFID.h"

extern uint8_t RFID_Index;

const unsigned char validUIDs[][4] = {
    {0x94, 0xFF, 0x5B, 0x80},
    {0x8D, 0xBF, 0x67, 0x80},
    {0xA3, 0x2B, 0x59, 0xD6},
    {0x07, 0xA2, 0x7E, 0xAA},
    {0xC3, 0xA1, 0x97, 0x98}
};


// —È÷§ UID  «∑Ò∫œ∑®
unsigned char IsValidUID(unsigned char *uid) 
{
    for (int i = 0; i < VALID_UID_COUNT; i++) 
    {
        if (uid[0] == validUIDs[i][0] && uid[1] == validUIDs[i][1] &&
            uid[2] == validUIDs[i][2] && uid[3] == validUIDs[i][3]) 
        {
            return 1;
        }
    }
    return 0;
}




// ø™√≈≤Ÿ◊˜
void OpenDoor(void) {
    GPIO_ResetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN); // ¥Úø™√≈À¯
    GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN); // µ„¡¡∫œ∑®ø®∆¨Ã· æ LED
    delay_ms(3000); // ø™√≈±£≥÷ 3 √Î
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN); // πÿ±’√≈À¯
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // œ®√∫œ∑®ø®∆¨Ã· æ LED
}

// ∑«∑®ø®∆¨Ã· æ
void IllegalCardAlert(void) {
//    GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN); // µ„¡¡∑«∑®ø®∆¨Ã· æ LED
    delay_ms(3000);
   // GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN); // œ®√∑«∑®ø®∆¨Ã· æ LED
}

// ≥ı ºªØ√≈À¯∫Õ◊¥Ã¨Ã· æ LED “˝Ω≈
void DoorLockAndLED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // √≈À¯øÿ÷∆“˝Ω≈≥ı ºªØ
    GPIO_InitStructure.GPIO_Pin = DOOR_LOCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DOOR_LOCK_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);

    // ∫œ∑®ø®∆¨Ã· æ LED “˝Ω≈≥ı ºªØ
    GPIO_InitStructure.GPIO_Pin = VALID_LED_PIN;
    GPIO_Init(VALID_LED_PORT, &GPIO_InitStructure);
//    GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN);

    // ∑«∑®ø®∆¨Ã· æ LED “˝Ω≈≥ı ºªØ
//    GPIO_InitStructure.GPIO_Pin = INVALID_LED_PIN;
    GPIO_Init(INVALID_LED_PORT, &GPIO_InitStructure);
    //GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN);
}

void RFID_Test(void)
{
    
    
}
uint8_t RFID(void)
{
    unsigned char status;  // ∑µªÿ∑˚
    unsigned int temp, i;
    unsigned char buf[20]; // ¥Ê∑≈ø®–Ú∫≈
    int should_skip = 0;   // ±Íº«±‰¡ø£¨”√”⁄æˆ∂® «∑ÒÃ¯π˝∫Û–¯¥˙¬Î

    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // œ®√∫œ∑®ø®∆¨Ã· æ LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // œ®√∑«∑®ø®∆¨Ã· æ LED

    status = MFRC522_Request(PICC_REQALL, buf); // —∞ø®
    if (status != MI_OK)
    {
        MFRC522_Reset();      // ÷ÿ÷√MFRC522ƒ£øÈ
        MFRC522_AntennaOff(); // πÿ±’ÃÏœﬂ
        MFRC522_AntennaOn();  // ¥Úø™ÃÏœﬂ
        should_skip = 1;      // …Ë÷√±Íº«£¨Ã¯π˝∫Û–¯¥˙¬Î
    }

    if (!should_skip)
    {
        printf("ø®µƒ¿‡–Õ:");
        for (i = 0; i < 2; i++)
        {
            temp = buf[i];
            printf("%X", temp);
        }

        status = MFRC522_Anticoll(buf); // ∑¿≥ÂÕª
        if (status != MI_OK)
        {
            MFRC522_Reset();      // ÷ÿ÷√MFRC522ƒ£øÈ
            MFRC522_AntennaOff(); // πÿ±’ÃÏœﬂ
            MFRC522_AntennaOn();  // ¥Úø™ÃÏœﬂ
            should_skip = 1;      // …Ë÷√±Íº«£¨Ã¯π˝∫Û–¯¥˙¬Î
        }
    }

    if (!should_skip)
    {
        // “‘œ¬Œ™≥¨º∂÷’∂À¥Ú”°≥ˆµƒƒ⁄»›
        printf("ø®–Ú¡–∫≈£∫"); // ≥¨º∂÷’∂Àœ‘ æ
        for (i = 0; i < 4; i++)
        {
            // ªÒ»°buf◊Èµƒ ˝æ›
            temp = buf[i];
            // Ω´buf ˝æ›∞¥16Ω¯÷∆¥Ú”°
            printf("%X", temp);
        }
        printf("\r\n");

        // —È÷§ UID
        if (IsValidUID(buf))
        {
            OpenDoor();
            return 1;
        }
        else
        {
            IllegalCardAlert();
            return 0;
        }
    }

    // »Áπ˚÷¥––µΩ’‚¿Ô£¨Àµ√˜«∞√Ê≤Ÿ◊˜ ß∞‹£¨ø…∏˘æ›–Ë“™∑µªÿÃÿ∂®÷µ±Ì æ ß∞‹
    return 2; 
}

uint8_t RFID_last(void)
{
    unsigned char status;		//∑µªÿ∑˚
	unsigned int temp,i;
    unsigned char buf[20];  //¥Ê∑≈ø®–Ú∫≈
    
          
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // œ®√∫œ∑®ø®∆¨Ã· æ LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // œ®√∑«∑®ø®∆¨Ã· æ LED
    
    
	status = MFRC522_Request(PICC_REQALL, buf);//—∞ø®
	if (status != MI_OK)
	{    
        MFRC522_Reset();      //÷ÿ÷√MFRC522ƒ£øÈ
		MFRC522_AntennaOff(); //πÿ±’ÃÏœﬂ
		MFRC522_AntennaOn();  //¥Úø™ÃÏœﬂ
		return 0;
        //continue;
	}

		printf("ø®µƒ¿‡–Õ:");
	for(i=0;i<2;i++)
	{
		temp=buf[i];
		printf("%X",temp);
	}	
		status = MFRC522_Anticoll(buf);//∑¿≥ÂÕª
	if (status != MI_OK)
	{    
        //continue;
        return 0;        
	}
			
	////////“‘œ¬Œ™≥¨º∂÷’∂À¥Ú”°≥ˆµƒƒ⁄»›////////////////////////
        printf("ø®–Ú¡–∫≈£∫");	//≥¨º∂÷’∂Àœ‘ æ,
	for(i=0;i<4;i++)
	{
        //ªÒ»°buf◊Èµƒ ˝æ›
		temp=buf[i];
        //Ω´buf ˝æ›∞¥16Ω¯÷∆¥Ú”°
		printf("%X",temp);
	}			
        printf("\r\n");
             // —È÷§ UID
    if (IsValidUID(buf)) 
    {
        OpenDoor();
        return 1;
    } 
    else 
    {
        IllegalCardAlert();
        return 0;
    }
//        delay_ms(500); //   µ±—” ±£¨±‹√‚∆µ∑±ºÏ≤‚
    
}    

void RFID_again(void)
{
    unsigned char status;		//◊µ‹òÿª
	unsigned int temp,i;
    unsigned char buf[20];  //’¶ÿÖﬂ®—≤€Ö
    
    while(1)
    {      
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // œ®ƒ∞€è◊®ﬂ®∆¨Õ° æ LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // œ®ƒ∞ÿá◊®ﬂ®∆¨Õ° æ LED
	status = MFRC522_Request(PICC_REQALL, buf);//—∞ﬂ®
	if (status != MI_OK)
	{    
        MFRC522_Reset();      //◊ò◊ÉMFRC522ƒ£‡†©
		MFRC522_AntennaOff(); //⁄ò“ïÕ¨–ü
		MFRC522_AntennaOn();  //’≤ﬂ™Õ¨–ü
		continue;
	}

		printf("ﬂ®÷Ñ`—ç:");
	for(i=0;i<2;i++)
	{
		temp=buf[i];
		printf("%X",temp);
	}	
		status = MFRC522_Anticoll(buf);//ÿÄ‘•Õª
	if (status != MI_OK)
	{    
        continue;    
	}
			
	////////”î–ÇŒ™”¨‹∂◊ï◊ã’≤”°‘∂÷Ñ≈ö…ù////////////////////////
        printf("ﬂ®—≤¬ê€Ö√∫");	//”¨‹∂◊ï◊ã–î æ,
	for(i=0;i<4;i++)
	{
        //‹±»°bufÿ©÷ÑÀΩﬂù
		temp=buf[i];
        //›´bufÀΩﬂù–¥16ﬁ∏◊Ü’≤”°
		printf("%X",temp);
	}			
        printf("\r\n");
             // “©÷§ UID
    if (IsValidUID(buf)) 
    {
        OpenDoor();
        
    } 
    else 
    {
        IllegalCardAlert();
    }
        delay_ms(500); // Àä’±“ì ±√¨“úƒ¢∆µ◊±›¨”¢
  }
    
}

/*
    …„œÒÕ∑—È÷§≥…π¶£¨∂¡µΩ∏ﬂµÁ∆Ω,PC1
*/
uint8_t Camera(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==1)
    {
        return 1;
    }
        return 0;
}



void Door_State(DeviceTypeDef *device)
{
    if(RFID_Index==1)
    {
        if(RFID()||Camera()==1)
        {
            GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN);
            //ø™√≈
            device->door_state=1;
        }
            device->door_state=0;
        RFID_Index=0;
    }
}



