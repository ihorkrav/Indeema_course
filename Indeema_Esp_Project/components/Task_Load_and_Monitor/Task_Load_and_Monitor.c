#include "Task_Load_and_Monitor.h"


void simulate_cpu_load_counting(uint32_t iterations) {
    // 'volatile' prevents the compiler from deleting this loop 
    // even though it technically "does nothing"
    for (volatile uint32_t i = 0; i < iterations; i++) {
        __asm__ __volatile__("nop");
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