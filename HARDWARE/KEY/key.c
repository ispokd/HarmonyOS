#include "key.h"

#define KEYBOARD_ROWS 4
#define KEYBOARD_COLS 4
#define PASSWORD_LENGTH 4

// �����к��е�����
#define KEYBOARD_ROW_PIN0 GPIO_Pin_4
#define KEYBOARD_ROW_PIN1 GPIO_Pin_5
#define KEYBOARD_ROW_PIN2 GPIO_Pin_6
#define KEYBOARD_ROW_PIN3 GPIO_Pin_7

#define KEYBOARD_COL_PIN0 GPIO_Pin_8
#define KEYBOARD_COL_PIN1 GPIO_Pin_9
#define KEYBOARD_COL_PIN2 GPIO_Pin_10
#define KEYBOARD_COL_PIN3 GPIO_Pin_11

#define KEYBOARD_GPIO_PORT GPIOC // ����������ӵ�GPIOC

PasswordState password_state = PASSWORD_STATE_IDLE; // ��ʼ״̬
uint8_t input_password[PASSWORD_LENGTH]; // �洢�û����������
uint8_t input_index = 0; // ��ǰ�����λ��


const char password[PASSWORD_LENGTH] = {1,2,3,4}; // ʾ������

// ����ֵӳ��
static const uint8_t key_values[KEYBOARD_ROWS][KEYBOARD_COLS] = {
    {1, 2, 10, 3},
    {4, 5, 11, 6},
    {7, 8, 12, 9},
    {13, 0, 16, 15} // ����15��ȷ����
};


void key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    // ��ʼ������������Ϊ���
    GPIO_InitStruct.GPIO_Pin = KEYBOARD_ROW_PIN0 | KEYBOARD_ROW_PIN1 | KEYBOARD_ROW_PIN2 | KEYBOARD_ROW_PIN3;
    GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);

    // ��ʼ������������Ϊ����
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // ���������
    GPIO_InitStruct.GPIO_Pin = KEYBOARD_COL_PIN0 | KEYBOARD_COL_PIN1 | KEYBOARD_COL_PIN2 | KEYBOARD_COL_PIN3;
    GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);

}


// ����ɨ�躯�������ֲ��䣩
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
        // ���͵�ǰ��
        GPIO_ResetBits(KEYBOARD_GPIO_PORT, row_pins[row]);
        for (uint8_t i = 0; i < KEYBOARD_ROWS; i++)
        {
            if (i != row)
                GPIO_SetBits(KEYBOARD_GPIO_PORT, row_pins[i]);
        }

        // ȥ����
        delay_ms(10);

        // ɨ����
        for (col = 0; col < KEYBOARD_COLS; col++)
        {
            if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, col_pins[col]) == Bit_RESET)
            {
                // �ȴ������ͷ�
                while (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, col_pins[col]) == Bit_RESET);

                // �ָ���ǰ��Ϊ�ߵ�ƽ
                GPIO_SetBits(KEYBOARD_GPIO_PORT, row_pins[row]);

                // ���ذ���ֵ
                return key_values[row][col];
            }
        }

        // �ָ���ǰ��Ϊ�ߵ�ƽ
        GPIO_SetBits(KEYBOARD_GPIO_PORT, row_pins[row]);
    }

    // û�а�������
    return 0;
}





int check_password(void)
{
    uint8_t key;

    switch (password_state)
    {
        case PASSWORD_STATE_IDLE:
            // ��ʼ��״̬
            input_index = 0;
            OLED_Clear(); // ����
            OLED_ShowString(1, 1, "Input Password:"); // ��ʾ��ʾ��Ϣ
            password_state = PASSWORD_STATE_INPUT; // ������������״̬
            break;

        case PASSWORD_STATE_INPUT:
            // ��������
            key = keyboard_scan();
            if (key != 0) // ����а�������
            {
                input_password[input_index] = key; // �洢����ֵ
                input_index++;

                // �� OLED ����ʾ�����������
                OLED_ShowNum(2, input_index, key, 1); // ��ʾ����ֵ
                delay_ms(200); // ������ʱ�����ⰴ������

                if (input_index >= PASSWORD_LENGTH)
                {
                    // ������ɣ�����ȴ�ȷ����״̬
                    OLED_ShowString(3, 1, "Press Enter..."); // ��ʾ�û�����ȷ����
                    password_state = PASSWORD_STATE_WAIT_ENTER;
                }
            }
            break;

        case PASSWORD_STATE_WAIT_ENTER:
            // �ȴ�ȷ����
            key = keyboard_scan();
            if (key == 16) // �������ȷ����
            {
                password_state = PASSWORD_STATE_CHECK; // ����������״̬
            }
            break;

        case PASSWORD_STATE_CHECK:
            // �������
            for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
            {
                if (input_password[i] != password[i])
                {
                    // �������
                    OLED_Clear();
					OLED_ShowString(5,1,"Error!");                   
                    password_state = PASSWORD_STATE_IDLE; // ���ؿ���״̬
                    delay_ms(2000);
                    return 2;
                }
                else
                {
                     // ������ȷ
                    OLED_Clear();
                    OLED_ShowString(5, 1, "Correct!"); // ��ʾ������ȷ
                    delay_ms(2000);
                    password_state = PASSWORD_STATE_IDLE; // ���ؿ���״̬
                    return 1;
                }
            }          
        default:
            break;
    }

    return 0; // Ĭ�Ϸ���0
}





