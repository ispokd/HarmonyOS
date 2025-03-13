#include "pwm.h"
#include "stm32f10x.h"

// pwm初始化，使用tim1，ch1定值
void pwm_init(uint16_t arr, uint16_t psc) {
    // GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    // 定时器初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 输出比较初始化
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 开启TIM1和GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化PA8为复用推挽输出模式，用于TIM1 CH1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置定时器基础参数
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // 配置TIM1 CH1为PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);  // 这里改为TIM_OC1Init

    // 使能通道1预加载
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    // 使能自动重载寄存器预加载
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    // 启动定时器
    TIM_Cmd(TIM1, ENABLE);
}               

// 设置TIM1的PWM占空比
void pwm_set_duty(uint16_t duty) {
    // 限制占空比的范围，保证它在0到100之间
    if (duty > 100) {
        duty = 100;
    } else if (duty < 0) {
        duty = 0;
    }

    // 根据占空比计算比较值（CCR1）
    uint16_t compare_value = (uint16_t)((duty / 100.0) * (TIM1->ARR));

    // 设置TIM1 CH1的比较值，从而调整占空比
    TIM1->CCR1 = compare_value;
}
void breath_light(void) {
    uint16_t duty;
    int8_t direction = 1;  // 用于控制占空比增加或减少

    while (1) {
        // 呼吸灯效果，逐渐增大占空比到100，再逐渐减小到0
        if (direction == 1) {
            for (duty = 0; duty <= 100; duty++) {
                pwm_set_duty(duty);  // 设置当前的占空比
                delay_ms(1);  // 延时，控制渐变速度
            }
            direction = -1;  // 切换为减少占空比
        } else {
            for (duty = 100; duty >= 0; duty--) {
                pwm_set_duty(duty);  // 设置当前的占空比
                delay_ms(1);  // 延时，控制渐变速度
            }
            direction = 1;  // 切换为增加占空比
        }
    }
}
