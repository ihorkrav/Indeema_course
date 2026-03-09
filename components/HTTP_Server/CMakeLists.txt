idf_component_register(SRCS "HTTP.c"
REQUIRES esp_http_server esp_wifi nvs_flash mdns
                    INCLUDE_DIRS "include")
