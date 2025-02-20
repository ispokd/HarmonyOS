#ifndef _MAIN_H
#define _MAIN_H

#include "stdint.h"

typedef enum
{
    Device_ERROR=0,
    Device_OK 
}DeviceAction;

typedef enum
{
    Interrupt_ERROR=0,
    Interrupt_OK
}InterrruptAction;


typedef struct
{
    uint8_t light_state;          //灯的状态    
    uint8_t sun_state;            //光照强度    
    uint8_t door_state;           //门的状态    
    uint8_t people;               //有无人
    uint8_t temperature;          //温度整数
    uint8_t temperature_decimal;  //温度小数
    uint8_t humidity;             //湿度整数
    uint8_t humidity_decimal;     //温度小数
    
}DeviceTypeDef;


typedef struct
{
    uint8_t Light;
    
}InterrruptTypeDef;





#endif


