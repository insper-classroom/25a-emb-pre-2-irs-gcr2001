#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
volatile bool event_flag = 0;
volatile uint32_t event_type = 0;

void btn_callback(uint gpio, uint32_t events) {
  event_flag = true;
  event_type = events;
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

      while (true) {
        if (event_flag) {
          if (event_type & GPIO_IRQ_EDGE_FALL) {
            printf("fall \n");
          } 
          if (event_type & GPIO_IRQ_EDGE_RISE) {
            printf("rise \n");
          }
          event_flag = false;
        }
      }
}
