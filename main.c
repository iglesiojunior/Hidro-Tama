#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include "ssd1306.h"
#include "sprites.h"
#include "sensors_utils.h"
#include "wifi_utils.h"
#include "pico/multicore.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_I2C_ADDR 0x3C
#define I2C_PORT i2c1
#define SDA_PIN 2
#define SCL_PIN 3
#define DHT_PIN 8
#define JOYSTICK_Y_PIN 26
#define JOYSTICK_THRESHOLD_UP 1000
#define JOYSTICK_THRESHOLD_DOWN 3000
#define INPUT_COOLDOWN_MS 200
#define BTN_A_PIN 5
#define BTN_B_PIN 6

ssd1306_t oled;

// Estados do Tamagotchi
int hunger = 30;
int hydration = 30;
int heat = 30;
int mood = 30;
uint8_t temperature;
uint8_t humidity;


// Atualiza os status do Tamagotchi ao longo do tempo
void update_tamagotchi() {
    while (1) {
        if(temperature > 30){
            hunger -= 4;  // A fome aumenta mais rápido
            heat -= 4;  // O calor diminui mais rápido
            hydration -= 4;  // A hidratação diminui mais rápido
        }else if(temperature < 30 && temperature >20){
            hunger -= 3;  
            heat -= 3;  
            hydration -= 3;
        }else{
            hunger -= 2;  
            heat -= 2;  
            hydration -= 2;
        }
        // Se a fome chegar a 100, ela não pode ultrapassar esse valor
        if (hunger >= 100) hunger = 100;
        
        // Se a hidratação ou o calor forem muito baixos, o humor piora mais rapidamente
        if (hydration <= 0 || heat <= 0) {
            mood -= 15;
        }
        
        // O humor nunca pode ser menor que 0
        if (mood <= 0) mood = 0;
        if (hunger <= 0) hunger = 0;
        if (hydration <= 0) hydration = 0;
        if (heat <= 0) heat = 0;

        if (!read_dht_sensor(&temperature, &humidity)) {
            temperature = 30; 
            humidity = 70;
        }

        
        send_http_json(hunger, hydration, heat, mood, temperature, humidity);//envio de mensagens diretamente para a API a cada atualização de estados

        sleep_ms(60000);  // Atualiza a cada segundo
    }
}

// Exibe o status do Tamagotchi na tela OLED
void status_tamagotchi() {
    while (1) {
        char buffer[34];
        ssd1306_clear(&oled);
        
        // Exibe fome e hidratação
        snprintf(buffer, sizeof(buffer), "Fome: %d \n Hid: %d", hunger, hydration);
        ssd1306_draw_string(&oled, 0, 0, 1, buffer);
        
        // Exibe calor e humor
        snprintf(buffer, sizeof(buffer), "Calor: %d \n Hum: %d", heat, mood);
        ssd1306_draw_string(&oled, 0, 10, 1, buffer);
        
        //Exibe Temperatura e Umidade
        snprintf(buffer, sizeof(buffer), "Temp: %d \n Umid: %d", temperature, humidity);
        ssd1306_draw_string(&oled, 0, 20, 1, buffer);

        ssd1306_show(&oled);
        
        // Sai do status se o botão B for pressionado
        if (gpio_get(BTN_B_PIN) == 0) break;
        
        sleep_ms(200);
    }
}

// Alimenta o Tamagotchi
void feed_tamagotchi() {
    alimentando(&oled);  // Animação de alimentação
    hunger -= 20;  // Reduz a fome
    if (hunger < 0) hunger = 0;
    
    mood += 5;  // Aumenta o humor
    if (mood > 100) mood = 100;
}

// Hidrata o Tamagotchi
void hydrate_tamagotchi() {
    hydration += 20;  // Aumenta a hidratação
    tomando_agua(&oled);  // Animação de beber água
    if (hydration > 100) hydration = 100;
    
    mood += 5;  // Aumenta o humor
}

