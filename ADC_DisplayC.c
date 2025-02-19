#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "pico/bootrom.h"
#include "lib/ssd1306.h"
#include "lib/font.h"


// configuração inicial do barramento I2C para o display OLED
#define PORTA_I2C i2c1
#define SDA 14
#define SCL 15
#define ENDERECO_DISPLAY 0x3C

// definição dos LEDs
#define LUZ_VERMELHA 11
#define LUZ_AMARELA 12
#define LUZ_AZUL 13

// configuração do joystick
#define EIXO_HORIZONTAL 26
#define EIXO_VERTICAL 27
#define BOTAO_JOYSTICK 22
#define BOTAO_EXTRA 5

// debounce e atualização do LED
static volatile uint32_t tempo_ultimo_clique = 0;
static volatile bool alterarPWM = true; 

#define BOTAO_BOOTSEL 6
void handler_interrupcao(uint gpio, uint32_t eventos) {
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());

    if (gpio == BOTAO_BOOTSEL) {
        reset_usb_boot(0, 0);
    } else if (gpio == BOTAO_JOYSTICK) {
        if (tempo_atual - tempo_ultimo_clique > 200000) { // debounce de 200ms
            tempo_ultimo_clique = tempo_atual;
            gpio_put(LUZ_AZUL, !gpio_get(LUZ_AZUL));
        }
    } else if (gpio == BOTAO_EXTRA) {
        if (tempo_atual - tempo_ultimo_clique > 200000) { // debounce de 200ms
            tempo_ultimo_clique = tempo_atual;
            alterarPWM = !alterarPWM;
        }
    }
}

// inicializa PWM nos pinos especificados
uint configurar_pwm(uint pino, uint limite) {
    gpio_set_function(pino, GPIO_FUNC_PWM);
    uint fatia_pwm = pwm_gpio_to_slice_num(pino);
    pwm_set_wrap(fatia_pwm, limite);
    pwm_set_enabled(fatia_pwm, true);
    return fatia_pwm;
}

// calcula intensidade do LED com base na posição horizontal do joystick
int calcular_brilhoX(int leituraX) {
    int resultado, intensidade;
    float fator = 0.0;

    if (leituraX > 2000) {
        intensidade = (2088 - (4088 - leituraX));
        fator = (float) intensidade / 2088;
        resultado = (fator * 60) + 60;
    } else if (leituraX < 1900) {
        intensidade = 1900 - leituraX;
        fator = (float) intensidade / 1900;
        resultado = 60 - (fator * 60);
    }

    if (alterarPWM) {
        pwm_set_gpio_level(LUZ_AMARELA, fator * 4096);
    }
    return resultado;
}

// calcula intensidade do LED conforme o eixo vertical do joystick
int calcular_brilhoY(int leituraY) {
    int resultado, intensidade;
    float fator = 0.0;

    if (leituraY > 2100) {
        intensidade = (1988 - (4088 - leituraY));
        fator = (float) intensidade / 1988;
        resultado = 30 - (fator * 30);
    } else if (leituraY < 2100) {
        intensidade = 2100 - leituraY;
        fator = (float) intensidade / 2100;
        resultado = 30 + (fator * 30);
    }

    if (alterarPWM) {
        pwm_set_gpio_level(LUZ_VERMELHA, fator * 4096);
    }
    return resultado;
}

int main() {
    stdio_init_all();
    gpio_init(LUZ_AZUL);
    gpio_set_dir(LUZ_AZUL, GPIO_OUT);
    adc_init();
    uint limite_pwm = 4096;

    // inicialização do display OLED via I2C
    i2c_init(PORTA_I2C, 400 * 1000);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);

    ssd1306_t tela;
    ssd1306_init(&tela, 128, 64, false, ENDERECO_DISPLAY, PORTA_I2C);
    ssd1306_config(&tela);
    ssd1306_send_data(&tela);

    ssd1306_fill(&tela, false);
    ssd1306_send_data(&tela);

    gpio_init(BOTAO_BOOTSEL);
    gpio_set_dir(BOTAO_BOOTSEL, GPIO_IN);
    gpio_pull_up(BOTAO_BOOTSEL);

    gpio_init(BOTAO_EXTRA);
    gpio_set_dir(BOTAO_EXTRA, GPIO_IN);
    gpio_pull_up(BOTAO_EXTRA);

    gpio_set_irq_enabled_with_callback(BOTAO_BOOTSEL, GPIO_IRQ_EDGE_FALL, true, &handler_interrupcao);
    gpio_set_irq_enabled_with_callback(BOTAO_EXTRA, GPIO_IRQ_EDGE_FALL, true, &handler_interrupcao);

    // configuração do PWM para os LEDs
    uint pwm_vermelho = configurar_pwm(LUZ_VERMELHA, limite_pwm);
    uint pwm_amarelo = configurar_pwm(LUZ_AMARELA, limite_pwm);

    // configuração do joystick
    gpio_init(BOTAO_JOYSTICK);
    gpio_set_dir(BOTAO_JOYSTICK, GPIO_IN);
    gpio_pull_up(BOTAO_JOYSTICK);

    adc_gpio_init(EIXO_HORIZONTAL);
    adc_gpio_init(EIXO_VERTICAL);

    uint16_t valor_adc_x;
    uint16_t valor_adc_y;
    char buffer_x[5];
    char buffer_y[5];

    int posX, posY;
    bool inverter_cor = true;

    gpio_set_irq_enabled_with_callback(BOTAO_JOYSTICK, GPIO_IRQ_EDGE_FALL, true, &handler_interrupcao);

    while (true) {
        adc_select_input(1);
        valor_adc_x = adc_read();
        adc_select_input(0);
        valor_adc_y = adc_read();

        sprintf(buffer_x, "%d", valor_adc_x);
        sprintf(buffer_y, "%d", valor_adc_y);

        posX = calcular_brilhoX(valor_adc_x);
        posY = calcular_brilhoY(valor_adc_y);

        ssd1306_fill(&tela, !inverter_cor);
        ssd1306_rect(&tela, 3, 3, 120, 60, inverter_cor, !inverter_cor);
        ssd1306_rect(&tela, posY, posX, 5, 5, inverter_cor, inverter_cor);
        ssd1306_send_data(&tela);
    }
}
