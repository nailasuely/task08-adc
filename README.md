<h1 align="center">
  <br>
    <img width="300px" src="https://github.com/nailasuely/task07-pwm/blob/main/src/logo.png">
  <br>
  Capítulo 8 - Conversores A/D
  <br>
</h1>
<div align="center">

<h4 align="center">Projeto da Residência Tecnológica em Sistemas Embarcados </h4>

<p align="center">
<div align="center">

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/nailasuely/task07-pwm/blob/main/LICENSE)

> Este projeto utiliza o microcontrolador RP2040 para controlar LEDs RGB e exibir a posição de um joystick analógico em um display OLED SSD1306. Os valores lidos pelo Conversor Analógico-Digital (ADC) do RP2040 são usados para ajustar o brilho dos LEDs e mover um quadrado no display

## Download do repositório

<p align="center">
  
```
gh repo clone nailasuely/task08-adc
```
</p>

</div>
</div>

<details open="open">
<summary>Sumário</summary>

- [📌 Requisitos](#-requisitos)
- [🔧 Componentes Utilizados](#-componentes-utilizados)
- [🎥 Vídeo de Demonstração](#-vídeo-de-demonstração)
- [👩‍💻 Autora](#-autora)
- [📚 Referências](#-referências)

  ## 📌 Requisitos  

✅ **Uso de interrupções (IRQ):** Todos os botões utilizam rotinas de interrupção.  
✅ **Debouncing via software:** Implementado para evitar múltiplos acionamentos acidentais.  
✅ **Controle de LEDs via PWM:** Brilho ajustado conforme a posição do joystick.  
✅ **Exibição gráfica no display SSD1306:** Quadrado móvel representando o joystick.  
✅ **Integração via protocolo I2C:** Comunicação eficiente com o display.  

![---](https://github.com/nailasuely/task05-clock/blob/main/src/prancheta.png)

## 🔧 Componentes Utilizados  

- **Placa de desenvolvimento:** RP2040 (BitDogLab)  
- **Display OLED SSD1306 (128x64)** – Comunicação via **I2C**  
- **Joystick analógico** – Leitura via **ADC**  
- **LED RGB** – Controle por **PWM**  
- **Botões físicos** – Controle via **GPIO**  

![---](https://github.com/nailasuely/task05-clock/blob/main/src/prancheta.png)

## 🎥 Vídeo de Demonstração
[Link do vídeo completo](https://youtu.be/ynmqqovq_2k)

## 👩‍💻 Autora

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/nailasuely" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/98486996?v=4" width="100px;" alt=""/>
      </a>
      <br /><sub><b> Naila Suele </b></sub>
    </td>

</table>

![---](https://github.com/nailasuely/task07-pwm/blob/main/src/prancheta.png)

## 📚 Referências
- [Projeto PWM IRQ - Professor Ricardo Prates](https://github.com/rmprates84/pwm_irq)
- [EmbarcaTech U4C8 - Repositório do Professor Wilton Lacerda Silva](https://github.com/wiltonlacerda/EmbarcaTechU4C8)
- [BitDogLab - Display OLED](https://github.com/BitDogLab/BitDogLab-C/tree/main/display_oled)


