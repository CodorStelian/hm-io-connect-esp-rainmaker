/**
 * @file ioc_standard_params.h
 * IO Connect - Standard types - Codor Stelian <codor.stelian.n@gmail.com>
 * NO LICENSE
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/********** STANDARD UI TYPES **********/

#define IOC_UI_TYPE_TOGGLE			"esp.ui.toggle"
#define IOC_UI_TYPE_PUSH_BTN_BIG	"esp.ui.push-btn-big"
#define IOC_UI_TYPE_SLIDER			"esp.ui.slider"
#define IOC_UI_TYPE_HUE_SLIDER		"esp.ui.hue-slider"
#define IOC_UI_TYPE_HUE_CIRCLE		"esp.ui.hue-circle"
#define IOC_UI_TYPE_DROPDOWN		"esp.ui.dropdown"
#define IOC_UI_TYPE_TEXT			"esp.ui.text"
#define IOC_UI_TYPE_HIDDEN			"esp.ui.hidden"
#define IOC_UI_TYPE_TRIGGER			"esp.ui.trigger"
#define IOC_UI_TYPE_KNOB_BTN			"esp.ui.knob-btn"

/********** STANDARD DEVICE TYPES **********/

#define IOC_DEVICE_SWITCH			"esp.device.switch"
#define IOC_DEVICE_LIGHTBULB		"esp.device.lightbulb"
#define IOC_DEVICE_LIGHTBULB_CCT	"esp.device.lightbulb-cct"
#define IOC_DEVICE_LIGHTBULB_RGB	"esp.device.lightbulb-rgb"
#define IOC_DEVICE_LOCK				"esp.device.lock"
#define IOC_DEVICE_THERMOSTAT		"esp.device.thermostat"
#define IOC_DEVICE_FAN				"esp.device.fan"
#define IOC_DEVICE_SENSOR			"esp.device.sensor"
#define IOC_DEVICE_TEMP_SENSOR		"esp.device.temperature-sensor"
#define IOC_DEVICE_HUMID_SENSOR		"esp.device.humidity-sensor"
#define IOC_DEVICE_LUMEN_SENSOR		"esp.device.luminosity-sensor"
#define IOC_DEVICE_OUTLET			"esp.device.outlet"

/********** STANDARD PARAM TYPES **********/

#define IOC_PARAM_NAME				"esp.param.name"
#define IOC_PARAM_POWER				"esp.param.power"
#define IOC_PARAM_BRIGHTNESS		"esp.param.brightness"
#define IOC_PARAM_HUE				"esp.param.hue"
#define IOC_PARAM_SATURATION		"esp.param.saturation"
#define IOC_PARAM_INTENSITY			"esp.param.intensity"
#define IOC_PARAM_CCT				"esp.param.cct"
#define IOC_PARAM_SPEED				"esp.param.speed"
#define IOC_PARAM_DIRECTION			"esp.param.direction"
#define IOC_PARAM_TEMPERATURE		"esp.param.temperature"
#define IOC_PARAM_HUMIDITY			"esp.param.humidity"
#define IOC_PARAM_LUMINOSITY		"esp.param.luminosity"
#define IOC_PARAM_REBOOT			"esp.param.reboot"
#define IOC_PARAM_FACTORY_RESET		"esp.param.factory-reset"
#define IOC_PARAM_WIFI_RESET		"esp.param.wifi-reset"

#ifdef __cplusplus
}
#endif