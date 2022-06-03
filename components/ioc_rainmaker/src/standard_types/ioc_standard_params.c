/**
 * @file ioc_standard_params.c
 * IO Connect - Standard params - Codor Stelian <codor.stelian.n@gmail.com>
 * NO LICENSE
 */
#include <stdint.h>
#include <esp_err.h>
#include <esp_rmaker_core.h>
#include <ioc_standard_types.h>

esp_rmaker_param_t *ioc_name_param_create(const char *param_name, const char *val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_NAME,
            esp_rmaker_str(val), PROP_FLAG_READ | PROP_FLAG_WRITE | PROP_FLAG_PERSIST);
    return param;
}

esp_rmaker_param_t *ioc_power_param_create(const char *param_name, bool val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_POWER,
            esp_rmaker_bool(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_TOGGLE);
    }
    return param;
}

esp_rmaker_param_t *ioc_power_btn_param_create(const char *param_name, bool val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_POWER,
            esp_rmaker_bool(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_PUSH_BTN_BIG);
    }
    return param;
}

esp_rmaker_param_t *ioc_brightness_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_BRIGHTNESS,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_SLIDER);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(100), esp_rmaker_int(1));
    }
    return param;
}

esp_rmaker_param_t *ioc_hue_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_HUE,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_HUE_SLIDER);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(360), esp_rmaker_int(1));
    }
    return param;
}

esp_rmaker_param_t *ioc_hue_circle_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_HUE,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_HUE_CIRCLE);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(360), esp_rmaker_int(1));
    }
    return param;
}

esp_rmaker_param_t *ioc_saturation_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_SATURATION,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_SLIDER);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(100), esp_rmaker_int(1));
    }
    return param;
}

esp_rmaker_param_t *ioc_intensity_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_INTENSITY,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_SLIDER);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(100), esp_rmaker_int(1));
    }
    return param;
}

esp_rmaker_param_t *ioc_cct_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_CCT,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_SLIDER);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(2700), esp_rmaker_int(6500), esp_rmaker_int(100));
    }
    return param;
}

esp_rmaker_param_t *ioc_direction_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_DIRECTION,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_DROPDOWN);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(1), esp_rmaker_int(1));
    }
    return param;
}

esp_rmaker_param_t *ioc_speed_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_SPEED,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_WRITE);
    if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_SLIDER);
        esp_rmaker_param_add_bounds(param, esp_rmaker_int(0), esp_rmaker_int(5), esp_rmaker_int(1));
    }
    return param;
}

esp_rmaker_param_t *ioc_temperature_param_create(const char *param_name, float val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_TEMPERATURE,
            esp_rmaker_float(val), PROP_FLAG_READ | PROP_FLAG_TIME_SERIES);
    return param;
}

esp_rmaker_param_t *ioc_humidity_param_create(const char *param_name, float val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_HUMIDITY,
            esp_rmaker_float(val), PROP_FLAG_READ | PROP_FLAG_TIME_SERIES);
    return param;
}

esp_rmaker_param_t *ioc_luminosity_param_create(const char *param_name, int val)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_LUMINOSITY,
            esp_rmaker_int(val), PROP_FLAG_READ | PROP_FLAG_TIME_SERIES);
    return param;
}

esp_rmaker_param_t *ioc_reboot_param_create(const char *param_name)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_REBOOT,
            esp_rmaker_bool(false), PROP_FLAG_READ | PROP_FLAG_WRITE);
	if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_TRIGGER);
	}
    return param;
}

esp_rmaker_param_t *ioc_factory_reset_param_create(const char *param_name)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_FACTORY_RESET,
            esp_rmaker_bool(false), PROP_FLAG_READ | PROP_FLAG_WRITE);
	if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_TRIGGER);
	}
    return param;
}

esp_rmaker_param_t *ioc_wifi_reset_param_create(const char *param_name)
{
    esp_rmaker_param_t *param = esp_rmaker_param_create(param_name, IOC_PARAM_WIFI_RESET,
            esp_rmaker_bool(false), PROP_FLAG_READ | PROP_FLAG_WRITE);
	if (param) {
        esp_rmaker_param_add_ui_type(param, IOC_UI_TYPE_TRIGGER);
	}
    return param;
}