/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

#include <esp_err.h>

typedef esp_err_t (*app_wifi_connect_fn)();

struct app_wifi_config
{
	// Function to be used to initiate WiFi connection.
	// Default is esp_wifi_connect().
    app_wifi_connect_fn wifi_connect;
};

//Types of Proof of Possession
typedef enum {
    //Use MAC address to generate PoP
    POP_TYPE_MAC,
    //Use random stream generated and stored in fctry partition during claiming process as PoP
    POP_TYPE_RANDOM
} app_wifi_pop_type_t;

esp_err_t app_wifi_init(const struct app_wifi_config *config);
esp_err_t app_wifi_start(app_wifi_pop_type_t pop_type);