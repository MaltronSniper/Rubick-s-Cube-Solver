#include <ALL.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_timer.h>

uint64_t value = 0;

void app_main(void)
{
    stepSetup();    
    
    vTaskDelay(pdMS_TO_TICKS(2000));

    ALGO();

    printf("\nStarting Shuffle...\n");

    for(int i = 0;i < 10;i++)
    {
        stepper_step2(follower[i]);
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
    
    value =  esp_timer_get_time() / 1000;

    printf("Starting Solve  ->  Move Count : %d\n", filler);

    for(int i = 0;i <= filler;i++)
    {
        stepper_step2(way[i]);
        printf("%c %d\n",way[i],i);
    }

    printf("\n");

    value -= esp_timer_get_time() / 1000;
    int milliseconds = -value % 1000;
    int seconds = (-value / 1000) % 60;
    int minute = (-value / 1000) / 60;

    printf("\nTime Taken :   Minute -> %d   Seconds -> %d   MilliSeconds -> %d\n",minute, seconds,  milliseconds);
}
