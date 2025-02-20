
#ifndef __wifi_H
#define __wifi_H

///
/*??????*/
#include "stm32f10x.h"                  // Device header
///
/*????*/
//?????????????
#define WIFI_SSID						"Success"  //WiFi?
#define WIFI_PWD						"123456789"  //WiFi??
#define HUAWEI_MQTT_USERNAME			"6786593eef99673c8adfd47b_stm32_dev"  //Username
#define HUAWEI_MQTT_PASSWORD			"45bd14bf62e53245b27bc115244cab6bbb3958c2a6ce0c7fb5bc0eb5f9f5ff26"  //Password
#define HUAWEI_MQTT_ClientID			"6786593eef99673c8adfd47b_stm32_dev_0_0_2025012609"  //ClientId
#define HUAWEI_MQTT_ADDRESS				"31c2163339.st1.iotda-device.cn-north-4.myhuaweicloud.com"  //????
#define HUAWEI_MQTT_PORT				"1883"  //???
#define HUAWEI_MQTT_DeviceID			"6786593eef99673c8adfd47b_stm32_dev"  //??ID
#define HUAWEI_MQTT_ServiceID			"device"  //??ID
///
/*???????*/

///
/*?????*/
void AT_write(char atstring[512]);//????OK
void HuaweiIot_init(void);
void HuaweiIot_DevDate_publish(char * att,uint16_t data);
///
#endif



