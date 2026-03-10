#include "driver/gpio.h"
#include "soc/adc_channel.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include <stdio.h>

void configure_joystick(void);
void read_joystick(int *x, int *y, bool *sw_pressed);
static int adc_to_servo(int adc_val);