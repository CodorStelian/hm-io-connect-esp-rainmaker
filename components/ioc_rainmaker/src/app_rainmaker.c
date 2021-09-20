/**
 * @file app_rainmaker.c
 * RainMaker Init - Codor Stelian <codor.stelian.n@gmail.com>
 * NO LICENSE
 */
#include <esp_log.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_ota.h>
#include <esp_rmaker_schedule.h>

#include <app_rainmaker.h>

//static const char TAG[] = "app_rainmaker";

esp_err_t app_rmaker_init(const char *node_name, const char *device_type, esp_rmaker_node_t **out_node)
{
    esp_err_t err = ESP_OK;

    // Create node
    esp_rmaker_config_t cfg = {
        .enable_time_sync = true,
    };
    esp_rmaker_node_t *node = esp_rmaker_node_init(&cfg, node_name, device_type);
    if (!node) goto error;

    // Enable OTA
    esp_rmaker_ota_config_t ota_config = {
        .server_cert = (char *)ESP_RMAKER_OTA_DEFAULT_SERVER_CERT,
    };
    err = esp_rmaker_ota_enable(&ota_config, OTA_USING_TOPICS);
    if (err != ESP_OK) goto error;

    // Enable timezone service
    err = esp_rmaker_timezone_service_enable();
    if (err != ESP_OK) goto error;

    //  Enable scheduling
    err = esp_rmaker_schedule_enable();
    if (err != ESP_OK) goto error;

    // Return
    *out_node = node;
    return ESP_OK;

    error:
        if (node) esp_rmaker_node_deinit(node);
        return err;
}