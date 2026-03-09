
#include "iot_servo.h"
#include "driver/ledc.h"
void servo_init(void);
void move_servo(float angle);
void deint_servo(void);