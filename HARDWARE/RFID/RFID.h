#ifndef _RFID_H
#define _RFID_H

#include "stm32f10x.h"
#include "delay.h"
#include "MFRC522.h"
#include "stdio.h"
#include "main.h"

// �Ϸ� UID �б��ɸ���ʵ������޸ĺ���չ

#define VALID_UID_COUNT (sizeof(validUIDs) / sizeof(validUIDs[0]))

//
#define DOOR_LOCK_PIN GPIO_Pin_0
#define DOOR_LOCK_PORT GPIOA

// �Ϸ���Ƭ��ʾ LED ���ţ�����ʹ�� PD1
#define VALID_LED_PIN GPIO_Pin_2
#define VALID_LED_PORT GPIOD

// �Ƿ���Ƭ��ʾ LED ���ţ�����ʹ�� PA8
#define INVALID_LED_PIN GPIO_Pin_8
#define INVALID_LED_PORT GPIOA




unsigned char IsValidUID(unsigned char *uid) ;
void OpenDoor(void);

void IllegalCardAlert(void);

void DoorLockAndLED_Init(void);

uint8_t RFID(void);

uint8_t RFID_last(void);


void RFID_again(void);


void Door_State(DeviceTypeDef *device);

uint8_t Camera(void);

#endif




