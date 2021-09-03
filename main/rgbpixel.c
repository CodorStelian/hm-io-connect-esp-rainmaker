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
#include <esp_log.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_types.h>

#include <driver/rmt.h>
#include <led_strip.h>
#include <string.h>
#include <rgbpixel.h>

#define RMT_TX_CHANNEL RMT_CHANNEL_0
#define RGBPIXEL_STRIP_GPIO CONFIG_RGBPIXEL_STRIP_OUTPUT_GPIO

#define DEFAULT_RGBPIXEL_STRIP_PIXELS 24
#define DEFAULT_RGBPIXEL_POWER_STATE false
#define DEFAULT_RGBPIXEL_HUE 180
#define DEFAULT_RGBPIXEL_SATURATION 100
#define DEFAULT_RGBPIXEL_BRIGHTNESS 15
#define DEFAULT_REFRESH_ANIM_PERIOD_RGBPIXEL 40 // Miliseconds
#define DEFAULT_ANIM_DURATION_RGBPIXEL 3 // Seconds

static led_strip_t *g_rgbpixel_strip;

static esp_timer_handle_t rgbpixel_anim_timer;
static esp_timer_handle_t rgbpixel_anim_duration_timer;
static bool g_rgbpixel_power_state = DEFAULT_RGBPIXEL_POWER_STATE;
static uint16_t g_rgbpixel_hue = DEFAULT_RGBPIXEL_HUE;
static uint16_t g_rgbpixel_saturation = DEFAULT_RGBPIXEL_SATURATION;
static uint16_t g_rgbpixel_value = DEFAULT_RGBPIXEL_BRIGHTNESS;
static uint8_t g_rgbpixel_strip_pixels = DEFAULT_RGBPIXEL_STRIP_PIXELS;

bool rgbpixel_anim_up = true;
uint8_t rgbpixel_anim_counter = 0;
uint8_t rgbpixel_anim_style = 0;
uint32_t rgbpixel_pulse_blue_min;
uint32_t rgbpixel_pulse_blue_max;
uint32_t rgbpixel_pulse_red_min;
uint32_t rgbpixel_pulse_red_max;
uint32_t rgbpixel_pulse_green_min;
uint32_t rgbpixel_pulse_green_max;
uint32_t rgbpixel_spin_blue_bg;
uint32_t rgbpixel_spin_blue_fg;

static const char *TAG = "rgbpixel";

static void rgbpixel_color_hsv2rgb(uint32_t hue, uint32_t saturation, uint32_t brightness, uint32_t *red, uint32_t *green, uint32_t *blue)
{
    hue %= 360; // hue -> [0,360]
    uint32_t rgb_max = brightness * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - saturation) / 100.0f;

    uint32_t quadrant = hue / 60;
    uint32_t diff = hue % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;

    switch (quadrant)
    {
    case 0:
        *red = rgb_max;
        *green = rgb_min + rgb_adj;
        *blue = rgb_min;
        break;
    case 1:
        *red = rgb_max - rgb_adj;
        *green = rgb_max;
        *blue = rgb_min;
        break;
    case 2:
        *red = rgb_min;
        *green = rgb_max;
        *blue = rgb_min + rgb_adj;
        break;
    case 3:
        *red = rgb_min;
        *green = rgb_max - rgb_adj;
        *blue = rgb_max;
        break;
    case 4:
        *red = rgb_min + rgb_adj;
        *green = rgb_min;
        *blue = rgb_max;
        break;
    default:
        *red = rgb_max;
        *green = rgb_min;
        *blue = rgb_max - rgb_adj;
        break;
    }
}

uint32_t rgbpixel_color_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;
}

esp_err_t rgbpixel_set_pixel(uint16_t pixel, uint32_t rgb_color)
{
    uint8_t red = (uint8_t)(rgb_color >> 16);
    uint8_t green = (uint8_t)(rgb_color >> 8);
    uint8_t blue = (uint8_t)rgb_color;
    red = red * g_rgbpixel_value * 0.01f;
    green = green * g_rgbpixel_value * 0.01f;
    blue = blue * g_rgbpixel_value * 0.01f;
    g_rgbpixel_strip->set_pixel(g_rgbpixel_strip, pixel, red, green, blue);
    return ESP_OK;
}

