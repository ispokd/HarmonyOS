/***************STM32F103R8T6**********************

********************LIGEN*************************/

#include "usart.h"
#include <stdarg.h>
#include "string.h"
#include "led.h"
#include "wifi.h"
//#define BUFFER_SIZE 256

////uint8_t uart1_rec_i = 0;
//extern uint8_t atok_rec_flag;
////extern uint16_t speed;

////char uart1_recdata = 0, uart1_recstring[256], sub_string[150], analysis_Str[256];

//uint16_t speed_flag;
//char buffer[BUFFER_SIZE];
////int bufferIndex = 0;
////char led_value[10];

#define BUFFER_SIZE 256
uint8_t uart1_rec_i = 0;
extern uint8_t atok_rec_flag;
char uart1_recdata = 0, uart1_recstring[256], sub_string[150], analysis_Str[256];
extern uint16_t speed;
uint16_t speed_flag;
char buffer[BUFFER_SIZE];
int bufferIndex = 0;
char led_value[10];



 
//void USART1_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	/* 使能 USART1 时钟*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

//	/* USART1 使用IO端口配置 */    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);    
//  
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
//    GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
//	  
//	/* USART1 工作模式配置 */
//	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
//	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
//	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
//	USART_Cmd(USART1, ENABLE);// USART1使能
//}


void usart_init(void) {
    GPIO_InitTypeDef gpio_init;
    NVIC_InitTypeDef nvic_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    gpio_init.GPIO_Pin = GPIO_Pin_9;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init);

    gpio_init.GPIO_Pin = GPIO_Pin_10;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_init);

    USART_InitTypeDef usart_init;
    usart_init.USART_BaudRate = 115200;
    usart_init.USART_WordLength = USART_WordLength_8b;
    usart_init.USART_StopBits = USART_StopBits_1;
    usart_init.USART_Parity = USART_Parity_No;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &usart_init);
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    nvic_init.NVIC_IRQChannel = USART1_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);
}

void usart_init2(void)
{
    GPIO_InitTypeDef GPIO_Init_Structure;                            //??GPIO???
    USART_InitTypeDef USART_Init_Structure;                          //???????
	NVIC_InitTypeDef  NVIC_Init_Structure;														//???????
	
    RCC_APB2PeriphClockCmd(USART2_GPIO_CLK,  ENABLE);                 //??GPIOA??
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);            //??APB2??????
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,  ENABLE);          //??USART1??
    
    //??PA2 TX
    GPIO_Init_Structure.GPIO_Mode  = GPIO_Mode_AF_PP;                //????
    GPIO_Init_Structure.GPIO_Pin   = USART2_TX_GPIO_PIN;
    GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_10MHz;
    
    GPIO_Init( USART2_GPIO_PORT, &GPIO_Init_Structure);
    
    //??PA3 RX
    GPIO_Init_Structure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                //????
    GPIO_Init_Structure.GPIO_Pin   = USART2_RX_GPIO_PIN;
    GPIO_Init( USART2_GPIO_PORT, &GPIO_Init_Structure);
	
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	
    USART_Init_Structure.USART_BaudRate = 115200;                                          //??????115200
    USART_Init_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;       //???????
    USART_Init_Structure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                       //???????
    USART_Init_Structure.USART_Parity = USART_Parity_No;                                   //????
    USART_Init_Structure.USART_StopBits = USART_StopBits_1;                                //?????
    USART_Init_Structure.USART_WordLength = USART_WordLength_8b;                           //???8?  
    USART_Init(USART2, &USART_Init_Structure);  
    USART_Cmd(USART2, ENABLE);
		
		//???????
	NVIC_Init_Structure.NVIC_IRQChannel 			=   USART2_IRQn;
	NVIC_Init_Structure.NVIC_IRQChannelCmd   	=   ENABLE;
	NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority  =  1;
	NVIC_Init_Structure.NVIC_IRQChannelSubPriority         =  3;
	NVIC_Init(&NVIC_Init_Structure);
}

