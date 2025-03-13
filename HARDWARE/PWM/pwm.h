#ifndef __pwm_H
#define __pwm_H
#include "stm32f10x.h"  // 包含STM32的标准库头文件

// 函数声明

/**
 * @brief  PWM初始化函数，配置TIM1的CH1通道
 * @param  arr: 自动重载寄存器的值
 * @param  psc: 预分频器的值
 * @retval 无
 */
void pwm_init(uint16_t arr, uint16_t psc);

/**
 * @brief  设置TIM1的PWM占空比
 * @param  duty: PWM占空比（0到100之间）
 * @retval 无
 */
void pwm_set_duty(uint16_t duty);
void breath_light(void);
#endif /* __PWM_H */
