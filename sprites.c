#include "ssd1306.h"
#include "sprites.h"

void mastigando(ssd1306_t *display){
    ssd1306_draw_square(display, 0, 0, 32, 32);

    //olho esquerdo
    ssd1306_clear_pixel(display, 3, 4);
    ssd1306_clear_pixel(display, 3, 5);
    ssd1306_clear_pixel(display, 3, 6);
    ssd1306_clear_pixel(display, 4, 7);
    ssd1306_clear_pixel(display, 4, 8);
    ssd1306_clear_pixel(display, 5, 9);
    ssd1306_clear_pixel(display, 6, 10);
    ssd1306_clear_pixel(display, 7, 10);
    ssd1306_clear_pixel(display, 8, 10);
    ssd1306_clear_pixel(display, 9, 10);
    ssd1306_clear_pixel(display, 10, 9);
    ssd1306_clear_pixel(display, 11, 8);
    ssd1306_clear_pixel(display, 11, 7);
    ssd1306_clear_pixel(display, 12, 6);
    ssd1306_clear_pixel(display, 12, 5);
    ssd1306_clear_pixel(display, 12, 4);
    //olho direito
    ssd1306_clear_pixel(display, 20, 4);
    ssd1306_clear_pixel(display, 20, 5);
    ssd1306_clear_pixel(display, 20, 6);
    ssd1306_clear_pixel(display, 21, 7);
    ssd1306_clear_pixel(display, 21, 8);
    ssd1306_clear_pixel(display, 22, 9);
    ssd1306_clear_pixel(display, 23, 10);
    ssd1306_clear_pixel(display, 24, 10);
    ssd1306_clear_pixel(display, 25, 10);
    ssd1306_clear_pixel(display, 26, 10);
    ssd1306_clear_pixel(display, 27, 9);
    ssd1306_clear_pixel(display, 28, 8);
    ssd1306_clear_pixel(display, 28, 7);
    ssd1306_clear_pixel(display, 29, 6);
    ssd1306_clear_pixel(display, 29, 5);
    ssd1306_clear_pixel(display, 29, 4);

    //boca
    ssd1306_clear_square(display, 12, 25, 10, 3);
    ssd1306_show(display);
    sleep_ms(500);

    //segundo frame
    ssd1306_draw_square(display, 12, 25, 10, 3);
    ssd1306_clear_square(display, 12, 26, 10, 1);

    ssd1306_show(display);
    sleep_ms(500);
    //terceiro frame
    ssd1306_draw_square(display, 12, 25, 10, 3);
    ssd1306_clear_square(display, 12, 25, 10, 3);
    
    ssd1306_show(display);
    sleep_ms(500);

    //quarto frame
    ssd1306_draw_square(display, 12, 25, 10, 3);
    ssd1306_clear_square(display, 12, 26, 10, 1);
    ssd1306_show(display);

}

