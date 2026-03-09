idf_component_register(SRCS "MQTT.c"
                        REQUIRES esp-tls esp_wifi esp_event nvs_flash mqtt cjson spiffs
                        INCLUDE_DIRS "include"

)
