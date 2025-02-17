#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include "sprites.h"
#include "sensors_utils.h"

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
int hunger = 100;
int hydration = 100;
int heat = 100;
int mood = 100;

// Atualiza os status do Tamagotchi ao longo do tempo
void update_tamagotchi() {
    hunger += 1;  // A fome aumenta com o tempo
    heat -= 1;  // O calor diminui
    hydration -= 1;  // A hidratação diminui
    
    // Se a fome chegar a 100, ela não pode ultrapassar esse valor
    if (hunger >= 100) hunger = 100;
    
    // Se a hidratação ou o calor forem muito baixos, o humor piora
    if (hydration <= 0 || heat <= 0) {
        mood -= 10;
    }
    
    // O humor nunca pode ser menor que 0
    if (mood <= 0) mood = 0;
}

// Exibe o status do Tamagotchi na tela OLED
void status_tamagotchi() {
    while (1) {
        char buffer[20];
        ssd1306_clear(&oled);
        
        // Exibe fome e hidratação
        snprintf(buffer, sizeof(buffer), "Fome: %d Hid: %d", hunger, hydration);
        ssd1306_draw_string(&oled, 0, 0, 1, buffer);
        
        // Exibe calor e humor
        snprintf(buffer, sizeof(buffer), "Calor: %d Hum: %d", heat, mood);
        ssd1306_draw_string(&oled, 0, 10, 1, buffer);
        
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

    // Espera até detectar um sopro forte no microfone
    while (1) {
        mic_value = read_microphone();
        if (mic_value > 2000) break;  // Sai do loop quando detectar o sopro
        sleep_ms(50);  // Pequeno atraso para evitar leituras excessivas
    }

    // Executa a animação e ajusta os atributos do Tamagotchi
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

    // Desenha o rosto do Tamagotchi
    ssd1306_clear_square(&oled, 0, 0, 32, 32);
    ssd1306_draw_square(&oled, 0, 0, 32, 32);
    
    // Olhos
    ssd1306_clear_square(&oled, 4, 4, 11, 12);
    ssd1306_clear_square(&oled, 19, 4, 11, 12);
    ssd1306_draw_square(&oled, 7, 7, 5, 6);
    ssd1306_draw_square(&oled, 22, 7, 5, 6);

    // Boca
    ssd1306_clear_square(&oled, 7, 24, 18, 1);

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
            else interaction_menu();
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
    init_microphone();
    init_dht_sensor();

    if (!ssd1306_init(&oled, SCREEN_WIDTH, SCREEN_HEIGHT, OLED_I2C_ADDR, I2C_PORT)) return 1;

    while (1) {
        menu();
        update_tamagotchi();
        sleep_ms(1000);
    }

    return 0;
}