void esperando(ssd1306_t *display){
    ssd1306_draw_square(display, 0, 0, 32, 32);

    //olho esquerdo
    ssd1306_clear_pixel(display, 3, 4);
    ssd1306_clear_pixel(display, 3, 5);
    ssd1306_clear_pixel(display, 3, 6);
    ssd1306_clear_pixel(display, 4, 7);
    ssd1306_clear_pixel(display, 4, 8);
    ssd1306_clear_pixel(display, 5, 9);
    ssd1306_clear_pixel(display, 6, 10);
    ssd1306_clear_pixel(display, 7, 10);
    ssd1306_clear_pixel(display, 8, 10);
    ssd1306_clear_pixel(display, 9, 10);
    ssd1306_clear_pixel(display, 10, 9);
    ssd1306_clear_pixel(display, 11, 8);
    ssd1306_clear_pixel(display, 11, 7);
    ssd1306_clear_pixel(display, 12, 6);
    ssd1306_clear_pixel(display, 12, 5);
    ssd1306_clear_pixel(display, 12, 4);
    //olho direito
    ssd1306_clear_pixel(display, 20, 4);
    ssd1306_clear_pixel(display, 20, 5);
    ssd1306_clear_pixel(display, 20, 6);
    ssd1306_clear_pixel(display, 21, 7);
    ssd1306_clear_pixel(display, 21, 8);
    ssd1306_clear_pixel(display, 22, 9);
    ssd1306_clear_pixel(display, 23, 10);
    ssd1306_clear_pixel(display, 24, 10);
    ssd1306_clear_pixel(display, 25, 10);
    ssd1306_clear_pixel(display, 26, 10);
    ssd1306_clear_pixel(display, 27, 9);
    ssd1306_clear_pixel(display, 28, 8);
    ssd1306_clear_pixel(display, 28, 7);
    ssd1306_clear_pixel(display, 29, 6);
    ssd1306_clear_pixel(display, 29, 5);
    ssd1306_clear_pixel(display, 29, 4);


    //boca
    ssd1306_clear_square(display, 15, 22, 4, 1);
    ssd1306_clear_square(display, 14, 23, 6, 1);
    ssd1306_clear_square(display, 13, 24, 8, 1);
    ssd1306_clear_square(display, 12, 25, 10, 1);
    ssd1306_clear_square(display, 12, 26, 10, 1);
    ssd1306_clear_square(display, 12, 27, 10, 1);
    ssd1306_clear_square(display, 13, 28, 8, 1);
    ssd1306_clear_square(display, 14, 29, 6, 1);
    ssd1306_clear_square(display, 15, 30, 4, 1);
}

void draw_tamagotchi_face_blink(ssd1306_t *display) {
    ssd1306_clear_square(display, 0, 0, 32, 32); // Limpa a tela

    // ssd1306_draw_string(display, 40, 5, 1, "Seja Bem vindo...");
    // ssd1306_draw_string(display, 40, 20, 1, texto);
    // Desenho do rosto (quadrado grande)
    ssd1306_draw_square(display, 0, 0, 32, 32); // Quadrado que representa o rosto (preto)

    // Desenhando os olhos (quadrados menores com cor diferente)
    // Primeiro, limpar os olhos (se houver algum quadrado anterior)
    ssd1306_clear_square(display, 4, 4, 11, 12); // Limpa o olho esquerdo
    ssd1306_clear_square(display, 19, 4, 11, 12); // Limpa o olho direito

    // // Agora, desenhar os olhos com cor diferente do rosto
    ssd1306_draw_square(display, 7, 7, 5, 6);  // Olho esquerdo (branco)
    ssd1306_draw_square(display, 22, 7, 5, 6);  // Olho direito (branco)

    // Desenhando a boca (linha horizontal)
    ssd1306_clear_square(display, 7, 24, 18, 1);  // Linha para a boca (preta)

    ssd1306_show(display); // Atualiza a tela

    sleep_ms(1000);

    ssd1306_draw_square(display, 4, 4, 11, 12); 
    ssd1306_draw_square(display, 19, 4, 11, 12);

    ssd1306_clear_square(display, 5, 9, 8, 1);
    ssd1306_clear_square(display, 21, 9, 8, 1);  

    ssd1306_show(display);

}


void refrescando(ssd1306_t *display){
    ssd1306_clear(display);

    ssd1306_draw_string(display, 40, 5, 1, "Mas que brisa");
    ssd1306_draw_string(display, 40, 20, 1, "Maravilhosa!");

    esperando(display);

    ssd1306_show(display);

    sleep_ms(2000);

    //limpa a boca
    ssd1306_draw_square(display, 0, 17, 32, 16);

    ssd1306_clear_square(display, 6, 27, 16, 1);
    ssd1306_clear_square(display, 23, 26, 2, 1);
    ssd1306_draw_pixel(display, 24, 25);
    ssd1306_draw_pixel(display, 25, 24);

    ssd1306_show(display);


}