void usary(char uart1_buffer[]) {
    for (int i = 0; uart1_buffer[i] != '\0'; i++) {
        // ????USART2??????????
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        // ??????
        USART_SendData(USART2, uart1_buffer[i]);
        // ????????
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
}




void USART2_IRQHandler(void)                    //??1??????
{
    char str_temp[128];
    char request_id[48];
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //????
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);            //??????
        uart1_recdata = USART_ReceiveData(USART2);
        USART_SendData(USART1, uart1_recdata);			
        if (uart1_recdata != '\n') {
            //??????????
            if ((uart1_rec_i == 0) && (uart1_recdata == ' ')) {
                //???????MQTT?????????,?????????????
                //????????????,????????,????-2022.9.3.1
            } else {
                uart1_recstring[uart1_rec_i] = uart1_recdata;
                //printf("%c",uart1_recstring[uart1_rec_i]);		//?????
                uart1_rec_i++;    //????????
            }
        } else {
            //printf("Rec Over,uart1_rec_i=%d,data=%s\r\n",uart1_rec_i,uart1_recstring);//?????,??????????
            if (uart1_rec_i >= 2) {
                strncpy(sub_string, uart1_recstring, 2);    //?????
                if (strcmp(sub_string, "OK") == 0) {
                    atok_rec_flag = 1;
                    //printf("Get Ok\r\n");//?????,?????????"ok"
                }

                strncpy(sub_string, uart1_recstring, 5);
                if (strcmp(sub_string, "ready") == 0) {
                    atok_rec_flag = 1;
                    //printf("Get ready\r\n");//?????,?????????圆eady詬
                }

                strncpy(sub_string, uart1_recstring, 5);
                if (strcmp(sub_string, "ERROR") == 0) {
                    atok_rec_flag = 0;
                    //printf("Get ERROR\r\n");//?????,?????????詤RROR詬
                }
                //?????????201,????JSON?????????,??????????????????
                //+MQTTSUBRECV:0,"$oc/devices/61fb2d7fde9933029funiot_esp8266_test01/sys/commands/request_id=4152fb5d-e5ae-4b89-b39d-283ba59cf033",68,{"paras":{"led":1},"service_id":"Dev_data","command_name":"Control"}
                if (uart1_rec_i >= 190) {
                    strncpy(analysis_Str, uart1_recstring, 12);                            //???荧MQTTSUBRECV詬
                    if (strcmp(analysis_Str, "+MQTTSUBRECV") == 0) {
                        memset(analysis_Str, 0, sizeof(analysis_Str));                    //?????
                        //printf("MQTT???????\r\n");
                        strncpy(request_id, uart1_recstring + 87, 36);   
                         request_id[36] = '\0';                           // ???????											//???request_id
                        strncpy(analysis_Str, uart1_recstring + 130, 5);            //???"paras"
                        if (strcmp(analysis_Str, "paras") == 0)                                //?????
                        {
                            memset(analysis_Str, 0, sizeof(analysis_Str));    //?????
                            strncpy(analysis_Str, uart1_recstring + 139, 3);    //???"led"
                            printf("att is %s\r\n",analysis_Str);
                            if (strcmp(analysis_Str, "led") == 0) {
                                printf("led set %c\r\n",uart1_recstring[145]);
                            strncpy(analysis_Str, uart1_recstring + 144, 8);
															if(strstr(analysis_Str,"OFF")!=NULL){
																	LED_Off();
                                } else if (strstr(analysis_Str,"ON")!=NULL) {
																	LED_On();
                                }
                                printf("AT+MQTTPUB=0,\"$oc/devices/%s/sys/commands/response/request_id=%s\",\"\",0,0\r\n",
                                       HUAWEI_MQTT_DeviceID, request_id); 
                                snprintf(buffer, sizeof(buffer), "AT+MQTTPUB=0,\"$oc/devices/%s/sys/commands/response/request_id=%s\",\"\",0,0\r\n", HUAWEI_MQTT_DeviceID, request_id);
																printf(buffer);
                                memset(analysis_Str, 0, sizeof(analysis_Str));//?????  		
                                memset(str_temp, 0, sizeof(str_temp));                //?????  		
																
                            }
                        }
                    }

                }
            }
            uart1_rec_i = 0;
            memset(uart1_recstring, 0, sizeof(uart1_recstring));//??uart1_recstring???
            memset(sub_string, 0, sizeof(sub_string));                    //??sub_string???				
        }
    }
}


//void USART2_IRQHandler(void) {
//    if (USART_GetITStatus(USART2, USART_IT_RXNE)) {
//        
//        char buffer[BUFFER_SIZE];
//        int bufferIndex = 0;
//        uint8_t receivedData = USART_ReceiveData(USART2);
//        buffer[bufferIndex++] = receivedData;
//        
//        USART_SendData(USART2, receivedData);
//    }
//}

int fputc(int ch, FILE *f)
{
		USART_SendData(USART2, (uint16_t) ch);
		
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}
 
int fgetc(FILE *f)
{

		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
 
		return (int)USART_ReceiveData(USART2);
}



