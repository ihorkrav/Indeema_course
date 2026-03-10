#include "esp_log.h"
#include "iot_servo.h"
#include "driver/ledc.h"
#include <stdio.h>

void servo_init(void);
void move_servo(float angle);
void deint_servo(void);