void rosto_padrao(ssd1306_t *display){
    ssd1306_draw_square(display, 0, 0, 32, 32); // Quadrado que representa o rosto (preto)

    // Desenhando os olhos (quadrados menores com cor diferente)
    // Primeiro, limpar os olhos (se houver algum quadrado anterior)
    ssd1306_clear_square(display, 4, 4, 11, 12); // Limpa o olho esquerdo
    ssd1306_clear_square(display, 19, 4, 11, 12); // Limpa o olho direito

    // // Agora, desenhar os olhos com cor diferente do rosto
    ssd1306_draw_square(display, 7, 7, 5, 6);  // Olho esquerdo (branco)
    ssd1306_draw_square(display, 22, 7, 5, 6);  // Olho direito (branco)

    // Desenhando a boca (linha horizontal)
    ssd1306_clear_square(display, 7, 24, 18, 1);  // Linha para a boca (preta)

    // ssd1306_show(display);
}

void alimentando(ssd1306_t *display){
    ssd1306_clear(display);
    ssd1306_draw_string(display, 40, 0, 1, "Obrigado pela");
    ssd1306_draw_string(display, 40, 15, 1, "comida!");

    //primeiro frame
    rosto_padrao(display);

    ssd1306_show(display);

    sleep_ms(500);
    //segundo frame
    esperando(display);

    ssd1306_draw_square(display, 35, 23, 14, 5);

    ssd1306_show(display);
    sleep_ms(500);
    //terceiro frame
    ssd1306_clear_square(display, 35, 23, 14, 5);
    esperando(display);
    ssd1306_draw_square(display, 26, 23, 14, 5);
    ssd1306_show(display);
    sleep_ms(500);
    //quarto frame
    ssd1306_clear_square(display, 26, 23, 14, 5);
    esperando(display);
    ssd1306_draw_square(display, 19, 23, 14, 5);
    ssd1306_show(display);
    sleep_ms(500);
    //quinto frame
    ssd1306_clear_square(display, 19, 23, 14, 5);
    esperando(display);
    ssd1306_draw_square(display, 16, 23, 6, 5);
    ssd1306_show(display);
    sleep_ms(500);
    //sexto frame
    mastigando(display);

    ssd1306_show(display);

}

void feliz(ssd1306_t *display){
    ssd1306_draw_square(display, 0, 0, 32, 32); // Quadrado que representa o rosto (preto)

    // Desenhando os olhos (quadrados menores com cor diferente)
    // Primeiro, limpar os olhos (se houver algum quadrado anterior)
    ssd1306_clear_square(display, 4, 4, 11, 12); // Limpa o olho esquerdo
    ssd1306_clear_square(display, 19, 4, 11, 12); // Limpa o olho direito

    // // Agora, desenhar os olhos com cor diferente do rosto
    ssd1306_draw_square(display, 7, 7, 5, 6);  // Olho esquerdo (branco)
    ssd1306_draw_square(display, 22, 7, 5, 6);  // Olho direito (branco)

    //boca
    ssd1306_draw_square(display, 6, 27, 20, 1);
    ssd1306_draw_square(display, 5, 26, 2, 1);
    ssd1306_draw_pixel(display, 4, 25);
    ssd1306_draw_pixel(display, 3, 24);
    ssd1306_draw_square(display, 26, 27, 2, 1);
    ssd1306_draw_pixel(display, 27, 26);
    ssd1306_draw_pixel(display, 28, 25);

}