static esp_err_t rgbpixel_set_pixels(uint32_t hue, uint32_t saturation, uint32_t brightness)
{
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    g_rgbpixel_hue = hue;
    g_rgbpixel_saturation = saturation;
    g_rgbpixel_value = brightness;

    rgbpixel_color_hsv2rgb(g_rgbpixel_hue, g_rgbpixel_saturation, g_rgbpixel_value, &red, &green, &blue);
    for (int i = 0; i < g_rgbpixel_strip_pixels; i++)
    {
        g_rgbpixel_strip->set_pixel(g_rgbpixel_strip, i, red, green, blue);
    }
    g_rgbpixel_strip->refresh(g_rgbpixel_strip, 100);

    return ESP_OK;
}

uint32_t rgbpixel_interpolate(uint32_t color_start, uint32_t color_end, double t)
{
    uint8_t red = (color_start >> 16 & 0xFF) * (1 - t) + (color_end >> 16 & 0xFF) * t;
    uint8_t green = (color_start >> 8 & 0xFF) * (1 - t) + (color_end >> 8 & 0xFF) * t;
    uint8_t blue = (color_start & 0xFF) * (1 - t) + (color_end & 0xFF) * t;
    return ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;
}

esp_err_t rgbpixel_anim_fill(uint32_t color)
{
    for (int i = 0; i < g_rgbpixel_strip_pixels; i++)
    {
        rgbpixel_set_pixel(i, color);
    }
    return ESP_OK;
}

esp_err_t rgbpixel_anim_pulse(uint32_t color_start, uint32_t color_end, double ratio, bool up)
{
    double t = (up) ? ratio : 1 - ratio;
    uint32_t c = rgbpixel_interpolate(color_start, color_end, t);
    rgbpixel_anim_fill(c);
    return ESP_OK;
}

esp_err_t rgbpixel_anim_spinner(uint32_t color_bg, uint32_t color_fg, uint8_t position)
{
    rgbpixel_anim_fill(color_bg);
    for (int i = position; i < position + 2; i++)
    {
        rgbpixel_set_pixel(i % g_rgbpixel_strip_pixels, color_fg);
    }
    return ESP_OK;
}

static void rgbpixel_anim(void *priv)
{
    if (rgbpixel_anim_counter <= 23)
        rgbpixel_anim_counter = rgbpixel_anim_counter + 1;
    else
        rgbpixel_anim_counter = 0;
    if (rgbpixel_anim_counter == 0)
    {
        rgbpixel_anim_up = !rgbpixel_anim_up; // swap
    }

    // 0.0->1.0 per duration
    double ratio = rgbpixel_anim_counter * 0.041;
    if (rgbpixel_anim_style == 0)
    {
        rgbpixel_anim_spinner(rgbpixel_spin_blue_bg, rgbpixel_spin_blue_fg, rgbpixel_anim_counter);
    }
    else if (rgbpixel_anim_style == 1)
    {
        rgbpixel_anim_pulse(rgbpixel_pulse_blue_min, rgbpixel_pulse_blue_max, ratio, rgbpixel_anim_up);
    }
    else if (rgbpixel_anim_style == 2)
    {
        rgbpixel_anim_pulse(rgbpixel_pulse_red_min, rgbpixel_pulse_red_max, ratio, rgbpixel_anim_up);
    }
    else if (rgbpixel_anim_style == 3)
    {
        rgbpixel_anim_pulse(rgbpixel_pulse_green_min, rgbpixel_pulse_green_max, ratio, rgbpixel_anim_up);
    }
    g_rgbpixel_strip->refresh(g_rgbpixel_strip, 100);
}

static void rgbpixel_anim_duration(void *priv)
{
    esp_timer_stop(rgbpixel_anim_timer);
    ESP_LOGI(TAG, "Enhanced rgbpixel animation is ending now");
    if (!g_rgbpixel_power_state)
        g_rgbpixel_strip->clear(g_rgbpixel_strip, 100);
    else
        rgbpixel_set(g_rgbpixel_hue, g_rgbpixel_saturation, g_rgbpixel_value);
}

esp_err_t rgbpixel_set_anim(const char *type)
{
    if (strcmp(type, "ERROR") == 0)
    {
        rgbpixel_anim_style = 2;
    }
    else if (strcmp(type, "OTA") == 0)
    {
        rgbpixel_anim_style = 2;
    }
    else if (strcmp(type, "LOAD") == 0)
    {
        rgbpixel_anim_style = 0;
    }
    else if (strcmp(type, "MOVE") == 0)
    {
        rgbpixel_anim_style = 1;
    }
    esp_timer_start_periodic(rgbpixel_anim_timer, DEFAULT_REFRESH_ANIM_PERIOD_RGBPIXEL * 1000U);
    esp_timer_start_once(rgbpixel_anim_duration_timer, DEFAULT_ANIM_DURATION_RGBPIXEL * 1000000U);
    return ESP_OK;
}

