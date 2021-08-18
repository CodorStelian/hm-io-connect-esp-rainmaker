#include <app_rainmaker.h>
#include <esp_log.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_ota.h>
#include <esp_rmaker_schedule.h>
#include <esp_wifi.h>

static const char TAG[] = "app_rainmaker";

esp_err_t app_rmaker_init(const char *node_name, const char *device_type, esp_rmaker_node_t **out_node)
{
    esp_err_t err = ESP_OK;

    // MAC address
    uint8_t eth_mac[6] = {};
    esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
	ESP_LOGI(TAG, "MAC address: %02x:%02x:%02x:%02x:%02x:%02x", eth_mac[0], eth_mac[1],eth_mac[2], eth_mac[3],eth_mac[4], eth_mac[5]);

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
    err = esp_rmaker_ota_enable(&ota_config, OTA_USING_PARAMS);
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
