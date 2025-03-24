#ifndef SENSORS_UTILS_H
#define SENSORS_UTILS_H
#include <stdint.h>  
#include "ssd1306.h"

    void init_microphone();
    uint16_t read_microphone();
    void mostrar_valor_microfone(ssd1306_t *oled, uint16_t mic_value);
    bool dht11_read(int *temperature, int *humidity);
    void init_dht_sensor();
    bool read_dht_sensor(uint8_t *temperature, uint8_t *humidity);
    void display_info(ssd1306_t *oled, uint8_t temperature, uint8_t humidity);

#endif