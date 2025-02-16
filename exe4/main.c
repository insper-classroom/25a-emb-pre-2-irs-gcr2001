#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile bool button_pressed_R = false;
volatile bool button_pressed_G = false;

void btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_FALL) { 
    if (gpio == BTN_PIN_R)
      button_pressed_R = true;
  }
  else if (events & GPIO_IRQ_EDGE_RISE) { 
    if (gpio == BTN_PIN_G)
      button_pressed_G = true;
  }
}

int main() {
  stdio_init_all();

  // LED
  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);
  gpio_put(LED_PIN_R, 0);
  
  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);
  gpio_put(LED_PIN_G, 0); 

  // BOTOES
  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                     &btn_callback);

  // callback led g (nao usar _with_callback.)
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  while (true) {
    if (button_pressed_R) {
      gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R)); 
      button_pressed_R = false; // Reseta a flag
    }
    if (button_pressed_G) {
      gpio_put(LED_PIN_G, !gpio_get(LED_PIN_G)); 
      button_pressed_G = false; // Reseta a flag
    }
  }
}
