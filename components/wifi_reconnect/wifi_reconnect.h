/**
 * @file wifi_reconnect.h
 * ESP WiFi reconnect
 * Forked from <https://github.com/mdvorak-iot/esp-wifi-reconnect>
 * NO LICENSE
 */
#pragma once

#include <esp_err.h>
#include <stdbool.h>

#ifndef WIFI_RECONNECT_CONNECT_TIMEOUT_MS
#define WIFI_RECONNECT_CONNECT_TIMEOUT_MS CONFIG_WIFI_RECONNECT_CONNECT_TIMEOUT_MS
#endif

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t wifi_reconnect_start();

void wifi_reconnect_enable(bool enable);

esp_err_t wifi_reconnect_resume();

esp_err_t wifi_reconnect_pause();

bool wifi_reconnect_is_ssid_stored();

bool wifi_reconnect_is_connected();

bool wifi_reconnect_wait_for_connection(uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif