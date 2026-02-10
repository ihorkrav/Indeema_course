#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_system.h"
#include <stdlib.h>
#include <limits.h>




void simulate_cpu_load(uint32_t load_ms) {
    if (load_ms == 0) return;

    int64_t start_time = esp_timer_get_time();
    int64_t target_duration_us = (int64_t)load_ms * 1000;

    while ((esp_timer_get_time() - start_time) < target_duration_us) {
       
        __asm__ __volatile__("nop");
    }
}

void vTask1(void *parameters)
{
    for(;;){
   
        simulate_cpu_load(500);
  
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void *parameters)
{
    
   while (1) {
        
        simulate_cpu_load(1000);
        
       
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void vTaskMonitor(void *parameters)
{
   
    char buffer[1024];

    while (1) {
        printf("\nTask Name\tStatus\tPrio\tStack\tNum\tCoreId\n");
        printf("------------------------------------------------------------\n");
        
        vTaskList(buffer);
        
        printf("%s\n", buffer);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(vTask1, "Task1", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTask2, "Task2", 2048, NULL, 1, NULL, 1);

    xTaskCreatePinnedToCore(vTaskMonitor, "Receiver", 2048, NULL, 1, NULL, 1);
}
