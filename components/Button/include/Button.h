//#include "button_handler.h"
#include "iot_button.h"
#include "esp_log.h"

void button_init();

static void button_click_cb(void *arg, void *data);
static void button_long_press_cb(void *arg, void *data) ;