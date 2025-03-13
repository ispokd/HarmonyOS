#include "key.h"

#define KEYBOARD_ROWS 4
#define KEYBOARD_COLS 4
#define PASSWORD_LENGTH 4

// 定义行和列的引脚
#define KEYBOARD_ROW_PIN0 GPIO_Pin_4
#define KEYBOARD_ROW_PIN1 GPIO_Pin_5
#define KEYBOARD_ROW_PIN2 GPIO_Pin_6
#define KEYBOARD_ROW_PIN3 GPIO_Pin_7

#define KEYBOARD_COL_PIN0 GPIO_Pin_8
#define KEYBOARD_COL_PIN1 GPIO_Pin_9
#define KEYBOARD_COL_PIN2 GPIO_Pin_10
#define KEYBOARD_COL_PIN3 GPIO_Pin_11

#define KEYBOARD_GPIO_PORT GPIOC // 假设键盘连接到GPIOC

PasswordState password_state = PASSWORD_STATE_IDLE; // 初始状态
uint8_t input_password[PASSWORD_LENGTH]; // 存储用户输入的密码
uint8_t input_index = 0; // 当前输入的位置


const char password[PASSWORD_LENGTH] = {1,2,3,4}; // 示例密码

// 按键值映射
static const uint8_t key_values[KEYBOARD_ROWS][KEYBOARD_COLS] = {
    {1, 2, 10, 3},
    {4, 5, 11, 6},
    {7, 8, 12, 9},
    {13, 0, 16, 15} // 按键15是确定键
};


void key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    // 初始化键盘行引脚为输出
    GPIO_InitStruct.GPIO_Pin = KEYBOARD_ROW_PIN0 | KEYBOARD_ROW_PIN1 | KEYBOARD_ROW_PIN2 | KEYBOARD_ROW_PIN3;
    GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);

    // 初始化键盘列引脚为输入
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 输入带上拉
    GPIO_InitStruct.GPIO_Pin = KEYBOARD_COL_PIN0 | KEYBOARD_COL_PIN1 | KEYBOARD_COL_PIN2 | KEYBOARD_COL_PIN3;
    GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);

}


// 键盘扫描函数（保持不变）
uint8_t keyboard_scan(void)
{
    uint8_t row, col;
    uint16_t row_pins[KEYBOARD_ROWS] = {
        KEYBOARD_ROW_PIN0, KEYBOARD_ROW_PIN1,
        KEYBOARD_ROW_PIN2, KEYBOARD_ROW_PIN3
    };
    uint16_t col_pins[KEYBOARD_COLS] = {
        KEYBOARD_COL_PIN0, KEYBOARD_COL_PIN1,
        KEYBOARD_COL_PIN2, KEYBOARD_COL_PIN3
    };

    for (row = 0; row < KEYBOARD_ROWS; row++)
    {
        // 拉低当前行
        GPIO_ResetBits(KEYBOARD_GPIO_PORT, row_pins[row]);
        for (uint8_t i = 0; i < KEYBOARD_ROWS; i++)
        {
            if (i != row)
                GPIO_SetBits(KEYBOARD_GPIO_PORT, row_pins[i]);
        }

        // 去抖动
        delay_ms(10);

        // 扫描列
        for (col = 0; col < KEYBOARD_COLS; col++)
        {
            if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, col_pins[col]) == Bit_RESET)
            {
                // 等待按键释放
                while (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, col_pins[col]) == Bit_RESET);

                // 恢复当前行为高电平
                GPIO_SetBits(KEYBOARD_GPIO_PORT, row_pins[row]);

                // 返回按键值
                return key_values[row][col];
            }
        }

        // 恢复当前行为高电平
        GPIO_SetBits(KEYBOARD_GPIO_PORT, row_pins[row]);
    }

    // 没有按键按下
    return 0;
}





int check_password(void)
{
    uint8_t key;

    switch (password_state)
    {
        case PASSWORD_STATE_IDLE:
            // 初始化状态
            input_index = 0;
            OLED_Clear(); // 清屏
            OLED_ShowString(1, 1, "Input Password:"); // 显示提示信息
            password_state = PASSWORD_STATE_INPUT; // 进入输入密码状态
            break;

        case PASSWORD_STATE_INPUT:
            // 输入密码
            key = keyboard_scan();
            if (key != 0) // 如果有按键按下
            {
                input_password[input_index] = key; // 存储按键值
                input_index++;

                // 在 OLED 上显示已输入的密码
                OLED_ShowNum(2, input_index, key, 1); // 显示按键值
                delay_ms(200); // 短暂延时，避免按键抖动

                if (input_index >= PASSWORD_LENGTH)
                {
                    // 输入完成，进入等待确定键状态
                    OLED_ShowString(3, 1, "Press Enter..."); // 提示用户按下确定键
                    password_state = PASSWORD_STATE_WAIT_ENTER;
                }
            }
            break;

        case PASSWORD_STATE_WAIT_ENTER:
            // 等待确定键
            key = keyboard_scan();
            if (key == 16) // 如果按下确定键
            {
                password_state = PASSWORD_STATE_CHECK; // 进入检查密码状态
            }
            break;

        case PASSWORD_STATE_CHECK:
            // 检查密码
            for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
            {
                if (input_password[i] != password[i])
                {
                    // 密码错误
                    OLED_Clear();
					OLED_ShowString(5,1,"Error!");                   
                    password_state = PASSWORD_STATE_IDLE; // 返回空闲状态
                    delay_ms(2000);
                    return 2;
                }
                else
                {
                     // 密码正确
                    OLED_Clear();
                    OLED_ShowString(5, 1, "Correct!"); // 显示密码正确
                    delay_ms(2000);
                    password_state = PASSWORD_STATE_IDLE; // 返回空闲状态
                    return 1;
                }
            }          
        default:
            break;
    }

    return 0; // 默认返回0
}





