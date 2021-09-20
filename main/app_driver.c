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
#include <esp_rmaker_core.h>
#include <ioc_standard_types.h>

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <app_priv.h>
#include <app_reset.h>
#include <iot_button.h>

#include <bh1750.h>
#include <sht3x.h>

#define I2C_SDA_GPIO CONFIG_I2C_SDA_GPIO
#define I2C_SCL_GPIO CONFIG_I2C_SCL_GPIO

#define ADDR_BH1750 BH1750_ADDR_LO
#define ADDR_SHT31 SHT3X_I2C_ADDR_GND
#define DEFAULT_REPORTING_PERIOD_BH1750 60 // Reporting period in seconds for BH1750 luminosity sensor
#define DEFAULT_REPORTING_PERIOD_SHT31 305 // Reporting period in seconds for SHT31 temperature and humidity sensor

/* This is the button that is used for toggling the power */
#define BUTTON_GPIO CONFIG_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL 0
#define WIFI_RESET_BUTTON_TIMEOUT 3
#define FACTORY_RESET_BUTTON_TIMEOUT 10

#define RELAY_O_GPIO CONFIG_RELAY_0_OUTPUT_GPIO
#define RELAY_1_GPIO CONFIG_RELAY_1_OUTPUT_GPIO
#define RELAY_2_GPIO CONFIG_RELAY_2_OUTPUT_GPIO
#define RELAY_3_GPIO CONFIG_RELAY_3_OUTPUT_GPIO
#define DEFAULT_LIGHT0_POWER_STATE false
#define DEFAULT_LIGHT3_POWER_STATE false
#define DEFAULT_LIGHT0_BRIGHTNESS 25

static bool g_light0_power_state = DEFAULT_LIGHT0_POWER_STATE;
static bool g_light3_power_state = DEFAULT_LIGHT3_POWER_STATE;
static uint16_t g_light0_value = DEFAULT_LIGHT0_BRIGHTNESS;

static i2c_dev_t dev17;
static sht3x_t dev31;
static TimerHandle_t bh1750_sensor_timer;
static TimerHandle_t sht31_sensor_timer;
static uint16_t g_sensor_luminosity = 0;
static float g_sensor_temperature = 0;
static float g_sensor_humidity = 0;

static const char *TAG = "app_driver";

float roundfp(float value, uint8_t precision)
{
    int valp = pow(10, precision);
    return roundf(value * valp) / valp;
}

static void app_driver_sensor_get_bh1750_data()
{
    uint16_t lux;
    if (bh1750_read(&dev17, &lux) != ESP_OK)
        ESP_LOGE(TAG, "BH1750 error, could not read sensor data");
    g_sensor_luminosity = lux;
}

static void app_driver_sensor_get_sht31_data()
{
    float temp;
    float humid;
    if (sht3x_measure(&dev31, &temp, &humid) != ESP_OK)
        ESP_LOGE(TAG, "SHT3x error, could not read sensor data");
    g_sensor_temperature = roundfp(temp, 1);
    g_sensor_humidity = roundfp(humid, 1);
}

static void app_driver_sensor_bh1750_update(void *pvParameters)
{
    app_driver_sensor_get_bh1750_data();
    esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(luminosity_sensor, IOC_PARAM_LUMINOSITY),
        esp_rmaker_int(g_sensor_luminosity));
}

static void app_driver_sensor_sht31_update(void *pvParameters)
{  
    app_driver_sensor_get_sht31_data();
    esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(temperature_sensor, IOC_PARAM_TEMPERATURE),
        esp_rmaker_float(g_sensor_temperature));
    esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(humidity_sensor, IOC_PARAM_HUMIDITY),
        esp_rmaker_float(g_sensor_humidity));
}

uint16_t app_driver_sensor_get_current_luminosity()
{
    return g_sensor_luminosity;
}

float app_driver_sensor_get_current_temperature()
{
    return g_sensor_temperature;
}

float app_driver_sensor_get_current_humidity()
{
    return g_sensor_humidity;
}

