/**
 * @file ioc_standard_devices.c
 * IO Connect - Standard devices - Codor Stelian <codor.stelian.n@gmail.com>
 * NO LICENSE
 */
#include <ioc_standard_types.h>
#include <ioc_standard_params.h>

esp_rmaker_device_t *ioc_switch_device_create(const char *dev_name,
        void *priv_data, bool power)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_SWITCH, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_power_param_create(IOC_DEF_POWER_NAME, power);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
	}
    return device;
}

esp_rmaker_device_t *ioc_lightbulb_device_create(const char *dev_name,
        void *priv_data, bool power)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_LIGHTBULB, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_power_param_create(IOC_DEF_POWER_NAME, power);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
    }
    return device;
}

esp_rmaker_device_t *ioc_lightbulb_rgb_device_create(const char *dev_name,
        void *priv_data, bool power)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_LIGHTBULB_RGB, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_power_param_create(IOC_DEF_POWER_NAME, power);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
    }
    return device;
}

esp_rmaker_device_t *ioc_lightbulb_cct_device_create(const char *dev_name,
        void *priv_data, bool power)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_LIGHTBULB_CCT, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_power_param_create(IOC_DEF_POWER_NAME, power);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
    }
    return device;
}

esp_rmaker_device_t *ioc_lock_device_create(const char *dev_name,
        void *priv_data, bool power)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_LOCK, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_power_param_create(IOC_DEF_POWER_NAME, power);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
	}
    return device;
}

esp_rmaker_device_t *ioc_thermostat_device_create(const char *dev_name,
        void *priv_data, float temperature)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_THERMOSTAT, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_temperature_param_create(IOC_DEF_TEMPERATURE_NAME, temperature);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
	}
    return device;
}

esp_rmaker_device_t *ioc_fan_device_create(const char *dev_name,
        void *priv_data, bool power)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_FAN, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_power_param_create(IOC_DEF_POWER_NAME, power);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
    }
    return device;
}

esp_rmaker_device_t *ioc_temp_sensor_device_create(const char *dev_name,
        void *priv_data, float temperature)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_TEMP_SENSOR, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_temperature_param_create(IOC_DEF_TEMPERATURE_NAME, temperature);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
	}
    return device;
}

esp_rmaker_device_t *ioc_humid_sensor_device_create(const char *dev_name,
        void *priv_data, float humidity)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_HUMID_SENSOR, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_humidity_param_create(IOC_DEF_HUMIDITY_NAME, humidity);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
	}
    return device;
}

esp_rmaker_device_t *ioc_lumen_sensor_device_create(const char *dev_name,
        void *priv_data, float luminosity)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_LUMEN_SENSOR, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_luminosity_param_create(IOC_DEF_LUMINOSITY_NAME, luminosity);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
	}
    return device;
}

esp_rmaker_device_t *ioc_outlet_device_create(const char *dev_name,
        void *priv_data, bool power)
{
    esp_rmaker_device_t *device = esp_rmaker_device_create(dev_name, IOC_DEVICE_OUTLET, priv_data);
    if (device) {
        esp_rmaker_device_add_param(device, ioc_name_param_create(IOC_DEF_NAME_PARAM, dev_name));
        esp_rmaker_param_t *primary = ioc_power_param_create(IOC_DEF_POWER_NAME, power);
        esp_rmaker_device_add_param(device, primary);
        esp_rmaker_device_assign_primary_param(device, primary);
	}
    return device;
}