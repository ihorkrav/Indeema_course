#include "Joystick.h"


// Implied global/static handles based on the code
static adc_oneshot_unit_handle_t s_adc;
static adc_channel_t s_chan_x;
static adc_channel_t s_chan_y;

// Implied GPIO defines
#define JOY_X_GPIO  4 
#define JOY_Y_GPIO  5
#define JOY_SW_GPIO 6
#define ADC_MIN  200
#define ADC_MAX  3900
static const char *TAG = "Joystick";


static int adc_to_servo(int adc_val)
{
    if (adc_val < ADC_MIN) adc_val = ADC_MIN;
    if (adc_val > ADC_MAX) adc_val = ADC_MAX;

    return (adc_val - ADC_MIN) * 180 / (ADC_MAX - ADC_MIN);
}
static void configure_adc_pin(int gpio, adc_channel_t *out_channel)
{
    adc_unit_t unit = ADC_UNIT_1;
    adc_channel_t channel = ADC_CHANNEL_0;

    ESP_ERROR_CHECK(adc_oneshot_io_to_channel(gpio, &unit, &channel));
    if(unit != ADC_UNIT_1){
        ESP_LOGE(TAG, "GPOI %d mapped to ADC%d.",gpio,unit+1);
        abort();
    }

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc, channel, &chan_cfg));
    *out_channel = channel;
}

void configure_joystick(void)
{
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &s_adc));

    configure_adc_pin(JOY_X_GPIO, &s_chan_x);
    configure_adc_pin(JOY_Y_GPIO, &s_chan_y);

    gpio_config_t sw_cfg = {
        .pin_bit_mask = 1ULL << JOY_SW_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&sw_cfg));
}



void read_joystick(int *x_deg, int *y_deg, bool *sw_pressed)
{
    int x_raw;
    int y_raw;

    ESP_ERROR_CHECK(adc_oneshot_read(s_adc, s_chan_x, &x_raw));
    ESP_ERROR_CHECK(adc_oneshot_read(s_adc, s_chan_y, &y_raw));

    *x_deg = adc_to_servo(x_raw);
    *y_deg = adc_to_servo(y_raw);

    *sw_pressed = (gpio_get_level(JOY_SW_GPIO) == 0);
}