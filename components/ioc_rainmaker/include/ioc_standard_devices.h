/**
 * @file ioc_standard_devices.h
 * IO Connect - Standard devices - Codor Stelian <codor.stelian.n@gmail.com>
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

/** Create a standard Switch device
 *
 * This creates a Switch device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] power Default value of the mandatory parameter "power"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_switch_device_create(const char *dev_name,
            void *priv_data, bool power);

/** Create a standard Lightbulb device
 *
 * This creates a Lightbulb device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] power Default value of the mandatory parameter "power"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_lightbulb_device_create(const char *dev_name,
            void *priv_data, bool power);

/** Create a standard RGB Lightbulb device
 *
 * This creates a RGB Lightbulb device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] power Default value of the mandatory parameter "power"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_lightbulb_rgb_device_create(const char *dev_name,
            void *priv_data, bool power);

/** Create a standard CCT Lightbulb device
 *
 * This creates a CCT Lightbulb device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] power Default value of the mandatory parameter "power"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_lightbulb_cct_device_create(const char *dev_name,
            void *priv_data, bool power);

/** Create a standard Lock device
 *
 * This creates an Lock device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] power Default value of the mandatory parameter "power"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_lock_device_create(const char *dev_name,
            void *priv_data, bool power);

/** Create a standard Thermostat device
 *
 * This creates an Thermostat device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] temperature Default value of the mandatory parameter "temperature"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_thermostat_device_create(const char *dev_name,
            void *priv_data, float temperature);

/** Create a standard Fan device
 *
 * This creates a Fan device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] power Default value of the mandatory parameter "power"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_fan_device_create(const char *dev_name,
            void *priv_data, bool power);

/** Create a standard Temperature Sensor device
 *
 * This creates a Temperature Sensor device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] temperature Default value of the mandatory parameter "temperature"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_temp_sensor_device_create(const char *dev_name,
            void *priv_data, float temperature);

/** Create a standard Humidity Sensor device
 *
 * This creates a Humidity Sensor device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in]  humidity Default value of the mandatory parameter "humidity"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_humid_sensor_device_create(const char *dev_name,
            void *priv_data, float humidity);

/** Create a standard Luminosity Sensor device
 *
 * This creates a Luminosity Sensor device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in]  luminosity Default value of the mandatory parameter "luminosity"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_lumen_sensor_device_create(const char *dev_name,
            void *priv_data, float luminosity);

/** Create a standard Outlet device
 *
 * This creates an Outlet device with the mandatory parameters and also assigns
 * the primary parameter. The default parameter names will be used.
 * Refer \ref cs_esp_rmaker_standard_params.h for default names.
 *
 * @param[in] dev_name The unique device name
 * @param[in] priv_data (Optional) Private data associated with the device. This should stay
 * allocated throughout the lifetime of the device
 * @param[in] power Default value of the mandatory parameter "power"
 *
 * @return Device handle on success.
 * @return NULL in case of failures.
 */
esp_rmaker_device_t *ioc_outlet_device_create(const char *dev_name,
            void *priv_data, bool power);

#ifdef __cplusplus
}
#endif