#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

void task1(void *arg)
{
    printf("the task2 has name: %s \n", pcTaskGetTaskName(myTask2Handle));
    int c = 0;
    while (1)
    {
        c = xTaskGetTickCount();
        printf("[%d] hello from task1 \n", c);
        vTaskDelay(1000 / portTICK_RATE_MS);
        if (c == 300)
        {
            vTaskSuspend(myTask2Handle);
            printf("task2 is suspended! %d \n", eTaskGetState(myTask2Handle));
        }
        if (c == 500)
        {
            vTaskResume(myTask2Handle);
            printf("task2 is resumed!  %d \n", eTaskGetState(myTask2Handle));
        }
        if (c == 700)
        {
            vTaskDelete(myTask2Handle);
            printf("task2 is deleted! %d \n", eTaskGetState(myTask2Handle));
        }

        /*
Running =  0,
Ready   =  1,
Blocked =  2,
Suspended= 3,
Deleted =  4,
Invalid =  5
*/
    }
}
void task2(void *arg)
{

    for (int i = 0; i < 10; i++)
    {
        printf("hello from task2 \n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
void app_main()
{

    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2Handle, 1);
}