esp_err_t app_driver_rgbpixel_init(void)
{
    rgbpixel_spin_blue_bg = rgbpixel_color_rgb(0, 0, 255);
    rgbpixel_spin_blue_fg = rgbpixel_color_rgb(0, 255, 255);
    rgbpixel_pulse_blue_min = rgbpixel_color_rgb(0, 0, 255);
    rgbpixel_pulse_blue_max = rgbpixel_color_rgb(0, 255, 255);
    rgbpixel_pulse_red_min = rgbpixel_color_rgb(40, 17, 0);
    rgbpixel_pulse_red_max = rgbpixel_color_rgb(255, 17, 0);
    rgbpixel_pulse_green_min = rgbpixel_color_rgb(0, 17, 0);
    rgbpixel_pulse_green_max = rgbpixel_color_rgb(0, 255, 0);

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(RGBPIXEL_STRIP_GPIO, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(g_rgbpixel_strip_pixels, (led_strip_dev_t)config.channel);
    g_rgbpixel_strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!g_rgbpixel_strip)
    {
        ESP_LOGE(TAG, "Install WS2812 driver failed");
        return ESP_FAIL;
    }
    if (g_rgbpixel_power_state)
    {
        rgbpixel_set_pixels(g_rgbpixel_hue, g_rgbpixel_saturation, g_rgbpixel_value);
    }
    else
    {
        g_rgbpixel_strip->clear(g_rgbpixel_strip, 100);
    }

    esp_timer_create_args_t rgbpixel_anim_timer_conf = {
        .callback = rgbpixel_anim,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "rgbpixel_anim_tm"};
    esp_timer_create_args_t rgbpixel_anim_duration_timer_conf = {
        .callback = rgbpixel_anim_duration,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "rgbpixel_anim_duration_tm"};
    if (esp_timer_create(&rgbpixel_anim_timer_conf, &rgbpixel_anim_timer) != ESP_OK)
    {
        return ESP_FAIL;
    }
    if (esp_timer_create(&rgbpixel_anim_duration_timer_conf, &rgbpixel_anim_duration_timer) != ESP_OK)
    {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t rgbpixel_set(uint32_t hue, uint32_t saturation, uint32_t brightness)
{
    // Whenever this function is called, rgb power will be ON
    if (!g_rgbpixel_power_state)
    {
        g_rgbpixel_power_state = true;
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(rgb_ring_light, ESP_RMAKER_PARAM_POWER),
            esp_rmaker_bool(g_rgbpixel_power_state));
    }
    return rgbpixel_set_pixels(hue, saturation, brightness);
}

esp_err_t rgbpixel_set_power_state(bool power)
{
    g_rgbpixel_power_state = power;
    if (power)
    {
        rgbpixel_set(g_rgbpixel_hue, g_rgbpixel_saturation, g_rgbpixel_value);
    }
    else
    {
        g_rgbpixel_strip->clear(g_rgbpixel_strip, 100);
    }
    return ESP_OK;
}

esp_err_t rgbpixel_set_hue(uint16_t hue)
{
    g_rgbpixel_hue = hue;
    return rgbpixel_set(g_rgbpixel_hue, g_rgbpixel_saturation, g_rgbpixel_value);
}

esp_err_t rgbpixel_set_saturation(uint16_t saturation)
{
    g_rgbpixel_saturation = saturation;
    return rgbpixel_set(g_rgbpixel_hue, g_rgbpixel_saturation, g_rgbpixel_value);
}

esp_err_t rgbpixel_set_brightness(uint16_t brightness)
{
    g_rgbpixel_value = brightness;
    return rgbpixel_set(g_rgbpixel_hue, g_rgbpixel_saturation, g_rgbpixel_value);
}

bool rgbpixel_get_power_state(void)
{
    return g_rgbpixel_power_state;
}

uint16_t rgbpixel_get_hue()
{
    return g_rgbpixel_hue;
}

uint16_t rgbpixel_get_saturation()
{
    return g_rgbpixel_saturation;
}

uint16_t rgbpixel_get_brightness()
{
    return g_rgbpixel_value;
}