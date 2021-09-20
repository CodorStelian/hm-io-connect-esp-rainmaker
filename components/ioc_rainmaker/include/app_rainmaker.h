/**
 * @file app_rainmaker.h
 * RainMaker Init - Codor Stelian <codor.stelian.n@gmail.com>
 * NO LICENSE
 */
#pragma once

#include <esp_rmaker_core.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t app_rmaker_init(const char *node_name, const char *device_type, esp_rmaker_node_t **out_node);

#ifdef __cplusplus
}
#endif