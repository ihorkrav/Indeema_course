#include "main.h"


typedef enum {
    MODE_JOYSTICK,
    MODE_UART
} control_mode_t;

static volatile int servo_angle = 90;
static volatile control_mode_t control_mode = MODE_JOYSTICK;
static SemaphoreHandle_t control_mutex;

void led_task(void *pvParameters)
{
    LED_Strip_Init();

    control_mode_t mode;

    while(true)
    {
        xSemaphoreTake(control_mutex, portMAX_DELAY);
        mode = control_mode;
        xSemaphoreGive(control_mutex);

        if(mode == MODE_JOYSTICK)
            LED_RED(255);
        else
            LED_BLUE(255);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void uart_task(void *pvParameters)
{
    uart_init();

    while (1)
    {
        int angle = uart_read_servo();

        if (angle != -1)
        {
            if(angle < 0) angle = 0;
            if(angle > 180) angle = 180;

            xSemaphoreTake(control_mutex, portMAX_DELAY);

            servo_angle = angle;
            control_mode = MODE_UART;

            xSemaphoreGive(control_mutex);

            move_servo(angle);

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
    static bool last_sw = false;

    configure_joystick();

    while (1)
    {
        read_joystick(&x_deg, &y_deg, &sw);

        xSemaphoreTake(control_mutex, portMAX_DELAY);

        if (control_mode == MODE_JOYSTICK)
        {
            servo_angle = x_deg;
            move_servo(x_deg);
        }

        if (sw && !last_sw && control_mode == MODE_UART)
        {
            control_mode = MODE_JOYSTICK;
            ESP_LOGI("JOY", "Joystick control restored");
        }

        xSemaphoreGive(control_mutex);

        last_sw = sw;

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void mqtt_task(void *pvParameters)
{
    wifi_init_sta();
    mqtt_app_start();

    char msg[64];
    int angle;

    while (1)
    {
        xSemaphoreTake(control_mutex, portMAX_DELAY);
        angle = servo_angle;
        xSemaphoreGive(control_mutex);

        snprintf(msg, sizeof(msg), "{\"servo\":%d}", angle);

        mqtt_send_data(msg);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    servo_init();

    control_mutex = xSemaphoreCreateMutex();

    xTaskCreate(uart_task, "uart_task", 4096, NULL, 5, NULL);
    xTaskCreate(joystick_task, "joystick_task", 4096, NULL, 4, NULL);
    xTaskCreate(mqtt_task, "mqtt_task", 4096, NULL, 3, NULL);
    xTaskCreate(led_task, "LED_task", 2048, NULL, 1, NULL);

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}