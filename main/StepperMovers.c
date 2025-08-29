#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include <string.h>
#include "driver/uart.h"

#define R 82
#define L 76
#define F 70
#define B 66
#define D 68
#define r 114
#define l 108
#define f 102
#define b 98
#define d 100

#define STEP_PIN_R    GPIO_NUM_18
#define DIR_PIN_R    GPIO_NUM_5

#define STEP_PIN_L    GPIO_NUM_21
#define DIR_PIN_L     GPIO_NUM_19

#define STEP_PIN_D    GPIO_NUM_33
#define DIR_PIN_D    GPIO_NUM_25

#define STEP_PIN_F    GPIO_NUM_27
#define DIR_PIN_F     GPIO_NUM_26

#define STEP_PIN_B    GPIO_NUM_12
#define DIR_PIN_B     GPIO_NUM_14

#define UART_PORT UART_NUM_0
#define BUF_SIZE 1024

#define MAX_LINE_LENGTH 64
char uart_line[MAX_LINE_LENGTH];
int uart_line_idx = 0;
#define BUF_SIZE 1024

void uart_console_config() {
    // Enable UART input via USB serial
    setvbuf(stdin, NULL, _IONBF, 0);  // Disable input buffering

    // Install UART driver for interrupt-driven reads and writes
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

float valuea = 0, valueb = 0, valuec = 0;
char which;
char data[128];
int ind = 0;
int spacer = 0;

void stepper_setup()
{
    gpio_reset_pin(STEP_PIN_R);
    gpio_reset_pin(DIR_PIN_R);
    gpio_reset_pin(STEP_PIN_L);
    gpio_reset_pin(DIR_PIN_L);
    gpio_reset_pin(STEP_PIN_B);
    gpio_reset_pin(DIR_PIN_B);
    gpio_reset_pin(STEP_PIN_F);
    gpio_reset_pin(DIR_PIN_F);
    gpio_reset_pin(STEP_PIN_D);
    gpio_reset_pin(DIR_PIN_D);
    

    gpio_set_direction(STEP_PIN_R, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIR_PIN_R, GPIO_MODE_OUTPUT);
    gpio_set_direction(STEP_PIN_L, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIR_PIN_L, GPIO_MODE_OUTPUT);
    gpio_set_direction(STEP_PIN_F, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIR_PIN_F, GPIO_MODE_OUTPUT);
    gpio_set_direction(STEP_PIN_B, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIR_PIN_B, GPIO_MODE_OUTPUT);
    gpio_set_direction(STEP_PIN_D, GPIO_MODE_OUTPUT);
    gpio_set_direction(DIR_PIN_D, GPIO_MODE_OUTPUT);
    
}

void stepper_step(char v, float RotationCount, int dir, int delay_us)
{
    if(v == 'R')
        gpio_set_level(DIR_PIN_R, dir);
    if(v == 'L')
        gpio_set_level(DIR_PIN_L, dir);
    if(v == 'F')
        gpio_set_level(DIR_PIN_F, dir);
    if(v == 'B')
        gpio_set_level(DIR_PIN_B, dir);
    if(v == 'D')
        gpio_set_level(DIR_PIN_D, dir);

    for (int i = 0; i < RotationCount * 200; i++) {
            if(v == 'R')
            {
                gpio_set_level(STEP_PIN_R, 1);
                esp_rom_delay_us(30);   // Short pulse // 50
                gpio_set_level(STEP_PIN_R, 0);
                esp_rom_delay_us(delay_us);   // Wait before next step // 2000
            }
            if(v == 'L')
            {
                gpio_set_level(STEP_PIN_L, 1);
                esp_rom_delay_us(30);   // Short pulse // 50
                gpio_set_level(STEP_PIN_L, 0);
                esp_rom_delay_us(delay_us);   // Wait before next step // 2000   
            }
    if(v == 'F')
            {
                gpio_set_level(STEP_PIN_F, 1);
                esp_rom_delay_us(30);   // Short pulse // 50
                gpio_set_level(STEP_PIN_F, 0);
                esp_rom_delay_us(delay_us);   // Wait before next step // 2000
            }   
    if(v == 'B')
            {
                gpio_set_level(STEP_PIN_B, 1);
                esp_rom_delay_us(30);   // Short pulse // 50
                gpio_set_level(STEP_PIN_B, 0);
                esp_rom_delay_us(delay_us);   // Wait before next step // 2000   
            }
    if(v == 'D')
            {
                gpio_set_level(STEP_PIN_D, 1);
                esp_rom_delay_us(30);   // Short pulse // 50
                gpio_set_level(STEP_PIN_D, 0);
                esp_rom_delay_us(delay_us);   // Wait before next step // 2000
            }       
    }
    vTaskDelay(pdMS_TO_TICKS(500));
}

void stepper_step2(char v)
{
    if(v == 'R')
        gpio_set_level(DIR_PIN_R, 0);
    if(v == 'L')
        gpio_set_level(DIR_PIN_L, 1);
    if(v == 'F')
        gpio_set_level(DIR_PIN_F, 0);
    if(v == 'B')
        gpio_set_level(DIR_PIN_B, 1);
    if(v == 'D')
        gpio_set_level(DIR_PIN_D, 1);

    if(v == 'r')
        gpio_set_level(DIR_PIN_R, 1);
    if(v == 'l')
        gpio_set_level(DIR_PIN_L, 0);
    if(v == 'f')
        gpio_set_level(DIR_PIN_F, 1);
    if(v == 'b')
        gpio_set_level(DIR_PIN_B, 0);
    if(v == 'd')
        gpio_set_level(DIR_PIN_D, 0);

    for (int i = 0; i < 0.25 * 3200; i++) {
            if(v == 'R' || v == 'r')
            {
                gpio_set_level(STEP_PIN_R, 1);
                esp_rom_delay_us(10);   // Short pulse // 50
                gpio_set_level(STEP_PIN_R, 0);
                esp_rom_delay_us(150);   // Wait before next step // 2000
            }
            if(v == 'L'|| v == 'l')
            {
                gpio_set_level(STEP_PIN_L, 1);
                esp_rom_delay_us(10);   // Short pulse // 50
                gpio_set_level(STEP_PIN_L, 0);
                esp_rom_delay_us(150);   // Wait before next step // 2000   
            }
    if(v == 'F' || v == 'f')
            {
                gpio_set_level(STEP_PIN_F, 1);
                esp_rom_delay_us(10);   // Short pulse // 50
                gpio_set_level(STEP_PIN_F, 0);
                esp_rom_delay_us(150);   // Wait before next step // 2000
            }   
    if(v == 'B' || v == 'b')
            {
                gpio_set_level(STEP_PIN_B, 1);
                esp_rom_delay_us(10);   // Short pulse // 50
                gpio_set_level(STEP_PIN_B, 0);
                esp_rom_delay_us(150);   // Wait before next step // 2000   
            }
    if(v == 'D' || v == 'd')
            {
                gpio_set_level(STEP_PIN_D, 1);
                esp_rom_delay_us(10);   // Short pulse // 50
                gpio_set_level(STEP_PIN_D, 0);
                esp_rom_delay_us(150);   // Wait before next step // 2000
            }       
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}

char flip(int vam)
{
    if (vam >= 97)
    {
        vam -= 32;
    }
    else
    {
        vam += 32;
    }
    return vam;
}

char moves[] = {R,R,L,L,F,B,D,D,F,F,R,L,B,R,L,D,D,B,B,F,L,L,R,R};
int val = sizeof(moves) / sizeof(char);

void stepSetup()
{
    stepper_setup();
    uart_console_config();
}


