#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_system.h"
#include <stdlib.h>
#include <limits.h>
#include <Task_Load_and_Monitor.h>



void vTask1(void *parameters)
{
    for(;;){
   
        simulate_cpu_load_counting(500);
  
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void *parameters)
{
    
   while (1) {
        
        simulate_cpu_load_counting(1000);
        
       
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void app_main(void)
{
    xTaskCreatePinnedToCore(vTask1, "Task1", 4896, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTask2, "Task2", 4896, NULL, 1, NULL, 1);

    xTaskCreatePinnedToCore(vTaskMonitor, "Receiver", 4896, NULL, 1, NULL, 1);
}