esp_err_t app_driver_sensor_init(void)
{
    ESP_ERROR_CHECK(i2cdev_init());
    memset(&dev17, 0, sizeof(i2c_dev_t));
    memset(&dev31, 0, sizeof(sht3x_t));
    ESP_ERROR_CHECK(bh1750_init_desc(&dev17, ADDR_BH1750, 0, I2C_SDA_GPIO, I2C_SCL_GPIO));
    ESP_ERROR_CHECK(sht3x_init_desc(&dev31, 0, ADDR_SHT31, I2C_SDA_GPIO, I2C_SCL_GPIO));
	
    esp_err_t err = bh1750_setup(&dev17, BH1750_MODE_CONTINUOUS, BH1750_RES_HIGH);
	if (err == ESP_OK)
	{
		bh1750_sensor_timer = xTimerCreate("app_driver_sensor_bh1750_update_tm", (DEFAULT_REPORTING_PERIOD_BH1750 * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, app_driver_sensor_bh1750_update);
		if (bh1750_sensor_timer) {
			xTimerStart(bh1750_sensor_timer, 0);
		}
        app_driver_sensor_get_bh1750_data();
	}
	else
    {
		ESP_LOGE(TAG, "Could not setup BH1750 luminosity sensor!");
	}
	
    err = sht3x_init(&dev31);
	if (err == ESP_OK)
	{
		sht31_sensor_timer = xTimerCreate("app_driver_sensor_sht31_update_tm", (DEFAULT_REPORTING_PERIOD_SHT31 * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, app_driver_sensor_sht31_update);
		if (sht31_sensor_timer) {
			xTimerStart(sht31_sensor_timer, 0);
		}
        app_driver_sensor_get_sht31_data();
	}
	else
    {
		ESP_LOGE(TAG, "Could not setup SHT31 temperature and humidity sensor!");
	}
	
    return ESP_OK;
}

static void push_btn_cb(void *arg)
{
    ESP_LOGI(TAG, "Change state of Bedroom Light and sync it with cloud");
    bool new_light0_state = !g_light0_power_state;
    app_driver_set_light0_power_state(new_light0_state);
    esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(bedroom_light, IOC_PARAM_POWER),
        esp_rmaker_bool(new_light0_state));
}

static void set_light0_power_state(bool target)
{
    gpio_set_level(RELAY_O_GPIO, target);
}
static void set_light1_power_state(bool target)
{
    gpio_set_level(RELAY_1_GPIO, target);
}
static void set_light2_power_state(bool target)
{
    gpio_set_level(RELAY_2_GPIO, target);
}
static void set_light3_power_state(bool target)
{
    gpio_set_level(RELAY_3_GPIO, target);
}

void app_driver_init()
{
    button_handle_t btn_handle = iot_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LEVEL);
    if (btn_handle)
    {
        /* Register a callback for a button tap (short press) event */
        iot_button_set_evt_cb(btn_handle, BUTTON_CB_TAP, push_btn_cb, NULL);
        /* Register Wi-Fi reset and factory reset functionality on same button */
        app_reset_button_register(btn_handle, WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
    }

    /* Configure the GPIO */
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
    };
    uint64_t pin_mask = (((uint64_t)1 << RELAY_O_GPIO) | ((uint64_t)1 << RELAY_1_GPIO) | ((uint64_t)1 << RELAY_2_GPIO) | ((uint64_t)1 << RELAY_3_GPIO));
    io_conf.pin_bit_mask = pin_mask;
    gpio_config(&io_conf);
    app_driver_sensor_init();
}

int IRAM_ATTR app_driver_set_light0()
{
    if (g_light0_power_state)
    {
        if (g_light0_value == 0)
        {
            set_light0_power_state(0);
            set_light1_power_state(0);
            set_light2_power_state(0);
        }
        else if (g_light0_value >= 1 && g_light0_value <= 25)
        {
            set_light0_power_state(1);
            set_light1_power_state(0);
            set_light2_power_state(0);
        }
        else if (g_light0_value >= 26 && g_light0_value <= 50)
        {
            set_light0_power_state(1);
            set_light1_power_state(0);
            set_light2_power_state(1);
        }
        else if (g_light0_value >= 51 && g_light0_value <= 75)
        {
            set_light0_power_state(1);
            set_light1_power_state(1);
            set_light2_power_state(0);
        }
        else if (g_light0_value >= 76)
        {
            set_light0_power_state(1);
            set_light1_power_state(1);
            set_light2_power_state(1);
        }
    }
    else
    {
        set_light0_power_state(0);
        set_light1_power_state(0);
        set_light2_power_state(0);
    }
    return ESP_OK;
}

esp_err_t app_driver_set_light0_power_state(bool power)
{
    g_light0_power_state = power;
    app_driver_set_light0();
    return ESP_OK;
}

int IRAM_ATTR app_driver_set_light3_power_state(bool state)
{
    if (g_light3_power_state != state)
    {
        g_light3_power_state = state;
        set_light3_power_state(g_light3_power_state);
    }
    return ESP_OK;
}

esp_err_t app_driver_set_light0_brightness(uint16_t brightness)
{
    g_light0_value = brightness;
    g_light0_power_state = 1;
    esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_type(bedroom_light, IOC_PARAM_POWER),
        esp_rmaker_bool(g_light0_power_state));
    return app_driver_set_light0();
}

bool app_driver_get_light0_power_state(void)
{
    return g_light0_power_state;
}

bool app_driver_get_light3_power_state(void)
{
    return g_light3_power_state;
}

uint16_t app_driver_get_light0_brightness()
{
    return g_light0_value;
}