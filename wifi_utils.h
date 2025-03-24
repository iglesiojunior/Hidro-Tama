#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H
#include <stdlib.h>
#include <string.h>
    
    bool init_wifi();
    bool send_http_json(float hunger, float hydratation, float heat, float mood, float temperatura, float umidade);

#endif