/**
 * @file ioc_standard_params.h
 * IO Connect - Standard params - Codor Stelian <codor.stelian.n@gmail.com>
 * NO LICENSE
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>
#include <esp_rmaker_core.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Suggested default names for the parameters.
 * These will also be used by default if you use any standard device helper APIs.
 *
 * @note These names are not mandatory. You can use the ESP RainMaker Core APIs
 * to create your own parameters with custom names, if required.
 */

#define IOC_DEF_NAME_PARAM			"Name"
#define IOC_DEF_POWER_NAME			"Power"
#define IOC_DEF_BRIGHTNESS_NAME		"Brightness"
#define IOC_DEF_HUE_NAME			"Hue"
#define IOC_DEF_SATURATION_NAME		"Saturation"
#define IOC_DEF_INTENSITY_NAME		"Intensity"
#define IOC_DEF_CCT_NAME			"CCT"
#define IOC_DEF_DIRECTION_NAME		"Direction"
#define IOC_DEF_SPEED_NAME			"Speed"
#define IOC_DEF_TEMPERATURE_NAME	"Temperature"
#define IOC_DEF_SET_TEMPERATURE_NAME	"Set temperature"
#define IOC_DEF_HUMIDITY_NAME		"Humidity"
#define IOC_DEF_LUMINOSITY_NAME		"Luminosity"
#define IOC_DEF_REBOOT_NAME			"Reboot"
#define IOC_DEF_FACTORY_RESET_NAME	"Factory Reset"
#define IOC_DEF_WIFI_RESET_NAME		"Wi-Fi Reset"

/**
 * Create standard name param
 *
 * This will create the standard name parameter.
 * This should be added to all devices for which you want a user customisable name.
 * The value should be same as the device name.
 *
 * All standard device creation APIs will add this internally.
 * No application registered callback will be called for this parameter,
 * and changes will be managed internally.
 *
 * @param[in] param_name Name of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_name_param_create(const char *param_name, const char *val);

/**
 * Create standard Power(Power_Toggle) param
 *
 * This will create the standard power parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_power_param_create(const char *param_name, bool val);

/**
 * Create standard Power_Btn param
 *
 * This will create the standard power parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_power_btn_param_create(const char *param_name, bool val);

/**
 * Create standard Brightness param
 *
 * This will create the standard brightness parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_brightness_param_create(const char *param_name, int val);

/**
 * Create standard Hue(Hue_Slider) param
 *
 * This will create the standard hue parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_hue_param_create(const char *param_name, int val);

/**
 * Create standard Hue_Circle param
 *
 * This will create the standard hue parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_hue_circle_param_create(const char *param_name, int val);

/**
 * Create standard Saturation param
 *
 * This will create the standard saturation parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_saturation_param_create(const char *param_name, int val);

/**
 * Create standard Intensity param
 *
 * This will create the standard intensity parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_intensity_param_create(const char *param_name, int val);

/**
 * Create standard CCT param
 *
 * This will create the standard cct parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_cct_param_create(const char *param_name, int val);

/**
 * Create standard Direction param
 *
 * This will create the standard direction parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_direction_param_create(const char *param_name, int val);

/**
 * Create standard Speed param
 *
 * This will create the standard speed parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_speed_param_create(const char *param_name, int val);

/**
 * Create standard Temperature param
 *
 * This will create the standard temperature parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_temperature_param_create(const char *param_name, float val);

/**
 * Create standard Humidity param
 *
 * This will create the standard humidity parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_humidity_param_create(const char *param_name, float val);

/**
 * Create standard Luminosity param
 *
 * This will create the standard luminosity parameter.
 *
 * @param[in] param_name Name of the parameter
 * @param[in] val Default Value of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_luminosity_param_create(const char *param_name, int val);

/**
 * Create standard Reboot param
 *
 * This will create the standard reboot parameter.
 * Set value to true (via write param) for the action to trigger.
 *
 * @param[in] param_name Name of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_reboot_param_create(const char *param_name);

/**
 * Create standard Factory Reset param
 *
 * This will create the standard factory reset parameter.
 * Set value to true (via write param) for the action to trigger.
 *
 * @param[in] param_name Name of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_factory_reset_param_create(const char *param_name);

/**
 * Create standard Wi-Fi Reset param
 *
 * This will create the standard Wi-Fi Reset parameter.
 * Set value to true (via write param) for the action to trigger.
 *
 * @param[in] param_name Name of the parameter
 *
 * @return Parameter handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_param_t *ioc_wifi_reset_param_create(const char *param_name);

#ifdef __cplusplus
}
#endif