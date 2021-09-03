/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
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