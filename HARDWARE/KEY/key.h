#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"

// 密码验证状态机
typedef enum {
    PASSWORD_STATE_IDLE,       // 空闲状态
    PASSWORD_STATE_INPUT,      // 输入密码状态
    PASSWORD_STATE_WAIT_ENTER, // 等待确定键状态
    PASSWORD_STATE_CHECK       // 检查密码状态
} PasswordState;


void key_Init(void);


uint8_t keyboard_scan(void);


int check_password(void);
	

#endif




