/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* MQTT over TLS Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "MQTT.h"

static const char *TAG = "mqtt_example";



#include "esp_spiffs.h"


static esp_mqtt_client_handle_t client = NULL;

void init_spiffs(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE("SPIFFS", "Failed to mount or format filesystem");
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data )
{
    esp_mqtt_event_handle_t event = event_data;
    client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, "esp-lection/cmd", 0);
        ESP_LOGI(TAG, "Sent subscribe successful, msg_id=%d", msg_id);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        
        msg_id = esp_mqtt_client_publish(client, "esp-lection/test", "Hello world", 0, 0, 0);
        ESP_LOGI(TAG, "Sent publish successful, msg_id=%d", msg_id);
        
       // else {ESP_LOGI(TAG, "Wifi connection error");}
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA Received");

        // 1. Validate the JSON (Recommended to prevent saving 'garbage' to flash)
        cJSON *root = cJSON_ParseWithLength(event->data, event->data_len);
        if (root == NULL) {
            ESP_LOGE(TAG, "Invalid JSON received, ignoring.");
            break;
        }
        cJSON_Delete(root); 

        // 2. Open file on the SPIFFS partition
        // Note: You must initialize SPIFFS in app_main for this to work!
        FILE* f = fopen("/spiffs/data.json", "w"); 
        if (f == NULL) {
            ESP_LOGE(TAG, "Failed to open file. Is SPIFFS mounted?");
            break;
        }

        // 3. Write exactly the number of bytes received
        int bytes_written = fprintf(f, "%.*s", event->data_len, event->data);
        fclose(f);

        if (bytes_written > 0) {
            ESP_LOGI(TAG, "File saved successfully to /spiffs/data.json");
        }
        break;

   

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            ESP_LOGE(TAG, "TCP Error: %s", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;

    default:
        break;
    }
}

void mqtt_send_data( const char *data)
{
    if (client == NULL) {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return;
    }

    int msg_id = esp_mqtt_client_publish(client,"esp-lection/test", data, 0, 0, 0);
    ESP_LOGI(TAG, "MQTT publish sent, msg_id=%d", msg_id);
}

void mqtt_app_start()
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_EXAMPLE_MQTT_BROKER_URI, // Ensure this starts with "mqtt://"
    };

    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}