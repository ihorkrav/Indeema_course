idf_component_register(SRCS "main.c"
                    REQUIRES Task_Load_and_Monitor LED Button WiFi_component HTTP_Server MQTT_Utility UART Servo Stepper Joystick
                  
                    INCLUDE_DIRS "."
)