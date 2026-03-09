idf_component_register(SRCS "Joystick.c"
                    REQUIRES esp_driver_gpio esp_adc
                    INCLUDE_DIRS "include")