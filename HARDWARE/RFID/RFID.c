#include "RFID.h"

extern uint8_t RFID_Index;

const unsigned char validUIDs[][4] = {
    {0x94, 0xFF, 0x5B, 0x80},
    {0x8D, 0xBF, 0x67, 0x80},
    {0xA3, 0x2B, 0x59, 0xD6},
    {0x07, 0xA2, 0x7E, 0xAA},
    {0xC3, 0xA1, 0x97, 0x98}
};


// 验证 UID 是否合法
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




// 开门操作
void OpenDoor(void) {
    GPIO_ResetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN); // 打开门锁
    GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN); // 点亮合法卡片提示 LED
    delay_ms(3000); // 开门保持 3 秒
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN); // 关闭门锁
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // 熄灭合法卡片提示 LED
}

// 非法卡片提示
void IllegalCardAlert(void) {
//    GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN); // 点亮非法卡片提示 LED
    delay_ms(3000);
   // GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN); // 熄灭非法卡片提示 LED
}

// 初始化门锁和状态提示 LED 引脚
void DoorLockAndLED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 门锁控制引脚初始化
    GPIO_InitStructure.GPIO_Pin = DOOR_LOCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DOOR_LOCK_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);

    // 合法卡片提示 LED 引脚初始化
    GPIO_InitStructure.GPIO_Pin = VALID_LED_PIN;
    GPIO_Init(VALID_LED_PORT, &GPIO_InitStructure);
//    GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN);

    // 非法卡片提示 LED 引脚初始化
//    GPIO_InitStructure.GPIO_Pin = INVALID_LED_PIN;
    GPIO_Init(INVALID_LED_PORT, &GPIO_InitStructure);
    //GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN);
}

void RFID_Test(void)
{
    
    
}
uint8_t RFID(void)
{
    unsigned char status;  // 返回符
    unsigned int temp, i;
    unsigned char buf[20]; // 存放卡序号
    int should_skip = 0;   // 标记变量，用于决定是否跳过后续代码

    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // 熄灭合法卡片提示 LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // 熄灭非法卡片提示 LED

    status = MFRC522_Request(PICC_REQALL, buf); // 寻卡
    if (status != MI_OK)
    {
        MFRC522_Reset();      // 重置MFRC522模块
        MFRC522_AntennaOff(); // 关闭天线
        MFRC522_AntennaOn();  // 打开天线
        should_skip = 1;      // 设置标记，跳过后续代码
    }

    if (!should_skip)
    {
        printf("卡的类型:");
        for (i = 0; i < 2; i++)
        {
            temp = buf[i];
            printf("%X", temp);
        }

        status = MFRC522_Anticoll(buf); // 防冲突
        if (status != MI_OK)
        {
            MFRC522_Reset();      // 重置MFRC522模块
            MFRC522_AntennaOff(); // 关闭天线
            MFRC522_AntennaOn();  // 打开天线
            should_skip = 1;      // 设置标记，跳过后续代码
        }
    }

    if (!should_skip)
    {
        // 以下为超级终端打印出的内容
        printf("卡序列号："); // 超级终端显示
        for (i = 0; i < 4; i++)
        {
            // 获取buf组的数据
            temp = buf[i];
            // 将buf数据按16进制打印
            printf("%X", temp);
        }
        printf("\r\n");

        // 验证 UID
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

    // 如果执行到这里，说明前面操作失败，可根据需要返回特定值表示失败
    return 2; 
}

uint8_t RFID_last(void)
{
    unsigned char status;		//返回符
	unsigned int temp,i;
    unsigned char buf[20];  //存放卡序号
    
          
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // 熄灭合法卡片提示 LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // 熄灭非法卡片提示 LED
    
    
	status = MFRC522_Request(PICC_REQALL, buf);//寻卡
	if (status != MI_OK)
	{    
        MFRC522_Reset();      //重置MFRC522模块
		MFRC522_AntennaOff(); //关闭天线
		MFRC522_AntennaOn();  //打开天线
		return 0;
        //continue;
	}

		printf("卡的类型:");
	for(i=0;i<2;i++)
	{
		temp=buf[i];
		printf("%X",temp);
	}	
		status = MFRC522_Anticoll(buf);//防冲突
	if (status != MI_OK)
	{    
        //continue;
        return 0;        
	}
			
	////////以下为超级终端打印出的内容////////////////////////
        printf("卡序列号：");	//超级终端显示,
	for(i=0;i<4;i++)
	{
        //获取buf组的数据
		temp=buf[i];
        //将buf数据按16进制打印
		printf("%X",temp);
	}			
        printf("\r\n");
             // 验证 UID
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
//        delay_ms(500); // 适当延时，避免频繁检测
    
}    

void RFID_again(void)
{
    unsigned char status;		//椎軜鼗
	unsigned int temp,i;
    unsigned char buf[20];  //咋貐擗巡蹍
    
    while(1)
    {      
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // 熄陌蹚专擗片汀示 LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // 熄陌貒专擗片汀示 LED
	status = MFRC522_Request(PICC_REQALL, buf);//寻擗
	if (status != MI_OK)
	{    
        MFRC522_Reset();      //讟變MFRC522模酄�
		MFRC522_AntennaOff(); //跇視同袩
		MFRC522_AntennaOn();  //詹擢同袩
		continue;
	}

		printf("擗謩`褝:");
	for(i=0;i<2;i++)
	{
		temp=buf[i];
		printf("%X",temp);
	}	
		status = MFRC522_Anticoll(buf);//貈豫突
	if (status != MI_OK)
	{    
        continue;    
	}
			
	////////訑袀为蝇芏讜讒詹印远謩艢蓾////////////////////////
        printf("擗巡聬蹍煤");	//蝇芏讜讒袛示,
	for(i=0;i<4;i++)
	{
        //鼙取buf丞謩私邼
		temp=buf[i];
        //莴buf私邼写16薷讍詹印
		printf("%X",temp);
	}			
        printf("\r\n");
             // 药证 UID
    if (IsValidUID(buf)) 
    {
        OpenDoor();
        
    } 
    else 
    {
        IllegalCardAlert();
    }
        delay_ms(500); // 藠毡覔时矛覝蘑频妆莠英
  }
    
}

/*
    摄像头验证成功，读到高电平,PC1
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
            //开门
            device->door_state=1;
        }
            device->door_state=0;
        RFID_Index=0;
    }
}



