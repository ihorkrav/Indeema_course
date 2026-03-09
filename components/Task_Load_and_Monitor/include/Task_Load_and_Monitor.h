#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void simulate_cpu_load_counting(uint32_t iterations);
void vTaskMonitor(void *parameters);
