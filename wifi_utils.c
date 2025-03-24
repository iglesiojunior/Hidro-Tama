#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "wifi_utils.h"

#define WIFI_SSID "moto g73 5G_3103"
#define WIFI_PASSWORD "meunome9988"
#define API_HOST "192.168.65.64"
#define API_PORT 5000
#define API_PATH "/enviar-dados"

#define MAX_REQUEST_LEN 512
#define MAX_RESPONSE_LEN 512

typedef struct {
    struct tcp_pcb *pcb;
    bool complete;
    bool connected;
    char request[MAX_REQUEST_LEN];
    char response[MAX_RESPONSE_LEN];
    size_t response_len;
    ip_addr_t remote_addr;
} HTTP_CLIENT_T;

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_client_close(HTTP_CLIENT_T *state);

static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    HTTP_CLIENT_T *state = (HTTP_CLIENT_T*)arg;

    if (!p) {
        state->complete = true;
        return ERR_OK;
    }

    if (p->tot_len > 0) {
        size_t to_copy = (p->tot_len < MAX_RESPONSE_LEN - state->response_len - 1) ? p->tot_len : MAX_RESPONSE_LEN - state->response_len - 1;
        pbuf_copy_partial(p, state->response + state->response_len, to_copy, 0);
        state->response_len += to_copy;
        state->response[state->response_len] = '\0';
    }

    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);
    return ERR_OK;
}

static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    return ERR_OK;
}

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("Erro ao conectar: %d\n", err);
        return err;
    }

    HTTP_CLIENT_T *state = (HTTP_CLIENT_T*)arg;
    state->connected = true;

    err = tcp_write(tpcb, state->request, strlen(state->request), TCP_WRITE_FLAG_COPY);
    if (err != ERR_OK) {
        printf("Erro ao enviar requisição: %d\n", err);
        return err;
    }

    err = tcp_output(tpcb);
    return err;
}

static void tcp_client_close(HTTP_CLIENT_T *state) {
    if (state->pcb) {
        tcp_close(state->pcb);
        state->pcb = NULL;
    }
}

bool send_http_json(float hunger, float hydratation, float heat, float mood, float temperatura, float umidade) {
    HTTP_CLIENT_T *state = (HTTP_CLIENT_T*)calloc(1, sizeof(HTTP_CLIENT_T));
    if (!state) {
        printf("Erro ao alocar memória\n");
        return false;
    }

    char json_data[256];
    snprintf(json_data, sizeof(json_data),
             "{\"fome\": %.1f, \"hidratação\": %.1f, \"calor\": %.1f, \"felicidade\": %.1f, \"temperatura\": %.1f, \"umidade\": %.1f, \"sensor_id\": \"pico_w_001\", \"timestamp\": \"2025-03-19T14:30:00Z\"}",
             hunger, hydratation, heat, mood, temperatura, umidade);

    snprintf(state->request, MAX_REQUEST_LEN,
             "POST %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             API_PATH, API_HOST, (int)strlen(json_data), json_data);

    printf("Enviando JSON para API...\n%s\n", state->request);

    ip_addr_t remote_addr;
    if (!ipaddr_aton(API_HOST, &remote_addr)) {
        printf("Erro ao resolver IP\n");
        free(state);
        return false;
    }
    state->remote_addr = remote_addr;

    state->pcb = tcp_new();
    if (!state->pcb) {
        printf("Erro ao criar PCB TCP\n");
        free(state);
        return false;
    }

    tcp_arg(state->pcb, state);
    tcp_recv(state->pcb, tcp_client_recv);
    tcp_sent(state->pcb, tcp_client_sent);

    err_t err = tcp_connect(state->pcb, &state->remote_addr, API_PORT, tcp_client_connected);
    if (err != ERR_OK) {
        printf("Erro ao conectar ao servidor: %d\n", err);
        tcp_client_close(state);
        free(state);
        return false;
    }

    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (!state->complete && (to_ms_since_boot(get_absolute_time()) - start_time < 10000)) {
        sleep_ms(100);
    }

    bool success = state->complete;
    if (success) {
        printf("Resposta da API: %s\n", state->response);
    } else {
        printf("Falha ao enviar JSON\n");
    }

    tcp_client_close(state);
    free(state);
    return success;
}

bool init_wifi() {
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi: %s...\n", WIFI_SSID);

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Erro ao conectar ao Wi-Fi\n");
        return false;
    }

    printf("Wi-Fi conectado!\n");
    return true;
}