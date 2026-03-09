#include <stdio.h>
#include "Servo.h"
#include "esp_log.h"
#define SERVO_GPIO  8

char* TAG = "Servo";
void servo_init(void)
{
    servo_config_t servo_cfg = {
        .max_angle = 180,
        .min_width_us = 500,
        .max_width_us = 2500,
        .freq = 50,
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = { SERVO_GPIO },
            .ch = {
                LEDC_CHANNEL_0,
            },
        },
        .channel_number = 1,
    };

    // Initialize the servo
    if(iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg)){
        ESP_LOGE(TAG, "Servo initialization error");
    }
    else
        ESP_LOGI(TAG, "Servo initialized");
}

void move_servo(float angle){
   // if(!(iot_servo_read_angle(LEDC_LOW_SPEED_MODE, 0, &angle)!=angle)){
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle);
    //}
    ESP_LOGI(TAG, "Servo moved to %.3f degrees", angle);
}

void deint_servo(void){
    iot_servo_deinit(LEDC_LOW_SPEED_MODE);
}