void triste(ssd1306_t *display){
    ssd1306_draw_square(display, 0, 0, 32, 32); // Quadrado que representa o rosto (preto)

    // Desenhando os olhos (quadrados menores com cor diferente)
    // Primeiro, limpar os olhos (se houver algum quadrado anterior)
    ssd1306_clear_square(display, 4, 4, 11, 12); // Limpa o olho esquerdo
    ssd1306_clear_square(display, 19, 4, 11, 12); // Limpa o olho direito

    // // Agora, desenhar os olhos com cor diferente do rosto
    ssd1306_draw_square(display, 7, 7, 5, 6);  // Olho esquerdo (branco)
    ssd1306_draw_square(display, 22, 7, 5, 6);  // Olho direito (branco)

    //boca
    ssd1306_draw_square(display, 6, 27, 20, 1);
    ssd1306_draw_square(display, 5, 28, 2, 1);
    ssd1306_draw_pixel(display, 4, 29);
    ssd1306_draw_pixel(display, 3, 30);
    ssd1306_draw_square(display, 26, 28, 2, 1);
    ssd1306_draw_pixel(display, 27, 29);
    ssd1306_draw_pixel(display, 28, 30);
}

void tomando_agua(ssd1306_t *display){
    ssd1306_clear(display);
    
    ssd1306_draw_square(display, 0, 0, 32, 32);

    //olho esquerdo
    ssd1306_clear_pixel(display, 3, 4);
    ssd1306_clear_pixel(display, 3, 5);
    ssd1306_clear_pixel(display, 3, 6);
    ssd1306_clear_pixel(display, 4, 7);
    ssd1306_clear_pixel(display, 4, 8);
    ssd1306_clear_pixel(display, 5, 9);
    ssd1306_clear_pixel(display, 6, 10);
    ssd1306_clear_pixel(display, 7, 10);
    ssd1306_clear_pixel(display, 8, 10);
    ssd1306_clear_pixel(display, 9, 10);
    ssd1306_clear_pixel(display, 10, 9);
    ssd1306_clear_pixel(display, 11, 8);
    ssd1306_clear_pixel(display, 11, 7);
    ssd1306_clear_pixel(display, 12, 6);
    ssd1306_clear_pixel(display, 12, 5);
    ssd1306_clear_pixel(display, 12, 4);
    //olho direito
    ssd1306_clear_pixel(display, 20, 4);
    ssd1306_clear_pixel(display, 20, 5);
    ssd1306_clear_pixel(display, 20, 6);
    ssd1306_clear_pixel(display, 21, 7);
    ssd1306_clear_pixel(display, 21, 8);
    ssd1306_clear_pixel(display, 22, 9);
    ssd1306_clear_pixel(display, 23, 10);
    ssd1306_clear_pixel(display, 24, 10);
    ssd1306_clear_pixel(display, 25, 10);
    ssd1306_clear_pixel(display, 26, 10);
    ssd1306_clear_pixel(display, 27, 9);
    ssd1306_clear_pixel(display, 28, 8);
    ssd1306_clear_pixel(display, 28, 7);
    ssd1306_clear_pixel(display, 29, 6);
    ssd1306_clear_pixel(display, 29, 5);
    ssd1306_clear_pixel(display, 29, 4);

    //boca
    ssd1306_clear_square(display, 12, 25, 10, 3);

    //copo e canudo
    ssd1306_draw_line(display, 47, 21, 47, 16);
    ssd1306_draw_line(display, 45, 15, 46, 15);
    ssd1306_draw_line(display, 42, 16, 44, 16);
    ssd1306_draw_line(display, 39, 17, 41, 17);
    ssd1306_draw_line(display, 37, 18, 38, 18);
    ssd1306_draw_line(display, 35, 19, 36, 19);
    ssd1306_draw_line(display, 33, 20, 34, 20);

    ssd1306_clear_square(display, 31, 21, 2, 1);
    ssd1306_clear_square(display, 29, 22, 2, 1);
    ssd1306_clear_square(display, 27, 23, 2, 1);
    ssd1306_clear_square(display, 24, 24, 3, 1);
    ssd1306_clear_square(display, 21, 25, 3, 1);

    ssd1306_draw_empty_square(display, 41, 22, 12, 11);

    for(int i = 23; i<32; i++){
        ssd1306_draw_line(display, 42, i, 51, i);
        ssd1306_show(display);
        sleep_ms(500);
    }

    ssd1306_show(display);
}