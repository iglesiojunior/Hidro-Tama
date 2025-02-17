# Projeto: Hidro-Tama Monitoramento com Display OLED SSD1306 no Raspberry Pi Pico W  

Este repositório contém o código para um sistema baseado no **Raspberry Pi Pico W**, utilizando um **display OLED SSD1306 (128x32 pixels)** para exibir informações de sensores. O projeto faz uso da biblioteca **pico-ssd1306**, manipulando diretamente os pixels da tela sem o uso de imagens bitmap.  

## 📌 **Funcionalidades**  
✔️ Leitura de sensores ambientais e exibição dos dados no display OLED.  
✔️ Manipulação direta de pixels para desenhar elementos gráficos na tela.  
✔️ Organização modular do código para fácil manutenção e expansão.  
✔️ Suporte a diferentes sensores com modularidade.  

## 🔧 **Metodologia de Desenvolvimento**  

### **1. Pesquisa e Definição de Requisitos**  
A fase inicial do projeto envolveu uma pesquisa detalhada sobre monitores OLED e técnicas de manipulação direta de pixels, além da análise de sensores compatíveis com o **Raspberry Pi Pico W**. Os principais requisitos definidos foram:  

- Uso de um **display OLED SSD1306 (128x32 pixels)** com comunicação I2C.  
- Implementação de um sistema modular para facilitar a expansão do código.  
- Processamento eficiente dos sensores, garantindo desempenho e precisão.  
- Implementação de gráficos simples sem o uso de imagens bitmap.  

### **2. Seleção de Hardware**  
Os componentes foram escolhidos com base nos requisitos do projeto, priorizando disponibilidade, desempenho e compatibilidade com o **Raspberry Pi Pico W**.  

Principais componentes:  
- **Display OLED SSD1306 (128x32 pixels)** – Baixo consumo de energia e alta legibilidade.  
- **Sensor DHT11** – Sensor de temperatura e umidade usado para a coleta de dados do ambiente que afetam diretamente o tamagochi.  
- **Botões e controles adicionais** – Para navegação no menu e interação com o usuário.  

### **3. Estrutura do Software**  
O software foi estruturado modularmente, separando funções de sensores, manipulação de gráficos e lógica principal do programa.  

As principais implementações incluem:  
- Algoritmos para leitura de sensores e conversão de valores.  
- Controle do display utilizando apenas comandos de desenho de pixels.  
- Implementação de um menu interativo usando botões e/ou joystick.  
- Otimização da interface gráfica para exibição eficiente em 128x32 pixels.  

📦 Projeto  
 ├── 📂 .vscode/            # Configurações do VS Code  
 ├── 📂 build/              # Diretório de compilação (gerado pelo CMake)  
 ├── 📂 libs/               # Bibliotecas externas e módulos adicionais  
 ├── 📄 .gitignore          # Arquivos ignorados pelo Git  
 ├── 📄 CMakeLists.txt      # Configuração do CMake  
 ├── 📄 pico_sdk_import.cmake  # Configuração do SDK do Raspberry Pi Pico  
 ├── 📄 main.c              # Código principal do projeto  
 ├── 📄 sensors_utils.c     # Implementação de funções para sensores  
 ├── 📄 sensors_utils.h     # Definições e protótipos para sensores  
 ├── 📄 sprites.c           # Funções para manipulação gráfica no OLED  
 ├── 📄 sprites.h           # Definições de elementos gráficos  


## 📺 **Fluxo de Telas**  
O sistema possui um menu interativo, permitindo ao usuário navegar entre diferentes telas de exibição.  

### **1️⃣ Tela Principal**  
📌 Exibe duas opções principais do programa: status e interação
📌 Exibe uma versão visual do pet indicando seu humor atual  
📌 Aguarda interação do usuário para avançar a alguma das duas opções  

### **2️⃣ Tela de Status dos Sensores**  
📌 Exibe as leituras dos status do tamagochi em tempo real.  
📌 Cada um desses status é influenciado pela captura do sensor dht11 de temperatura e umidade e pela passagem do tempo.

### **3️⃣ Tela de Interação**
📌 Exibe três interações básicas disponíveis:
    - Alimentar: Alimenta uma vez o pet e diminui seu status de fome.
    - Refrescar: Espera o usuário soprar no microfone e exibe uma mensagem visual de refrescamento. Além disso diminui o status de calor, aumenta o status de hidratação e aumenta o humor.
    - Hidratar: Exibe uma mensagem visual do pet bebendo um copo de água por canudinho e aumenta o status de hidratação e diminui o status de calor.
