#include "sensors_utils.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include <stdio.h>

#define MIC_ADC_PIN 28
#define DHT_PIN 8

void init_microphone() {
    adc_init();
    adc_gpio_init(MIC_ADC_PIN);  // Configura o pino ADC
    adc_select_input(2);  // Seleciona o canal ADC correspondente ao pino
}

uint16_t read_microphone() {
    // Aqui, o ADC é configurado para a entrada do microfone (dependendo de como o microfone está conectado)
    uint16_t mic_value = adc_read();  // Exemplo de leitura do ADC
    return mic_value;
}

void mostrar_valor_microfone(ssd1306_t *oled, uint16_t mic_value) {
    char texto[16];
    snprintf(texto, sizeof(texto), "Mic: %d", mic_value);  // Formata o valor do mic

    ssd1306_clear(oled);  // Limpa a tela
    ssd1306_draw_string(oled, 0, 0, 1, texto);  // Exibe o valor
    ssd1306_show(oled);  // Atualiza a tela
}

void dht11_read(int *temperature, int *humidity) {
    uint32_t data = 0;  // Variável para armazenar os dados brutos do sensor
    uint8_t bits[5] = {0}; // Array para armazenar os bits de dados

    // Configura o pino do DHT como saída
    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);    // Coloca o pino em LOW
    sleep_ms(18);             // Aguarda 18ms
    gpio_put(DHT_PIN, 1);    // Coloca o pino em HIGH
    sleep_us(40);             // Aguarda 40us
    gpio_set_dir(DHT_PIN, GPIO_IN); // Configura o pino como entrada

    // Leitura dos dados
    while (gpio_get(DHT_PIN) == 1); // Aguarda o pino ir para LOW
    while (gpio_get(DHT_PIN) == 0); // Aguarda o pino ir para HIGH
    while (gpio_get(DHT_PIN) == 1); // Aguarda o pino ir para LOW

    // Leitura dos 40 bits de dados
    for (int i = 0; i < 40; i++) {
        while (gpio_get(DHT_PIN) == 0); // Aguarda o pino ir para HIGH
        sleep_us(28);                   // Aguarda 28us
        if (gpio_get(DHT_PIN) == 1)     // Se o pino estiver em HIGH, o bit é 1
            bits[i / 8] |= (1 << (7 - (i % 8))); // Armazena o bit no array
        while (gpio_get(DHT_PIN) == 1); // Aguarda o pino ir para LOW
    }

    // Verificação da integridade dos dados
    if ((bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
        *humidity = bits[0];       // Umidade
        *temperature = bits[2];    // Temperatura
    } else {
        *temperature = -1;          // Erro na leitura
        *humidity = -1;             // Erro na leitura
    }
}

void init_dht_sensor() {
    gpio_init(DHT_PIN);
    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 1);
}

// Função para leitura do sensor DHT11/DHT22
bool read_dht_sensor(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    int i, j;

    // Inicia a comunicação com o sensor
    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);
    sleep_ms(18); // Pulso de início
    gpio_put(DHT_PIN, 1);
    sleep_us(40);
    gpio_set_dir(DHT_PIN, GPIO_IN);

    // Aguarda resposta do sensor
    uint32_t timeout = 10000;
    while (gpio_get(DHT_PIN) == 0 && timeout--);
    timeout = 10000;
    while (gpio_get(DHT_PIN) == 1 && timeout--);

    // Leitura dos 40 bits de dados
    for (i = 0; i < 5; i++) {
        for (j = 7; j >= 0; j--) {
            timeout = 10000;
            while (gpio_get(DHT_PIN) == 0 && timeout--);
            sleep_us(30);
            if (gpio_get(DHT_PIN)) {
                data[i] |= (1 << j);
            }
            timeout = 10000;
            while (gpio_get(DHT_PIN) == 1 && timeout--);
        }
    }

    // Verifica checksum (paridade)
    if ((data[0] + data[1] + data[2] + data[3]) != data[4]) {
        return false; // Erro na leitura
    }

    *humidity = data[0];
    *temperature = data[2];

    return true; // Leitura bem-sucedida
}

void display_info(ssd1306_t *oled, uint8_t temperature, uint8_t humidity) {
    char buffer[20];

    ssd1306_clear_square(oled, 40, 0, 88, 32);  // Limpa a tela
    snprintf(buffer, sizeof(buffer), "Temp: %d C", temperature);
    ssd1306_draw_string(oled, 40, 0, 1, buffer);

    snprintf(buffer, sizeof(buffer), "Umid: %d%%", humidity);
    ssd1306_draw_string(oled, 40, 20, 1, buffer);

    // ssd1306_show(oled);  // Atualiza o display
}