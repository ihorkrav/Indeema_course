#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "Wifi.h"
#include "HTTP.h"
#include "MQTT.h"
#include "UART.h"
#include "Servo.h"
#include "Stepper.h"
#include "Joystick.h"

void move_servo_task(void *pvParameters) {


    // Cast the parameter back to an integer
    servo_init();
    int angle = (int)pvParameters; 

    while(1) {

        move_servo(150);
        // Your logic to move the servo to 'angle'
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void mqtt_task(void *pvParameters){
wifi_init_sta();
    mqtt_app_start();



}
void app_main(void) {
    uart_init();
     servo_init();
    // configure_joystick();
    // // esp_err_t ret = nvs_flash_init();
    // // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // //     ESP_ERROR_CHECK(nvs_flash_erase());
    // //     ret = nvs_flash_init();
    // // }
    // // ESP_ERROR_CHECK(ret);

    // esp_err_t ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     // NVS partition was truncated and needs to be erased
    //     // Retry nvs_flash_init
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     ret = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(ret);
    // wifi_init_sta();
    // //wifi_init_softap();
    // mqtt_app_start();
    // // //start_webserver();
    // // Pass 100 as the parameter (cast to void*)
    // // xTaskCreate(move_servo_task, "servo_task", 4096, (void *)100, 10, NULL);
    // // Note the function name change and the 7th argument for the Core ID
    // // xTaskCreatePinnedToCore(
    //     //     stepper_task,    // Function name
    //     //     "stepper task",   // Name for debugging
    //     //     4096,            // Stack size
    //     //     NULL,            // Parameters
    //     //     2,               // Priority
    //     //     NULL,            // Task handle (use NULL if you don't need to delete the task later)
    //     //     1                // Core ID (0 or 1)
    //     // );
    // //xTaskCreate(echo_task, "uart_echo_task", 2048, NULL, 10, NULL);
  

    // int x_deg;
    // int y_deg;
    // bool sw;

    // //move_servo(90);

    // while(true){
    // read_joystick(&x_deg, &y_deg, &sw);


    // move_servo(x_deg);
    // char msg[64];

    // sprintf(msg, "{\"x\":%d,\"y\":%d}", x_deg, y_deg);

    // mqtt_send_data(msg);
    // vTaskDelay(pdMS_TO_TICKS(50));
    // }
while (true)
{
    int angle = uart_read_servo();

    if (angle != -1) {
        move_servo(angle);
        ESP_LOGI("MAIN", "Servo moved to %d", angle);
    }

    vTaskDelay(pdMS_TO_TICKS(20));
}
}