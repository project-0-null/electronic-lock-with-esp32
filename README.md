Aqui está uma versão formatada para um arquivo `README.md` do GitHub, consolidando as informações do código fonte, do diagrama de simulação e da especificação do projeto.

***

# Fechadura Eletrônica com ESP32 (Electronic Lock)

Este repositório contém o código-fonte e a documentação de um sistema de controle de acesso (fechadura eletrônica) desenvolvido para a disciplina de **Sistemas Embarcados I** da **UFES** (Universidade Federal do Espírito Santo).

O projeto utiliza um microcontrolador **ESP32-S3** para controlar uma tranca magnética, interfaceando com o usuário através de um teclado matricial e um display LCD 16x2.

## 🎯 Objetivo e Intuito

O principal objetivo deste projeto é pedagógico: implementar drivers de periféricos **"do zero" (bare metal)**, sem a utilização de bibliotecas externas de alto nível (como `LiquidCrystal` ou `Keypad`).

O código foi estruturado para demonstrar o domínio sobre:
*   **Protocolos de Comunicação:** Implementação manual do protocolo 4-bit para displays LCD baseados no controlador HD44780.
*   **Gerenciamento de Tempo e Interrupções:** Uso de *Hardware Timers* do ESP32 para varredura de teclado não-bloqueante.
*   **Manipulação de GPIOs:** Leitura e escrita direta em registradores e pinos digitais.
*   **Máquina de Estados:** Lógica de controle de acesso e menus de configuração.

## ⚙️ Funcionalidades

Baseado na análise do firmware, o sistema possui as seguintes características:

*   **Autenticação de Usuários:** Suporte para múltiplas senhas (3 usuários comuns e 1 administrador).
*   **Feedback Visual:** Exibição de mensagens personalizadas no LCD ("Bem-vindo Miguel", "Bem-vindo Lorenzo", etc.) e mascaramento de senha com asteriscos (`*`).
*   **Controle de Hardware:** Acionamento de uma tranca magnética (via transistor/relé) mediante senha correta.
*   **Menu de Administrador:** Acesso diferenciado para configurações (Menu: "1-senha 2-Tranca") ao digitar a senha de admin.
*   **Tratamento de Debounce:** Algoritmo robusto para evitar leituras falsas ou múltiplas no teclado.

## 🛠️ Pinagem (Hardware Mapping)

A pinagem abaixo foi extraída do código-fonte e do arquivo de configuração do simulador (`diagram.json`).

### Microcontrolador: ESP32-S3

| Periférico | Função | Pino ESP32 (GPIO) | Detalhes |
| :--- | :--- | :--- | :--- |
| **Tranca** | Atuador | **GPIO 47** | Nível ALTO libera a tranca (via TIP122) |
| **LCD 16x2** | RS (Register Select) | **GPIO 3** | Controle de comando/dados |
| | Enable (E) | **GPIO 8** | Pulso de clock para o LCD |
| | Dados D4 | **GPIO 18** | Barramento de dados 4-bit |
| | Dados D5 | **GPIO 17** | Barramento de dados 4-bit |
| | Dados D6 | **GPIO 16** | Barramento de dados 4-bit |
| | Dados D7 | **GPIO 15** | Barramento de dados 4-bit |
| **Teclado** | Linha 1 | **GPIO 48** | Entrada com Pull-down |
| | Linha 2 | **GPIO 45** | Entrada com Pull-down |
| | Linha 3 | **GPIO 0** | Entrada com Pull-down |
| | Linha 4 | **GPIO 35** | Entrada com Pull-down |
| | Coluna 1 | **GPIO 36** | Saída para varredura |
| | Coluna 2 | **GPIO 37** | Saída para varredura |
| | Coluna 3 | **GPIO 38** | Saída para varredura |
| | Coluna 4 | **GPIO 39** | Saída para varredura |
| **Sensores** | LDR / Pot | **GPIO 1** | Leitura Analógica (Controle Backlight) |

> **Nota de Hardware:** O projeto físico utiliza um buffer **74HC245** para isolar o teclado do ESP32 e garantir a integridade dos sinais, além de transistores para o acionamento de potência da tranca e do backlight.

## 📂 Estrutura do Código

O projeto é modularizado para facilitar a manutenção e leitura:

### `main.ino`
Contém a lógica principal da aplicação. Inicializa os periféricos, gerencia o buffer de senha (`senha_entry`) e verifica as credenciais comparando com as strings armazenadas. Decide qual mensagem exibir no LCD e quando acionar o pino da tranca.

### `lcd.h`
Driver personalizado para o display LCD.
*   **`EnablePulse()`**: Gera o pulso de clock manual necessário para o controlador do LCD.
*   **`write4bits()`** e **`Sendbyte()`**: Implementa a lógica de bit-shift para enviar bytes divididos em dois *nibbles* (4 bits), permitindo o uso de menos pinos.
*   **`initializationLCD()`**: Executa a sequência exata de comandos (datasheet) para inicializar o display em modo 4 bits.

### `teclado.h`
Driver para o teclado matricial.
*   **Interrupção por Timer:** Utiliza `timerAttachInterrupt` para varrer o teclado a cada 10ms, garantindo que o loop principal não fique bloqueado.
*   **`DetectaTecla()`**: Implementa lógica de *debounce* baseada em tempo (`millis`) para filtrar ruídos mecânicos das teclas.
*   **Varredura:** Alterna o nível lógico das colunas e lê as linhas para identificar a coordenada (X,Y) do botão pressionado.

## 🚀 Como Utilizar

1.  **Montagem:** Realize as conexões conforme a tabela de pinagem acima.
2.  **Upload:** Carregue o código no ESP32-S3 via Arduino IDE.
3.  **Operação:**
    *   O LCD exibirá `Senha:`.
    *   Digite a senha utilizando o teclado numérico.
    *   Pressione `*` ou complete 4 dígitos para validar.

### 🔐 Credenciais Padrão (Hardcoded)

Conforme definido em `main.ino`:

| Usuário | Senha | Ação |
| :--- | :--- | :--- |
| **Miguel** | `1234` | Libera Tranca + Mensagem Personalizada |
| **Lorenzo** | `5678` | Libera Tranca + Mensagem Personalizada |
| **Tais** | `4321` | Libera Tranca + Mensagem Personalizada |
| **Admin** | `0000` | Acesso ao Menu de Configuração |

---
*Projeto desenvolvido por [Miguel e Lorenzo] para a disciplina de Sistemas Embarcados I.*
