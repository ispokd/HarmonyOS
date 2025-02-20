#include "delay.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "wifi.h"

uint8_t atok_rec_flag;


void HuaweiIot_init(void)
{	
		char str_temp[256];		
		delay_ms(1000);			
    for(uint8_t i=0;i<10;i++)
    {
        if(atok_rec_flag==1)
        {
            AT_write("AT");				
            printf("AT+RST\r\n");		
            delay_ms(2000);					
            AT_write("AT");					
            AT_write("AT+CWMODE=1");					
			sprintf(str_temp,"AT+CWJAP=\"%s\",\"%s\"",WIFI_SSID,WIFI_PWD);	//??WiFi
            AT_write(str_temp);					
			sprintf(str_temp,"AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"",HUAWEI_MQTT_USERNAME,HUAWEI_MQTT_PASSWORD);//??MQTT?????????
            AT_write(str_temp);
			sprintf(str_temp,"AT+MQTTCLIENTID=0,\"%s\"",HUAWEI_MQTT_ClientID);//??MQTT?ClientID
            AT_write(str_temp);
			sprintf(str_temp,"AT+MQTTCONN=0,\"%s\",%s,1",HUAWEI_MQTT_ADDRESS,HUAWEI_MQTT_PORT);//??MQTT????????
			AT_write(str_temp);
			sprintf(str_temp,"AT+MQTTSUB=0,\"$oc/devices/%s/sys/properties/report\",1",HUAWEI_MQTT_DeviceID);	//???????????
			AT_write(str_temp);
			sprintf(str_temp,"AT+MQTTSUB=0,\"$oc/devices/%s/sys/commands/#\",1",HUAWEI_MQTT_DeviceID);//???????????
            AT_write(str_temp);
			sprintf(str_temp,"AT+MQTTSUB=0,\"$oc/devices/%s/sys/commands/response/#\",1",HUAWEI_MQTT_DeviceID);//???????????
            AT_write(str_temp);
            break;
        }    
        else
        {
						if(i==9)
						{
							printf("Connected ESP01s Fail!\r\n");	//????
							break;
						}
						else 
						{
							printf("AT\r\n");
							delay_ms(5000);
						}
						
        }            
    }
}

void AT_write(char atstring[512])
{
    atok_rec_flag=0;
    printf("%s\r\n",atstring);
		while(1)
		{
			if(atok_rec_flag==1)	
			{
				atok_rec_flag=0;
				break;
			}
			else 
				delay_ms(50);
		}	
}

void HuaweiIot_DevDate_publish(char *att,uint16_t data)
{
	printf("AT+MQTTPUB=0,\"$oc/devices/%s/sys/properties/report\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"%s\\\"\\,\\\"properties\\\":{\\\"%s\\\": %d}}]}\",0,0\r\n",HUAWEI_MQTT_DeviceID,HUAWEI_MQTT_ServiceID,att,data);
}
