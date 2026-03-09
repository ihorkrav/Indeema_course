#include "HTTP.h"
   // Required for hostname resolution
#include "esp_log.h"
#include "esp_http_server.h"

static const char *TAG = "HTTP_SERVER";

const char* index_html = 
"<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>"
"<style>body{font-family:sans-serif; text-align:center; padding:20px;} "
"input{margin:10px; padding:12px; width:80%; max-width:300px; border-radius:5px; border:1px solid #ccc;}</style></head>"
"<body><h2>ESP32 WiFi Config</h2>"
"<p>Accessing via: <b>esp32.local</b></p>"
"<form action='/save' method='POST'>"
"SSID:<br><input type='text' name='ssid' required><br>"
"Password:<br><input type='password' name='password'><br>"
"<input type='submit' value='Apply & Restart' style='background-color:#4CAF50; color:white; border:none; cursor:pointer;'>"
"</form></body></html>";

// mDNS Initialization: This is what makes "esp32.local" work
static void start_mdns_service() {
    esp_err_t err = mdns_init();
    if (err) {
        ESP_LOGE(TAG, "mDNS Init failed: %d", err);
        return;
    }
    // Set the hostname: you will access it via http://esp32.local
    mdns_hostname_set("esp32");
    mdns_instance_name_set("ESP32 Config Server");
    
    // Add HTTP service so other devices can discover it
    mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0);
    ESP_LOGI(TAG, "mDNS started. Hostname: esp32.local");
}

esp_err_t settings_get_handler(httpd_req_t *req) {
    httpd_resp_send(req, index_html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t settings_post_handler(httpd_req_t *req) {
    char buf[128];
    int ret = httpd_req_recv(req, buf, req->content_len);
    if (ret <= 0) return ESP_FAIL;

    buf[ret] = '\0'; 
    ESP_LOGI(TAG, "Received Form Data: %s", buf);

    httpd_resp_send(req, "<h1>Success!</h1><p>Restarting...</p>", HTTPD_RESP_USE_STRLEN);
    
    vTaskDelay(pdMS_TO_TICKS(2000));
    esp_restart(); 
    return ESP_OK;
}

void start_webserver(void) {
    // 1. Start mDNS first
    start_mdns_service();

    // 2. Start the HTTP Server
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t get_uri = { .uri = "/", .method = HTTP_GET, .handler = settings_get_handler };
        httpd_register_uri_handler(server, &get_uri);

        httpd_uri_t post_uri = { .uri = "/save", .method = HTTP_POST, .handler = settings_post_handler };
        httpd_register_uri_handler(server, &post_uri);
        
        ESP_LOGI(TAG, "Web server running. URL: http://esp32.local");
    }
}