idf_component_register(SRCS "main.c"
                    REQUIRES  LED  WiFi_component  MQTT_Utility UART Servo  Joystick
                  
                   
                    INCLUDE_DIRS "include"
)