// Refresca o Tamagotchi quando o microfone detecta um sopro
void interact_with_tamagotchi() {
    uint16_t mic_value;
    adc_select_input(2);

    // Espera até detectar um sopro forte no microfone
    ssd1306_clear(&oled);
    rosto_padrao(&oled);
    ssd1306_draw_string(&oled, 40, 10, 1, "Sopre no mic!");
    ssd1306_show(&oled);
    while (1) {
        mic_value = read_microphone();
        if (mic_value > 3000) break;  // Sai do loop quando detectar o sopro
        sleep_ms(50);  // Pequeno atraso para evitar leituras excessivas
    }

    // Executa a animação e ajusta os atributos do Tamagotchi
    ssd1306_clear(&oled);
    refrescando(&oled);
    hydration += 20;
    if (hydration > 100) hydration = 100;
    
    mood += 5;
    heat += 10;
    if (heat > 100) heat = 100;
}


// Exibe o menu principal na tela OLED
void display_menu(int selected_option) {
    ssd1306_clear(&oled);
    
    // Opções do menu principal
    ssd1306_draw_string(&oled, 40, 10, 1, selected_option == 0 ? "> Status" : "  Status");
    ssd1306_draw_string(&oled, 40, 20, 1, selected_option == 1 ? "> Interagir" : "  Interagir");

    if(mood <= 33){
        triste(&oled);
    }else if(mood > 33 && mood <= 66){
        rosto_padrao(&oled);
    }else if(mood > 66){
        feliz(&oled);
    }

    ssd1306_show(&oled);
}

// Exibe o menu de interações
void display_interaction_menu(int selected_option) {
    ssd1306_clear(&oled);
    ssd1306_draw_string(&oled, 0, 0, 1, "Interagir");
    ssd1306_draw_string(&oled, 0, 10, 1, selected_option == 0 ? "> Alimentar" : "  Alimentar");
    ssd1306_draw_string(&oled, 0, 20, 1, selected_option == 1 ? "> Hidratar" : "  Hidratar");
    ssd1306_draw_string(&oled, 65, 10, 1, selected_option == 2 ? "> Refrescar" : "  Refrescar");
    ssd1306_show(&oled);
}

// Controla a navegação no menu de interações
void interaction_menu() {
    int selected_option = 0;
    while (1) {
        display_interaction_menu(selected_option);
        adc_select_input(1);
        uint16_t joystick_y = adc_read();
        
        // Movimenta o menu
        if (joystick_y > JOYSTICK_THRESHOLD_DOWN) selected_option = (selected_option + 1) % 3;
        else if (joystick_y < JOYSTICK_THRESHOLD_UP) selected_option = (selected_option - 1 + 3) % 3;
        
        // Confirma a opção
        if (gpio_get(BTN_A_PIN) == 0) {
            if (selected_option == 0) feed_tamagotchi();
            else if (selected_option == 1) hydrate_tamagotchi();
            else interact_with_tamagotchi();
        }
        
        // Sai do menu ao pressionar B
        if (gpio_get(BTN_B_PIN) == 0) break;
        
        sleep_ms(200);
    }
}

// Menu principal
void menu() {
    int selected_option = 0;
    while (1) {
        display_menu(selected_option);
        adc_select_input(1);
        uint16_t joystick_y = adc_read();
        
        // Alterna entre as opções
        if (joystick_y > JOYSTICK_THRESHOLD_DOWN) selected_option = 0;
        else if (joystick_y < JOYSTICK_THRESHOLD_UP) selected_option = 1;
        
        // Confirma a opção
        if (gpio_get(BTN_A_PIN) == 0) {
            if (selected_option == 0) status_tamagotchi();
            else sleep_ms(1500); interaction_menu();
        }
        
        sleep_ms(200);
    }
}

// Inicialização do programa
int main() {
    stdio_init_all();
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);
    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN);
    adc_init();
    adc_gpio_init(JOYSTICK_Y_PIN);
    init_dht_sensor();
    init_microphone();

    if (!init_wifi()) {
        return -1;
    }

    if (!ssd1306_init(&oled, SCREEN_WIDTH, SCREEN_HEIGHT, OLED_I2C_ADDR, I2C_PORT)) return 1;

    if (send_http_json(hunger, hydration, heat, mood, 35.0, 60.0)) {
        printf("JSON enviado com sucesso!\n");
    } else {
        printf("Erro ao enviar JSON\n");
    }

    multicore_launch_core1(update_tamagotchi);

    while (1) {
        menu();
        sleep_ms(1000);
    }

    cyw43_arch_deinit();
    return 0;
}
