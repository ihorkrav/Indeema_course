#include "Button.h"

static const char *TAG = "BUTTON_APP";

// // External button on GPIO 6
// #define EXTERNAL_BUTTON_GPIO 7

// button_handle_t gpio_btn = NULL;
// void button_init(){
// const button_config_t btn_cfg = {0};
// const button_gpio_config_t btn_gpio_cfg = {
//     .gpio_num = 7,
//     .active_level = 0,
// };

// esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &gpio_btn);
// if(NULL == gpio_btn) {
//     ESP_LOGE(TAG, "Button create failed");
// }
// }

// static void button_single_click_cb(void *arg,void *usr_data)
// {
//     ESP_LOGI(TAG, "BUTTON_SINGLE_CLICK");
// }


// iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, NULL, button_single_click_cb,NULL);

// static void button_long_press_1_cb(void *arg,void *usr_data)
// {
//     ESP_LOGI(TAG, "BUTTON_LONG_PRESS_START_1");
// }

// static void button_long_press_2_cb(void *arg,void *usr_data)
// {
//     ESP_LOGI(TAG, "BUTTON_LONG_PRESS_START_2");
// }

// button_event_args_t args = {
//     .long_press.press_time = 2000,
// };

// iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_START, &args, button_auto_check_cb_1, NULL);

// args.long_press.press_time = 5000;
// iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_START, &args, button_long_press_2_cb, NULL);