/**
 * @file wifi_reconnect.c
 * ESP WiFi reconnect
 * Forked from <https://github.com/mdvorak-iot/esp-wifi-reconnect>
 * NO LICENSE
 */
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <esp_wifi.h>

#include <freertos/event_groups.h>
#include <wifi_reconnect.h>

static const char TAG[] = "wifi_reconnect";

// Reconnect incremental backoff, in seconds
static const uint8_t DELAYS[] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233};

#define RECONNECT_BIT BIT0
#define CONNECTED_BIT BIT1
// Negative state is used for wait_for_reconnect, to avoid state polling
#define NOT_CONNECTED_BIT BIT2

static EventGroupHandle_t wifi_event_group;
static uint32_t connect_timeout = WIFI_RECONNECT_CONNECT_TIMEOUT_MS;

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

static inline bool is_ssid_stored(wifi_config_t *conf)
{
    esp_err_t err = esp_wifi_get_config(WIFI_IF_STA, conf);
    return err == ESP_OK && conf->sta.ssid[0] != '\0';
}

static bool wait_for_reconnect()
{
    // NOTE this will return immediately, if both bits are already set
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, RECONNECT_BIT | NOT_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    return (bits & RECONNECT_BIT) != 0 && (bits & NOT_CONNECTED_BIT) != 0;
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "WiFi disconnected");
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        xEventGroupSetBits(wifi_event_group, NOT_CONNECTED_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));

        // Mark as connected, also enable reconnect automatically
        xEventGroupClearBits(wifi_event_group, NOT_CONNECTED_BIT);
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT | RECONNECT_BIT);
    }
}

_Noreturn static void wifi_reconnect_task(void *unused)
{
    ESP_LOGI(TAG, "WiFi reconnect started");
    uint8_t failures = 0;

    // Infinite task loop
    for (;;)
    {
        esp_task_wdt_reset();

        wifi_config_t conf = {};
        if (wait_for_reconnect() && is_ssid_stored(&conf))
        {
            // Simple back-off algorithm
            TickType_t waitFor = DELAYS[failures] * 1000;
            failures = MIN(sizeof(DELAYS) / sizeof(uint8_t) - 1, failures + 1);

            // Delay
            if (waitFor > 0)
            {
                ESP_LOGI(TAG, "Waiting for %d ms", waitFor);
                vTaskDelay(waitFor / portTICK_PERIOD_MS);
            }

            // Start reconnect
            ESP_LOGI(TAG, "Connecting to '%s', timeout %d ms", conf.sta.ssid, connect_timeout);
			esp_err_t err;
            err = esp_wifi_connect();
			if (err != ESP_OK) {
				ESP_LOGE(TAG, "esp_wifi_connect() returned error 0x%x.", err);
                esp_wifi_stop(); 
                esp_wifi_start();
			}

            // Wait for connection
            bool connected = (xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, pdFALSE, pdTRUE, connect_timeout / portTICK_PERIOD_MS) & CONNECTED_BIT) != 0;

            // Reset failures, if connected successfully
            if (connected)
            {
                failures = 0;
                ESP_LOGI(TAG, "Connected successfully");
            }
            else
            {
                ESP_LOGI(TAG, "Connect timeout");
            }
        }
        else
        {
            // Simply wait for a moment
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

esp_err_t wifi_reconnect_start()
{
    esp_err_t err;

    // Prepare event group
    wifi_event_group = xEventGroupCreate();
    configASSERT(wifi_event_group);
    xEventGroupSetBits(wifi_event_group, NOT_CONNECTED_BIT); // Negative bit must be set immediately

    // Register event handlers
    err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_event_handler, NULL);
    if (err != ESP_OK)
        return err;

    err = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);
    if (err != ESP_OK)
        return err;

    // Create background task
    BaseType_t ret = xTaskCreate(wifi_reconnect_task, "wifi_reconnect", 4096, NULL, tskIDLE_PRIORITY + 1, NULL);
    return ret == pdPASS ? ESP_OK : ESP_FAIL;
}

void wifi_reconnect_enable(bool enable)
{
    if (enable)
    {
        xEventGroupSetBits(wifi_event_group, RECONNECT_BIT);
        ESP_LOGI(TAG, "WiFi reconnect enabled");
    }
    else
    {
        xEventGroupClearBits(wifi_event_group, RECONNECT_BIT);
        ESP_LOGI(TAG, "WiFi reconnect disabled");
    }
}

esp_err_t wifi_reconnect_pause()
{
    wifi_reconnect_enable(false);
    return ESP_OK;
}

esp_err_t wifi_reconnect_resume()
{
    wifi_reconnect_enable(true);
    return ESP_OK;
}

bool wifi_reconnect_is_ssid_stored()
{
    wifi_config_t conf;
    return is_ssid_stored(&conf);
}

bool wifi_reconnect_is_connected()
{
    EventBits_t bits = xEventGroupGetBits(wifi_event_group);
    return (bits & CONNECTED_BIT) != 0;
}

bool wifi_reconnect_wait_for_connection(uint32_t timeout_ms)
{
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, pdFALSE, pdTRUE, timeout_ms / portTICK_PERIOD_MS);
    return (bits & CONNECTED_BIT) != 0;
}