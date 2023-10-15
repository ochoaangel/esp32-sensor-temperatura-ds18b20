#include <stdio.h>
#include "ds18x20.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define GPIO_PIN 4
#define MAX_SENSORS 10
#define DELAY_MS 10000

void read_temperatures() {
    onewire_addr_t sensor_addr[MAX_SENSORS];
    float temperature[MAX_SENSORS];
    size_t num_sensors;

    esp_err_t err = ds18x20_scan_devices(GPIO_PIN, sensor_addr, MAX_SENSORS, &num_sensors);
    if (err != ESP_OK) {
        printf("Error al escanear los dispositivos DS18x20\n");
        return;
    }

    if (num_sensors > MAX_SENSORS) {
        printf("El número de sensores conectados es mayor que el máximo permitido.\n");
        return;
    }

    while (1) {
        for (size_t i = 0; i < num_sensors; i++) {
            err = ds18x20_measure_and_read(GPIO_PIN, sensor_addr[i], &temperature[i]);
            if (err != ESP_OK) {
                printf("Error al leer la temperatura del sensor %zu\n", i+1);
            } else {
                printf("Sensor %zu: Temperatura: %.2f°C\n", i+1, temperature[i]);
            }
        }
                printf("----------------------------------------------\n");
        vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
    }
}

void app_main() {
    read_temperatures();
}