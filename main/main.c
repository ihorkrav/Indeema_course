#include "main.h"


typedef enum {
    MODE_JOYSTICK,
    MODE_UART
} control_mode_t;

static volatile int servo_angle = 90;
static volatile control_mode_t control_mode = MODE_JOYSTICK;

void uart_task(void *pvParameters)
{
    while (1)
    {
        int angle = uart_read_servo();

        if (angle != -1)
        {
            servo_angle = angle;
            move_servo(angle);

            control_mode = MODE_UART;

            ESP_LOGI("UART", "Servo via UART: %d", angle);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void joystick_task(void *pvParameters)
{
    int x_deg;
    int y_deg;
    bool sw;

    configure_joystick();

    while (1)
    {
        read_joystick(&x_deg, &y_deg, &sw);

        if (control_mode == MODE_JOYSTICK)
        {
            servo_angle = x_deg;
            move_servo(x_deg);
        }

        if (sw && control_mode == MODE_UART)
        {
            control_mode = MODE_JOYSTICK;
            ESP_LOGI("JOY", "Joystick control restored");
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void mqtt_task(void *pvParameters)
{
    wifi_init_sta();
    mqtt_app_start();

    char msg[64];

    while (1)
    {
        sprintf(msg, "{\"servo\":%d}", servo_angle);

        mqtt_send_data(msg);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    uart_init();
    servo_init();

    xTaskCreate(uart_task, "uart_task", 4096, NULL, 5, NULL);
    xTaskCreate(joystick_task, "joystick_task", 4096, NULL, 5, NULL);
    xTaskCreate(mqtt_task, "mqtt_task", 4096, NULL, 5, NULL);
}