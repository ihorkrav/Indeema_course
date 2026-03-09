#include "Stepper.h"
static const char *TAG = "STEPPER_SYSTEM";

// Using safe GPIOs for ESP32-S3
#define STEP_IN1 7
#define STEP_IN2 6
#define STEP_IN3 5
#define STEP_IN4 4

// Full-Step High-Torque Sequence (2 phases active)
// This is the most "aggressive" sequence to force a stuck motor to turn.
const uint8_t motor_steps[4][4] = {
    {1, 1, 0, 0}, // Phase A & B
    {0, 1, 1, 0}, // Phase B & C
    {0, 0, 1, 1}, // Phase C & D
    {1, 0, 0, 1}  // Phase D & A
};

void stepper_task(void *pvParameters) {
    ESP_LOGI(TAG, "Initializing Stepper GPIOs...");

    // 1. Reset pins to a known state
    gpio_reset_pin(STEP_IN1);
    gpio_reset_pin(STEP_IN2);
    gpio_reset_pin(STEP_IN3);
    gpio_reset_pin(STEP_IN4);

    // 2. Configure as Outputs
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << STEP_IN1) | (1ULL << STEP_IN2) | 
                        (1ULL << STEP_IN3) | (1ULL << STEP_IN4),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    uint8_t current_step = 0;

    ESP_LOGI(TAG, "Starting step loop (1 sec interval for debug)...");

    while (1) {
        // Apply the pattern from our sequence
        gpio_set_level(STEP_IN1, motor_steps[current_step][0]);
        gpio_set_level(STEP_IN2, motor_steps[current_step][1]);
        gpio_set_level(STEP_IN3, motor_steps[current_step][2]);
        gpio_set_level(STEP_IN4, motor_steps[current_step][3]);

        ESP_LOGI(TAG, "Pattern: [%d %d %d %d] - Step Index: %d", 
                 motor_steps[current_step][0], 
                 motor_steps[current_step][1], 
                 motor_steps[current_step][2], 
                 motor_steps[current_step][3], 
                 current_step);

        current_step++;
        if (current_step >= 4) {
            current_step = 0;
        }

        // Delay 1000ms. If the motor works, you will see the shaft 
        // tick slowly. If you only see LEDs, increase voltage to 5V.
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}