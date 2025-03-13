#include "RFID.h"

extern uint8_t RFID_Index;

const unsigned char validUIDs[][4] = {
    {0x94, 0xFF, 0x5B, 0x80},
    {0x8D, 0xBF, 0x67, 0x80},
    {0xA3, 0x2B, 0x59, 0xD6},
    {0x07, 0xA2, 0x7E, 0xAA},
    {0xC3, 0xA1, 0x97, 0x98}
};


// ��֤ UID �Ƿ�Ϸ�
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




// ���Ų���
void OpenDoor(void) {
    GPIO_ResetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN); // ������
    GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN); // �����Ϸ���Ƭ��ʾ LED
    delay_ms(3000); // ���ű��� 3 ��
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN); // �ر�����
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // Ϩ��Ϸ���Ƭ��ʾ LED
}

// �Ƿ���Ƭ��ʾ
void IllegalCardAlert(void) {
//    GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN); // �����Ƿ���Ƭ��ʾ LED
    delay_ms(3000);
   // GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN); // Ϩ��Ƿ���Ƭ��ʾ LED
}

// ��ʼ��������״̬��ʾ LED ����
void DoorLockAndLED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // �����������ų�ʼ��
    GPIO_InitStructure.GPIO_Pin = DOOR_LOCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DOOR_LOCK_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);

    // �Ϸ���Ƭ��ʾ LED ���ų�ʼ��
    GPIO_InitStructure.GPIO_Pin = VALID_LED_PIN;
    GPIO_Init(VALID_LED_PORT, &GPIO_InitStructure);
//    GPIO_ResetBits(VALID_LED_PORT, VALID_LED_PIN);

    // �Ƿ���Ƭ��ʾ LED ���ų�ʼ��
//    GPIO_InitStructure.GPIO_Pin = INVALID_LED_PIN;
    GPIO_Init(INVALID_LED_PORT, &GPIO_InitStructure);
    //GPIO_ResetBits(INVALID_LED_PORT, INVALID_LED_PIN);
}

void RFID_Test(void)
{
    
    
}
uint8_t RFID(void)
{
    unsigned char status;  // ���ط�
    unsigned int temp, i;
    unsigned char buf[20]; // ��ſ����
    int should_skip = 0;   // ��Ǳ��������ھ����Ƿ�������������

    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // Ϩ��Ϸ���Ƭ��ʾ LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // Ϩ��Ƿ���Ƭ��ʾ LED

    status = MFRC522_Request(PICC_REQALL, buf); // Ѱ��
    if (status != MI_OK)
    {
        MFRC522_Reset();      // ����MFRC522ģ��
        MFRC522_AntennaOff(); // �ر�����
        MFRC522_AntennaOn();  // ������
        should_skip = 1;      // ���ñ�ǣ�������������
    }

    if (!should_skip)
    {
        printf("��������:");
        for (i = 0; i < 2; i++)
        {
            temp = buf[i];
            printf("%X", temp);
        }

        status = MFRC522_Anticoll(buf); // ����ͻ
        if (status != MI_OK)
        {
            MFRC522_Reset();      // ����MFRC522ģ��
            MFRC522_AntennaOff(); // �ر�����
            MFRC522_AntennaOn();  // ������
            should_skip = 1;      // ���ñ�ǣ�������������
        }
    }

    if (!should_skip)
    {
        // ����Ϊ�����ն˴�ӡ��������
        printf("�����кţ�"); // �����ն���ʾ
        for (i = 0; i < 4; i++)
        {
            // ��ȡbuf�������
            temp = buf[i];
            // ��buf���ݰ�16���ƴ�ӡ
            printf("%X", temp);
        }
        printf("\r\n");

        // ��֤ UID
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

    // ���ִ�е����˵��ǰ�����ʧ�ܣ��ɸ�����Ҫ�����ض�ֵ��ʾʧ��
    return 2; 
}

uint8_t RFID_last(void)
{
    unsigned char status;		//���ط�
	unsigned int temp,i;
    unsigned char buf[20];  //��ſ����
    
          
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // Ϩ��Ϸ���Ƭ��ʾ LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // Ϩ��Ƿ���Ƭ��ʾ LED
    
    
	status = MFRC522_Request(PICC_REQALL, buf);//Ѱ��
	if (status != MI_OK)
	{    
        MFRC522_Reset();      //����MFRC522ģ��
		MFRC522_AntennaOff(); //�ر�����
		MFRC522_AntennaOn();  //������
		return 0;
        //continue;
	}

		printf("��������:");
	for(i=0;i<2;i++)
	{
		temp=buf[i];
		printf("%X",temp);
	}	
		status = MFRC522_Anticoll(buf);//����ͻ
	if (status != MI_OK)
	{    
        //continue;
        return 0;        
	}
			
	////////����Ϊ�����ն˴�ӡ��������////////////////////////
        printf("�����кţ�");	//�����ն���ʾ,
	for(i=0;i<4;i++)
	{
        //��ȡbuf�������
		temp=buf[i];
        //��buf���ݰ�16���ƴ�ӡ
		printf("%X",temp);
	}			
        printf("\r\n");
             // ��֤ UID
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
//        delay_ms(500); // �ʵ���ʱ������Ƶ�����
    
}    

void RFID_again(void)
{
    unsigned char status;		//׵ܘػ
	unsigned int temp,i;
    unsigned char buf[20];  //զ؅ߨѲۅ
    
    while(1)
    {      
    GPIO_SetBits(DOOR_LOCK_PORT, DOOR_LOCK_PIN);
    GPIO_SetBits(VALID_LED_PORT, VALID_LED_PIN); // ϨİۏרߨƬ͡ʾ LED
//    GPIO_SetBits(INVALID_LED_PORT, INVALID_LED_PIN); // Ϩİ؇רߨƬ͡ʾ LED
	status = MFRC522_Request(PICC_REQALL, buf);//Ѱߨ
	if (status != MI_OK)
	{    
        MFRC522_Reset();      //ט׃MFRC522ģࠩ
		MFRC522_AntennaOff(); //ژҕͬП
		MFRC522_AntennaOn();  //ղߪͬП
		continue;
	}

		printf("ߨք`э:");
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
        printf("ߨѲۅú");	//Ӭܶו׋Дʾ,
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
        OpenDoor();
        
    } 
    else 
    {
        IllegalCardAlert();
    }
        delay_ms(500); // ˊձғʱìҜĢƵױݬӢ
  }
    
}

/*
    ����ͷ��֤�ɹ��������ߵ�ƽ,PC1
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
            //����
            device->door_state=1;
        }
            device->door_state=0;
        RFID_Index=0;
    }
}



