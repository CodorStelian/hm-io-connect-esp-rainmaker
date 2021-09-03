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
#pragma once

#include <esp_err.h>
#include <stdbool.h>
#include <stdint.h>

extern esp_rmaker_device_t *bedroom_light;
extern esp_rmaker_device_t *wall_light;
extern esp_rmaker_device_t *temperature_sensor;
extern esp_rmaker_device_t *humidity_sensor;
extern esp_rmaker_device_t *luminosity_sensor;

void app_driver_init(void);

esp_err_t app_driver_set_light0_power_state(bool power);
esp_err_t app_driver_set_light0_brightness(uint16_t brightness);
int app_driver_set_light3_power_state(bool state);

bool app_driver_get_light0_power_state(void);
bool app_driver_get_light3_power_state(void);
uint16_t app_driver_get_light0_brightness();
uint16_t app_driver_sensor_get_current_luminosity();
float app_driver_sensor_get_current_temperature();
float app_driver_sensor_get_current_humidity();