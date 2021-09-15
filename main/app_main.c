/*
 * BSD 2-Clause License
 * 
 * Copyright (c) 2021, Codor Stelian <codor.stelian.n@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <esp_log.h>
#include <esp_event.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_common_events.h>
#include <esp_wifi.h>
#include <wifi_provisioning/manager.h>

#include <nvs_flash.h>
#include <string.h>

#include <app_insights.h>
#include <app_priv.h>
#include <app_rainmaker.h>
#include <app_wifi.h>

#include <rgbpixel.h>
#include <wifi_reconnect.h>

static const char TAG[] = "app_main";
static const char TAG_EVENT[] = "app_main_EVENT";

// Devices
esp_rmaker_device_t *bedroom_light;
esp_rmaker_device_t *wall_light;
esp_rmaker_device_t *rgb_ring_light;
esp_rmaker_device_t *temperature_sensor;
esp_rmaker_device_t *humidity_sensor;
esp_rmaker_device_t *luminosity_sensor;

#define APP_DEVICE_NAME CONFIG_APP_DEVICE_NAME
#define APP_DEVICE_TYPE CONFIG_APP_DEVICE_TYPE

// Program
static void app_devices_init(esp_rmaker_node_t *node);

// Callback to handle commands received from the RainMaker cloud
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
                          const esp_rmaker_param_val_t val, void *priv_data, esp_rmaker_write_ctx_t *ctx)
{
    const char *device_name = esp_rmaker_device_get_name(device);
    const char *param_name = esp_rmaker_param_get_name(param);
    if (ctx)
    {
        ESP_LOGI(TAG, "Received write request via : %s", esp_rmaker_device_cb_src_to_str(ctx->src));
    }
    if (strcmp(device_name, "RGB Light") == 0)
    {
        if (strcmp(param_name, ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s", val.val.b ? "true" : "false", device_name, param_name);
            rgbpixel_set_power_state(val.val.b);
            rgbpixel_start_anim(0, true);
        }
        else if (strcmp(param_name, "Brightness") == 0)
        {
            ESP_LOGI(TAG, "Received value = %d for %s - %s", val.val.i, device_name, param_name);
            rgbpixel_set_brightness(val.val.i);
        }
        else if (strcmp(param_name, "Hue") == 0)
        {
            ESP_LOGI(TAG, "Received value = %d for %s - %s", val.val.i, device_name, param_name);
            rgbpixel_set_hue(val.val.i);
        }
        else if (strcmp(param_name, "Saturation") == 0)
        {
            ESP_LOGI(TAG, "Received value = %d for %s - %s", val.val.i, device_name, param_name);
            rgbpixel_set_saturation(val.val.i);
        }
    }
    else if (strcmp(device_name, "Bedroom Light") == 0)
    {
        if (strcmp(param_name, ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s", val.val.b ? "true" : "false", device_name, param_name);
            app_driver_set_light0_power_state(val.val.b);
            rgbpixel_start_anim(0, true);
        }
        else if (strcmp(param_name, "Brightness") == 0)
        {
            ESP_LOGI(TAG, "Received value = %d for %s - %s", val.val.i, device_name, param_name);
            app_driver_set_light0_brightness(val.val.i);
        }
    }
    else if (strcmp(device_name, "Wall Light") == 0)
    {
        ESP_LOGI(TAG, "Received value = %s for %s - %s", val.val.b ? "true" : "false", device_name, param_name);
        if (strcmp(param_name, ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            app_driver_set_light3_power_state(val.val.b);
            rgbpixel_start_anim(0, true);
        }
    }
    else
    {
        // Silently ignoring invalid params
        return ESP_OK;
    }
    esp_rmaker_param_update_and_report(param, val);
    return ESP_OK;
}

// Event handler for catching Wi-Fi events
static void event_handler(void* arg, esp_event_base_t event_base,
                          int event_id, void* event_data)
{
    if (event_base == WIFI_PROV_EVENT) {
        switch (event_id) {
            case WIFI_PROV_START:
                ESP_LOGI(TAG_EVENT, "Provisioning started");
                rgbpixel_start_anim(1, false);
                break;
            case WIFI_PROV_CRED_SUCCESS:
                ESP_LOGI(TAG_EVENT, "Provisioning successful");
                rgbpixel_start_anim(4, true);
                break;
            default:
                break;
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG_EVENT, "WiFi disconnected");
        rgbpixel_start_anim(5, false);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG_EVENT, "WiFi connected");
        rgbpixel_start_anim(2, true);
    }
}

// Event handler for catching RainMaker events
static void rmk_event_handler(void* arg, esp_event_base_t event_base,
                          int event_id, void* event_data)
{
    if (event_base == RMAKER_EVENT) {
        switch (event_id) {
            case RMAKER_EVENT_INIT_DONE:
                ESP_LOGI(TAG_EVENT, "RainMaker Initialised.");
                rgbpixel_start_anim(1, false);
                break;
            case RMAKER_EVENT_CLAIM_STARTED:
                ESP_LOGI(TAG_EVENT, "RainMaker Claim Started.");
                rgbpixel_start_anim(1, false);
                break;
            case RMAKER_EVENT_CLAIM_SUCCESSFUL:
                ESP_LOGI(TAG_EVENT, "RainMaker Claim Successful.");
                rgbpixel_start_anim(2, true);
                break;
            case RMAKER_EVENT_CLAIM_FAILED:
                ESP_LOGI(TAG_EVENT, "RainMaker Claim Failed.");
                rgbpixel_start_anim(3, true);
                break;
            default:
                ESP_LOGW(TAG_EVENT, "Unhandled RainMaker Event: %d", event_id);
        }
    } else if (event_base == RMAKER_COMMON_EVENT) {
        switch (event_id) {
            case RMAKER_EVENT_REBOOT:
                ESP_LOGI(TAG_EVENT, "Rebooting in %d seconds.", *((uint8_t *)event_data));
                rgbpixel_start_anim(1, false);
                break;
            case RMAKER_EVENT_WIFI_RESET:
                ESP_LOGI(TAG_EVENT, "Wi-Fi credentials reset.");
                rgbpixel_start_anim(1, false);
                break;
            case RMAKER_EVENT_FACTORY_RESET:
                ESP_LOGI(TAG_EVENT, "Node reset to factory defaults.");
                rgbpixel_start_anim(1, false);
                break;
            case RMAKER_MQTT_EVENT_CONNECTED:
                ESP_LOGI(TAG_EVENT, "MQTT Connected.");
                break;
            case RMAKER_MQTT_EVENT_DISCONNECTED:
                ESP_LOGI(TAG_EVENT, "MQTT Disconnected.");
                break;
            case RMAKER_MQTT_EVENT_PUBLISHED:
                ESP_LOGI(TAG_EVENT, "MQTT Published. Msg id: %d.", *((int *)event_data));
                break;
            default:
                ESP_LOGW(TAG_EVENT, "Unhandled RainMaker Common Event: %d", event_id);
        }
    } else {
        ESP_LOGW(TAG_EVENT, "Invalid event received!");
    }
}

void setup()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Setup
    err = app_driver_rgbpixel_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Could not setup rgbpixel!");
    }
    app_driver_init();

    // Wi-Fi
    struct app_wifi_config wifi_cfg = {
        .wifi_connect = wifi_reconnect_resume,
    };
    ESP_ERROR_CHECK(app_wifi_init(&wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MAX_MODEM));

    // Register an event handler to catch Wi-Fi, IP and Provisioning events
    err = esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    err = esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &event_handler, NULL);
    err = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL);
    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "Could not register Wi-Fi event handler");
    }

    // Start Wi-Fi reconnect
    ESP_ERROR_CHECK(wifi_reconnect_start());

    // Register an event handler to catch RainMaker events
    err = esp_event_handler_register(RMAKER_EVENT, ESP_EVENT_ANY_ID, &rmk_event_handler, NULL);
    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "Could not register RainMaker event handler");
    }

    // RainMaker
    esp_rmaker_node_t *node = NULL;
    ESP_ERROR_CHECK(app_rmaker_init(CONFIG_APP_DEVICE_NAME, CONFIG_APP_DEVICE_TYPE, &node));

    // Devices
    app_devices_init(node);

    // Enable Insights
    app_insights_enable();

    // Start
    ESP_ERROR_CHECK(esp_rmaker_start());

    // Start the Wi-Fi.
    // If the node is provisioned, it will start connection attempts,
    // else, it will start Wi-Fi provisioning. The function will return
    // after a connection has been successfully established
    err = app_wifi_start(POP_TYPE_MAC);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Could not start WiFi!");
    }

    // Done
    ESP_LOGI(TAG, "Setup complete");
}

static void app_devices_init(esp_rmaker_node_t *node)
{
    // Create devices
    bedroom_light = esp_rmaker_lightbulb_device_create("Bedroom Light", NULL, app_driver_get_light0_power_state());
    ESP_ERROR_CHECK(esp_rmaker_device_add_cb(bedroom_light, write_cb, NULL));
    ESP_ERROR_CHECK(esp_rmaker_device_add_param(bedroom_light, esp_rmaker_brightness_param_create("Brightness", app_driver_get_light0_brightness())));
    ESP_ERROR_CHECK(esp_rmaker_node_add_device(node, bedroom_light));

    wall_light = esp_rmaker_lightbulb_device_create("Wall Light", NULL, app_driver_get_light3_power_state());
    ESP_ERROR_CHECK(esp_rmaker_device_add_cb(wall_light, write_cb, NULL));
    ESP_ERROR_CHECK(esp_rmaker_node_add_device(node, wall_light));

    rgb_ring_light = esp_rmaker_lightbulb_device_create("RGB Light", NULL, rgbpixel_get_power_state());
    ESP_ERROR_CHECK(esp_rmaker_device_add_cb(rgb_ring_light, write_cb, NULL));
    ESP_ERROR_CHECK(esp_rmaker_device_add_param(rgb_ring_light, esp_rmaker_hue_param_create("Hue", rgbpixel_get_hue())));
    //esp_rmaker_param_t *hue_param = esp_rmaker_param_create("Hue", ESP_RMAKER_PARAM_HUE,
    //        esp_rmaker_int(rgbpixel_get_hue()), PROP_FLAG_READ | PROP_FLAG_WRITE);
    //ESP_ERROR_CHECK(esp_rmaker_param_add_ui_type(hue_param, "esp.ui.hue-circle"));
    //ESP_ERROR_CHECK(esp_rmaker_param_add_bounds(hue_param, esp_rmaker_int(0), esp_rmaker_int(360), esp_rmaker_int(1)));
    //ESP_ERROR_CHECK(esp_rmaker_device_add_param(rgb_ring_light, hue_param));
    ESP_ERROR_CHECK(esp_rmaker_device_add_param(rgb_ring_light, esp_rmaker_saturation_param_create("Saturation", rgbpixel_get_saturation())));
    ESP_ERROR_CHECK(esp_rmaker_device_add_param(rgb_ring_light, esp_rmaker_brightness_param_create("Brightness", rgbpixel_get_brightness())));
    ESP_ERROR_CHECK(esp_rmaker_node_add_device(node, rgb_ring_light));

    temperature_sensor = esp_rmaker_temp_sensor_device_create("Temperature Sensor", NULL, app_driver_sensor_get_current_temperature());
    ESP_ERROR_CHECK(esp_rmaker_node_add_device(node, temperature_sensor));

    humidity_sensor = esp_rmaker_device_create("Humidity Sensor", NULL, NULL);
    ESP_ERROR_CHECK(esp_rmaker_device_add_param(humidity_sensor, esp_rmaker_name_param_create("Name", "Humidity Sensor")));
    esp_rmaker_param_t *humidity_param = esp_rmaker_param_create("Humidity", NULL, esp_rmaker_float(app_driver_sensor_get_current_humidity()), PROP_FLAG_READ);
    ESP_ERROR_CHECK(esp_rmaker_device_add_param(humidity_sensor, humidity_param));
    ESP_ERROR_CHECK(esp_rmaker_device_assign_primary_param(humidity_sensor, humidity_param));
    ESP_ERROR_CHECK(esp_rmaker_node_add_device(node, humidity_sensor));

    luminosity_sensor = esp_rmaker_device_create("Luminosity Sensor", NULL, NULL);
    ESP_ERROR_CHECK(esp_rmaker_device_add_param(luminosity_sensor, esp_rmaker_name_param_create("Name", "Luminosity Sensor")));
    esp_rmaker_param_t *luminosity_param = esp_rmaker_param_create("Luminosity", NULL, esp_rmaker_int(app_driver_sensor_get_current_luminosity()), PROP_FLAG_READ);
    ESP_ERROR_CHECK(esp_rmaker_device_add_param(luminosity_sensor, luminosity_param));
    ESP_ERROR_CHECK(esp_rmaker_device_assign_primary_param(luminosity_sensor, luminosity_param));
    ESP_ERROR_CHECK(esp_rmaker_node_add_device(node, luminosity_sensor));
}

void app_main()
{
    setup();

    //Add code to run
}