/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "unity.h"
#include "esp_err.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2

esp_err_t err;

void led_init_config()
{
    gpio_pad_select_gpio(LED_PIN);
    ESP_LOGI("led_init_config()", "LED pin set to GPIO NUM 2");
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    ESP_LOGI("led_init_config()", "LED pin set to GPIO OUTPUT MODE");
}

void Task_Delay(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(1499/portTICK_PERIOD_MS);
        ESP_LOGI("Task_Delay","LED on after 3 seconds");
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(1501));
    }
}

void Task_Delay_Until(void *pvParameters)
{
    const TickType_t LastWakeTime = xTaskGetTickCount();
    while(1)
    {
        vTaskDelay(1501/portTICK_PERIOD_MS);
        ESP_LOGI("Task_Delay_Until","LED off after 3 seconds");
        gpio_set_level(LED_PIN, 0);
        vTaskDelayUntil(&LastWakeTime, pdMS_TO_TICKS(1499));
    }
}

TEST_CASE("blink_led_3_sec", "Toggling led 3 seconds using vTaskDelay() & vTaskDelayUntil() with same priority & same core 0")
{
    ESP_LOGI("TEST_CASE", "Running Test case");
    xTaskCreatePinnedToCore(Task_Delay, "task using vTaskDelay() function", 4098, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Task_Delay_Until, "task using vTaskDelayUntil() function", 4098, NULL, 1, NULL, 0);
}

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);    
    led_init_config();
    unity_run_menu();
}