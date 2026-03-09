#include <stdio.h>
#include "LED.h"



#define BLINK_GPIO 48

led_strip_handle_t led_strip = NULL;
void LED_Strip_Init(){
/// LED strip common configuration
led_strip_config_t strip_config = {
    .strip_gpio_num = BLINK_GPIO,  // The GPIO that connected to the LED strip's data line
    .max_leds = 1,                 // The number of LEDs in the strip,
    .led_model = LED_MODEL_WS2812, // LED strip model, it determines the bit timing
    .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // The color component format is G-R-B
    .flags = {
        .invert_out = false, // don't invert the output signal
    }
};

/// SPI backend specific configuration
led_strip_spi_config_t spi_config = {
    .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
    .spi_bus = SPI2_HOST,           // SPI bus ID
    .flags = {
        .with_dma = true, // Using DMA can improve performance and help drive more LEDs
    }
};
ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
}

void LED_RED(int value){


led_strip_set_pixel(led_strip, 0, value, 0, 0); // Set first LED to Red

// 2. Push the color to the physical LED
led_strip_refresh(led_strip);
}

void LED_GREEN(int value){


led_strip_set_pixel(led_strip, 0, 0, value, 0); // Set first LED to Red

// 2. Push the color to the physical LED
led_strip_refresh(led_strip);
}

void LED_BLUE(int value){


led_strip_set_pixel(led_strip, 0, 0, 0, value); // Set first LED to Red

// 2. Push the color to the physical LED
led_strip_refresh(led_strip);
}

void LED_STOP(){
    


led_strip_set_pixel(led_strip, 0, 0, 0, 0); // Set first LED to Red

// 2. Push the color to the physical LED
led_strip_refresh(led_strip);
}




