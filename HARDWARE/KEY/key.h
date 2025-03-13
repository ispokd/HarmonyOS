#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"

// ������֤״̬��
typedef enum {
    PASSWORD_STATE_IDLE,       // ����״̬
    PASSWORD_STATE_INPUT,      // ��������״̬
    PASSWORD_STATE_WAIT_ENTER, // �ȴ�ȷ����״̬
    PASSWORD_STATE_CHECK       // �������״̬
} PasswordState;


void key_Init(void);


uint8_t keyboard_scan(void);


int check_password(void);
	

#endif




