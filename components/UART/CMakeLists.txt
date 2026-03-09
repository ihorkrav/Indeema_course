idf_component_register(SRCS "UART.c"
                    REQUIRES esp_driver_uart esp_driver_gpio
                    INCLUDE_DIRS "include")