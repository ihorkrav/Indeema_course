#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "esp_spiffs.h"

#include "mqtt_client.h"
#include "cJSON.h"


void mqtt_app_start(void);
void mqtt_send_data(const char *data);